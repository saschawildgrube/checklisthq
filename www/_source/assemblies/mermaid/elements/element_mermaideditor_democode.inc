$this->IncludeElement('mermaideditor');
$mermaideditor = new CMermaidEditorElement($this->GetWebsite(), 'default');

$strCode = <<<'MERMAID'
flowchart
    Start["Receive request"] --> Review{"Approved?"}
    Review -->|Yes| Process["Process request"]
    Review -->|No| Revise["Revise request"]
    Revise --> Review
    Process --> Finish["Done"]
MERMAID;

$strOutput = $mermaideditor->RenderMermaidEditor($strCode);
