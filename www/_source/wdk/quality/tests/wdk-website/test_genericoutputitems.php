<?php
	
	require_once(GetWDKDir().'wdk_unittest.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('Test Generic Output Items');
		}
		
		function OnInit()
		{
			parent::OnInit(); 
			return true;
		}
		
		function TestCase_GenericOutputItem($strContent,$arrayExpectedOutput)
		{
			$this->TestCase_CheckURL(
				$strTestWebsiteURL = 'http://'.GetRootURL().'quality/testwebsite/?content='.$strContent,
				$arrayExpectedOutput);
			
			$this->Trace('');	
			$this->Trace('');	
			
		}
		
		function OnTest()
		{
			parent::OnTest();
			
			$this->SetResult(true);
			
			$this->TestCase_GenericOutputItem(
				'test-genericoutputitem',
				array(
					HtmlEncode('http://'.GetRootURL().'quality/testwebsite/en'),
					HtmlEncode('http://'.GetRootURL().'quality/testwebsite/en/'),
					HtmlEncode('http://'.GetRootURL().'quality/testwebsite/?layout=default&command=image&id=test'),
					'<i class="fa fa-test fa-fw" aria-hidden="true"></i>'
				)
				);

				
				
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			return true;
		}
		
		
	}
	
	
		


		
