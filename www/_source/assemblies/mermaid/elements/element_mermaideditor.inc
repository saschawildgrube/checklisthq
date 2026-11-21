<?php

	require_once(GetWDKDir().'wdk_element_react.inc');

	class CMermaidEditorElement extends CReactElement
	{
		function __construct(CWebSite $website, $strStyleID = 'default')
		{
			parent::__construct($website, 'mermaideditor', $strStyleID, false, false);
			$this->AddJavaScript('mermaid');
			$this->AddJavaScript('react-mermaid');
			$this->AddJavaScript('react-mermaidviewer');
			$this->AddJavaScript('react-mermaideditor');
			$this->AddStyleSheet('mermaideditor');
			$this->GetWebsite()->ActivateFontAwesome4();
		}

		function RenderMermaidEditor($arrayProps = array())
		{
			$strID = 'mermaideditor_'.MakeNumericToken(10);
			$this->AddJavaScriptReactRenderByID('MermaidEditor', $strID, $arrayProps);
			return '<div id="'.$strID.'" class="wdk-mermaideditor"></div>';
		}
	}
