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
		
		function GetConfigIsHideView()
		{
			return $this->GetModuleConfigBoolean('hideview');	
		}		
		
		function GetConfigIsNetworkGraph()
		{
			return $this->GetModuleConfigBoolean('networkgraph');	
		}			
		function GetConfigIsZoomableSunburst()
		{
			return $this->GetModuleConfigBoolean('zoomablesunburst');	
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
			if ($strToolID == 'networkgraph')
			{
				if ($this->GetConfigIsNetworkGraph() == true)
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
			return parent::OnIsToolVisible($strToolID);	
		}
		
		
		function PrepareTabNav($strState)
		{
			if ($this->IsViewOnly() == true)
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
			
			if ($this->IsToolVisible('networkgraph') == true)
			{

				$arrayParams = array();
	   		$arrayParams[$this->GetModuleVarNameState()] = 'networkgraph';
	   		$tabnavigationDef->AddTab(
	      		'networkgraph',
	      		'?TID_WIKIPAD_TAB_NETWORKGRAPH?',
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
			
			if ($strState == '' && $this->GetConfigIsNetworkGraph() == true)
			{
				$strState = 'networkgraph';	
			}
			if ($strState == '' && $this->GetConfigIsZoomableSunburst() == true)
			{
				$strState = 'zoomablesunburst';	
			}
			
			switch ($strState)
			{
				case 'networkgraph':
				{
					return $this->StateNetworkGraph();	
				}
				case 'zoomablesunburst':
				{
					return $this->StateZoomableSunburst();	
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

			$this->Trace('StateView');
			
			if ($this->GetConfigIsHideView() == true)
			{
				if ($this->GetConfigIsNetworkgraph() == true)
				{
					return $this->StateNetworkgraph();	
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
		
		
		function StateNetworkGraph()
		{
			$this->Trace('StateNetworkgraph');
			
			$this->PrepareTabNav('networkgraph');
			
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

			// Derive links		

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
			
			// Render the graph
		
			$arrayData = array();	
			$arrayData['nodes'] = $arrayNodes;
			$arrayData['links'] = $arrayLinks;
			
			$this->IncludeElement('network');
			$network = new CNetworkElement($this->GetWebsite(),'default');

			$this->SetOutputItem('NETWORKGRAPH',$network->RenderNetwork($arrayData));	
			
			return 'networkgraph';
		}
		
		
		function StateZoomableSunburst()
		{
			$this->Trace('StateZoomableSunburst');
			
			$this->PrepareTabNav('zoomablesunburst');		
			
			$strContent = $this->GetData($this->GetDataName());	
			
			//$this->Debug(ParseWiki($strContent));
			
			$arrayData = MakeTreeDataFromWiki($strContent);
				
			$this->Debug($arrayData);
				
			$this->IncludeElement('zoomablesunburst');
			$zoomablesunburst = new CZoomableSunBurstElement($this->GetWebsite(),'default');
			$this->SetOutputItem('ZOOMABLESUNBURST',$zoomablesunburst->RenderZoomableSunburst($arrayData));

			return 'zoomablesunburst';
		}


	}

