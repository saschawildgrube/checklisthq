$arrayLegends = array(
'Strongly disagree',
'Disagree',
'Neither agree nor disagree',
'Agree',
'Strongly agree');
$strCSV =
'Question,1,2,3,4,5
Question 1,24,294,594,1927,376
Question 2,2,2,0,7,0
Question 3,2,0,2,4,2
Question 4,0,2,1,7,6
Question 5,0,1,3,16,4
Question 6,1,1,2,9,3
Question 7,0,0,1,4,0
Question 8,0,0,0,0,2';
$arrayData = ParseCSV($strCSV);

$this->IncludeElement('divergingstackedbarchart');
$divergingstackedbarchart = new CDivergingStackedBarChartElement($this->GetWebsite(),'default');
$strOutput = $divergingstackedbarchart->RenderDivergingStackedBarChart($arrayData,$arrayLegends);