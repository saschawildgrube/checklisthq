<?php

	require_once(GetWDKDir().'wdk_websitesatellite.inc');
	require_once(GetWDKDir().'modules/cms/staticfilebrowser/module_staticfilebrowser.inc');

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('Static file browser classification and containment');
		}

		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);
			foreach (array('readme.txt','script.BAT','source.php','source.inc','app.tsx','data.json','.htaccess') as $strFile)
			{
				if (!CStaticFileBrowserModule::IsTextFile($strFile))
				{
					$this->SetResult(false);
				}
			}
			foreach (array('image.png','archive.zip','app.exe','file.pdf','file.txt.png','unknown') as $strFile)
			{
				if (CStaticFileBrowserModule::IsTextFile($strFile))
				{
					$this->SetResult(false);
				}
			}
			foreach (array('env.cfg','nested/env.cfg','nested\\ENV.CFG','env.production.cfg','google123.html','.htaccess') as $strFile)
			{
				if (!CStaticFileBrowserModule::IsBlockedFile($strFile,array('/^env.*\.cfg$/i','/^google.*\.html$/i','/^\.htaccess$/i')))
				{
					$this->SetResult(false);
				}
			}
			if (CStaticFileBrowserModule::IsBlockedFile('env.cfg.txt',array('/^env.*\.cfg$/i','/^google.*\.html$/i','/^\.htaccess$/i')) ||
				CStaticFileBrowserModule::IsBlockedFile('google.html.txt',array('/^google.*\.html$/i')) ||
				CStaticFileBrowserModule::IsBlockedFile('other.cfg',array('/^env.*\.cfg$/i')) ||
				CStaticFileBrowserModule::IsBlockedFile('env.cfg',array()) ||
				CStaticFileBrowserModule::ResolveFile(dirname(__FILE__),basename(__FILE__),array('/^test_basic\.php$/i')) !== false)
			{
				$this->SetResult(false);
			}
			$strRoot = dirname(__FILE__);
			if (CStaticFileBrowserModule::ResolveFile($strRoot,basename(__FILE__)) !== realpath(__FILE__) ||
				CStaticFileBrowserModule::ResolveFile($strRoot,'../wdk-wikipad/test_sections.php') !== false ||
				CStaticFileBrowserModule::ResolveFile($strRoot,'missing.txt') !== false ||
				CStaticFileBrowserModule::ResolveFile($strRoot,"bad\0.txt") !== false ||
				CStaticFileBrowserModule::ResolveFile($strRoot,'.') !== false)
			{
				$this->SetResult(false);
			}
		}
	}
