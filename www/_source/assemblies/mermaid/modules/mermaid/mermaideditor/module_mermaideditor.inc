<?php

	require_once(GetWDKDir().'wdk_module.inc');

	class CMermaidEditorModule extends CModule
	{
		function __construct(CWebSite $website, $strInstanceID = '', $arrayConfig = array())
		{
			parent::__construct($website, 'mermaid/mermaideditor', $strInstanceID, $arrayConfig);
		}

		function OnRenderModule($strState)
		{
			$this->IncludeElement('mermaideditor');
			$editor = new CMermaidEditorElement($this->GetWebsite(), 'default');
			$this->SetOutputItem('EDITOR', $editor->RenderMermaidEditor(array('source' => (string)$this->GetModuleConfig('source'))));
			return 'start';
		}
	}
