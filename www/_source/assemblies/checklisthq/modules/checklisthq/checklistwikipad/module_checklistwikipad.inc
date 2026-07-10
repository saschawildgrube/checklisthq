<?php

	require_once(GetWDKDir().'modules/cms/wikipad/module_wikipad.inc');
	require_once(GetAssemblyDir('checklisthq').'checklisthq_wiki.inc');

	class CChecklistWikiPadModule extends CWikiPadModule
	{
		var $m_form;
		var $m_strTID_ButtonAdd;
		var $m_strTID_ButtonAddPriorityHigh;
		var $m_strTID_ButtonAddPriorityMedium;
		var $m_strTID_ButtonAddPriorityLow;
		var $m_strTID_ButtonAddChecklist;
		var $m_strHtmlLinkLineMoveDownTemplate;
		var $m_strHtmlLinkLineMoveTopTemplate;
		var $m_strHtmlLinkLineDeleteTemplate;
		var $m_strHtmlLinkLineMoveToWaitingForTemplate;
		var $m_strHtmlLinkLineMoveToNextActionTemplate;
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
	
		function GetConfigGetChecklistTarget()
		{
			return $this->GetModuleConfig('checklists');	
		}		
		
		function GetConfigIsChecklists()
		{
			return ($this->GetConfigGetChecklistTarget() != '');
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
				case 'submit':
				{
					return $this->StateSubmit();	
				}
				case 'submitchecklist':
				{
					return $this->StateSubmitChecklist();	
				}
				default:	
				{
					return parent::OnRenderModule($strState);
				}
			}
		}
	
	
		function WikiEncode($strWiki)
		{
			$strEncoded = StringTrim($strWiki);
			$strEncoded = HtmlEncode($strEncoded);
			$strEncoded = ReplaceString($strEncoded,"\n",'___NEWLINE___');
			$strEncoded = ReplaceString($strEncoded,'[','___BRACKET_OPEN___');
			$strEncoded = ReplaceString($strEncoded,']','___BRACKET_CLOSE___');
			$strEncoded = ReplaceString($strEncoded,'*','___ASTERIX___');
			return $strEncoded;
		}			

		function WikiDecode($strEncoded)
		{
			$strWiki = StringTrim($strEncoded);
			$strWiki = ReplaceString($strWiki,'___ASTERIX___','*');
			$strWiki = ReplaceString($strWiki,'___BRACKET_CLOSE___',']');
			$strWiki = ReplaceString($strWiki,'___BRACKET_OPEN___','[');
			$strWiki = ReplaceString($strWiki,'___NEWLINE___',"\n");
			$strWiki = HtmlDecode($strWiki);

			return $strWiki;
		}			


		function MakeAddChecklistForm($strChecklistTitle,$strChecklistContent)
		{
			$formDef = new CFormDefinition();

			$arrayParams = array(); 
			
			//$arrayParams[$this->GetModuleVarNameState()] = 'submitchecklist';
			$arrayParams[$this->GetModuleVarName('checklist_title')] = $this->WikiEncode($strChecklistTitle);
			$arrayParams[$this->GetModuleVarName('checklist_content')] = $this->WikiEncode($strChecklistContent);
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams); 

			$formDef->SetHiddenValue($this->GetModuleVarNameState(),'submitchecklist');

			
			if ($this->m_strTID_ButtonAddChecklist == '')
			{
				$this->m_strTID_ButtonAddChecklist = $this->LoadString('TID_WIKIPAD_BUTTON_ADD_CHECKLIST');
			}
			
			$formDef->AddControlButtonSubmit($this->m_strTID_ButtonAddChecklist,'SUBMIT_CHECKLIST','add_checklist');
			if ($this->m_form == null)
			{
				$this->m_form = new CFormElement($this->GetWebsite(),'compact');		
			}
	
			$formDef->AddControlText('TEXT',$this->GetModuleVarName('checklist_prefix'),'','',30,30); 


			return StringRemoveControlChars($this->m_form->RenderForm($formDef,array()));
		}
	
		function MakeAddTaskForm($nLinePriorityLow,$nLinePriorityMedium,$nLinePriorityHigh)
		{
			$formDef = new CFormDefinition();

			$arrayParams = array(); 
			$arrayParams[$this->GetModuleVarNameState()] = 'submitline';
			$arrayParams[$this->GetModuleVarName('action')] = 'add';
			$arrayParams[$this->GetModuleVarName('line_priority_low')] = ''.$nLinePriorityLow;
			$arrayParams[$this->GetModuleVarName('line_priority_medium')] = ''.$nLinePriorityMedium;
			$arrayParams[$this->GetModuleVarName('line_priority_high')] = ''.$nLinePriorityHigh;
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$formDef->AddControlText('TEXT',$this->GetModuleVarName('text'),'','',60,200); 
			
			/*if ($this->m_strTID_ButtonAdd == '')
			{
				$this->m_strTID_ButtonAdd = $this->LoadString('TID_WIKIPAD_BUTTON_ADD');
			}*/
			if ($this->m_strTID_ButtonAddPriorityHigh == '')
			{
				$this->m_strTID_ButtonAddPriorityHigh = $this->LoadString('TID_WIKIPAD_BUTTON_ADD_PRIORITY_HIGH');
			}
			if ($this->m_strTID_ButtonAddPriorityMedium == '')
			{
				$this->m_strTID_ButtonAddPriorityMedium = $this->LoadString('TID_WIKIPAD_BUTTON_ADD_PRIORITY_MEDIUM');
			}
			if ($this->m_strTID_ButtonAddPriorityLow == '')
			{
				$this->m_strTID_ButtonAddPriorityLow = $this->LoadString('TID_WIKIPAD_BUTTON_ADD_PRIORITY_LOW');
			}
			
			$formDef->AddControlButtonSubmit($this->m_strTID_ButtonAddPriorityLow,'SUBMIT_PRIORITY_LOW','add_low');
			if ($nLinePriorityMedium > 0)
			{			
				$formDef->AddControlButtonSubmit($this->m_strTID_ButtonAddPriorityMedium,'SUBMIT_PRIORITY_MEDIUM','add_medium');
			}
			if ($nLinePriorityHigh > 0)
			{			
				$formDef->AddControlButtonSubmit($this->m_strTID_ButtonAddPriorityHigh,'SUBMIT_PRIORITY_HIGH','add_high');
			}
			if ($this->m_form == null)
			{
				$this->m_form = new CFormElement($this->GetWebsite(),'compact');		
			}
			return StringRemoveControlChars($this->m_form->RenderForm($formDef,array()));
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
	
		function MakeHtmlLinkMoveToNextAction($nLine,$strLineHash)
		{
			if ($this->m_strHtmlLinkLineMoveToNextActionTemplate == '')
			{
				$this->m_strHtmlLinkLineMoveToNextActionTemplate = $this->MakeHtmlTemplateLinkLineAction(
					'movetonextaction','rocket',
					$this->LoadString('TID_WIKIPAD_TOOL_LINEMOVETONEXTACTION'));
			}
			$strHtml = ReplaceString($this->m_strHtmlLinkLineMoveToNextActionTemplate,'LINE',$nLine);
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

			
			
		function ComputeTaskListInsertPositions($arrayLines)
		{
			$nLineCount = ArrayCount($arrayLines);
			$arrayPositions = array();
			$nNextHeader = -1;
			$nNextSection = -1;

			for ($nLine = $nLineCount - 1; $nLine >= 0; $nLine--)
			{
				$strLine = $arrayLines[$nLine];
				if (StringSection($strLine,0,4) == '----')
				{
					$nNextSection = $nLine;
				}
				if (StringSection($strLine,0,1) == '=')
				{
					$nLineToAddHigh = $nLine + 1;
					$nLineToAddLow = ($nNextHeader != -1) ? $nNextHeader : $nLineCount;
					$nLineToAddMedium = ($nNextSection != -1) ? $nNextSection : (($nNextHeader != -1) ? $nNextHeader : $nLineCount);
					$arrayPositions[$nLine] = array(
						'low' => $nLineToAddLow,
						'medium' => $nLineToAddMedium,
						'high' => $nLineToAddHigh
					);
					$nNextHeader = $nLine;
				}
			}

			return $arrayPositions;
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
				$nLineCount = ArrayCount($arrayLines);
				$arrayInsertPositions = $this->ComputeTaskListInsertPositions($arrayLines);
				$strSection = 'nextaction'; // the default
				$arrayOutput = array();
				
				$strTID_LineMoveToWaitingFor = $this->LoadString('TID_WIKIPAD_TOOL_LINEMOVETOWAITINGFOR');

			
				$strMode = '';
				$bFirstLineInSection = false;
				
				foreach ($arrayLines as $nLine => $strLine)
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
						$arrayOutput[] = $strLine."\n";
						continue;

					default:
						$nPosPreStart = FindString($strLine,'<pre>');
						$nPosPreEnd = FindString($strLine,'</pre>');
						if ($nPosPreStart != -1)
						{
							$strPre = StringSection($strLine,$nPosPreStart);
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
						if (isset($arrayInsertPositions[$nLine]))
						{
							$strHtmlFormAddTask = "\n".$this->MakeAddTaskForm(
								$arrayInsertPositions[$nLine]['low'], 
								$arrayInsertPositions[$nLine]['medium'], 
								$arrayInsertPositions[$nLine]['high']);
							$bAddTaskFormAdded = true;
						}
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
							$strHtmlLinkLineMoveToNextAction = $this->MakeHtmlLinkMoveToNextAction($nLine,$strLineHash);
						}

						if ($strSection == 'nextaction' && $bFirstLineInSection == false)   
						{
							$strHtmlLinkLineMoveTop = $this->MakeHtmlLinkMoveTop($nLine,$strLineHash);
						}

						$bFirstLineInSection = false;
					}
					$arrayOutput[] =
						$strLine
						.$strHtmlFormAddTask
						.$strHtmlLinkLineMoveToWaitingFor
						.$strHtmlLinkLineMoveToNextAction
						.$strHtmlLinkLineMoveTop
						.$strHtmlLinkLineMoveToOtherContext
						.$strHtmlLinkLineMoveDown 
						.$strHtmlLinkLineDelete
						."\n".$strPre;
					
				}
				
				if ($bAddTaskFormAdded == false)
				{
					array_unshift($arrayOutput, $this->MakeAddTaskForm(0,0,0)."\n");
				}
				$strContent = implode('',$arrayOutput);
			}
			
			if ($this->GetConfigIsChecklists() == true)
			{
				$arrayLines = StringExplode($strContent,"\n",false);
				$strContent = "__TOC__\n"; 
				$strChecklistHeader = '';
				$strChecklistContent = '';
				foreach ($arrayLines as $strLine)
				{
					if ($strChecklistHeader == '')
					{
						if (StringSection($strLine,0,1) != '=')
						{
							$strContent .= $strLine;
						}
						else
						{
							$strChecklistHeader = $strLine; 
							$strChecklistContent = '';
						}
					}
					else // checklist header is set
					{
						if (StringSection($strLine,0,1) != '=')
						{
							$strChecklistContent .= $strLine . "\n";
						}
						else
						{
							$strContent .= $strChecklistHeader . "\n";
							$strContent .= $this->MakeAddChecklistForm(
								ReplaceString($strChecklistHeader,'=',''),
								$strChecklistContent);
							$strContent .= "\n";
							$strContent .= $strChecklistContent;
							$strChecklistHeader = $strLine;
							$strChecklistContent = '';
						}
					}
				}
				if ($strChecklistHeader != '')
				{
					$strContent .= $strChecklistHeader . "\n";
					$strContent .= $this->MakeAddChecklistForm(
						ReplaceString($strChecklistHeader,'=',''),
						$strChecklistContent);
					$strContent .= "\n";
					$strContent .= $strChecklistContent;
				}
			}

			$wiki = new CWikiElement($this->GetWebSite(),'default');
			$this->SetOutputItem(
				'WIKI',
				$wiki->RenderWiki($strContent));
					
			return 'view';
		}
		
		
		function StateSubmit()
		{	
			$this->Trace('CChecklistWikiPadModule::StateSubmit');
			
			$strModuleVarNamePrefix = $this->GetWebsite()->GetPlugin('checklisthq')->GetContextID();
			
			$arrayParams = array(); 
			$arrayParams[$this->GetModuleVarName('state')] = 'view';
			$arrayParams[$this->GetModuleVarName('action')] = '';
			$strUrl = $this->GetWebsite()->MakeURL($arrayParams);
			$this->GetWebsite()->Redirect($strUrl);
			
			return parent::StateSubmit();
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
			$nLineToAddPriorityLow = GetIntegerValue($this->GetModuleVar('line_priority_low'));
			$nLineToAddPriorityMedium = GetIntegerValue($this->GetModuleVar('line_priority_medium'));
			$nLineToAddPriorityHigh = GetIntegerValue($this->GetModuleVar('line_priority_high'));
			$strLineToModifyHash = $this->GetModuleVar('linehash');
			$strAction = $this->GetModuleVar('action');
			$strLineText = $this->GetModuleVar('text');
			$strSubmit = $this->GetParam('submit');
			

			$this->SetModuleVar('line','');
			$this->SetModuleVar('line_priority_low','');
			$this->SetModuleVar('line_priority_medium','');
			$this->SetModuleVar('line_priority_high','');
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
				if ($nLineToModify > $nMaxLine
					|| $nLineToAddPriorityLow > $nMaxLine
					|| $nLineToAddPriorityMedium > $nMaxLine
					|| $nLineToAddPriorityHigh > $nMaxLine)		
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
					
					$nLineToAdd = $nLineToAddPriorityLow;
					if ($strSubmit == 'add_medium')
					{
						$nLineToAdd = $nLineToAddPriorityMedium;
					}
					if ($strSubmit == 'add_high')
					{
						$nLineToAdd = $nLineToAddPriorityHigh;
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


			// Clean up content
			$strContent = ReplaceString($strContent,"\r\n","\n");  
			$strContent = StringTrim($strContent);
			$strContent = ReplaceString($strContent,"\n----\n----\n","\n----\n");    
			$strContent = ReplaceString($strContent,"=\n----\n","=\n");

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
					//$arrayParams[$strModuleVarNamePrefix.'line_priority_low'] = '0';
					$arrayParams[$strModuleVarNamePrefix.'text'] = $strLineToBeMovedToNextActionOtherContext; 
					$arrayParams['content'] = 'tasks'; 
					$arrayParams['checklisthq_context'] = ''.$nOtherContext; 

					$strUrl = $this->GetWebsite()->MakeURL($arrayParams);
					$this->GetWebsite()->Redirect($strUrl);
				}
			}
			
			return $this->StateView();
		}
		
		function StateSubmitChecklist()
		{	
			$this->Trace('CChecklistWikiPadModule::StateSubmitChecklist');
			
			$strChecklistTarget = $this->GetModuleConfig('checklists');
			if ($strChecklistTarget == '')
			{
				$this->SetErrorMessageCode('CHECKLIST_TARGET_NOT_SPECIFIED'); 
				$this->SetState('');
				return $this->StateView();
			}
			
			
			
			$strChecklistTitle = $this->WikiDecode($this->GetModuleVar('checklist_title'));
			$strChecklistContent = $this->WikiDecode($this->GetModuleVar('checklist_content'));
			$strChecklistPrefix = $this->GetModuleVar('checklist_prefix');
			
			if ($strChecklistContent == '')
			{
				$this->SetErrorMessageCode('CHECKLIST_EMPTY'); 
				$this->SetState('');
				return $this->StateView();			
			}
			
			if ($strChecklistPrefix != '')
			{
				$aLines = StringExplode(StringTrim($strChecklistContent),"\n");
				$strChecklistContent = '';
				foreach ($aLines as $strLine)
				{
					if (FindString($strLine,'* ') == 0)
					{
						$strLine = ReplaceString($strLine,'* ', '* '.$strChecklistPrefix.' ');
					}
					else
					{
						$strLine = '* ' . $strChecklistPrefix . ' ' . $strLine;
					}
					$strChecklistContent .= $strLine . "\n";
				}
			}
			

			$strTaskList = $this->GetData('wikipad_' . $strChecklistTarget);
			
			$strNextAction = '=NEXT ACTION=';
			if (FindString($strTaskList,$strNextAction) != -1)
			{
				$strTaskList = ReplaceString($strTaskList,$strNextAction, $strNextAction . "\n".  $strChecklistContent . "\n");
			}
			else
			{
				$strTaskList = $strNextAction . "\n" . $strChecklistContent . "\n" . $strTaskList;
			}


			$this->SetData('wikipad_' . $strChecklistTarget,$strTaskList);

			$this->SetOutputItem('CHECKLIST_ADDED',$strChecklistTitle);
			$this->SetInfoMessageCode('CHECKLIST_ADDED'); 
			$this->SetState('');
			
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

