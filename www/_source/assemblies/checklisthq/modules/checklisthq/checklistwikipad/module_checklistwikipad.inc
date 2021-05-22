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
		
		

		function OnRenderModule($strState)
		{
			if ($this->IsViewOnly() == true)
			{
				$strState = 'view';	
			}
			
			switch ($strState)
			{
				case 'submit_line_delete':
				{
					return $this->StateSubmitLineDelete();	
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
			
			$this->PrepareTabNav('view');


			$strContent = $this->GetData($this->GetDataName());
			if ($strContent === false)
			{
				$this->SetErrorMessageCode('WIKIPAD_GET');
				$this->SetOutputItem('WIKI','');
				$strContent = '';
				return parent::StateView();
				
			}
			$arrayLines = StringExplode($strContent,"\n",true);
			
			$linkicon = new CLinkIconElement($this->GetWebsite(),'default');
			$strContent = '';
			$nLine = 0;
			foreach ($arrayLines as $strLine)
			{
				$strHtmlLinkLineDelete = '';
				if (	StringSection($strLine,0,1) == '*'
					||	StringSection($strLine,0,1) == '#')
				{
					$arrayParams = array(
						$this->GetModuleVarName('line') => $nLine,
						$this->GetModuleVarNameState() => 'submit_line_delete'
						);
					$strUrlLineDelete = $this->GetWebsite()->MakeURL($arrayParams);
					$strHtmlLinkLineDelete = '&nbsp;'.$linkicon->RenderLinkIcon(
						$strUrlLineDelete,
						'check-square-o',
						'?TID_WIKIPAD_TOOL_DELETELINE?');
				}
				$strContent .=
					$strLine
					.$strHtmlLinkLineDelete
					//.'<!-- Line: '.$nLine.'-->'
					."\n";
				$nLine++;
			}

			$wiki = new CWikiElement($this->GetWebSite(),'default');
			$this->SetOutputItem(
				'WIKI',
				$wiki->RenderWiki(
					$strContent));
	
			return 'view';
		}
		
	
		function StateSubmitLineDelete()
		{
			$this->Trace('StateSubmitLineDelete');
	
			$strContent = $this->GetData($this->GetDataName());
			if ($strContent === false)
			{
				$this->SetErrorMessageCode('WIKIPAD_GET');
				$this->SetOutputItem('WIKI','');
				$strContent = '';
				return $this->StateView();
			}

			$nLineToDelete = GetIntegerValue($this->GetModuleVar('line'));
			
			if ($nLineToDelete < 0)
			{
				$this->Trace('Line is < 0');
				return $this->StateView();
			}
			
			$arrayLines = StringExplode($strContent,"\n",true);
			if ($nLineToDelete+1 > ArrayCount($arrayLines))		
			{
				$this->Trace('Line+1 is > number of lines');
				return $this->StateView();
			}
				
			$this->SetOutputItem('LINE_DELETED','<pre>'.$arrayLines[$nLineToDelete].'</pre>');
			$this->SetInfoMessageCode('LINEDELETED');
			
			$strContent = '';
			$nLine = 0;
			foreach ($arrayLines as $strLine)
			{
				if ($nLine != $nLineToDelete)
				{
					$strContent .=
						$strLine
						."\n";
				}
				$nLine++;
			}

			$this->SetModuleVar('line','');
					
			if ($this->SetData($this->GetDataName(),$strContent) == false)
			{
				$this->SetErrorMessageCode('WIKIPAD_SET');
			}
			
			return $this->StateView();
		}

	}

