$this->IncludeElement("tableofcontents");
$this->IncludeElement("header");
$this->IncludeElement("paragraph");

$tableofcontents = new CTableOfContentsElement($this->GetWebsite(),"default");
$header = new CHeaderElement($this->GetWebsite(),"default");
$paragraph = new CParagraphElement($this->GetWebsite(),"default");

$strOutput = $tableofcontents->RenderTableOfContents();
$strOutput .= $header->RenderHeader("Introduction",2);
$strOutput .= $paragraph->RenderParagraph("The table of contents automatically links to the headings below it.");
$strOutput .= $header->RenderHeader("Getting Started",2);
$strOutput .= $paragraph->RenderParagraph("Select an entry in the table of contents to jump to its section.");
$strOutput .= $header->RenderHeader("First Steps",3);
$strOutput .= $paragraph->RenderParagraph("Subheadings appear as nested entries in the table of contents.");
$strOutput .= $header->RenderHeader("Summary",2);
$strOutput .= $paragraph->RenderParagraph("Use heading levels to organize the sections of your page.");
