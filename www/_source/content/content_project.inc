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
			$arrayConfig['zoomablesunburst'] = true;
			$arrayConfig['hideview'] = true;
 
			$this->GetWebsite()->IncludeModule('checklisthq/checklistwikipad');
			$wikipad = new CChecklistWikiPadModule(
				$this->GetWebsite(),
				'project',
				$arrayConfig);
			$arrayItems['MODULE_WIKIPAD'] = $wikipad->RenderModule();

		}
	}
	
