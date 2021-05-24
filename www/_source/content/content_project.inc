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
			$arrayConfig['defaultcontent'] = $this->GetWebsite()->LoadString('TID_CONTENT_PROJECTDEFAULT');
			$arrayConfig['networkgraph'] = true;
			$arrayConfig['hideview'] = true;
 
			$this->GetWebsite()->IncludeModule('checklisthq/checklistwikipad');
			$wikipad = new CChecklistWikiPadModule(
				$this->GetWebsite(),
				'project',
				$arrayConfig);
			$arrayItems['MODULE_WIKIPAD'] = $wikipad->RenderModule();


			// Parse the wiki-syntax to extract the project tree

		


		}
	}
	
