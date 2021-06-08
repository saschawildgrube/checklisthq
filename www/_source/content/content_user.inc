<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$message = new CMessageElement($this->GetWebsite(),'info');
			$arrayItems['MESSAGE'] = $message->RenderMessage($this->GetWebsite()->LoadString('TID_INFO_USER'));
			$arrayItems['MODULE_WIKIPAD_HEADER'] = '';
			$arrayItems['MODULE_WIKIPAD_LINKS'] = '';
			$arrayItems['MODULE_WIKIPAD_PROJECT_ZOOMABLESUNBURST'] = '';

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
 
			$this->IncludeModule('checklisthq/checklistwikipad');


			$bLinks = GetBoolValue($this->GetWebsite()->GetData('USER',$strTempUserID,'dashboard_links',true));
			if ($bLinks == true)
			{
				$wikipad_header = new CChecklistWikiPadModule(
					$this->GetWebsite(),
					'header',
					$arrayConfig);
					
				$wikipad_links = new CChecklistWikiPadModule(
					$this->GetWebsite(),
					'links',
					$arrayConfig);

				$arrayItems['MODULE_WIKIPAD_HEADER'] = $wikipad_header->RenderModule();
				$arrayItems['MODULE_WIKIPAD_LINKS'] = $wikipad_links->RenderModule();
			}
			else
			{
				$arrayItems['MODULE_WIKIPAD_HEADER'] = '';
				$arrayItems['MODULE_WIKIPAD_LINKS'] = '';
			}


			$bProjectZoomableSunburst = GetBoolValue($this->GetWebsite()->GetData('USER',$strTempUserID,'dashboard_project_zoomablesunburst',false));
			if ($bProjectZoomableSunburst == true)
			{
				$arrayConfig['viewonly'] = false;
				$arrayConfig['hidetabnav'] = true;
				$arrayConfig['zoomablesunburst'] = true;
				$wikipad_project_zoomablesunburst = new CChecklistWikiPadModule(
					$this->GetWebsite(),
					'project',
					$arrayConfig);
				$wikipad_project_zoomablesunburst->SetState('zoomablesunburst');
				$arrayItems['MODULE_WIKIPAD_PROJECT_ZOOMABLESUNBURST'] = $wikipad_project_zoomablesunburst->RenderModule();
			}
			else
			{
				$arrayItems['MODULE_WIKIPAD_PROJECT_ZOOMABLESUNBURST'] = '';
			}
		
		
			$arrayItems['MESSAGE'] = '';
			
			$this->GetWebsite()->SetAuthUserID($strRealUserID);
			
			
		
		}
	}
