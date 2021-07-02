<?php

	require_once('env.inc');
	require_once(GetWDKDir().'wdk_website_dynamic.inc');
	require_once(GetAssemblyDir('google').'google_appurl.inc');
	
  class CChecklistHQWebSite extends CDynamicWebSite
	{
		
		public $m_moduleSignin;
		public $m_moduleSignOut;
		public $m_moduleLogin;
		public $m_moduleUserEmail;

		
		
		function __construct(
			$arrayParams)
		{
			
			$arrayMeta = array();
			$arrayMeta['version'] = '0.1';
			$arrayMeta['author'] = 'Sascha Wildgrube';
			$arrayMeta['publisher'] = 'Sascha Wildgrube';
			$arrayMeta['copyright'] = '?TID_COPYRIGHT?';
			$arrayMeta['copyright_since'] = '2017';
			
		
			$config = new CConfig();
			$config->AddConfigStoreLocation(GetConfigDir());
			$config->LoadConfig(GetEnvConfigID());
			$arrayConfig = $config->GetDataArray();
			
			$arrayConfig['session_mode'] = SESSION_MODE_SERVER_COOKIE;
			$arrayConfig['session_duration'] = 60 * 60 * 24 * 5; // 5 days
			
			$arrayConfig['csrf_defuse'] = true;      
			
			$arrayConfig['csvformat'] = 'excel';
			$arrayConfig['languages'] = array('EN');
						
			$arrayConfig['layouts'] = array('bootstrap');
			$arrayConfig['protocols'] = array('https');
			
			$arrayConfig['admin_email'] = GetAdminEmail();
			$arrayConfig['seourl'] = 'seourl';
			$arrayConfig['opengraph'] = true;
			
			$arrayConfig['passwordpolicy'] = array(
				'minimumlength' => 6,
				'requiredcharsnumeric' => true,
				'requiredcharsalpha' => true);
			
			$arrayConfig['defaultprivileges'] = array();
			$arrayConfig['privileges'] = array('SYSTEMADMIN');
			
			
			parent::__construct(
				GetRootURL(),
				GetSourceDir(),
				GetDocumentRootDir(),
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				'website_checklisthq');
					
		}
		
		function OnInitAssemblyLine(&$assemblyLine)
		{
			$assembly = new CAssembly($this,'checklisthq');
			$assemblyLine->AddAssembly($assembly);
			
			$assembly = new CAssembly($this,'google');
			$assemblyLine->AddAssembly($assembly);
			
			$assembly = new CAssembly($this,'d3');
			$assemblyLine->AddAssembly($assembly);	
			
			parent::OnInitAssemblyLine($assemblyLine);
		}
		
		function OnInit()
		{
			$bResult = parent::OnInit();

			$this->AddHeader('<!--AVAILABILITY-CHECK-->');
			
			$this->SetTimezone('Europe/Berlin');
			
			$this->LoadStringResources('systemadmin');
			
			$this->SetFramework('fullwidth');
			
			$this->RegisterGenericOutputItem('GOOGLEAPPURL',array('app','email'));			
			
			$passwordpolicy = new CPasswordPolicy();
			$passwordpolicy->SetPolicy($this->GetConfig('passwordpolicy'));
			
			$this->IncludeModule('user/signin');
			$this->m_moduleSignin = new CSigninModule($this,'signin',$passwordpolicy,'signin','index');
			$this->RegisterModule($this->m_moduleSignin);

			$this->IncludeModule('user/signout');
			$this->m_moduleSignOut = new CSignOutModule($this,'signout');
			$this->RegisterModule($this->m_moduleSignOut);

			$this->IncludeModule('user/login');
			$this->m_moduleLogin = new CLoginModule($this,'login','logout');   
			$this->RegisterModule($this->m_moduleLogin);
	
			$this->IncludeModule('user/useremail');
			$this->m_moduleUserEmail = new CUserEmailModule($this,false,true,'verifymail',60*60);
			$this->RegisterModule($this->m_moduleUserEmail);
			
			// Initialize Logo
			$linkLogo = new CLinkElement($this,'logo');
			$icon = new CIconElement($this,'default');
			$this->SetOutputItem(
				'LOGO',
				$linkLogo->RenderLink(
					$this->GetRootURL(),
					$icon->RenderIcon('tasks')
					.'&nbsp;'
					.'?TID_TITLE?'));
	
		
			return $bResult;
		}
		

		
		function GetSiteName()
		{
			return 'Checklist HQ';
		}
		
		function GetSiteTeamEmail()
		{
			return 'no-reply@'.GetMailDomain();
		}

		function GetSiteTeamName()
		{
			return 'Sascha Wildgrube';
		}
		



		function OnPreContent()
		{
	
			if ($this->GetAuthUserID() != '')
			{
				if ($this->GetContent() == 'index' || $this->GetContent() == 'login')
				{
					$this->SetContent('tasks');	
				}
			}
		}

		function OnPostContent()
		{
			// DATETIME
			$this->IncludeElement('datetime');
			$datetime = new CDateTimeElement($this,'default');
			$this->SetOutputItem('DATETIME',$datetime->RenderDateTime(GetTimeNow()));	
		
			// USER STATUS
			$this->SetOutputItem('USERSTATUS',$this->m_moduleLogin->RenderModule('status'));

			// HEADER
			$arrayConfig = array();
			$arrayConfig['userspecific'] = true;
			$arrayConfig['viewonly'] = true;
			$arrayConfig['defaultcontent'] = $this->LoadString('TID_CONTENT_HEADERDEFAULT');
 
			$this->IncludeModule('cms/wikipad');
			$headerwikipad = new CWikiPadModule(
				$this,
				'header',
				$arrayConfig);
			$this->SetOutputItem('MODULE_HEADER',$headerwikipad->RenderModule());
			
			// FORK ME ON GITHUB
			$this->IncludeElement('forkmeongithub');
			$forkmeongithub = new CForkMeOnGithubElement($this,'default');
			$this->SetOutputItem('FORKMEONGITHUB',$forkmeongithub->RenderForkMeOnGithub('saschawildgrube','checklisthq'));
			


		}
		
		
		function OnRenderSpecialContent($strContentID)
		{
			if ($this->GetVar('signin') != '')
			{
				$strOutput = $this->m_moduleSignin->RenderModule('validation');
				$strOutput .= $this->m_moduleLogin->RenderModule('form');
				return $strOutput;
			}			
			if ($this->GetVar('verifymail') != '')
			{
				$this->m_moduleUserEmail->SetState('validation');
				$strOutput = $this->m_moduleUserEmail->RenderModule();
				return $strOutput;
			}
			return '';
		}

		function OnRenderGenericOutputItem($strItem, $arrayAttributes)
		{
			switch ($strItem)
			{
			case 'GOOGLEAPPURL':
				return Google_MakeAppURL(
					ArrayGetValue($arrayAttributes,'app'),
					ArrayGetValue($arrayAttributes,'email'));
			default:
				return parent::OnRenderGenericOutputItem($strItem, $arrayAttributes);
			}
		}
		
		


		function OnRenderNavigation($strOutput)
		{

			$navigationtree = new CNavigationTree();
			$navigationtree->LoadNavigationTree($this,'');
			$navigationtree->SetCurrentContext($this->GetContent());
			$navigationtree->SetContextParameterName('content');

			if (FindString($strOutput,'{NAVIGATION_HORIZONTALPATH}') != -1)
			{
				$navigation = new CNavigationElement($this,'horizontalpath');
				$this->SetOutputItem('NAVIGATION_HORIZONTALPATH',$navigation->RenderNavigationHorizontalPath($navigationtree));
	    }

			if (FindString($strOutput, '{NAVIGATION_VERTICALPATH}') != -1)
			{ 
				$navigation = new CNavigationElement($this,'verticalpath');
				$this->SetOutputItem('NAVIGATION_VERTICALPATH',
					$navigation->RenderNavigationVerticalPath($navigationtree,0));
      }
        
      $this->SetOutputItem('NAVIGATION_CURRENTPATH',''); 
			/*
			if (FindString($strOutput, '{NAVIGATION_CURRENTPATH}') != -1)
			{
				$navigation = new CNavigationElement($this,'currentpath');
				$this->SetOutputItem('NAVIGATION_CURRENTPATH',
					$navigation->RenderNavigationCurrentPath($navigationtree));
			}
			*/
			
			if (FindString($strOutput, '{NAVIGATION_VERTICALCURRENTPATH}') != -1)
			{ 
				$navigation = new CNavigationElement($this,'verticalcurrentpath');
				$this->SetOutputItem('NAVIGATION_VERTICALCURRENTPATH',
					$navigation->RenderNavigationVerticalCurrentPath($navigationtree));
			}
     
			if (FindString($strOutput, '{NAVIGATION_FOOTER}') != -1)
			{
				$navigationtree_footer = new CNavigationTree();
				$navigationtree_footer->LoadNavigationTree($this,'footer');
				$navigationtree_footer->SetCurrentContext($this->GetContent());
				$navigationtree_footer->SetContextParameterName('content');
         		
				$navigation = new CNavigationElement($this,'footer');
				$this->SetOutputItem('NAVIGATION_FOOTER',$navigation->RenderNavigationHorizontalPath($navigationtree_footer));
			}
		}
		


		function OnCheckCondition($strCondition)
		{
			if ($strCondition == 'dev')
			{
				if (GetEnvID() == 'dev')
				{
					return true;
				}	
			}
			
			if ($strCondition == 'useradmin')
			{
				if ($this->CheckCondition('systemadmin') == true)
				{
					return true;
				}
			}

			if ($strCondition == 'admin')
			{
				if ($this->CheckCondition('useradmin') == true)
				{
					return true;
				}
				if ($this->CheckCondition('systemadmin') == true)
				{
					return true;
				}
			}			
			
			
			return parent::OnCheckCondition($strCondition);	
		}

		function OnParseSEOURL($strSEOURL)
		{
			if ($strSEOURL != '')
			{
				$arraySegments = StringExplode($strSEOURL,'/');
				$nSegments = ArrayCount($arraySegments);
				if ($nSegments >= 2)
				{
					$strSegment1 = $arraySegments[0];
					if ($strSegment1 == 'user')
					{
						$strUserName = $arraySegments[1];
						$this->SetContent('user');
						$this->SetVar('user',$strUserName);
						$this->Trace($this->GetAllVars());						
						$this->Trace('OnParseSEOURL: user/'.$strUserName);
						return;
					}
				}
			}
			parent::OnParseSEOURL($strSEOURL);	
		}

		function OnUserLogin($strUserName,$strAuthenticationPayload)
		{
			$bResult = parent::OnUserLogin($strUserName,$strAuthenticationPayload);
			if ($bResult == true)
			{
				$this->SendMailToAdmin('LOGIN '.$strUserName,'');
			}
			return $bResult;
		}
		
		function OnGetOpenGraphImageURL()
		{
			return $this->GetImageURL('opengraph_checklisthq_sunburst','png');
		}

	}
	
	