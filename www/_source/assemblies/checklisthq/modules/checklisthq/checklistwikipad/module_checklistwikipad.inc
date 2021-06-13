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
	
		function MakeAddTaskForm($nLine)
		{
			$this->m_formDef = new CFormDefinition();

			$arrayParams = array(); 
			$arrayParams[$this->GetModuleVarNameState()] = 'submitline';
			$arrayParams[$this->GetModuleVarName('action')] = 'add';
			$arrayParams[$this->GetModuleVarName('line')] = ''.$nLine;
			$this->m_formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$this->m_formDef->AddControlText('TEXT',$this->GetModuleVarName('text'),'','',60,200); 
			$this->m_formDef->AddControlButtonSubmit('?TID_WIKIPAD_BUTTON_ADD?');

			$form = new CFormElement($this->GetWebsite(),'oneline');
			return StringRemoveControlChars($form->RenderForm($this->m_formDef,array()));
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
				
				$bAddTaskFormAdded = false;
				$linkicon = new CLinkIconElement($this->GetWebsite(),'default');
				$strContent = '';
				$nLine = 0;
				$nLineCount = ArrayCount($arrayLines);
				$strSection = 'nextaction'; // the default
				foreach ($arrayLines as $strLine)
				{
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
					
					$strHtmlFormAddTask = '';
					$strHtmlLinkLineDelete = '';
					$strHtmlLinkLineMoveDown = '';
					$strHtmlLinkLineMoveToNextAction = ''; 
					$strHtmlLinkLineMoveToWaitingFor = '';
					
					
					if (StringSection($strLine,0,1) == '=')
					{
						$strHtmlFormAddTask = "\n".$this->MakeAddTaskForm($nLine+1);
						$bAddTaskFormAdded = true;
					}
					
					if (	StringSection($strLine,0,1) == '*'
						||	StringSection($strLine,0,1) == '#')
					{
						
						$strLineHash = MakeHash($strLine);

						if ($nLine < $nLineCount - 1)
						{
							$arrayParams = array(
								$this->GetModuleVarName('line') => $nLine,
								$this->GetModuleVarName('linehash') => $strLineHash,
								$this->GetModuleVarName('action') => 'movedown',
								$this->GetModuleVarNameState() => 'submitline'
								);
							$strHtmlLinkLineMoveDown = '&nbsp;'.$linkicon->RenderLinkIcon(
								$this->GetWebsite()->MakeURL($arrayParams),
								'hand-o-down',
								'?TID_WIKIPAD_TOOL_LINEMOVEDOWN?');
						}

						$arrayParams = array(
							$this->GetModuleVarName('line') => $nLine,
							$this->GetModuleVarName('linehash') => $strLineHash,
							$this->GetModuleVarName('action') => 'delete',
							$this->GetModuleVarNameState() => 'submitline'
							);
						$strHtmlLinkLineDelete = '&nbsp;'.$linkicon->RenderLinkIcon(
							$this->GetWebsite()->MakeURL($arrayParams),
							'check-square-o',
							'?TID_WIKIPAD_TOOL_LINEDELETE?');

						if ($strSection != 'waitingfor')
						{
							$arrayParams = array(
								$this->GetModuleVarName('line') => $nLine,
								$this->GetModuleVarName('linehash') => $strLineHash,
								$this->GetModuleVarName('action') => 'movetowaitingfor',
								$this->GetModuleVarNameState() => 'submitline'
								);
							$strHtmlLinkLineMoveToWaitingFor = '&nbsp;'.$linkicon->RenderLinkIcon(
								$this->GetWebsite()->MakeURL($arrayParams),
								'hourglass-start',
								'?TID_WIKIPAD_TOOL_LINEMOVETOWAITINGFOR?');
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
								'?TID_WIKIPAD_TOOL_LINEMOVETONEXTACTION?');
						}


/*
						$arrayParams = array(
							$this->GetModuleVarName('line') => $nLine,
							$this->GetModuleVarName('action') => 'movetop',
							$this->GetModuleVarNameState() => 'submitline'
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
						.$strHtmlFormAddTask
						.$strHtmlLinkLineMoveToWaitingFor
						.$strHtmlLinkLineMoveToNextAction
						.$strHtmlLinkLineMoveDown
						.$strHtmlLinkLineDelete
						."\n";
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
				$wiki->RenderWiki(
					$strContent));
	
			return 'view';
		}
		
	
		function StateSubmitLine()
		{
			$this->Trace('StateSubmitLine');
			$strContent = $this->GetData($this->GetDataName());
			if ($strContent === false)
			{
				$this->SetErrorMessageCode('WIKIPAD_GET');
				$this->SetOutputItem('WIKI','');
				$strContent = '';
				return $this->StateView();
			}

			$nLineToModify = GetIntegerValue($this->GetModuleVar('line'));
			$strLineToModifyHash = $this->GetModuleVar('linehash');
			$strAction = $this->GetModuleVar('action');
			$strLineText = $this->GetModuleVar('text');
	
			$this->SetModuleVar('line','');
			$this->SetModuleVar('linehash','');
			$this->SetModuleVar('action','');
			$this->SetModuleVar('text','');
						
			if ($strAction != '')
			{
			
				if ($nLineToModify < 0)
				{
					$this->Trace('Line is < 0');
					return $this->StateView();
				}
				
				$arrayLines = StringExplode($strContent,"\n",true);
				
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
				
				$bHasNextAction = (FindString($strContent,'=NEXT ACTION=') != -1)?(true):(false);
				$bHasWaitingFor = (FindString($strContent,'=WAITING FOR=') != -1)?(true):(false);
				
			
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
						$this->Trace($strLine);
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
					if (StringTrim($strLineText) == '')
					{
						return $this->StateView();
					}
					
					if (	StringSection($strLineText,0,1) != '*'
						&&	StringSection($strLineText,0,1) != '#')
					{
						$strLineText = '* '.$strLineText;
					}
					
					if ($nLineToModify == 0)
					{
							$strContent =
								$strLineText
								."\n"
								. $strContent;
						
					}
					else
					{
						$this->Trace($arrayLines);
						$strContent = '';
						$nLine = 0;
						foreach ($arrayLines as $strLine)
						{
							$this->Trace($nLine.' / '.$nLineToModify);
							$this->Trace($strLine);
							$strContent .=
								$strLine  
								."\n";
							if ($nLine == ($nLineToModify - 1))  
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
			}

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

