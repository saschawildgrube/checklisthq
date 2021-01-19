<?php

	require_once(GetSourceDir().'assemblies/google/element_react_googlemaps.inc');

	class CGoogleMapsElement extends CGoogleMapsReactElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'googlemaps',
				$strStyleID,
				false,  
				false); 
		}
		
		function RenderGoogleMaps($fLat, $fLong, $nZoom = 3, $strType = 'terrain')
		{
			$arrayProps = array();
			//$arrayProps['query'] = $strQuery;
			$arrayProps['lat'] = $fLat;
			$arrayProps['long'] = $fLong;
			$arrayProps['zoom'] = $nZoom;
			$arrayProps['type'] = $strType;
			$this->AddJavaScript('react_googlemap');
			return $this->RenderReactComponent(
				'GoogleMapsReactComponent',
				$arrayProps);
		}
	}

