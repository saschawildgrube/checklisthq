<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("ReplaceString");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_ReplaceString($strString,$strFrom,$strTo,$strExpectedResult)
		{
			$this->Trace("TestCase_ReplaceString");
			$this->Trace("strString = \"$strString\", strFrom = \"$strFrom\", strTo = \"$strTo\"");
			$this->Trace("Expected Result:       \"$strExpectedResult\"");
			$strResult = ReplaceString($strString,$strFrom,$strTo);
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

		function TestCase_ReplaceStringIgnoreCase($strString,$strFrom,$strTo,$strExpectedResult)
		{
			$this->Trace("TestCase_ReplaceStringIgnoreCase");
			$this->Trace("strString = \"$strString\", strFrom = \"$strFrom\", strTo = \"$strTo\"");
			$this->Trace("Expected Result:                 \"$strExpectedResult\"");
			$strResult = ReplaceStringIgnoreCase($strString,$strFrom,$strTo);
			$this->Trace("ReplaceStringIgnoreCase returns: \"$strResult\"");
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
			
		
			$this->TestCase_ReplaceString(
				"abcdefghijklmnopqrstuvwxyz",
				"ghi",
				"GHI",
				"abcdefGHIjklmnopqrstuvwxyz");

			$this->TestCase_ReplaceString(
				"Loram ipsum dolor",
				"m",
				"mam",
				"Loramam ipsumam dolor");

			$this->TestCase_ReplaceString(
				u("���"),
				u("�"),
				"AE",
				u("AE��"));
			
			$this->TestCase_ReplaceString(
				u("���"),
				u("�"),
				u("AE"),
				u("AE��"));
			
			$this->TestCase_ReplaceString(
				u("�"),
				u("�"),
				u("AE"),
				u("�"));
	
				
			$this->TestCase_ReplaceString(
				u("123 blubber 123 hello world"),
				u("123"),
				u("dadada"),
				u("dadada blubber dadada hello world"));

			$this->TestCase_ReplaceString(
				u("abcdefg"),
				u("A"),
				u("xxx"),
				u("abcdefg"));

				
			
			$this->TestCase_ReplaceStringIgnoreCase(
				"abcdefGhijklmnopqrstuvwxyz",
				"ghI",
				"GHI",
				"abcdefGHIjklmnopqrstuvwxyz");

			$this->TestCase_ReplaceStringIgnoreCase(
				"Loram ipsuM dolor",
				"M",
				"mam",
				"Loramam ipsumam dolor");
				
			$this->TestCase_ReplaceStringIgnoreCase(
				u("���"),
				u("�"),
				"AE",
				u("AE��"));

			$this->TestCase_ReplaceStringIgnoreCase(
				u("�����"),
				u("�"),
				"AE",
				u("AE��AEAE"));	
				
			$this->TestCase_ReplaceStringIgnoreCase(
				"aaaaa",
				"a",
				"bb",
				u("bbbbbbbbbb"));				

			$this->TestCase_ReplaceStringIgnoreCase(
				u("�����"),
				u("�"),
				u("��"),
				u("����������"));				

			$this->TestCase_ReplaceStringIgnoreCase(
				u("�����"),
				u("�"),
				u("��"),
				u("����������"));				

			$this->TestCase_ReplaceStringIgnoreCase(
				u("�����"),
				u("�"),
				"AE",
				u("AEAEAEAEAE"));		

			$this->TestCase_ReplaceStringIgnoreCase(
				u("�����"),
				u("�"),
				"AE",
				u("AEAE�AEAE"));	

			$this->TestCase_ReplaceStringIgnoreCase(
				u("���"),
				u("�"),
				"&Auml;",
				u("&Auml;��"));	
				
			$this->TestCase_ReplaceStringIgnoreCase(
				u("abC blubber ABC hello world"),
				u("abc"),
				u("dadada"),
				u("dadada blubber dadada hello world"));

			$this->TestCase_ReplaceStringIgnoreCase(
				"<p>This text contains a highlighted word. This is a real highlight! Highlighting is case insensitive.</p>",
				"highlight",
				"<strong>highlight</strong>",
				"<p>This text contains a <strong>highlight</strong>ed word. This is a real <strong>highlight</strong>! <strong>highlight</strong>ing is case insensitive.</p>");

		}
		

	}
	
	

		
