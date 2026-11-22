$this->IncludeElement('mermaidviewer');
$mermaidviewer = new CMermaidViewerElement($this->GetWebsite(), 'default');

$strCode = <<<'MERMAID'
flowchart
    Start["Receive request"] --> Review{"Approved?"}
    Review -->|Yes| Process["Process request"]
    Review -->|No| Revise["Revise request"]
    Revise --> Review
    Process --> Finish["Done"]
MERMAID;

$strOutput = $mermaidviewer->RenderMermaidViewer($strCode);
