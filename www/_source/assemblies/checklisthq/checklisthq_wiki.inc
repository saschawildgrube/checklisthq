<?php


	require_once(GetWDKDir().'wdk_wiki.inc');
	require_once(GetWDKDir().'wdk_datastructures.inc');


	function GetLevelFromWikiLine($strLine)
	{
		$strLine = StringTranslateCharacters($strLine,'#','*');
		if (FindString($strLine,'******') == 0)
		{
			return 16;  
		}
		if (FindString($strLine,'*****') == 0)
		{
			return 15;
		}
		if (FindString($strLine,'****') == 0)
		{
			return 14;
		}
		if (FindString($strLine,'***') == 0)
		{
			return 13;
		}
		if (FindString($strLine,'**') == 0)
		{
			return 12;
		}
		if (FindString($strLine,'*') == 0)
		{
			return 11;
		}
		if (FindString($strLine,'======') == 0)
		{
			return 6;
		}
		if (FindString($strLine,'=====') == 0)
		{
			return 5;
		}
		if (FindString($strLine,'====') == 0)
		{
			return 4;
		}
		if (FindString($strLine,'===') == 0)
		{
			return 3;
		}
		if (FindString($strLine,'==') == 0)
		{
			return 2;
		}
		if (FindString($strLine,'=') == 0)
		{
			return 1;
		}
		return 0;
	}




	function MakeTreeDataFromWiki($strWiki)
	{
		$arrayWiki = ParseWiki($strWiki);

		$arrayWikiLines = ArrayGetValue($arrayWiki,'lines');

		$arrayItems = [];
		foreach ($arrayWikiLines as $arrayLine)
		{
			$arrayItem = [];
			$strLine = ArrayGetValue($arrayLine,'raw');
			$nLevel = GetLevelFromWikiLine($strLine);
			if ($nLevel > 0)
			{
				$arrayItem['level'] = $nLevel - 1;
				$strLabel = StringTrim(StringRemoveCharacters($strLine,'=*'));
				$arrayItem['label'] = $strLabel;
				$arrayItems[] = $arrayItem;
			}
		}
	
		// Correct levels
		$arrayItems = AlignListLevels($arrayItems,'level');
		
		// Transform to tree data structure
		$arrayTree = TransformListToTree($arrayItems,'level','children');

		// Add the value for the zoomable sunburst to work with
		function CallbackMapSetValue($key,$value,$arrayParams)
		{
			if (IsArray($value))
			{
				if (ArrayKeyExists($value,'label')) 
				{
					if (ArrayCount(ArrayGetValue($value,'children')) == 0)
					{
						$value['value'] = 10;
					}
					else
					{
						$value['children'] = ArrayMap($value['children'],'CallbackMapSetValue',[]);
					}	
				}
				else
				{
					return ArrayMap($value,'CallbackMapSetValue',[]);
				}
			}
			return $value;
		}
		$arrayTree = ArrayMap($arrayTree,'CallbackMapSetValue',[]);


		// Put the tree into a root node
		$arrayData = [];
		$arrayData['label'] = 'root';
		$arrayData['children'] = $arrayTree;

		return $arrayData;
	}
