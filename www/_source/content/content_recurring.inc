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
				'recurring',
				$arrayConfig);
			$arrayItems['MODULE_WIKIPAD'] = $wikipad->RenderModule();

/*		
			// only in case of post
			if ($this->GetWebsite()->GetRequestMethod() == 'post')
			{
				// when the user tried to save data
				if ($wikipad->GetInitialState() == 'submit')
				{
					// and no messages are on the stack
					if ($this->GetWebsite()->CountAllMessages() == 0)
					{
						// redirect to itself to prevent the message in stupid Safari mobile browser
						$this->GetWebsite()->Redirect($this->GetWebsite()->GetCurrentURL());
					}
				}
			}
	*/		
		
		}
	}