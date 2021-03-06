<?php



	function GetDocumentRootDir()
	{
		$strDocRoot = $_SERVER["DOCUMENT_ROOT"];
		/*
			Unfortunately some web servers add the trailing slash to the document root
			path while others do not. The code below makes sure this works in all cases.
			However, once you found out how your server is configured, you should remove
			this code.
		*/
		if (substr($strDocRoot,-1,1) !=  "/")
		{
			$strDocRoot.="/";
		}
		return $strDocRoot."quality/testwebsite/";
	}

	function GetSourceDir()
	{
		$strDocRoot = $_SERVER["DOCUMENT_ROOT"];
		if (substr($strDocRoot,-1,1) !=  "/")
		{
			$strDocRoot.="/";
		}		
		return $strDocRoot."_source/wdk/quality/testwebsite/_source/";
	}
	
	function GetWDKDir()
	{
		$strDocRoot = $_SERVER["DOCUMENT_ROOT"];
		if (substr($strDocRoot,-1,1) !=  "/")
		{
			$strDocRoot.="/";
		}		
		return $strDocRoot."_source/wdk/";
	}
	
	function GetConfigDir()
	{
		return GetSourceDir();	
	}

	function GetWebservicesDir()
	{
		return GetDocumentRootDir() . "webservices/";
	}
	
	function GetQualityDir()
	{
		return GetDocumentRootDir() . "quality/";
	}
	
	function GetRootURL()
	{
		return $_SERVER["SERVER_NAME"]."/quality/testwebsite/";
	}

	function GetWebservicesURL()
	{
		return GetRootURL()."webservices/";	
	}

	function GetAdminEmail()
	{
		return "admin@".GetMailDomain();	
	}
	
	function GetSystemEmail()
	{
		return "noreply@".GetMailDomain();	
	}

	function GetSystemEmailSenderName()
	{
		return GetRootURL()." (".GetEnvID().")";	
	}


	function GetMailDomain()
	{
		$strMailHost = $_SERVER["SERVER_NAME"];
		$strMailHost = ReplaceString($strMailHost,"www.","");
		return $strMailHost;	
	}

	
	function GetEnvID()
	{
		return "prod";	
	}
	
	function GetEnvConfigID()
	{
		return "env";
	}
	
	function GetOperationMode()
	{
		return "normal";	
	}
	
	function GetGpgPath()
	{
		return "";
	}
	
	function GetTempDir()
	{
		return "/tmp/";	
	}

	function GetErrorLogPath()
	{
		if (GetEnvID() == "dev")
		{
			return GetDocumentRootDir()."../../logs/dev.checklisthq.com/error_log";	
		}
		return GetDocumentRootDir()."../../logs/error_log";		}


