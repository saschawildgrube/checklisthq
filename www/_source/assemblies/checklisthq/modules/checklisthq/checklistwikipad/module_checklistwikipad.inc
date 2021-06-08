<?php

	require_once(GetWDKDir().'modules/cms/wikipad/module_wikipad.inc');
	require_once(GetAssemblyDir('checklisthq').'checklisthq_wiki.inc');

	class CChecklistWikiPadModule extends CWikiPadModule
	{
		function __construct(
			CWebSite $website,
			$strWikiPageID = 'default',
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				$strWikiPageID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$bResult = parent::OnInit();
			$this->LoadStringResources('module-checklistwikipad');
			return $bResult;
		}
		
		
		function GetConfigIsTaskList()
		{
			return $this->GetModuleConfigBoolean('tasklist');	
		}	
	
		function GetConfigIsHideTabNav()
		{
			return $this->GetModuleConfigBoolean('hidetabnav');	
		}		
		
		function GetConfigIsHideView()
		{
			return $this->GetModuleConfigBoolean('hideview');	
		}		
		
		function GetConfigIsNetwork()
		{
			return $this->GetModuleConfigBoolean('network');	
		}			
		function GetConfigIsZoomableSunburst()
		{
			return $this->GetModuleConfigBoolean('zoomablesunburst');	
		}			
		function GetConfigIsRadialTidyTree()
		{
			return $this->GetModuleConfigBoolean('radialtidytree');	
		}			
	
		
		function OnIsToolVisible($strToolID)
		{
			if ($strToolID == 'view')
			{
				if ($this->GetConfigIsHideView() == true)
				{
					return false;
				}
			}
			if ($strToolID == 'network')
			{
				if ($this->GetConfigIsNetwork() == true)
				{
					return true;
				}
				else
				{
					return false;	
				}
			}			
			if ($strToolID == 'zoomablesunburst')
			{
				if ($this->GetConfigIsZoomableSunburst() == true)
				{
					return true;
				}
				else
				{
					return false;	
				}
			}	
			if ($strToolID == 'radialtidytree')
			{
				if ($this->GetConfigIsRadialTidyTree() == true)
				{
					return true;
				}
				else
				{
					return false;	
				}
			}									
			return parent::OnIsToolVisible($strToolID);	
		}
		
		
		function PrepareTabNav($strState)
		{
			if ($this->IsViewOnly() == true)
			{
				$this->SetOutputItem('TABNAV','');
				return;
			}
			
			if ($this->GetConfigIsHideTabNav() == true)
			{
				$this->SetOutputItem('TABNAV','');  
				return;
			}			
			
			$tabnavigationDef = new CTabNavigationDefinition();

			if ($this->IsToolVisible('view') == true)
			{
				$arrayParams = array();
	   		$arrayParams[$this->GetModuleVarNameState()] = 'view';
	   		$tabnavigationDef->AddTab(
	      		'view',
	      		'?TID_WIKIPAD_TAB_VIEW?',
	      		$this->GetWebsite()->MakeURL($arrayParams));
			}

			if ($this->IsToolVisible('edit') == true)
			{

				$arrayParams = array();
	   		$arrayParams[$this->GetModuleVarNameState()] = 'edit';
	   		$tabnavigationDef->AddTab(
	      		'edit',
	      		'?TID_WIKIPAD_TAB_EDIT?',
	      		$this->GetWebsite()->MakeURL($arrayParams));
			}
			
			if ($this->IsToolVisible('zoomablesunburst') == true)
			{

				$arrayParams = array();
	   		$arrayParams[$this->GetModuleVarNameState()] = 'zoomablesunburst';
	   		$tabnavigationDef->AddTab(
	      		'zoomablesunburst',
	      		'?TID_WIKIPAD_TAB_ZOOMABLESUNBURST?',
	      		$this->GetWebsite()->MakeURL($arrayParams));
			}	

			if ($this->IsToolVisible('radialtidytree') == true)
			{

				$arrayParams = array();
	   		$arrayParams[$this->GetModuleVarNameState()] = 'radialtidytree';
	   		$tabnavigationDef->AddTab(
	      		'radialtidytree',
	      		'?TID_WIKIPAD_TAB_RADIALTIDYTREE?',
	      		$this->GetWebsite()->MakeURL($arrayParams));
			}	

			if ($this->IsToolVisible('network') == true)
			{

				$arrayParams = array();
	   		$arrayParams[$this->GetModuleVarNameState()] = 'network';
	   		$tabnavigationDef->AddTab(
	      		'network',
	      		'?TID_WIKIPAD_TAB_NETWORK?',
	      		$this->GetWebsite()->MakeURL($arrayParams));
			}	

			$tabnav = new CTabNavigationElement(
			   $this->GetWebsite(),
   			'default');

			$this->SetOutputItem('TABNAV',
				$tabnav->RenderTabNavigation(
					$tabnavigationDef,
					$strState));	
		}		
		

		function OnRenderModule($strState)
		{
			if ($this->IsViewOnly() == true)
			{
				$strState = 'view';	
			}
			
			if ($strState == '' && $this->GetConfigIsNetwork() == true)
			{
				$strState = 'network';	
			}
			if ($strState == '' && $this->GetConfigIsZoomableSunburst() == true)
			{
				$strState = 'zoomablesunburst';	
			}
			if ($strState == '' && $this->GetConfigIsRadialTidyTree() == true)
			{
				$strState = 'radialtidytree';	
			}
						
			switch ($strState)
			{
				case 'network':
				{
					return $this->StateNetwork();	
				}
				case 'zoomablesunburst':
				{
					return $this->StateZoomableSunburst();	
				}				
				case 'radialtidytree':
				{
					return $this->StateRadialTidyTree();	
				}				
				case 'submit_line':
				{
					return $this->StateSubmitLine();	
				}
				default:	
				{
					return parent::OnRenderModule($strState);
				}
			}
		}
	
		
		
		function StateView()
		{
			$strPreviousState = $this->GetPreviousState();
			$this->Trace('Previous state: '.$strPreviousState);
			if ($this->GetConfigIsZoomableSunburst() == true && $strPreviousState == 'zoomablesunburst')
			{
				return $this->StateZoomableSunburst();	
			}
			if ($this->GetConfigIsNetwork() == true && $strPreviousState == 'network')
			{
				return $this->StateNetwork();
			}
			
			if ($this->GetConfigIsHideView() == true)
			{
				if ($this->GetConfigIsZoomableSunburst() == true)
				{
					return $this->StateZoomableSunburst();	
				}
				if ($this->GetConfigIsNetwork() == true)
				{
					return $this->StateNetwork();	
				}
				return $this->StateEdit();
			}
			
			$this->PrepareTabNav('view');


			$strContent = $this->GetData($this->GetDataName());
			if ($strContent === false)
			{
				$this->SetErrorMessageCode('WIKIPAD_GET');
				$this->SetOutputItem('WIKI','');
				$strContent = '';
				return parent::StateView();
			}

			if ($this->GetConfigIsTaskList() == true)
			{
				$arrayLines = StringExplode($strContent,"\n",true);
				
				$linkicon = new CLinkIconElement($this->GetWebsite(),'default');
				$strContent = '';
				$nLine = 0;
				$nLineCount = ArrayCount($arrayLines);
				foreach ($arrayLines as $strLine)
				{
					$strHtmlLinkLineDelete = '';
					$strHtmlLinkLineMoveDown = '';
					
					if (	StringSection($strLine,0,1) == '*'
						||	StringSection($strLine,0,1) == '#')
					{

						if ($nLine < $nLineCount - 1)
						{
							$arrayParams = array(
								$this->GetModuleVarName('line') => $nLine,
								$this->GetModuleVarName('action') => 'movedown',
								$this->GetModuleVarNameState() => 'submit_line'
								);
							$strHtmlLinkLineMoveDown = '&nbsp;'.$linkicon->RenderLinkIcon(
								$this->GetWebsite()->MakeURL($arrayParams),
								'hand-o-down',
								'?TID_WIKIPAD_TOOL_LINEMOVEDOWN?');
						}

						$arrayParams = array(
							$this->GetModuleVarName('line') => $nLine,
							$this->GetModuleVarName('action') => 'delete',
							$this->GetModuleVarNameState() => 'submit_line'
							);
						$strHtmlLinkLineDelete = '&nbsp;'.$linkicon->RenderLinkIcon(
							$this->GetWebsite()->MakeURL($arrayParams),
							'check-square-o',
							'?TID_WIKIPAD_TOOL_LINEDELETE?');

/*
						$arrayParams = array(
							$this->GetModuleVarName('line') => $nLine,
							$this->GetModuleVarName('action') => 'movetop',
							$this->GetModuleVarNameState() => 'submit_line'
							);
						$strUrlLineMoveTop = $this->GetWebsite()->MakeURL($arrayParams);
						$strHtmlLinkLineMoveTop = '&nbsp;'.$linkicon->RenderLinkIcon(
							$this->GetWebsite()->MakeURL($arrayParams),
							'check-square-o',
							'?TID_WIKIPAD_TOOL_LINEMOVETOP?');
*/


					}
					$strContent .=
						$strLine
						.$strHtmlLinkLineMoveDown
						.$strHtmlLinkLineDelete
						."\n";
					$nLine++;
				}
			}

			$wiki = new CWikiElement($this->GetWebSite(),'default');
			$this->SetOutputItem(
				'WIKI',
				$wiki->RenderWiki(
					$strContent));
	
			return 'view';
		}
		
	
		function StateSubmitLine()
		{
			$strContent = $this->GetData($this->GetDataName());
			if ($strContent === false)
			{
				$this->SetErrorMessageCode('WIKIPAD_GET');
				$this->SetOutputItem('WIKI','');
				$strContent = '';
				return $this->StateView();
			}

			$nLineToModify = GetIntegerValue($this->GetModuleVar('line'));
			
			if ($nLineToModify < 0)
			{
				$this->Trace('Line is < 0');
				return $this->StateView();
			}
			
			$arrayLines = StringExplode($strContent,"\n",true);
			if ($nLineToModify+1 > ArrayCount($arrayLines))		
			{
				$this->Trace('Line+1 is > number of lines');
				return $this->StateView();
			}
			
			$strAction = $this->GetModuleVar('action');
			
			if ($strAction == 'delete')
			{
				$this->SetOutputItem('LINE_DELETED','<pre>'.$arrayLines[$nLineToModify].'</pre>');
				$this->SetInfoMessageCode('LINEDELETED');
				
				$strContent = '';
				$nLine = 0;
				foreach ($arrayLines as $strLine)
				{
					if ($nLine != $nLineToModify)
					{
						$strContent .=
							$strLine
							."\n";
					}
					$nLine++;
				}
			}
			else if ($strAction == 'movedown')
			{
				//$this->SetOutputItem('LINE_MOVED_DOWN','<pre>'.$arrayLines[$nLineToModify].'</pre>');
				//$this->SetInfoMessageCode('LINEMOVEDDOWN');
				
				$strContent = '';
				$nLine = 0;
				$strLineToMove = '';
				foreach ($arrayLines as $strLine)
				{
					if ($strLineToMove != '')
					{
						if (	StringSection($strLine,0,1) == '='
							||	StringSection($strLine,0,3) == '---')
						{
							$strContent .=
								$strLineToMove
								."\n"
								.$strLine
								."\n";
							$strLineToMove = '';
							$nLine++;
							continue;
						}
					}
					if ($nLine != $nLineToModify)
					{
						$strContent .=
							$strLine
							."\n";
					}
					else
					{
						$strLineToMove = $strLine;
					}
					$nLine++;
				}
				if ($strLineToMove != '')
				{
					$strContent .= $strLineToMove;
				}
			}


			$this->SetModuleVar('line','');
			$this->SetModuleVar('action','');
					

			if ($this->SetData($this->GetDataName(),$strContent) == false)
			{
				$this->SetErrorMessageCode('WIKIPAD_SET');
			}
			
			return $this->StateView();
		}
		
		
		function StateNetwork()
		{
			$this->SetState('network');
			
			$this->PrepareTabNav('network');
			
			$strContent = $this->GetData($this->GetDataName());	

			$arrayCurrent = array();
			$nCurrentLevel = 0;

			$arrayNodes = array();
	
			$arrayLines = StringExplode($strContent,"\n",true);
			
			$nID = 0;
			foreach ($arrayLines as $strLine)
			{
				$strLine = StringTrim($strLine);
				$nLevel = GetLevelFromWikiLine($strLine);
				if ($nLevel > 0)
				{
					$arrayNode = array();
					$arrayNode['id'] = ''.$nID++;
					$arrayNode['label'] = StringRemoveCharacters($strLine,'=*');
					$arrayNode['level'] = $nLevel;
					$arrayNodes[] = $arrayNode;
				}								
			}

			$arrayLinks = array();
			$nCurrentIndex = 0;
			foreach ($arrayNodes as $arrayNode)
			{
				$nLevel = ArrayGetValue($arrayNode,'level');
				if ($nLevel > 0)
				{
					$nIndex = $nCurrentIndex - 1;
					while ($nIndex >= 0)
					{
						$arrayPreviousNode = $arrayNodes[$nIndex];
						$nPreviousLevel = ArrayGetValue($arrayPreviousNode,'level');
						if ($nPreviousLevel < $nLevel)
						{
							$arrayLink = array();
							$arrayLink['source'] = ArrayGetValue($arrayNode,'id');
							$arrayLink['target'] = ArrayGetValue($arrayPreviousNode,'id');
							$arrayLink['strength'] = 0.50; 
							$arrayLinks[] = $arrayLink;
							break;	
						}
						$nIndex--;	
					}
					
					
				}	
				
				$nCurrentIndex++;	
			}
		
			$arrayData = array();	
			$arrayData['nodes'] = $arrayNodes;
			$arrayData['links'] = $arrayLinks;
			
			$this->IncludeElement('network');
			$network = new CNetworkElement($this->GetWebsite(),'default');

			$this->SetOutputItem('NETWORK',$network->RenderNetwork($arrayData));	
			
			return 'network';
		}
		
		
		function StateZoomableSunburst()
		{
			$this->SetState('zoomablesunburst');
			
			$this->PrepareTabNav('zoomablesunburst');		
			
			$strContent = $this->GetData($this->GetDataName());	
	
			$arrayData = MakeTreeDataFromWiki($strContent);
			
			$arrayConfig = [];
			$arrayConfig['hidevalue'] = true;
			$arrayConfig['sort'] = '';
			
			
			$this->IncludeElement('zoomablesunburst');
			$zoomablesunburst = new CZoomableSunBurstElement($this->GetWebsite(),'default');
			$this->SetOutputItem('ZOOMABLESUNBURST',$zoomablesunburst->RenderZoomableSunburst($arrayData,$arrayConfig));

			return 'zoomablesunburst';
		}

		function StateRadialTidyTree()
		{
			$this->SetState('radialtidytree');
			
			$this->PrepareTabNav('radialtidytree');		
			
			$strContent = $this->GetData($this->GetDataName());	
	
			$arrayData = MakeTreeDataFromWiki($strContent);
			$arrayData['label'] = ' ';
			
			$this->IncludeElement('radialtidytree');
			$radialtidytree = new CRadialTidyTreeElement($this->GetWebsite(),'default');
			$this->SetOutputItem('RADIALTIDYTREE',$radialtidytree->RenderRadialTidyTree($arrayData));

			return 'radialtidytree';
		}


	}

