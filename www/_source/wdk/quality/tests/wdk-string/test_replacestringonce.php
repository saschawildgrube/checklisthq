<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("ReplaceStringOnce");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_ReplaceStringOnce($strString,$strFrom,$strTo,$strExpectedResult)
		{
			$this->Trace("TestCase_ReplaceStringOnce");
			$this->Trace("strString = \"$strString\", strFrom = \"$strFrom\", strTo = \"$strTo\"");
			$this->Trace("Expected Result:       \"$strExpectedResult\"");
			$strResult = ReplaceStringOnce($strString,$strFrom,$strTo);
			$this->Trace("ReplaceString returns: \"$strResult\"");
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

		function TestCase_ReplaceStringOnceIgnoreCase($strString,$strFrom,$strTo,$strExpectedResult)
		{
			$this->Trace("TestCase_ReplaceStringOnceIgnoreCase");
			$this->Trace("strString = \"$strString\", strFrom = \"$strFrom\", strTo = \"$strTo\"");
			$this->Trace("Expected Result:                 \"$strExpectedResult\"");
			$strResult = ReplaceStringOnceIgnoreCase($strString,$strFrom,$strTo);
			$this->Trace("ReplaceStringOnceIgnoreCase returns: \"$strResult\"");
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

		
		function OnTest()
		{
			parent::OnTest();
			
		
			$this->TestCase_ReplaceStringOnce(
				"abcdefghijklmnopqrstuvwxyz",
				"ghi",
				"GHI",
				"abcdefGHIjklmnopqrstuvwxyz");

			$this->TestCase_ReplaceStringOnce(
				"Loram ipsum dolor",
				"m",
				"mam",
				"Loramam ipsum dolor");

			$this->TestCase_ReplaceStringOnce(
				u("���"),
				u("�"),
				"AE",
				u("AE��"));
			
			$this->TestCase_ReplaceStringOnce(
				u("���"),
				u("�"),
				u("AE"),
				u("AE��"));
			
			$this->TestCase_ReplaceStringOnce(
				u("�"),
				u("�"),
				u("AE"),
				u("�"));
	
				
			$this->TestCase_ReplaceStringOnce(
				u("123 blubber 123 hello world"),
				u("123"),
				u("dadada"),
				u("dadada blubber 123 hello world"));
			
			
			$this->TestCase_ReplaceStringOnceIgnoreCase(
				"abcdefGhijklmnopqrstuvwxyz",
				"ghI",
				"GHI",
				"abcdefGHIjklmnopqrstuvwxyz");

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				"Loram ipsuM dolor",
				"M",
				"mam",
				"Loramam ipsuM dolor");
				
			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("���"),
				u("�"),
				"AE",
				u("AE��"));

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("�����"),
				u("�"),
				"AE",
				u("AE����"));	
				
			$this->TestCase_ReplaceStringOnceIgnoreCase(
				"aaaaa",
				"a",
				"bb",
				u("bbaaaa"));				

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("�����"),
				u("�"),
				u("��"),
				u("������"));				

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("�����"),
				u("�"),
				u("��"),
				u("������"));				

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("�����"),
				u("�"),
				"AE",
				u("AE����"));		

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("�����"),
				u("�"),
				"AE",
				u("AE����"));	

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("���"),
				u("�"),
				"&Auml;",
				u("&Auml;��"));	
				
			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("abC blubber ABC hello world"),
				u("abc"),
				u("dadada"),
				u("dadada blubber ABC hello world"));

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				"<p>This text contains a highlighted word. This is a real highlight! Highlighting is case insensitive.</p>",
				"highlight",
				"<strong>highlight</strong>",
				"<p>This text contains a <strong>highlight</strong>ed word. This is a real highlight! Highlighting is case insensitive.</p>");

		}
		

	}
	
	

		
