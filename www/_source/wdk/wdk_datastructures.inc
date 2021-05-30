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
/*
		foreach ($arrayInput as $arrayItem)
		{
			$nLevel = GetIntegerValue(ArrayGetValue($arrayItem,$strLevelKey));
			if ($nLevel == 0)
			{
				$arrayTree[] = $arrayItem;
			}
		}
*/
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
		
				// Create tree structure
	
	/*
		$arrayChildren[] = array(
			'label' => $strLabel,
			//'children' => []
			'value' => 100);
*/

/*
		function TreeDataMakeNode($arrayItems,$nIndex)
		{
			$arrayNode = [];
			
			if ($nIndex >= ArrayCount($arrayItems))
			{
				return false;	
			}
			
			$arrayItem = $arrayItems[$nIndex];
			
			$nLevel = ArrayGetValue($arrayItem,'level_corrected');
			
			$arrayNode['label'] = ArrayGetValue($arrayItem,'level_corrected');
			
			
			return $arrayNode
		}
*/


/*

		$arrayStack = [];
		$nStackLevel = 1;
		foreach ($arrayItems as $arrayItem2)
		{
			$nLevel = ArrayGetValue($arrayItem2,'level_corrected');
			$nLabel = ArrayGetValue($arrayItem2,'label');
			if ($nLevel > $nStackLevel)
			{
				$nStackLevel++;
			}
			if ($nLevel == $nStackLevel)
			{
				$arrayNode = [];
				$arrayNode['label'] = $strLabel;
				$arrayNode['value'] = 1;
				$arrayStack[$nStackLevel][] = $arrayNode;				
			}
			else
			{
				for ($nStackIndex = $nStackLevel; $nStackIndex >= $nLevel; $nStackIndex--)
				{
						
				}	
			}
			
			if ($nLevel > $nStackLevel)
			{
				$nStackLevel++;
			}
			if ($nLevel >= $nStackLevel)
			{
				$arrayStack[$nStackLevel][] = array(
					'label' =>
						ArrayGetValue($arrayItem2,'label')
						.' ('
						.ArrayGetValue($arrayItem2,'level')
						.' -> '
						.ArrayGetValue($arrayItem2,'level_corrected')
						.')',
					'value' => 1);		
			}
			else
			{
				$arrayStack[$nStackLevel]	
			}
			
			
			$arrayChildren[] = array(
				'label' =>
					ArrayGetValue($arrayItem2,'label')
					.' ('
					.ArrayGetValue($arrayItem2,'level')
					.' -> '
					.ArrayGetValue($arrayItem2,'level_corrected')
					.')',
				'value' => 1);	
		}
	
	*/
	
	/*
		for ($nLevelIndex = 1; $nLevelIndex < 20; $nLevelIndex++)
		{
			$arrayChildren;
			foreach ($arrayItems as $arrayItem2)
			{
				$nLevel = ArrayGetValue($arrayItem2,'level_corrected');
				if ($nLevel == $nLevelIndex)
				{
					$nLabel = ArrayGetValue($arrayItem2,'label');
				}
			}			
			
		}
		*/
		
		
		
		

			//		$strJson = '{ "label": "flare", "children": [  {   "label": "analytics",   "children": [    {     "label": "cluster",     "children": [      {"label": "AgglomerativeCluster", "value": 3938},      {"label": "CommunityStructure", "value": 3812},      {"label": "HierarchicalCluster", "value": 6714},      {"label": "MergeEdge", "value": 743}     ]    },    {     "label": "graph",     "children": [      {"label": "BetweennessCentrality", "value": 3534},      {"label": "LinkDistance", "value": 5731},      {"label": "MaxFlowMinCut", "value": 7840},      {"label": "ShortestPaths", "value": 5914},      {"label": "SpanningTree", "value": 3416}     ]    },    {     "label": "optimization",     "children": [      {"label": "AspectRatioBanker", "value": 7074}     ]    }   ]  },  {   "label": "animate",   "children": [    {"label": "Easing", "value": 17010},    {"label": "FunctionSequence", "value": 5842},    {     "label": "interpolate",     "children": [      {"label": "ArrayInterpolator", "value": 1983},      {"label": "ColorInterpolator", "value": 2047},      {"label": "DateInterpolator", "value": 1375},      {"label": "Interpolator", "value": 8746},      {"label": "MatrixInterpolator", "value": 2202},      {"label": "NumberInterpolator", "value": 1382},      {"label": "ObjectInterpolator", "value": 1629},      {"label": "PointInterpolator", "value": 1675},      {"label": "RectangleInterpolator", "value": 2042}     ]    },    {"label": "ISchedulable", "value": 1041},    {"label": "Parallel", "value": 5176},    {"label": "Pause", "value": 449},    {"label": "Scheduler", "value": 5593},    {"label": "Sequence", "value": 5534},    {"label": "Transition", "value": 9201},    {"label": "Transitioner", "value": 19975},    {"label": "TransitionEvent", "value": 1116},    {"label": "Tween", "value": 6006}   ]  },  {   "label": "data",   "children": [    {     "label": "converters",     "children": [      {"label": "Converters", "value": 721},      {"label": "DelimitedTextConverter", "value": 4294},      {"label": "GraphMLConverter", "value": 9800},      {"label": "IDataConverter", "value": 1314},      {"label": "JSONConverter", "value": 2220}     ]    },    {"label": "DataField", "value": 1759},    {"label": "DataSchema", "value": 2165},    {"label": "DataSet", "value": 586},    {"label": "DataSource", "value": 3331},    {"label": "DataTable", "value": 772},    {"label": "DataUtil", "value": 3322}   ]  },  {   "label": "display",   "children": [    {"label": "DirtySprite", "value": 8833},    {"label": "LineSprite", "value": 1732},    {"label": "RectSprite", "value": 3623},    {"label": "TextSprite", "value": 10066}   ]  },  {   "label": "flex",   "children": [    {"label": "FlareVis", "value": 4116}   ]  },  {   "label": "physics",   "children": [    {"label": "DragForce", "value": 1082},    {"label": "GravityForce", "value": 1336},    {"label": "IForce", "value": 319},    {"label": "NBodyForce", "value": 10498},    {"label": "Particle", "value": 2822},    {"label": "Simulation", "value": 9983},    {"label": "Spring", "value": 2213},    {"label": "SpringForce", "value": 1681}   ]  },  {   "label": "query",   "children": [    {"label": "AggregateExpression", "value": 1616},    {"label": "And", "value": 1027},    {"label": "Arithmetic", "value": 3891},    {"label": "Average", "value": 891},    {"label": "BinaryExpression", "value": 2893},    {"label": "Comparison", "value": 5103},    {"label": "CompositeExpression", "value": 3677},    {"label": "Count", "value": 781},    {"label": "DateUtil", "value": 4141},    {"label": "Distinct", "value": 933},    {"label": "Expression", "value": 5130},    {"label": "ExpressionIterator", "value": 3617},    {"label": "Fn", "value": 3240},    {"label": "If", "value": 2732},    {"label": "IsA", "value": 2039},    {"label": "Literal", "value": 1214},    {"label": "Match", "value": 3748},    {"label": "Maximum", "value": 843},    {     "label": "methods",     "children": [      {"label": "add", "value": 593},      {"label": "and", "value": 330},      {"label": "average", "value": 287},      {"label": "count", "value": 277},      {"label": "distinct", "value": 292},      {"label": "div", "value": 595},      {"label": "eq", "value": 594},      {"label": "fn", "value": 460},      {"label": "gt", "value": 603},      {"label": "gte", "value": 625},      {"label": "iff", "value": 748},      {"label": "isa", "value": 461},      {"label": "lt", "value": 597},      {"label": "lte", "value": 619},      {"label": "max", "value": 283},      {"label": "min", "value": 283},      {"label": "mod", "value": 591},      {"label": "mul", "value": 603},      {"label": "neq", "value": 599},      {"label": "not", "value": 386},      {"label": "or", "value": 323},      {"label": "orderby", "value": 307},      {"label": "range", "value": 772},      {"label": "select", "value": 296},      {"label": "stddev", "value": 363},      {"label": "sub", "value": 600},      {"label": "sum", "value": 280},      {"label": "update", "value": 307},      {"label": "variance", "value": 335},      {"label": "where", "value": 299},      {"label": "xor", "value": 354},      {"label": "_", "value": 264}     ]    },    {"label": "Minimum", "value": 843},    {"label": "Not", "value": 1554},    {"label": "Or", "value": 970},    {"label": "Query", "value": 13896},    {"label": "Range", "value": 1594},    {"label": "StringUtil", "value": 4130},    {"label": "Sum", "value": 791},    {"label": "Variable", "value": 1124},    {"label": "Variance", "value": 1876},    {"label": "Xor", "value": 1101}   ]  },  {   "label": "scale",   "children": [    {"label": "IScaleMap", "value": 2105},    {"label": "LinearScale", "value": 1316},    {"label": "LogScale", "value": 3151},    {"label": "OrdinalScale", "value": 3770},    {"label": "QuantileScale", "value": 2435},    {"label": "QuantitativeScale", "value": 4839},    {"label": "RootScale", "value": 1756},    {"label": "Scale", "value": 4268},    {"label": "ScaleType", "value": 1821},    {"label": "TimeScale", "value": 5833}   ]  },  {   "label": "util",   "children": [    {"label": "Arrays", "value": 8258},    {"label": "Colors", "value": 10001},    {"label": "Dates", "value": 8217},    {"label": "Displays", "value": 12555},    {"label": "Filter", "value": 2324},    {"label": "Geometry", "value": 10993},    {     "label": "heap",     "children": [      {"label": "FibonacciHeap", "value": 9354},      {"label": "HeapNode", "value": 1233}     ]    },    {"label": "IEvaluable", "value": 335},    {"label": "IPredicate", "value": 383},    {"label": "IValueProxy", "value": 874},    {     "label": "math",     "children": [      {"label": "DenseMatrix", "value": 3165},      {"label": "IMatrix", "value": 2815},      {"label": "SparseMatrix", "value": 3366}     ]    },    {"label": "Maths", "value": 17705},    {"label": "Orientation", "value": 1486},    {     "label": "palette",     "children": [      {"label": "ColorPalette", "value": 6367},      {"label": "Palette", "value": 1229},      {"label": "ShapePalette", "value": 2059},      {"label": "SizePalette", "value": 2291}     ]    },    {"label": "Property", "value": 5559},    {"label": "Shapes", "value": 19118},    {"label": "Sort", "value": 6887},    {"label": "Stats", "value": 6557},    {"label": "Strings", "value": 22026}   ]  },  {   "label": "vis",   "children": [    {     "label": "axis",     "children": [      {"label": "Axes", "value": 1302},      {"label": "Axis", "value": 24593},      {"label": "AxisGridLine", "value": 652},      {"label": "AxisLabel", "value": 636},      {"label": "CartesianAxes", "value": 6703}     ]    },    {     "label": "controls",     "children": [      {"label": "AnchorControl", "value": 2138},      {"label": "ClickControl", "value": 3824},      {"label": "Control", "value": 1353},      {"label": "ControlList", "value": 4665},      {"label": "DragControl", "value": 2649},      {"label": "ExpandControl", "value": 2832},      {"label": "HoverControl", "value": 4896},      {"label": "IControl", "value": 763},      {"label": "PanZoomControl", "value": 5222},      {"label": "SelectionControl", "value": 7862},      {"label": "TooltipControl", "value": 8435}     ]    },    {     "label": "data",     "children": [      {"label": "Data", "value": 20544},      {"label": "DataList", "value": 19788},      {"label": "DataSprite", "value": 10349},      {"label": "EdgeSprite", "value": 3301},      {"label": "NodeSprite", "value": 19382},      {       "label": "render",       "children": [        {"label": "ArrowType", "value": 698},        {"label": "EdgeRenderer", "value": 5569},        {"label": "IRenderer", "value": 353},        {"label": "ShapeRenderer", "value": 2247}       ]      },      {"label": "ScaleBinding", "value": 11275},      {"label": "Tree", "value": 7147},      {"label": "TreeBuilder", "value": 9930}     ]    },    {     "label": "events",     "children": [      {"label": "DataEvent", "value": 2313},      {"label": "SelectionEvent", "value": 1880},      {"label": "TooltipEvent", "value": 1701},      {"label": "VisualizationEvent", "value": 1117}     ]    },    {     "label": "legend",     "children": [      {"label": "Legend", "value": 20859},      {"label": "LegendItem", "value": 4614},      {"label": "LegendRange", "value": 10530}     ]    },    {     "label": "operator",     "children": [      {       "label": "distortion",       "children": [        {"label": "BifocalDistortion", "value": 4461},        {"label": "Distortion", "value": 6314},        {"label": "FisheyeDistortion", "value": 3444}       ]      },      {       "label": "encoder",       "children": [        {"label": "ColorEncoder", "value": 3179},        {"label": "Encoder", "value": 4060},        {"label": "PropertyEncoder", "value": 4138},        {"label": "ShapeEncoder", "value": 1690},        {"label": "SizeEncoder", "value": 1830}       ]      },      {       "label": "filter",       "children": [        {"label": "FisheyeTreeFilter", "value": 5219},        {"label": "GraphDistanceFilter", "value": 3165},        {"label": "VisibilityFilter", "value": 3509}       ]      },      {"label": "IOperator", "value": 1286},      {       "label": "label",       "children": [        {"label": "Labeler", "value": 9956},        {"label": "RadialLabeler", "value": 3899},        {"label": "StackedAreaLabeler", "value": 3202}       ]      },      {       "label": "layout",       "children": [        {"label": "AxisLayout", "value": 6725},        {"label": "BundledEdgeRouter", "value": 3727},        {"label": "CircleLayout", "value": 9317},        {"label": "CirclePackingLayout", "value": 12003},        {"label": "DendrogramLayout", "value": 4853},        {"label": "ForceDirectedLayout", "value": 8411},        {"label": "IcicleTreeLayout", "value": 4864},        {"label": "IndentedTreeLayout", "value": 3174},        {"label": "Layout", "value": 7881},        {"label": "NodeLinkTreeLayout", "value": 12870},        {"label": "PieLayout", "value": 2728},        {"label": "RadialTreeLayout", "value": 12348},        {"label": "RandomLayout", "value": 870},        {"label": "StackedAreaLayout", "value": 9121},        {"label": "TreeMapLayout", "value": 9191}       ]      },      {"label": "Operator", "value": 2490},      {"label": "OperatorList", "value": 5248},      {"label": "OperatorSequence", "value": 4190},      {"label": "OperatorSwitch", "value": 2581},      {"label": "SortOperator", "value": 2023}     ]    },    {"label": "Visualization", "value": 16540}   ]  } ]}';
			//$arrayData = json_decode($strJson,true);

		
		

	}


