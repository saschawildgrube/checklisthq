<?php

	require_once(GetWDKDir().'wdk_element_react.inc');
	require_once(GetAssemblyDir('d3').'d3.inc');

	abstract class CD3ReactElement extends CReactElement
	{
		function __construct(
			CWebSite &$website,
			$strElementID,
			$strStyleID,
			$bLayoutCFG,
			$bLayoutHTML)
		{
			parent::__construct(
				$website,
				$strElementID,
				$strStyleID, 
				$bLayoutCFG,
				$bLayoutHTML);
				
			ActivateD3v7($this->GetWebsite());
			$this->AddJavaScript('react-d3'); 
		}
	}

