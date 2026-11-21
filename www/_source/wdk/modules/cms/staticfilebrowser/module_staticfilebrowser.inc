<?php

	require_once(GetWDKDir()."wdk_module.inc");
	
	class CStaticFileBrowserModule extends CModule
	{
		
		private $m_arrayErrors;
		private $m_strRootDir = "";
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"cms/staticfilebrowser",
				$strInstanceID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$this->m_arrayErrors = array();
			
			$strRootDir = $this->GetModuleConfig("rootdir");
			if (!IsDirectory($strRootDir))
			{
				$this->SetErrorMessageCode("CONFIG_ROOTDIR_INVALID");
				return false;	
			}
			$arrayToolbarConfig = array(
				'browse' => array('refresh'),
				);
			$this->SetToolbarConfig($arrayToolbarConfig);
			return parent::OnInit();
		}
		
	
		function OnRenderModule($strState)
		{
			$this->Trace("OnRenderModule(\"$strState\")");
			switch ($strState)
			{
				case 'view':
					return $this->StateView();
				case 'download':
					return $this->StateDownload();
				case 'refresh':
				case '':
					$this->SetState('browse');
				default:
					return $this->StateBrowse();
			}
		}
		
	
		function RenderToolbar()
		{
			$toolbarDef = new CToolbarDefinition();

			$strCurrentDir = $this->GetModuleVar("currentdir");
			if ($strCurrentDir != "")
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "browse";
				$arrayParams[$this->GetModuleVarName("currentdir")] = "";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "chevron-right", "?TID_STATICFILEBROWSER_TOOL_ROOTDIR?");
			
				$strParentDir = GetParentDirectoryFromPath($strCurrentDir);
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "browse";
				$arrayParams[$this->GetModuleVarName("currentdir")] = $strParentDir;
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "chevron-up", "?TID_STATICFILEBROWSER_TOOL_PARENTDIR?");

			}
			
			
			if ($this->IsToolVisible('refresh'))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = 'browse';
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, 'refresh', '?TID_STATICFILEBROWSER_TOOL_REFRESH?');
			}

			if ($toolbarDef->GetToolCount() == 0)
			{
				$this->SetOutputItem("TOOLBAR",""); 
				return;
			}
			$toolbar = new CToolbarElement($this->GetWebsite(),'default');
			$this->SetOutputItem("TOOLBAR",$toolbar->RenderToolbar($toolbarDef)); 
		}		
		
		function OnIsToolVisible($strToolID)
		{
			if ($strToolID == 'refresh')
			{
				if ($this->GetModuleConfig('hiderefresh') == true)
				{
					return false;	
				}	
			}
			return parent::OnIsToolVisible($strToolID);
		}
		
		
		static function IsTextFile($strPath)
		{
			$strExtension = StringLowerCase(GetExtentionFromPath(ReplaceString($strPath,"\\","/")));
			return in_array($strExtension,array(
				'txt','text','log','csv','tsv','md','markdown','rst','adoc','tex',
				'bat','cmd','ps1','psm1','psd1','sh','bash','zsh','fish',
				'php','php3','php4','php5','php7','php8','phtml','inc','phps',
				'html','htm','xhtml','css','scss','sass','less','js','mjs','cjs','jsx','ts','tsx',
				'json','jsonc','jsonl','xml','xsl','xslt','xsd','svg','yaml','yml','toml',
				'ini','conf','config','cfg','properties','env','htaccess','htpasswd',
				'py','pyw','rb','pl','pm','lua','r','sql','graphql','gql',
				'c','h','cc','cpp','cxx','hpp','hxx','cs','java','kt','kts','scala',
				'go','rs','swift','m','mm','vb','vbs','asp','aspx','jsp','vue','svelte',
				'dart','ex','exs','erl','hrl','hs','clj','cljs','lisp','cl','el',
				'asm','s','pas','pp','f','f90','cmake','make','mk','gradle','groovy',
				'tpl','twig','mustache','hbs','diff','patch','gitignore','gitattributes',
				'editorconfig','dockerignore'),true);
		}

		// Blocked filename regular expressions apply in every folder and to resolved symbolic-link targets.
		static function IsBlockedFile($strFile,$arrayBlockedFiles)
		{
			if (!is_array($arrayBlockedFiles))
			{
				return false;
			}
			$strFileName = StringLowerCase(GetFileNameFromPath(ReplaceString($strFile,"\\","/")));
			foreach ($arrayBlockedFiles as $strBlockedFile)
			{
				if (RegExpMatch($strFileName,$strBlockedFile))
				{
					return true;
				}
			}
			return false;
		}

		static function ResolveFile($strRootDir,$strFile,$arrayBlockedFiles = array())
		{
			if ($strRootDir == "" || FindString($strFile,"\0") != -1 || self::IsBlockedFile($strFile,$arrayBlockedFiles))
			{
				return false;
			}
			$strRoot = realpath($strRootDir);
			if ($strRoot === false || !is_dir($strRoot))
			{
				return false;
			}
			$strRoot = rtrim($strRoot,"/\\").DIRECTORY_SEPARATOR;
			$strPath = realpath($strRoot.$strFile);
			if ($strPath === false || !IsFile($strPath) || !is_readable($strPath) || self::IsBlockedFile($strPath,$arrayBlockedFiles))
			{
				return false;
			}
			$bInsideRoot = false;
			if (DIRECTORY_SEPARATOR == "\\")
			{
				$bInsideRoot = strncasecmp($strPath,$strRoot,strlen($strRoot)) == 0;
			}
			else
			{
				$bInsideRoot = strncmp($strPath,$strRoot,strlen($strRoot)) == 0;
			}
			if ($bInsideRoot)
			{
				return $strPath;
			}
			return false;
		}

		function GetRootDir()
		{
			$strRootDir = $this->GetModuleConfig("rootdir");
			return $strRootDir;
		}
		
		function GetCurrentDirRelative()
		{
			$strCurrentDirRel = $this->GetModuleVar("currentdir");
			if (FindString($strCurrentDirRel,"..") != -1)
			{
				$strCurrentDirRel = "";
			}
			return $strCurrentDirRel;
			
		}

		function GetCurrentDirAbsolute()
		{
			$strRootDir = $this->GetRootDir();
			$strCurrentDirRel = $this->GetCurrentDirRelative();
			$strCurrentDir = $strRootDir . $strCurrentDirRel;
			if (!IsDirectory($strCurrentDir))
			{
				$strCurrentDir = $strRootDir;
			}	
			return $strCurrentDir;
		}
	
		function StringTrimSlashes($strString)
		{
			$strString = StringRemoveTrailingCharacters($strString,"/");
			$strString = StringRemoveLeadingCharacters($strString,"/");
			return $strString;
		}
	
		function GetFileInfoText($strPath)
		{
			//$this->Trace("GetFileInfoText(\"$strPath\")");
			$strTID = MakeTID("FILEINFO_".$this->StringTrimSlashes($strPath))."";
			//$this->Trace("GetFileInfoText: $strTID");
			$strText = $this->LoadString($strTID);
			if ($strText != "")
			{
				return $strText;
			}

			$strFileName = GetFileNameFromPath($strPath);
			if ($strFileName != "")
			{
				$strTID = MakeTID("FILEINFO_".GetFileNameFromPath($strPath))."";
				//$this->Trace("GetFileInfoText: $strTID");
				$strText = $this->LoadString($strTID);
				if ($strText != "")
				{
					return $strText;
				}

				$strTID = MakeTID("FILEINFO_".GetFileTitleFromPath($strPath))."";
				//$this->Trace("GetFileInfoText: $strTID");
				$strText = $this->LoadString($strTID);
				if ($strText != "")
				{
					return $strText;
				}
			}
			return "";
		}


		function StateBrowsePortfolio()
		{
			$this->Trace("StateBrowsePortfolio");
			
			$strRootDir = $this->GetModuleConfig("rootdir");
			
			$strCurrentDirRel = $this->GetCurrentDirRelative();
			$strCurrentDirAbs = $this->GetCurrentDirAbsolute();

			$arrayDir = ReadDirectory($strCurrentDirAbs,false,false);

			$this->IncludeElement("portfolio");
			$portfolio = new CPortfolioElement($this->GetWebsite(),"downloads");

			$arrayPortfolio = array();
			foreach ($arrayDir as $strPathRel)
			{
				$arrayRow = array();
				
				$strLink = "";
				$strPathAbs = $strCurrentDirAbs . $strPathRel;
				
				if (IsFile($strPathAbs))
				{
					$arrayParams = array();
					$arrayParams[$this->GetModuleVarNameState()] = "download";
					if (self::IsTextFile($strPathRel))
					{
						$arrayParams[$this->GetModuleVarNameState()] = "view";
					}
					$arrayParams[$this->GetModuleVarName("file")] = $strCurrentDirRel.$strPathRel;
					$strLink = $this->GetWebsite()->MakeURL($arrayParams);
					$strFileInfo = $this->GetFileInfoText($strPathAbs);
									
					$strLinkText = "?TID_LINK_DOWNLOAD?";
					if (self::IsTextFile($strPathRel))
					{
						$strLinkText = "?TID_STATICFILEBROWSER_LINK_VIEW?";
					}
					if (self::IsBlockedFile($strPathRel,$this->GetModuleConfig("blockedfiles")))
					{
						$arrayPortfolio[] = array(
							"payload" => '<h2 class="wdk-staticfilebrowser-blocked">'.HtmlEncode($strPathRel).'</h2><p>'.$strFileInfo.'</p>');
					}
					else
					{
						$arrayPortfolio[] = array(
							"title" => $strPathRel,
							"text" => $strFileInfo,
							"linkurl" => $strLink,
							"linktext" => $strLinkText);
					}
					
				}
				
			}
			
			$strOutput = $portfolio->RenderPortfolio($arrayPortfolio);
			$this->SetOutputItem("PORTFOLIO",$strOutput);
 


			return "browse";
			
			
		}


		function StateBrowseItemManager()
		{
			$this->Trace("StateBrowseItemManager");
			
			$strRootDir = $this->GetModuleConfig("rootdir");
			
			$strCurrentDirRel = $this->GetCurrentDirRelative();
			$strCurrentDirAbs = $this->GetCurrentDirAbsolute();

			$arrayDir = ReadDirectory($strCurrentDirAbs,false,false);

			$bHideSize = $this->GetModuleConfig("hidesize");
			$bHideLastChange = $this->GetModuleConfig("hidelastchange");
			$bHideIcon = $this->GetModuleConfig("hideicon");
			$bHideInfo = $this->GetModuleConfig("hideinfo");
			$bHideTableHeader = $this->GetModuleConfig("hidetableheader");
			

			$this->IncludeElement("itemmanager");
			
			$linkicon = new CLinkIconElement($this->GetWebsite(),"default");
			$link = new CLinkElement($this->GetWebsite(),"default");
			$itemmanager = new CItemManagerElement($this->GetWebsite(),"default");
			
			$itemmanagerdata = new CItemManagerData();
			
			if ($bHideTableHeader == true)
			{
				$itemmanagerdata->SetHideTableHeader(true);	
			}

			/*
			$strFilterOptionTab = $this->GetWebsite()->GetVar("itemmanager_filteroptions_tab");
			if ($strFilterOptionTab == "")
			{
			   $strFilterOptionTab = "all";   
			}
			*/
			

			$arrayTable = array();
			foreach ($arrayDir as $strPathRel)
			{
				$arrayRow = array();

				
				$arrayRow["NAME"] = HtmlEncode($strPathRel);
				$arrayRow["INFO"] = "";
				$arrayRow["TEXT"] = "";
				$arrayRow["SIZE"] = "";
				$arrayRow["LASTCHANGE"] = "";
				
				$strLink = "";
				$strPathAbs = $strCurrentDirAbs . $strPathRel;
				
				if ($bHideInfo == false)
				{
					$arrayRow["INFO"] = $this->GetFileInfoText($strPathAbs);
				}
				
				if (IsDirectory($strPathAbs))
				{
					$arrayParams = array();
					$arrayParams[$this->GetModuleVarNameState()] = "browse";
					$arrayParams[$this->GetModuleVarName("currentdir")] = $strCurrentDirRel.$strPathRel;
					$strLink = $this->GetWebsite()->MakeURL($arrayParams);
					
					if ($bHideIcon == false)
					{
						$arrayRow["NAME"] = $linkicon->RenderLinkIcon($strLink,'folder',$strPathRel);
					}
				}
				else if (IsFile($strPathAbs))
				{
					$arrayParams = array();
					$arrayParams[$this->GetModuleVarNameState()] = "download";
					if (self::IsTextFile($strPathRel))
					{
						$arrayParams[$this->GetModuleVarNameState()] = "view";
					}
					$arrayParams[$this->GetModuleVarName("file")] = $strCurrentDirRel.$strPathRel;
					$strLink = $this->GetWebsite()->MakeURL($arrayParams);
					
					if ($bHideSize == false)
					{
						$arrayRow["SIZE"] = "".GetFileSize($strPathAbs);
					}
					if ($bHideLastChange == false)
					{
						$arrayRow["LASTCHANGE"] = $this->GetWebsite()->RenderDateTime(GetFileLastChangeTime($strPathAbs));
					}
					if ($bHideIcon == false)
					{
						$strFileCategory = GetFileCategory($strPathAbs);
						
						$arrayRow["NAME"] = $linkicon->RenderLinkIcon($strLink,GetIconIdByFileCategory($strFileCategory),$strPathRel);
					}
				}
				
				if ($strLink != "")
				{
					if ($bHideIcon == true)
					{
						$arrayRow["NAME"] = "";
					}
					$arrayRow["NAME"] = $arrayRow["NAME"] .$link->RenderLink($strLink,$strPathRel);
				}
					
				if (self::IsBlockedFile($strPathRel,$this->GetModuleConfig("blockedfiles")))
				{
					$arrayRow["NAME"] = '<span class="wdk-staticfilebrowser-blocked">'.HtmlEncode($strPathRel).'</span>';
				}
				$arrayTable[] = $arrayRow;
				
			}
			
			$itemmanagerdata->SetItemList($arrayTable);

			/*



$arrayFilterOptionTabs = array(
   "all" => "All",
   "even" => "Even",
   "uneven" => "Uneven"
   );
$itemmanagerdata->SetFilterOptionsTabNavigation(
   $strFilterOptionTab,
   "itemmanager_filteroptions_tab",
   $arrayFilterOptionTabs);

*/


			//$itemmanagerdata->AddColumn("ICON"," ","",false);
			$itemmanagerdata->AddColumn("NAME","?TID_STATICFILEBROWSER_TABLEHEADER_NAME?","",!$bHideInfo);
			if ($bHideInfo == false)
			{
				$itemmanagerdata->AddColumn("INFO"," ","",false);
			}
			if ($bHideSize == false)
			{
				$itemmanagerdata->AddColumn("SIZE","?TID_STATICFILEBROWSER_TABLEHEADER_SIZE?","",false,"","right");
			}
			if ($bHideLastChange == false)
			{
				$itemmanagerdata->AddColumn("LASTCHANGE","?TID_STATICFILEBROWSER_TABLEHEADER_LASTCHANGE?","",false);
			}

/*
$arrayColumnTabs = array(
   "primary" => "Primary",
   "secondary" => "Secondary",
   "tertiary" => "Tertiary",
   "unused" => "Unused");
 
$itemmanagerdata->SetColumnsTabNavigation(
   $this->GetWebsite()->GetVar("itemmanager_columns_tab"),
   "itemmanager_columns_tab",
   $arrayColumnTabs);
*/
/*
$itemmanagerdata->SetSortOptions(
   "itemmanager_sort_option",
   $this->GetVar("itemmanager_sort_option"),
   "itemmanager_sort_order",
   $this->GetVar("itemmanager_sort_order")
   );
*/
  		
			
			$strOutput = $itemmanager->RenderItemManager($itemmanagerdata);
			$this->SetOutputItem("LIST",$strOutput);
 
			$this->RenderToolBar();


			return "browse";
		}

		function StateBrowse()
		{
			$this->SetOutputItem("STATICFILEBROWSER_FOLDERPATH",$this->RenderFilePath(
				realpath($this->GetCurrentDirAbsolute()),true));
			switch ($this->GetStyle())
			{
			case "portfolio":
				return $this->StateBrowsePortfolio();
			default:
				return $this->StateBrowseItemManager();
			}
		}

		
		function RenderFilePath($strPath,$bDirectory = false)
		{
			$strRoot = ReplaceString(realpath($this->GetRootDir()),"\\","/");
			$strRoot = StringRemoveTrailingCharacters($strRoot,"/",true);
			$strPath = ReplaceString($strPath,"\\","/");
			$strPath = StringRemoveTrailingCharacters($strPath,"/",true);
			$strRelativePath = StringSection($strPath,StringLength($strRoot) + 1);
			$arraySections = StringExplode($strRelativePath,"/");
			$this->IncludeElement("staticfileviewer");
			$link = new CLinkElement($this->GetWebsite(),"default");
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "browse";
			$arrayParams[$this->GetModuleVarName("file")] = "";
			$arrayParams[$this->GetModuleVarName("currentdir")] = "";
			$strOutput = "";
			$strRelativeDir = "";
			foreach ($arraySections as $nIndex => $strSection)
			{
				if ($strSection == "")
				{
					continue;
				}
				$strOutput .= "/";
				$strLabel = CStaticFileViewerElement::EncodeText($strSection);
				if ($bDirectory || $nIndex < count($arraySections) - 1)
				{
					$strRelativeDir .= $strSection."/";
					$arrayParams[$this->GetModuleVarName("currentdir")] = $strRelativeDir;
					$strOutput .= $link->RenderLink($this->GetWebsite()->MakeURL($arrayParams),$strLabel);
				}
				else
				{
					$strOutput .= $strLabel;
				}
			}
			if ($bDirectory && $strOutput == "")
			{
				$strOutput = "/";
			}
			return $strOutput;
		}

		function StateView()
		{
			$strFile = $this->GetModuleVar("file");
			$strPath = self::ResolveFile($this->GetRootDir(),$strFile,$this->GetModuleConfig("blockedfiles"));
			$strText = false;
			if ($strPath !== false && self::IsTextFile($strFile) && self::IsTextFile($strPath))
			{
				$strText = FileReadBinary($strPath);
			}
			if ($strText === false)
			{
				$this->SetErrorMessageCode("FILE_INVALID");
				return $this->StateBrowse();
			}
			$this->IncludeElement("staticfileviewer");
			$this->SetOutputItem("STATICFILEVIEWER_FILENAME",CStaticFileViewerElement::EncodeText(
				GetFileNameFromPath(ReplaceString($strFile,"\\","/"))));
			$this->SetOutputItem("STATICFILEVIEWER_FILEPATH",$this->RenderFilePath($strPath));
			$viewer = new CStaticFileViewerElement($this->GetWebsite(),"default");
			$this->SetOutputItem("STATICFILEVIEWER",$viewer->RenderStaticFileViewer($strText));
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "browse";
			$toolbarDef = new CToolbarDefinition();
			$toolbarDef->AddTool($this->GetWebsite()->MakeURL($arrayParams),"chevron-left","?TID_STATICFILEBROWSER_TOOL_BACK?");
			$arrayParams[$this->GetModuleVarNameState()] = "download";
			$arrayParams[$this->GetModuleVarName("file")] = $this->GetModuleVar("file");
			$toolbarDef->AddTool($this->GetWebsite()->MakeURL($arrayParams),"download","?TID_STATICFILEBROWSER_LINK_DOWNLOAD?");
			$toolbar = new CToolbarElement($this->GetWebsite(),'default');
			$this->SetOutputItem("TOOLBAR",$toolbar->RenderToolbar($toolbarDef));
			return "view";
		}

		function StateDownload()
		{
			$strFilePathRel = $this->GetModuleVar("file");
			$strFilePathAbs = self::ResolveFile($this->GetRootDir(),$strFilePathRel,$this->GetModuleConfig("blockedfiles"));
			$strFileName = GetFilenameFromPath($strFilePathRel);
			
			if ($strFilePathAbs !== false)
			{
				
				$this->GetWebsite()->DownloadFile(
					"application/unknown",
					$strFileName,
					FileReadBinary($strFilePathAbs));
			}

			$this->SetModuleVar("file","");
			return $this->StateBrowse();
		}
		


	}

