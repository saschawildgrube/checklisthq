<?php
	
	require_once(GetWDKDir()."wdk_parameter.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test StringCheckCharset");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		}

		function TestCase_StringCheckCharSet($strCharset,$strToken,$bExpectedResult)
		{
			$this->Trace("TestCase_StringCheckCharSet");
			$this->Trace("strCharset = \"$strCharset\", strToken = \"$strToken\"");
			$this->Trace("Expected Result: ".RenderBool($bExpectedResult)."");
			$bResult = StringCheckCharSet($strToken,$strCharset);
			$this->Trace("StringCheckCharset returns: ".RenderBool($bResult));
			if ($bResult == $bExpectedResult)
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

		
		function OnTest()
		{
			parent::OnTest();
			
			$this->TestCase_StringCheckCharSet(u("���"),u("��������"),true);
			$this->TestCase_StringCheckCharSet(u("��"),u("��������"),false);
			
			$this->TestCase_StringCheckCharSet("qwertzuiopasdfghjklyxcvbnm","read",true);
			$this->TestCase_StringCheckCharSet("qwertzuiopasdfghjklyxcvbnm","read2",false);
			
			$this->TestCase_StringCheckCharSet("1234567890","123",true);
			$this->TestCase_StringCheckCharSet("1234567890","abc",false); 

			$this->TestCase_StringCheckCharSet(CHARSET_TID,"TID_EXAMPLE",true);
			$this->TestCase_StringCheckCharSet(CHARSET_TID,"TID_ExAMPLE",false); 
			
			$this->TestCase_StringCheckCharSet(CHARSET_URL,"https://www.websitedevkit.com/webservices/system/test/?command=cleanup&accesscode=1",true);
			$this->TestCase_StringCheckCharSet(CHARSET_URL,"https://www.websitedevkit.com/webservices/system/test/?command=cleanup&accesscode=1���",false);

			$this->TestCase_StringCheckCharSet(CHARSET_ALPHA.CHARSET_LINEBREAK,"a\nb",true);
			$this->TestCase_StringCheckCharSet(CHARSET_ALPHA.CHARSET_LINEBREAK,'a\nb',false);


			
		}
		
	}
	
	

		
