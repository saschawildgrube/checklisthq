<?php

	require_once(GetWDKDir()."wdk_module.inc");
	
	class CUserDashboardConfigModule extends CModule
	{
		
		private $m_arrayErrors;
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"checklisthq/userdashboardconfig",
				$strInstanceID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$this->m_arrayErrors = array();
			$this->SetOutputItem('MESSAGE','');
			
			return true;
		}
		
	
		function OnRenderModule($strState)
		{
			$this->Trace("OnRenderModule(\"$strState\")");
			switch ($strState)
			{
				case "form":
					return $this->StateForm();
				case "submit":
					return $this->StateSubmit();
				case "":
					$this->SetState("form");
				default:
					return $this->StateForm();
			}
		}
		
	
		
		
		function StateForm()
		{
			$this->SetState("form");
			
			$strMode = $this->GetWebsite()->GetAuthUserData('dashboard','inactive');


			$formDef = new CFormDefinition();
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "submit";
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			$formDef->AddControlStatic("INFO","?TID_USERDASHBOARDCONFIG_TEXT_INFO?");
			
			$arrayOptions = array();
			$arrayOptions["inactive"] = "?TID_USERDASHBOARDCONFIG_MODE_INACTIVE?";
			$arrayOptions["active"] = "?TID_USERDASHBOARDCONFIG_MODE_ACTIVE?";
			$formDef->AddControlSelect(
				"MODE",
				$this->GetModuleVarName("mode"),
				"?TID_USERDASHBOARDCONFIG_LABEL_MODE?",
				$arrayOptions,
				$strMode);
			
			$formDef->AddControlButtonSubmit("?TID_BUTTON_SAVE?");
			
			if ($strMode == 'active')
			{
				$strDashboardURL = $this->GetWebsite()->GetRootURL().'user/'.$this->GetWebsite()->GetAuthUserName();
				$link = new CLinkElement($this->GetWebsite(),'default');
				$strDashboardLink = $link->RenderLink($strDashboardURL,$strDashboardURL,true);
				$this->SetOutputItem('LINK_DASHBOARD',$strDashboardLink);
				$formDef->AddControlStatic("ACTIVE","?TID_USERDASHBOARDCONFIG_TEXT_ACTIVE?");
			}
			
			
			$form = new CFormElement($this->GetWebsite(),"default");
			
			$this->SetOutputItem( 
				"FORM",
				$form->RenderForm(
					$formDef,
					array(),					
					$this->m_arrayErrors));
					
		
			return "form";
		}
		
		function StateSubmit()
		{
			$strMode = $this->GetModuleVar('mode');
			if ($strMode == '')
			{
				$this->m_arrayErrors['MODE'] = '?TID_USERDASHBOARDCONFIG_ERROR_MODE_INVALID?';
			}
			
			if (ArrayCount($this->m_arrayErrors) > 0)			
			{
				return $this->StateForm();	
			}

			$this->GetWebsite()->SetAuthUserData('dashboard',$strMode);
			
			$message = new CMessageElement($this->GetWebsite(),'info');
			$this->SetOutputItem('MESSAGE',$message->RenderMessage('?TID_USERDASHBOARDCONFIG_INFO_SUCCESS?'));
			
			$this->SetModuleVar('mode','');
			
			return $this->StateForm();
		}
		
		
		



	}

