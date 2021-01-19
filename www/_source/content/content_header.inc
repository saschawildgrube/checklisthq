<?php

	class CMyContent extends CContentLogin
	{
		function OnRenderContent(&$arrayItems)
		{

			$arrayConfig = array();
			$arrayConfig["columns"] = 100;    
			$arrayConfig["lines"] = 10;
			$arrayConfig["userspecific"] = true;
			$arrayConfig["defaultcontent"] = $this->GetWebsite()->LoadString("TID_CONTENT_HEADERDEFAULT");
 
			$this->GetWebsite()->IncludeModule("cms/wikipad");
			$wikipad = new CWikiPadModule(
				$this->GetWebsite(),
				"header",
				$arrayConfig);
			$arrayItems["MODULE_WIKIPAD"] = $wikipad->RenderModule();

		}
	}
