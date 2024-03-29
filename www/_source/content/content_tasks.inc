<?php

	class CMyContent extends CContentLogin
	{
		function OnRenderContent(&$arrayItems)
		{

			$arrayConfig = array();
			$arrayConfig['columns'] = 100;    
			$arrayConfig['lines'] = 15;
			$arrayConfig['userspecific'] = true;
			$arrayConfig['showsubmitbuttonsaveandstay'] = true;
			$arrayConfig['defaultcontent'] = $this->GetWebsite()->LoadString('TID_CONTENT_TASKSDEFAULT');
			$arrayConfig['tasklist'] = true; // Enable the tool icons to directly modify a bullet led row
 
			$this->GetWebsite()->IncludeModule('checklisthq/checklistwikipad');
			
			$wikipad = new CChecklistWikiPadModule(
				$this->GetWebsite(),
				$this->GetWebsite()->GetPlugin('checklisthq')->GetWikiPageID('tasks'),
				$arrayConfig);
			$arrayItems['MODULE_WIKIPAD'] = $wikipad->RenderModule();
		
		}
		
	}