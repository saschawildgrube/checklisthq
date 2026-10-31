<?php

	// Static calls share a PHP process; headers belong to the outer request only.
	class CWebServiceTransportContext
	{
		public static $nDepth = 0;
	}

	function WebServiceTransportHeader($strHeader)
	{
		if (CWebServiceTransportContext::$nDepth == 0)
		{
			header($strHeader);
		}
	}

	function GetStaticWebserviceEntryPoint($strURL)
	{
		if (!function_exists('GetWebservicesURL') || !function_exists('GetWebservicesDir'))
		{
			return false;
		}
		$strRootURL = GetWebservicesURL();
		if (strpos($strRootURL,'://') === false)
		{
			$strRootURL = 'https://'.$strRootURL;
		}
		$arrayRoot = parse_url($strRootURL);
		$arrayURL = parse_url($strURL);
		if ($arrayRoot === false || $arrayURL === false
			|| !isset($arrayURL['host'],$arrayRoot['host'],$arrayURL['scheme'])
			|| !in_array($arrayURL['scheme'],array('http','https'))
			|| strcasecmp($arrayURL['host'],$arrayRoot['host']) != 0
			|| isset($arrayURL['user']) || isset($arrayURL['pass'])
			|| (isset($arrayURL['port']) ? $arrayURL['port'] : ($arrayURL['scheme'] == 'https' ? 443 : 80))
				!= (isset($arrayRoot['port']) ? $arrayRoot['port'] : ($arrayURL['scheme'] == 'https' ? 443 : 80)))
		{
			return false;
		}
		$strRootPath = rtrim(isset($arrayRoot['path']) ? $arrayRoot['path'] : '/','/').'/';
		$strPath = isset($arrayURL['path']) ? $arrayURL['path'] : '';
		if (strpos($strPath,$strRootPath) !== 0)
		{
			return false;
		}
		$strService = substr($strPath,strlen($strRootPath));
		$strService = preg_replace('~/index\.php$~','',$strService);
		$strService = rtrim($strService,'/');
		if (!preg_match('~^[a-zA-Z0-9_-]+(?:/[a-zA-Z0-9_-]+)+$~D',$strService))
		{
			return false;
		}
		$strRoot = realpath(GetWebservicesDir());
		$strEntry = realpath(GetWebservicesDir().'/'.$strService.'/index.php');
		if ($strRoot === false || $strEntry === false || !is_file($strEntry))
		{
			return false;
		}
		$strPrefix = rtrim(str_replace('\\','/',$strRoot),'/').'/';
		$strEntryNormalized = str_replace('\\','/',$strEntry);
		if (DIRECTORY_SEPARATOR == '\\')
		{
			$strPrefix = strtolower($strPrefix);
			$strEntryNormalized = strtolower($strEntryNormalized);
		}
		if (strpos($strEntryNormalized,$strPrefix) !== 0)
		{
			return false;
		}
		return $strEntry;
	}

	function IncludeStaticWebserviceEntryPoint($strEntryPoint)
	{
		// Keep entry-point variables separate from dispatcher bookkeeping.
		include($strEntryPoint);
	}

	function StaticWebserviceRequest($strURL,$arrayParams)
	{
		$strEntryPoint = GetStaticWebserviceEntryPoint($strURL);
		if ($strEntryPoint === false || CWebServiceTransportContext::$nDepth >= 32)
		{
			return false;
		}
		$arraySavedGlobals = array();
		foreach (array('_GET','_POST','_REQUEST','_COOKIE','_FILES','_SERVER') as $strKey)
		{
			if (array_key_exists($strKey,$GLOBALS))
			{
				$arraySavedGlobals[$strKey] = $GLOBALS[$strKey];
			}
		}
		$strWorkingDir = getcwd();
		$strIncludePath = get_include_path();
		$strTimeZone = date_default_timezone_get();
		$nErrorReporting = error_reporting();
		$arrayIni = array();
		foreach (array('display_errors','allow_url_fopen','max_execution_time','expose_php') as $strKey)
		{
			$arrayIni[$strKey] = ini_get($strKey);
		}
		$arrayHeaders = headers_list();
		$nStatus = http_response_code();
		$nBufferLevel = ob_get_level();
		CWebServiceTransportContext::$nDepth++;
		try
		{
			$arrayURL = parse_url($strURL);
			$_GET = array();
			parse_str(isset($arrayURL['query']) ? $arrayURL['query'] : '',$_GET);
			$_POST = array();
			parse_str(is_array($arrayParams) ? MakeURLParameters($arrayParams) : GetStringValue($arrayParams),$_POST);
			$_REQUEST = array_merge($_GET,$_POST);
			$_COOKIE = array();
			$_FILES = array();
			// Do not inherit the visitor's authentication, cookies or proxy headers.
			foreach (array_keys($_SERVER) as $strKey)
			{
				if (strpos($strKey,'HTTP_') === 0 || strpos($strKey,'PHP_AUTH_') === 0
					|| in_array($strKey,array('AUTH_TYPE','REMOTE_USER','REDIRECT_REMOTE_USER')))
				{
					unset($_SERVER[$strKey]);
				}
			}
			$_SERVER['SERVER_NAME'] = $arrayURL['host'];
			$_SERVER['HTTP_HOST'] = $arrayURL['host'].(isset($arrayURL['port']) ? ':'.$arrayURL['port'] : '');
			$_SERVER['HTTPS'] = $arrayURL['scheme'] == 'https' ? 'on' : '';
			$_SERVER['SERVER_PORT'] = isset($arrayURL['port']) ? $arrayURL['port'] : ($arrayURL['scheme'] == 'https' ? 443 : 80);
			$_SERVER['REQUEST_METHOD'] = 'POST';
			$_SERVER['QUERY_STRING'] = isset($arrayURL['query']) ? $arrayURL['query'] : '';
			$_SERVER['REQUEST_URI'] = $arrayURL['path'].($_SERVER['QUERY_STRING'] != '' ? '?'.$_SERVER['QUERY_STRING'] : '');
			$_SERVER['SCRIPT_NAME'] = rtrim($arrayURL['path'],'/');
			if (substr($_SERVER['SCRIPT_NAME'],-10) != '/index.php')
			{
				$_SERVER['SCRIPT_NAME'] .= '/index.php';
			}
			$_SERVER['PHP_SELF'] = $_SERVER['SCRIPT_NAME'];
			$_SERVER['SCRIPT_FILENAME'] = $strEntryPoint;
			$_SERVER['SCRIPT_URI'] = $arrayURL['scheme'].'://'.$_SERVER['HTTP_HOST'].$arrayURL['path'];
			$_SERVER['REMOTE_ADDR'] = isset($_SERVER['SERVER_ADDR']) ? $_SERVER['SERVER_ADDR'] : '127.0.0.1';
			$_SERVER['REQUEST_TIME'] = time();
			$_SERVER['REQUEST_TIME_FLOAT'] = microtime(true);
			$_SERVER['CONTENT_TYPE'] = 'application/x-www-form-urlencoded';
			$_SERVER['CONTENT_LENGTH'] = strlen(is_array($arrayParams) ? MakeURLParameters($arrayParams) : GetStringValue($arrayParams));
			if (!chdir(dirname($strEntryPoint)))
			{
				return false;
			}
			set_include_path(__DIR__.PATH_SEPARATOR.$strWorkingDir.PATH_SEPARATOR.$strIncludePath);
			ob_start();
			IncludeStaticWebserviceEntryPoint($strEntryPoint);
			while (ob_get_level() > $nBufferLevel + 1)
			{
				ob_end_flush();
			}
			return ob_get_level() == $nBufferLevel + 1 ? ob_get_contents() : false;
		}
		catch (Throwable $error)
		{
			return false;
		}
		finally
		{
			while (ob_get_level() > $nBufferLevel)
			{
				ob_end_clean();
			}
			foreach (array('_GET','_POST','_REQUEST','_COOKIE','_FILES','_SERVER') as $strKey)
			{
				if (array_key_exists($strKey,$arraySavedGlobals))
				{
					$GLOBALS[$strKey] = $arraySavedGlobals[$strKey];
				}
				else
				{
					unset($GLOBALS[$strKey]);
				}
			}
			chdir($strWorkingDir);
			set_include_path($strIncludePath);
			date_default_timezone_set($strTimeZone);
			foreach ($arrayIni as $strKey => $strValue)
			{
				if (ini_get($strKey) !== $strValue)
				{
					ini_set($strKey,$strValue);
				}
			}
			error_reporting($nErrorReporting);
			if (!headers_sent())
			{
				header_remove();
				foreach ($arrayHeaders as $strHeader)
				{
					header($strHeader,false);
				}
				if ($nStatus !== false)
				{
					http_response_code($nStatus);
				}
			}
			CWebServiceTransportContext::$nDepth--;
		}
	}
