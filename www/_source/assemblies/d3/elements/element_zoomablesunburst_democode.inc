$this->IncludeElement('zoomablesunburst');

$zoomablesunburst = new CZoomableSunburstElement($this->GetWebsite(),'default');

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
$arrayData['label'] = 'WDK';
$arrayData['children'] = TransformListToTree($arrayList,'level');

$strOutput = $zoomablesunburst->RenderZoomableSunburst($arrayData);