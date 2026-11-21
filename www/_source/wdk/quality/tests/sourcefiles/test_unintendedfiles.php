<?php
	
	require_once(GetWDKDir()."wdk_unittest_recursivefilecheck.inc");
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		private $m_aConflictPatterns = array(
			"conflicted copy", // Dropbox
			"[Conflict]",      // Google Drive
			"[conflicted",     // pCloud
			"(conflicted");    // pCloud

		function __construct()
		{
			parent::__construct("Check for unintended files");
		}
		
		function OnTestCaseCheckFile($strFilePath)
		{
			$bUnintendedFile = false;
			$strExtention = GetExtentionFromPath($strFilePath);
			$strFileName = GetFileNameFromPath($strFilePath);
			
			// No temp files wanted
			if (	$strExtention == "tmp"
				||  $strExtention == "db")
			{
				$bUnintendedFile = true;
			}
			
			// No files allowed that do not match the general file name conventions
			else if (StringCheckCharSet($strFilePath,CHARSET_FILEPATH) == false)
			{
				$bUnintendedFile = true;
			}
			
			// Sync tools sometimes leave conflicted copies.
			foreach ($this->m_aConflictPatterns as $strPattern)
			{
				if (FindString($strFileName,$strPattern) != -1)
				{
					$bUnintendedFile = true;
					break;
				}
			}

			if ($bUnintendedFile == true)
			{
				$this->Trace("$strFilePath");
				$this->SetResult(false);				
			}
		}

		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);
			$this->CheckDocumentRootDirectory();  
		}
	}
	
	

		
