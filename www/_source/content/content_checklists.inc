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
			$arrayConfig["defaultcontent"] = $this->GetWebsite()->LoadString("TID_CONTENT_CHECKLISTSDEFAULT");
			
			// Allow to add a section as a checklist to the given task list
 			$arrayConfig['checklists'] = $this->GetWebsite()->GetPlugin('checklisthq')->GetWikiPageID('tasks');
 
			$this->GetWebsite()->IncludeModule("checklisthq/checklistwikipad");
			$wikipad = new CChecklistWikiPadModule(
				$this->GetWebsite(),
				$this->GetWebsite()->GetPlugin('checklisthq')->GetWikiPageID('checklists'),
				$arrayConfig);
			$arrayItems["MODULE_WIKIPAD"] = $wikipad->RenderModule();
		
		}
	}