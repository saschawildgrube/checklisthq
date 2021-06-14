<?php



	function GetDocumentRootDir()
	{
		return $_SERVER['DOCUMENT_ROOT'] . '/';	
	}

	function GetSourceDir()
	{
		return GetDocumentRootDir().'_source/';
	}
	
	function GetWDKDir()
	{
		return GetSourceDir().'wdk/';
	}
	
	function GetConfigDir()
	{
		return GetSourceDir();	
	}

	function GetWebservicesDir()
	{
		return GetDocumentRootDir() . 'webservices/';
	}
	
	function GetQualityDir()
	{
		return GetDocumentRootDir() . 'quality/';
	}
	
	function GetRootURL()
	{
		return $_SERVER['SERVER_NAME'].'/';	
	}

	function GetWebservicesURL()
	{
		return GetRootURL().'webservices/';	
	}

	function GetAdminEmail()
	{
		return 'sascha@wildgrube.com';	
	}
	
	function GetSystemEmail()
	{
		return 'noreply@'.GetMailDomain();	
	}

	function GetSystemEmailSenderName()
	{
		return GetRootURL().' ('.GetEnvID().')';	
	}


	function GetMailDomain()
	{
		$strMailHost = $_SERVER['SERVER_NAME'];
		$strMailHost = ReplaceString($strMailHost,'www.','');
		$strMailHost = ReplaceString($strMailHost,'dev.','');		
		return $strMailHost;	
	}

	
	function GetEnvID()
	{
		if (FindString(GetRootURL(),'dev.') != -1)
		{
			return 'dev';
		}
		return 'prod';	
	}
	
	function GetEnvConfigID()
	{
		return 'env-'.GetEnvID();
	}
	
	function GetOperationMode()
	{
		return 'normal';	
	}
	
	function GetGpgPath()
	{
		return '';
	}
	
	function GetTempDir()
	{
		return '/tmp/';	
	}

	function GetWebservicesDefaultFormat()
	{
		return 'json';  
	}

	function GetErrorLogPath()
	{
		if (GetEnvID() == 'dev')
		{
			return GetDocumentRootDir().'../../logs-dev/error_log';
		}
		return GetDocumentRootDir().'../../logs/error_log';	
	}

