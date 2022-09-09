<?php

	require_once(GetWDKDir().'modules/cms/wikipad/module_wikipad.inc');
	require_once(GetAssemblyDir('checklisthq').'checklisthq_wiki.inc');

	class CChecklistWikiPadModule extends CWikiPadModule
	{
		var $m_form;
		var $m_strTID_ButtonAdd;
		var $m_strTID_ButtonAddPriority;
		var $m_strHtmlLinkLineMoveDownTemplate;
		var $m_strHtmlLinkLineMoveTopTemplate;
		var $m_strHtmlLinkLineDeleteTemplate;
		var $m_strHtmlLinkLineMoveToWaitingForTemplate;
		var $m_strHtmlLinkLineMoveToOtherContextTemplate;
		
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
				case 'submitline':
				{
					return $this->StateSubmitLine();	
				}
				default:	
				{
					return parent::OnRenderModule($strState);
				}
			}
		}
	
		function MakeAddTaskForm($nLine,$nLinePriority = 0)
		{
			$this->m_formDef = new CFormDefinition();

			$arrayParams = array(); 
			$arrayParams[$this->GetModuleVarNameState()] = 'submitline';
			$arrayParams[$this->GetModuleVarName('action')] = 'add';
			$arrayParams[$this->GetModuleVarName('line')] = ''.$nLine;
			if ($nLinePriority > 0)
			{
				$arrayParams[$this->GetModuleVarName('line_priority')] = ''.$nLinePriority;
			}
			$this->m_formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$this->m_formDef->AddControlText('TEXT',$this->GetModuleVarName('text'),'','',60,200); 
			
			if ($this->m_strTID_ButtonAdd == '')
			{
				$this->m_strTID_ButtonAdd = $this->LoadString('TID_WIKIPAD_BUTTON_ADD');
			}
			if ($this->m_strTID_ButtonAddPriority == '')
			{
				$this->m_strTID_ButtonAddPriority = $this->LoadString('TID_WIKIPAD_BUTTON_ADD_PRIORITY');
			}
			
			$this->m_formDef->AddControlButtonSubmit($this->m_strTID_ButtonAdd,'SUBMIT');
			if ($nLinePriority > 0)
			{			
				$this->m_formDef->AddControlButtonSubmit($this->m_strTID_ButtonAddPriority,'SUBMIT_PRIORITY','priority');
			}
			if ($this->m_form == null)
			{
				$this->m_form = new CFormElement($this->GetWebsite(),'compact');		
			}
			return StringRemoveControlChars($this->m_form->RenderForm($this->m_formDef,array()));
		}
		
		function MakeHtmlTemplateLinkLineAction($strAction,$strIcon,$strLabel)
		{
			$linkicon = new CLinkIconElement($this->GetWebsite(),'default');
			$arrayParams = array(
				$this->GetModuleVarName('line') => 'LINE',
				$this->GetModuleVarName('linehash') => 'HASH',
				$this->GetModuleVarName('action') => $strAction,
				$this->GetModuleVarNameState() => 'submitline'
				);
			$strTemplate = '&nbsp;'.$linkicon->RenderLinkIcon(
				$this->GetWebsite()->MakeURL($arrayParams),
				$strIcon,
				$strLabel); 
			return $strTemplate;
		}
		
		function MakeHtmlLinkMoveDown($nLine,$strLineHash)
		{
			if ($this->m_strHtmlLinkLineMoveDownTemplate == '')
			{
				$this->m_strHtmlLinkLineMoveDownTemplate = $this->MakeHtmlTemplateLinkLineAction(
					'movedown','hand-o-down',
					$this->LoadString('TID_WIKIPAD_TOOL_LINEMOVEDOWN'));
			}
			$strHtml = ReplaceString($this->m_strHtmlLinkLineMoveDownTemplate,'LINE',$nLine);
			$strHtml = ReplaceString($strHtml,'HASH',$strLineHash);
			return $strHtml;
		}
	
		function MakeHtmlLinkMoveTop($nLine,$strLineHash)
		{
			if ($this->m_strHtmlLinkLineMoveTopTemplate == '')
			{
				$this->m_strHtmlLinkLineMoveTopTemplate = $this->MakeHtmlTemplateLinkLineAction(
					'movetop','rocket',
					$this->LoadString('TID_WIKIPAD_TOOL_LINEMOVETOP'));	
			}
			$strHtml = ReplaceString($this->m_strHtmlLinkLineMoveTopTemplate,'LINE',$nLine);
			$strHtml = ReplaceString($strHtml,'HASH',$strLineHash);
			return $strHtml; 
		}	

		function MakeHtmlLinkDelete($nLine,$strLineHash)
		{
			if ($this->m_strHtmlLinkLineDeleteTemplate == '')
			{
				$this->m_strHtmlLinkLineDeleteTemplate = $this->MakeHtmlTemplateLinkLineAction(
					'delete','check-square-o',
					$this->LoadString('TID_WIKIPAD_TOOL_LINEDELETE'));	
			}
			$strHtml = ReplaceString($this->m_strHtmlLinkLineDeleteTemplate,'LINE',$nLine);
			$strHtml = ReplaceString($strHtml,'HASH',$strLineHash);
			return $strHtml;
		}	

		function MakeHtmlLinkMoveToWaitingFor($nLine,$strLineHash)
		{
			if ($this->m_strHtmlLinkLineMoveToWaitingForTemplate == '')
			{
				$this->m_strHtmlLinkLineMoveToWaitingForTemplate = $this->MakeHtmlTemplateLinkLineAction(
					'movetowaitingfor','hourglass-start',
					$this->LoadString('TID_WIKIPAD_TOOL_LINEMOVETOWAITINGFOR'));	
			}
			$strHtml = ReplaceString($this->m_strHtmlLinkLineMoveToWaitingForTemplate,'LINE',$nLine);
			$strHtml = ReplaceString($strHtml,'HASH',$strLineHash);
			return $strHtml;
		}		
	
		function MakeHtmlLinkMoveToOtherContext($nLine,$strLineHash)
		{
			if ($this->m_strHtmlLinkLineMoveToOtherContextTemplate == '')
			{
				$this->m_strHtmlLinkLineMoveToOtherContextTemplate = $this->MakeHtmlTemplateLinkLineAction(
					'movetoothercontext','exchange',
					$this->LoadString('TID_WIKIPAD_TOOL_LINEMOVETOOTHERCONTEXT'));	
			}
			$strHtml = ReplaceString($this->m_strHtmlLinkLineMoveToOtherContextTemplate,'LINE',$nLine);
			$strHtml = ReplaceString($strHtml,'HASH',$strLineHash);
	
			$nContext = $this->GetWebsite()->GetPlugin('checklisthq')->GetContextID();
			$nOtherContext = 1-$nContext;
			$strOtherContextName = $this->GetWebsite()->GetPlugin('checklisthq')->GetContextName($nOtherContext);
			$strHtml = ReplaceString($strHtml,'{CONTEXT}',$strOtherContextName);
	
			return $strHtml;
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
				$this->GetWebsite()->MaintainScrollPosition();
				
				
				$arrayLines = StringExplode($strContent,"\n",false);
				
				$bAddTaskFormAdded = false;
				$linkicon = new CLinkIconElement($this->GetWebsite(),'default');
				$strContent = '';
				$nLine = 0;
				$nLineCount = ArrayCount($arrayLines);
				$strSection = 'nextaction'; // the default
				
				$strTID_LineMoveToWaitingFor = $this->LoadString('TID_WIKIPAD_TOOL_LINEMOVETOWAITINGFOR');
				$strTID_LineMoveToNextAction = $this->LoadString('TID_WIKIPAD_TOOL_LINEMOVETONEXTACTION');

			
				$strMode = '';
				$bFirstLineInSection = false;
				
				foreach ($arrayLines as $strLine)
				{
					$strPre = '';
					switch ($strMode)
					{
					case 'pre':
						$nPosPreEnd = FindString($strLine,'</pre>');
						if ($nPosPreEnd != -1)
						{
							$strMode = '';
						}
						$strContent .= $strLine."\n";
						$nLine++;
						continue 2;

					default:
						$nPosPreStart = FindString($strLine,'<pre>');
						$nPosPreEnd = FindString($strLine,'</pre>');
						if ($nPosPreStart != -1)
						{
							$strPre = StringSection($strLine,$nPosPreStart)."\n";   
							$strLine = StringSection($strLine,0,$nPosPreStart);
							
							if ($nPosPreEnd == -1)
							{
								$strMode = 'pre';
							}

/*							
							if ($nPosPreEnd != -1)
							{
								$strMode = '';
							}
	*/
							
						}					
						break;	
					}
					
					$strOldSection = $strSection;
					
					if (FindString($strLine,'=WAITING FOR=') != -1)
					{
						$strSection = 'waitingfor';
					}
					else if (FindString($strLine,'=NEXT ACTION=') != -1)
					{
						$strSection = 'nextaction';	
					}
					else if (StringSection($strLine,0,1) == '=')
					{
						$strSection = '';	
					}
					if ($bFirstLineInSection == false)
					{
						$bFirstLineInSection = ($strOldSection != $strSection) ? (true) : (false);
					}

					
					$strHtmlFormAddTask = '';
					$strHtmlLinkLineDelete = '';
					$strHtmlLinkLineMoveDown = '';
					$strHtmlLinkLineMoveTop = '';
					$strHtmlLinkLineMoveToNextAction = ''; 
					$strHtmlLinkLineMoveToWaitingFor = '';
					$strHtmlLinkLineMoveToOtherContext = '';
					
					
					if (StringSection($strLine,0,1) == '=')
					{
						$nLineToAdd = $nLine+1;
						
						for ($nLineSearch = $nLine+1; $nLineSearch < ArrayCount($arrayLines); $nLineSearch++)
						{
							$strLineSearch = $arrayLines[$nLineSearch];
							if (StringSection($strLineSearch,0,1) == '=')
							{
								break;
							}
							if (StringSection($strLineSearch,0,4) == '----')
							{
								break;
							}
						}
						$nLineToAdd = $nLineSearch;
						
						$strHtmlFormAddTask = "\n".$this->MakeAddTaskForm($nLineToAdd,$nLine+1);
						$bAddTaskFormAdded = true;
					}
					
					if (	StringSection($strLine,0,1) == '*'
						||	StringSection($strLine,0,1) == '#')
					{
						
						$strLineHash = MakeHash($strLine);

						if ($nLine < $nLineCount - 1)
						{
							$strHtmlLinkLineMoveDown = $this->MakeHtmlLinkMoveDown($nLine,$strLineHash);
						}
						
						$strHtmlLinkLineMoveToOtherContext = $this->MakeHtmlLinkMoveToOtherContext($nLine,$strLineHash);
						
						$strHtmlLinkLineDelete = $this->MakeHtmlLinkDelete($nLine,$strLineHash);

						if ($strSection != 'waitingfor')
						{
							$strHtmlLinkLineMoveToWaitingFor = $this->MakeHtmlLinkMoveToWaitingFor($nLine,$strLineHash);
	/*						$arrayParams = array(
								$this->GetModuleVarName('line') => $nLine,
								$this->GetModuleVarName('linehash') => $strLineHash,
								$this->GetModuleVarName('action') => 'movetowaitingfor',
								$this->GetModuleVarNameState() => 'submitline'
								);
							$strHtmlLinkLineMoveToWaitingFor = '&nbsp;'.$linkicon->RenderLinkIcon(
								$this->GetWebsite()->MakeURL($arrayParams),
								'hourglass-start',
								$strTID_LineMoveToWaitingFor);
		*/
						}

						if ($strSection != 'nextaction')   
						{
							$arrayParams = array(
								$this->GetModuleVarName('line') => $nLine,
								$this->GetModuleVarName('linehash') => $strLineHash,
								$this->GetModuleVarName('action') => 'movetonextaction',
								$this->GetModuleVarNameState() => 'submitline'
								);
							$strHtmlLinkLineMoveToWaitingFor = '&nbsp;'.$linkicon->RenderLinkIcon(
								$this->GetWebsite()->MakeURL($arrayParams),
								'rocket',
								$strTID_LineMoveToNextAction);
						}

						if ($strSection == 'nextaction' && $bFirstLineInSection == false)   
						{
							$strHtmlLinkLineMoveTop = $this->MakeHtmlLinkMoveTop($nLine,$strLineHash);
						}

						$bFirstLineInSection = false;
					}
					$strContent .=
						$strLine
						.$strHtmlFormAddTask
						.$strHtmlLinkLineMoveToWaitingFor
						.$strHtmlLinkLineMoveToNextAction
						.$strHtmlLinkLineMoveTop
						.$strHtmlLinkLineMoveToOtherContext
						.$strHtmlLinkLineMoveDown 
						.$strHtmlLinkLineDelete
						."\n".$strPre;
					$nLine++;
					
				}
				
				if ($bAddTaskFormAdded == false)
				{
						$strContent = $this->MakeAddTaskForm(0)."\n".$strContent;
				}
			} 

			$wiki = new CWikiElement($this->GetWebSite(),'default');
			$this->SetOutputItem(
				'WIKI',
				$wiki->RenderWiki($strContent));
					
			return 'view';
		}
		
	
		function StateSubmitLine()
		{
			$this->Trace('StateSubmitLine');
			$strContentOld = $this->GetData($this->GetDataName());
			if ($strContentOld === false)
			{
				$this->SetErrorMessageCode('WIKIPAD_GET');
				$this->SetOutputItem('WIKI','');
				return $this->StateView();
			}

			$nLineToModify = GetIntegerValue($this->GetModuleVar('line'));
			$nLineToModifyPriority = GetIntegerValue($this->GetModuleVar('line_priority'));
			$strLineToModifyHash = $this->GetModuleVar('linehash');
			$strAction = $this->GetModuleVar('action');
			$strLineText = $this->GetModuleVar('text');
			$strSubmit = $this->GetParam('submit');
			
			$this->Debug($nLineToModifyPriority);
			$this->Debug($strSubmit);
	
			$this->SetModuleVar('line','');
			$this->SetModuleVar('linehash','');
			$this->SetModuleVar('action','');
			$this->SetModuleVar('text','');
			
			$strLineToBeMovedToNextActionOtherContext = '';
			
			if ($strAction == '')
			{
				$strContent = $strContentOld;
			}
			else
			{
			
				if ($nLineToModify < 0)
				{
					if ($strAction != 'add')
					{
						$this->Trace('Line is < 0');
						return $this->StateView();
					}
				}
				
				$arrayLines = StringExplode($strContentOld,"\n",false);
				
				$nMaxLine = ArrayCount($arrayLines) - 1;
				if ($strAction == 'add')
				{
					$nMaxLine++;
				}
				if ($nLineToModify > $nMaxLine)		
				{
					$this->SetErrorMessageCode('LINE');
					return $this->StateView();
				}
	
				if ($strAction != 'add')
				{
					$strLineToModify = $arrayLines[$nLineToModify];
					$strLineHash = MakeHash($strLineToModify);
					if ($strLineHash != $strLineToModifyHash)
					{
						$this->SetErrorMessageCode('LINE');
						return $this->StateView();
					}
				}
				
				$bHasNextAction = (FindString($strContentOld,'=NEXT ACTION=') != -1)?(true):(false);
				$bHasWaitingFor = (FindString($strContentOld,'=WAITING FOR=') != -1)?(true):(false);
				
				if ($strAction == 'movetop') // we interpret that as 'movetonextaction'
				{
					$strAction = 'movetonextaction';
				}
			
				if ($strAction == 'delete')
				{
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
					$this->SetOutputItem('LINE_DELETED','<pre>'.$arrayLines[$nLineToModify].'</pre>');
					$this->SetInfoMessageCode('LINEDELETED');
				}
				else if ($strAction == 'movedown')
				{
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
				else if ($strAction == 'movetonextaction')
				{
					if ($bHasNextAction == false)
					{
						$arrayLines[] = '=NEXT ACTION=';
					}
	
					$strContent = '';
					$nLine = 0;
					foreach ($arrayLines as $strLine)
					{
						//$this->Trace($strLine);
						if ($nLine != $nLineToModify)
						{
							$strContent .=
								$strLine
								."\n";
							if (FindString($strLine,'=NEXT ACTION=') != -1)
							{
								$strContent .=
									$strLineToModify
									."\n";
							}
						}
						$nLine++;
					}
					
					$this->SetOutputItem('LINE_MOVED','<pre>'.$strLineToModify.'</pre>');
					$this->SetInfoMessageCode('LINEMOVEDTONEXTACTION');	
				}
				else if ($strAction == 'movetowaitingfor')
				{
					$strContent = '';
		
					if ($bHasWaitingFor == false)  
					{
						$strContent .=
							'=WAITING FOR='
							."\n"
							.$strLineToModify
							."\n";
					}
					
					if ($bHasNextAction == false)
					{
						$strContent .=
							'=NEXT ACTION='
							."\n";
					}
	
					$nLine = 0;
					foreach ($arrayLines as $strLine)
					{
						$this->Trace($strLine);
						if ($nLine != $nLineToModify)
						{
							$strContent .=  
								$strLine
								."\n";
							if (FindString($strLine,'=WAITING FOR=') != -1)
							{
								$strContent .=
									$strLineToModify
									."\n";
							}
						}
						$nLine++;
					}
					
					$this->SetOutputItem('LINE_MOVED','<pre>'.$strLineToModify.'</pre>');
					$this->SetInfoMessageCode('LINEMOVEDTOWAITINGFOR');	
				}
				else if ($strAction == 'add')
				{
					$strLineText = StringTrim($strLineText);
					if ($strLineText == '')
					{
						return $this->StateView();
					}
					
					if (	StringSection($strLineText,0,1) != '*'
						&&	StringSection($strLineText,0,1) != '#')
					{
						$strLineText = '* '.$strLineText;
					}
					
					if (FindString($strContentOld,$strLineText) != -1)
					{
						$this->SetOutputItem('LINE_ADD','<pre>'.$strLineText.'</pre>');
						$this->SetErrorMessageCode('DUPLICATELINE');
						return $this->StateView();
					}
					
					$nLineToAdd = $nLineToModify;
					if ($strSubmit == 'priority')
					{
						$nLineToAdd = $nLineToModifyPriority;
					}
					
					if ($nLineToAdd == 0)
					{
							$strContent =
								$strLineText
								."\n"
								. $strContentOld;
						
					}
					else
					{
						$strContent = '';
						$nLine = 0;
						foreach ($arrayLines as $strLine)
						{
							$strContent .=
								$strLine  
								."\n";
							if ($nLine == ($nLineToAdd - 1))  
							{
								$strContent .=
									$strLineText
									."\n";
							}
							$nLine++;
						}
					}
					$this->SetOutputItem('LINE_ADDED','<pre>'.$strLineText.'</pre>');
					$this->SetInfoMessageCode('LINEADDED');
				}
				else if ($strAction == 'movetoothercontext')
				{
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
						else
						{
							$strLineToBeMovedToNextActionOtherContext = $strLine;
						}
						$nLine++;
					}
				}
				else // unknown line action
				{
					$this->SetErrorMessageCode('LINEACTIONUNKNOWN');
					$strContent = $strContentOld;
				}


			}

			$strContent = StringTrim($strContent);

			if ($this->SetData($this->GetDataName(),$strContent) == false)
			{
				$this->SetErrorMessageCode('WIKIPAD_SET');
			}
			else
			{
				if ($strLineToBeMovedToNextActionOtherContext != '')
				{
					// TODO: It does not make sense to display a message directly before redirecting.
					// This should be refactored to avoid the additional round-trip
					//$this->SetOutputItem('LINE_MOVED','<pre>'.$arrayLines[$nLineToModify].'</pre>');
					//$this->SetInfoMessageCode('LINEMOVEDTOOTHERCONTEXT');

					$nOtherContext = 1 - $this->GetWebsite()->GetPlugin('checklisthq')->GetContextID();

					$strModuleVarNamePrefix = 'wikipad_tasks_';
					if ($nOtherContext > 0)
					{
						$strModuleVarNamePrefix .= $nOtherContext.'_'; 
					}

					$arrayParams = array(); 
					$arrayParams[$strModuleVarNamePrefix.'state'] = 'submitline';
					$arrayParams[$strModuleVarNamePrefix.'action'] = 'add';
					$arrayParams[$strModuleVarNamePrefix.'line'] = '';
					$arrayParams[$strModuleVarNamePrefix.'text'] = $strLineToBeMovedToNextActionOtherContext; 
					$arrayParams['content'] = 'tasks'; 
					$arrayParams['checklisthq_context'] = ''.$nOtherContext; 

					$strUrl = $this->GetWebsite()->MakeURL($arrayParams);
					$this->GetWebsite()->Redirect($strUrl);
				}
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
			$arrayNodes = AlignListLevels($arrayNodes,'level');
			
			foreach ($arrayNodes as &$arrayNode2)
			{
				$nLevel = ArrayGetValue($arrayNode2,'level');
				switch ($nLevel)
				{
				case 0:
					$arrayNode2['type'] = 'darkblue';
					break;
				case 1:
					$arrayNode2['type'] = 'blue';
					break;
				default:
					$arrayNode2['type'] = 'lightblue';
					break;
				}
				
			}
			

			$arrayLinks = array();
			$nCurrentIndex = 0;
			foreach ($arrayNodes as $arrayNode)
			{
				$nLevel = ArrayGetValue($arrayNode,'level');
				if ($nLevel >= 0)
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

