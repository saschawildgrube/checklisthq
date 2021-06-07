$arrayData = FileReadCSV(
	$this->GetWebsite()->GetDataFilePath('sankey.csv'));

$this->IncludeElement('sankey');
$sankey = new CSankeyElement($this->GetWebsite(),'default');
$strOutput = $sankey->RenderSankey($arrayData,'TWh');