$this->IncludeElement('jsoneditor');

$jsoneditor = new CJsonEditorElement($this->GetWebsite(), 'default');
$strOutput = $jsoneditor->RenderJsonEditor();
