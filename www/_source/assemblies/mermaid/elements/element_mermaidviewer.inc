<?php

	require_once(GetWDKDir().'wdk_element_react.inc');
	require_once(GetAssemblyDir('mermaid').'mermaid.inc');

	class CMermaidViewerElement extends CReactElement
	{
		function __construct(CWebSite $website, $strStyleID = 'default')
		{
			parent::__construct($website, 'mermaidviewer', $strStyleID, false, false);
			ActivateMermaid($this->GetWebsite());
			$this->AddJavaScript('react-mermaid');
			$this->AddJavaScript('react-mermaidviewer');
			$this->AddStyleSheet('mermaidviewer');
		}

		function RenderMermaidViewer($strCode)
		{
			$strID = 'mermaidviewer_'.MakeNumericToken(10);
			$this->AddJavaScriptReactRenderByID('MermaidViewer', $strID, array('source' => $strCode));
			return '<div id="'.$strID.'" class="wdk-mermaidviewer"></div>';
		}
	}
