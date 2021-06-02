<?php


	function TransformListToTree($arrayList, $strLevelKey = 'level', $strChildrenKey = 'children', $nRootLevel = 0, $nStartIndex = 0)
	{
		if (IsArray($arrayList) == false)
		{
			return [];	
		}
		if ($strLevelKey == '')
		{
			$strLevelKey = 'level';
		}
		if ($strChildrenKey == '')
		{
			$strChildrenKey = 'children';
		}
		
		
		
		$arrayTree = [];
		$nLevel = $nRootLevel;
		for ($nIndex = $nStartIndex; $nIndex < ArrayCount($arrayList) && $nLevel >= $nRootLevel; $nIndex++)
		{
			$arrayItem = $arrayList[$nIndex];
			$nLevel = GetIntegerValue(ArrayGetValue($arrayItem,$strLevelKey));
			if ($nLevel == $nRootLevel)
			{
				$arrayChildren = TransformListToTree($arrayList,$strLevelKey,$strChildrenKey,$nLevel+1, $nIndex+1);
				if (ArrayCount($arrayChildren) > 0)
				{
					$arrayItem[$strChildrenKey] = $arrayChildren;
				}
				$arrayTree[] = $arrayItem;
			}
		}
		
		return $arrayTree;	
	
	}


