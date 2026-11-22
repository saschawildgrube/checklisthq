$this->IncludeElement("staticfileviewer");

$strSourceCodeFile = $this->GetWebsite()->GetAssemblyLine()->GetElementDemoCodeFilePath("staticfileviewer");
$strText = FileRead($strSourceCodeFile);

$staticfileviewer = new CStaticFileViewerElement($this->GetWebsite(),"default");
$strOutput = $staticfileviewer->RenderStaticFileViewer($strText);
