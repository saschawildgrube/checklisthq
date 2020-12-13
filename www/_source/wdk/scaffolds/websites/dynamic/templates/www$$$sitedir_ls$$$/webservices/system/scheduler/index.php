<?php
	
	error_reporting (E_ALL);
	ini_set('display_errors', 1);
	
	require_once ('../../../_source/env.inc');
	require_once (GetSourceDir().'webservices_directory.inc');
	
	$strServiceDir = GetWebservicesDir(). '/system/scheduler/';
	$strServiceSourceDir = GetWDKDir().'webservices/system/scheduler/';
	require_once ($strServiceSourceDir.'webservice_scheduler.inc');

	$arrayConfig = array();
	$arrayConfig['protocols'] = array('http','https');
	$arrayConfig['admin_email'] = GetAdminEmail();
	$arrayConfig['webservices'] = GetWebservicesDirectory();
	$arrayConfig['disable_log'] = true;
	$arrayConfig['max_timeout'] = 90;
	$arrayConfig['crontab_heartbeat'] = true;

	$arrayParams = array();
	//$arrayParams ['trace'] = '1';
	
	$webservice = new CSchedulerWebService(
		$strServiceSourceDir,
		$strServiceDir,
		$arrayConfig,
		$arrayParams);
		
