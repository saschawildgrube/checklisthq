<?php

	class CMyContent extends CContentLogin
	{
		function CallbackRenderContent(&$arrayItems)
		{
		
			$this->GetWebsite()->IncludeModule("checklisthq/userdashboardconfig");
			$userdashboardconfig = new CUserDashboardConfigModule($this->GetWebsite());
			$arrayItems["MODULE_USERDASHBOARDCONFIG"] = $userdashboardconfig->RenderModule("");


			$passwordpolicy = new CPasswordPolicy();
			$passwordpolicy->SetPolicy($this->GetConfig("passwordpolicy"));

			$this->GetWebsite()->IncludeModule("user/userpassword");
			$userpassword = new CUserPasswordModule($this->GetWebsite(),$passwordpolicy);
			$arrayItems["MODULE_USERPASSWORD"] = $userpassword->RenderModule("");

			$arrayItems["MODULE_USEREMAIL"] = $this->GetWebsite()->m_moduleUserEmail->RenderModule("");
			
			$arrayItems["MODULE_USERSIGNOUT"] = $this->GetWebsite()->m_moduleSignOut->RenderModule("");
		}
	}
