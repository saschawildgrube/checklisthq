<?php

	require_once(GetWDKDir().'wdk_element_react.inc');

	abstract class CGoogleMapsReactElement extends CReactElement
	{
		private $m_strApiKey;
		function __construct(
			CWebSite &$website,
			$strElementID,
			$strStyleID,
			$bLayoutCFG,
			$bLayoutHTML)
		{
			$this->m_strApiKey = null;
			parent::__construct(
				$website,
				$strElementID,
				$strStyleID,
				$bLayoutCFG,
				$bLayoutHTML);
			
			$strApiKey = $this->GetWebsite()->GetConfig("googlemaps_apikey");
			if ($strApiKey != '')
			{
				$this->m_strApiKey = $strApiKey;
				$this->AddJavaScriptURL('googlemaps','https://maps.googleapis.com/maps/api/js?key='.$strApiKey); 
				$this->AddJavaScript('react-googlemaps'); 
			}
		}
		
		function RenderReactComponent(
			$strReactComponent,
			$arrayProps = array())
		{
			if ($this->m_strApiKey == null)
			{
				return '<div style="color:red;">CGoogleMapsElement: ERROR: The Google API key has not been specified.</div>';
			}
			
			$strDomElementID = $this->GetElementID().'_'.MakeNumericToken(10);
			
			$this->AddJavaScriptReactRenderByID(
				$strReactComponent,
				$strDomElementID,
				$arrayProps);
				
			$strOutput = '<div id="'.$strDomElementID.'"></div>';
			return $strOutput;				
		}	
		
	}

