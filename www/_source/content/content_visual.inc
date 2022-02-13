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
			$arrayConfig['defaultcontent'] = $this->GetWebsite()->LoadString('TID_CONTENT_VISUALDEFAULT');
			$arrayConfig['network'] = true;
			$arrayConfig['zoomablesunburst'] = true;
			$arrayConfig['radialtidytree'] = true;
			$arrayConfig['hideview'] = true;
 
			$this->GetWebsite()->IncludeModule('checklisthq/checklistwikipad');
			$wikipad = new CChecklistWikiPadModule(
				$this->GetWebsite(),
				$this->GetWebsite()->GetPlugin('checklisthq')->GetWikiPageID('visual'),
				$arrayConfig);
			$arrayItems['MODULE_WIKIPAD'] = $wikipad->RenderModule();

		}
	}
