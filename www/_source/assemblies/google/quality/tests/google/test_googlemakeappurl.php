<?php
	
	require_once(GetAssemblyDir('google').'google_appurl.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringSection");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_Google_MakeAppURL($strAppID,$strEmail,$strExpectedResult)
		{
			$this->Trace("TestCase_Google_MakeAppURL");
			$this->Trace("strAppID	       : \"$strAppID\"");
			$this->Trace("strEmail	       : \"$strEmail\"");
			$this->Trace("Expected Result  : \"$strExpectedResult\"");
			$strResult = Google_MakeAppURL($strAppID,$strEmail); 
			$this->Trace("Google_MakeAppURL: \"$strResult\"");
			if ($strResult == $strExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
			}
			$this->Trace("");
		}


		function CallbackTest()
		{
			parent::CallbackTest(); 

			$this->TestCase_Google_MakeAppURL(
				'mail',
				'',
				'https://mail.google.com/mail/u/');

			$this->TestCase_Google_MakeAppURL(
				'mail',
				'johndoe@example.com',
				'https://accounts.google.com/AccountChooser?continue=https%3A%2F%2Fmail.google.com%2Fmail%2Fu%2F%3Fauthuser%3Djohndoe%40example.com&service=mail&Email=johndoe@example.com');

			$this->TestCase_Google_MakeAppURL(
				'calendar',
				'johndoe@example.com',
				'https://accounts.google.com/AccountChooser?continue=https%3A%2F%2Fcalendar.google.com%2Fcalendar%2F%3Fauthuser%3Djohndoe%40example.com&Email=johndoe@example.com');


		
		}
		

	}
	
	

		
