<?php

	function Google_MakeAppURL($strAppId, $strEmail)
	{
		$strAppURL = '';
		switch ($strAppId)
		{
		case 'mail':
			$strAppURL = 'https://mail.google.com/mail/u/';
			break;
		case 'calendar':
			$strAppURL = 'https://calendar.google.com/calendar/';
			break;
		case 'contacts':
			$strAppURL = 'https://contacts.google.com/';
			break;
		case 'drive':
			$strAppURL = 'https://drive.google.com/drive/';
			break;			
		case 'docs':
			$strAppURL = 'https://docs.google.com/';
			break;		
		case 'sheets':
			$strAppURL = 'https://docs.google.com/spreadsheets/';
			break;		
		case 'slides':
			$strAppURL = 'https://docs.google.com/presentation/';
			break;	
		default:
			return false;
				
		}
		
		if (!IsValidEmail($strEmail))
		{
			return $strAppURL;	
		}
		
		$strAppURL = $strAppURL . '?authuser='.$strEmail;
		$strAppURL = URLEncode($strAppURL);
		
		$strServiceParam = '';
		if ($strAppId == 'mail')
		{
			$strServiceParam = '&service='.$strAppId;
		}
		
		$strAppURL = 'https://accounts.google.com/AccountChooser?continue=' . $strAppURL . $strServiceParam . '&Email='.$strEmail;
		
		return $strAppURL;
	}
