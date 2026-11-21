<?php

	class CMyContent extends CContentSystemAdmin
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("cms/staticfilebrowser");
			$arrayConfig = array();
			$arrayConfig["blockedfiles"] = $this->GetWebsite()->GetConfig("sourcefiles_blockedfiles");
			$arrayConfig["rootdir"] = GetDocumentRootDir();
			$arrayConfig["hideinfo"] = true;
			$staticfilebrowser = new CStaticFileBrowserModule($this->GetWebsite(),"documentroot",$arrayConfig);
			$arrayItems["MODULE_STATICFILEBROWSER"] = $staticfilebrowser->RenderModule();
		}
	}
	
