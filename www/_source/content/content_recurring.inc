<?php

	class CMyContent extends CContentLogin
	{
		function OnRenderContent(&$arrayItems)
		{

			$arrayConfig = array();
			$arrayConfig['columns'] = 100;    
			$arrayConfig['lines'] = 15;
			$arrayConfig['userspecific'] = true;
			$arrayConfig['defaultcontent'] = $this->GetWebsite()->LoadString('TID_CONTENT_RECURRINGDEFAULT');
 
			$this->GetWebsite()->IncludeModule('cms/wikipad');
			$wikipad = new CWikiPadModule(
				$this->GetWebsite(),
				$this->GetWebsite()->GetPlugin('checklisthq')->GetWikiPageID('recurring'),
				$arrayConfig);
			$arrayItems['MODULE_WIKIPAD'] = $wikipad->RenderModule();
			
		}
	}