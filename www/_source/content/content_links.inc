<?php

	class CMyContent extends CContentLogin
	{
		function OnRenderContent(&$arrayItems)
		{

			$arrayConfig = array();
			$arrayConfig["columns"] = 100;    
			$arrayConfig["lines"] = 15;
			$arrayConfig["userspecific"] = true;
			$arrayConfig["showsubmitbuttonsaveandstay"] = true;
			$arrayConfig["defaultcontent"] = $this->GetWebsite()->LoadString("TID_CONTENT_LINKSDEFAULT");
 
			$this->GetWebsite()->IncludeModule("cms/wikipad");
			$wikipad = new CWikiPadModule(
				$this->GetWebsite(),
				"links",
				$arrayConfig);
			$arrayItems["MODULE_WIKIPAD"] = $wikipad->RenderModule();

		}
	}
	
