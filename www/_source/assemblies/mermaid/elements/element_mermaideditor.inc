<?php

	require_once(GetWDKDir().'wdk_element_react.inc');
	require_once(GetAssemblyDir('mermaid').'mermaid.inc');

	class CMermaidEditorElement extends CReactElement
	{
		function __construct(CWebSite $website, $strStyleID = 'default')
		{
			parent::__construct($website, 'mermaideditor', $strStyleID, false, false);
			ActivateMermaid($this->GetWebsite());
			$this->AddJavaScript('react-mermaid');
			$this->AddJavaScript('react-mermaidviewer');
			$this->AddJavaScript('react-mermaideditor');
			$this->AddStyleSheet('mermaideditor');
			$this->GetWebsite()->ActivateFontAwesome4();
		}

		function RenderMermaidEditor($strCode)
		{
			$strID = 'mermaideditor_'.MakeNumericToken(10);
			$this->AddJavaScriptReactRenderByID('MermaidEditor', $strID, array('source' => $strCode));
			return '<div id="'.$strID.'" class="wdk-mermaideditor"></div>';
		}
	}
