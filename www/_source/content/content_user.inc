<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$message = new CMessageElement($this->GetWebsite(),'info');
			$arrayItems['MESSAGE'] = $message->RenderMessage($this->GetWebsite()->LoadString('TID_INFO_USER'));
			$arrayItems['MODULE_WIKIPAD_HEADER'] = '';
			$arrayItems['MODULE_WIKIPAD_LINKS'] = '';
			

			$strUserName = $this->GetVar('user');
			$this->SetVar('user','');
			
			if ($strUserName == '')
			{
				return;
			}
			
			$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());
			

			$arrayParams = array();
			$arrayParams['command'] = 'get';
			$arrayParams['user_name'] = $strUserName;
			$consumer->ConsumeWebService('system/user',$arrayParams);
			if ($consumer->GetError() != '')	
			{
				$this->Trace('Error: "'.$consumer->GetError().'"');
				return;
			}					
			
			$strRealUserID = $this->GetWebsite()->GetAuthUserID();
			
			$strTempUserID = $consumer->GetResultValue('USER','USER_ID');
			
			
			// Check if the user WANTs to display the dashboard page
			$strDashboardMode = $this->GetWebsite()->GetData('USER',$strTempUserID,'dashboard','inactive');
			
			if ($strDashboardMode == '')
			{
				return;	
			}
			if ($strDashboardMode == 'inactive')
			{
				return;	
			}
			
			
			
			$this->GetWebsite()->SetAuthUserID($strTempUserID);			
			
			$arrayConfig = array();
			$arrayConfig['userspecific'] = true;
			$arrayConfig['viewonly'] = true;
			$arrayConfig['defaultcontent'] = ' ';
 
			$this->IncludeModule('cms/wikipad');
			
			$headerwikipad = new CWikiPadModule(
				$this->GetWebsite(),
				'header',
				$arrayConfig);
				
			$linkswikipad = new CWikiPadModule(
				$this->GetWebsite(),
				'links',
				$arrayConfig);
				
				
			$arrayItems['MODULE_WIKIPAD_HEADER'] = $headerwikipad->RenderModule();
			$arrayItems['MODULE_WIKIPAD_LINKS'] = $linkswikipad->RenderModule();
			$arrayItems['MESSAGE'] = '';
			
			$this->GetWebsite()->SetAuthUserID($strRealUserID);
			
			
		
		}
	}
