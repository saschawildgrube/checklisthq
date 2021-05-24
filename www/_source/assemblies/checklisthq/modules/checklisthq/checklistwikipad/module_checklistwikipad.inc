<?php

	require_once(GetWDKDir().'modules/cms/wikipad/module_wikipad.inc');

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
		
		function OnIsToolVisible($strToolID)
		{
			if ($strToolID == 'view')
			{
				if ($this->GetCOnfigIsHideView() == true)
				{
					return false;
				}
			}
			return parent::OnIsToolVisible($strToolID);	
		}
		

		function OnRenderModule($strState)
		{
			if ($this->IsViewOnly() == true)
			{
				$strState = 'view';	
			}
			
			switch ($strState)
			{
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
			
			if ($this->GetConfigIsHideView())
			{
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
				foreach ($arrayLines as $strLine)
				{
					$strHtmlLinkLineDelete = '';
					$strHtmlLinkLineMoveDown = '';
					
					if (	StringSection($strLine,0,1) == '*'
						||	StringSection($strLine,0,1) == '#')
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
			}

			$this->SetModuleVar('line','');
			$this->SetModuleVar('action','');
					

			if ($this->SetData($this->GetDataName(),$strContent) == false)
			{
				$this->SetErrorMessageCode('WIKIPAD_SET');
			}
			
			return $this->StateView();
		}

	}

