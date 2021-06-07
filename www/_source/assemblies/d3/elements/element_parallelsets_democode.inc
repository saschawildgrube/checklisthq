$arrayDimensions = array(
	'Survived',
	'Sex',
	'Age',
	'Class');
$arrayData = FileReadCSV(
	$this->GetWebsite()->GetDataFilePath('parallelsets.csv'));
	
$this->IncludeElement('parallelsets');
$parallelsets = new CParallelSetsElement(
	$this->GetWebsite(),'default');
$strOutput = $parallelsets->RenderParallelSets(
	$arrayDimensions,$arrayData,'Count');