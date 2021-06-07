$strRootPath = GetWDKDir().'modules/';
$arrayFileObjects = ReadDirectory($strRootPath,true);
$arrayList = [];
foreach ($arrayFileObjects as $strPath)
{
	$strFullPath = $strRootPath.$strPath;
	$arrayListItem = [];
	$strLevel = StringFilter($strPath,'/');
	$nLevel = StringLength($strLevel);
	if (IsFile($strFullPath))
	{
		$arrayListItem['label'] = GetFileTitleFromPath($strFullPath);
		$arrayListItem['value'] = GetFileSize($strFullPath,true);
	}
	if (IsDirectory($strFullPath))
	{
		$nLevel--;	
		$arrayListItem['label'] = GetFolderNameFromPath($strFullPath);
	}
	$arrayListItem['level'] = $nLevel;	
	$arrayList[] = $arrayListItem;
}
require_once(GetWDKDir().'wdk_datastructures.inc');
$arrayData = [];
$arrayData['label'] = 'wdk modules'; 
$arrayData['children'] = TransformListToTree($arrayList,'level');

$arrayConfig = [];
$arrayConfig['hidevalue'] = false;
$arrayConfig['sort'] = 'label';  // or 'value' or ''

$this->IncludeElement('zoomablesunburst');
$zoomablesunburst = new CZoomableSunburstElement($this->GetWebsite(),'default');
$strOutput = $zoomablesunburst->RenderZoomableSunburst($arrayData,$arrayConfig);