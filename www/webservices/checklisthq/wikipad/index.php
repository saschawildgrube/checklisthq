<?php
	
	error_reporting (E_ALL);
	ini_set('display_errors', 1);
	
	require_once ('../../../_source/env.inc');
	require_once (GetSourceDir().'webservices_directory.inc');
	
	$strServiceDir = GetWebservicesDir(). '/system/data/';
	$strServiceSourceDir = GetSourceDir().'assemblies/checklisthq/webservices/checklisthq/wikipad/';
	require_once ($strServiceSourceDir.'webservice_wikipad.inc');
	
	$arrayConfig = array();
	$arrayConfig['protocols'] = array('http','https');
	$arrayConfig['admin_email'] = GetAdminEmail();
	$arrayConfig['webservices'] = GetWebservicesDirectory();
		
	 
	$arrayParams = array();
	//$arrayParams ['trace'] = '1';
	
	$webservice = new CWikipadWebService(
		$strServiceSourceDir,
		$strServiceDir,
		$arrayConfig,
		$arrayParams);
		
