<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			if ($this->GetWebsite()->GetAuthUserID() == "")	
			{
				$arrayItems["MODULE_LOGIN"] = $this->GetWebsite()->m_moduleLogin->RenderModule("form");
				$arrayItems["MODULE_SIGNIN"] = $this->GetWebsite()->m_moduleSignin->RenderModule();
			}
			else
			{
				$arrayItems["MODULE_LOGIN"] = "";
				$arrayItems["MODULE_SIGNIN"] = "";
				
			}
			
		
		}
	}
	
