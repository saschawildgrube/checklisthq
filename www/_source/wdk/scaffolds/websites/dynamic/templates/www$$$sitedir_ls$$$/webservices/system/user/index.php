<?php
	
	error_reporting (E_ALL);
	ini_set('display_errors', 1);
	
	require_once ('../../../_source/env.inc');
	require_once (GetSourceDir().'webservices_directory.inc');
	
	$strServiceDir = GetWebservicesDir(). 'system/user/';
	$strServiceSourceDir = GetWDKDir().'webservices/system/user/';
	require_once ($strServiceSourceDir.'webservice_user.inc');
	
	$arrayConfig = array();
	$arrayConfig['protocols'] = array('http','https');
	$arrayConfig['admin_email'] = GetAdminEmail();
	$arrayConfig['webservices'] = GetWebservicesDirectory();
	$arrayConfig['blacklist'] = $strServiceSourceDir.'blacklist.txt';
	
	$arrayParams = array();
	//$arrayParams ['trace'] = '1';
	
	$webservice = new CUserWebService(
		$strServiceSourceDir,
		$strServiceDir,
		$arrayConfig,
		$arrayParams);
		
