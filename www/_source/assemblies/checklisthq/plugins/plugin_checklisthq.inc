<?php

	//require_once(GetAssemblyDir('checklisthq').'xxx.inc');
	
	class CChecklistHQPlugin extends CWebsitePlugin
	{
		private $m_nContext;
		function __construct(
			$website)
		{
			parent::__construct(
				$website,
				'checklisthq');
		}
		
		function OnInit()
		{
			return parent::OnInit();
		}
		
		function SetContextID($nContextID)
		{
			$this->SetVar('checklisthq_context',GetIntegerValue($nContextID));
		}
		
		function GetContextID()
		{
			return GetIntegerValue($this->GetVar('checklisthq_context'));
		}
		
		function GetContextName($nContext)
		{
			$nContext = GetIntegerValue($nContext);
			switch ($nContext)
			{
			case 0:
				return $this->GetWebsite()->LoadString('TID_CONTEXT_PERSONAL');
			case 1:
				return $this->GetWebsite()->LoadString('TID_CONTEXT_OFFICE');
			default:
				return 'UNKNOWN';
			}
		}

		function GetWikiPageID($strWikiPageID)
		{
			if ($this->GetContextID() == 0)
			{
				return $strWikiPageID;
			}
			else
			{
				return $strWikiPageID . '_' . $this->GetContextID();	
			}
		}
		
		function OnCheckCondition($strCondition)
		{
			$nPos = FindString($strCondition,'context');
			if ($nPos != -1)
			{
				$nContextID = GetIntegerValue(StringSection($strCondition,7+$nPos));
				if ($this->GetContextID() == $nContextID)
				{
					return true;
				}
			}
			return parent::OnCheckCondition($strCondition);	
		}		
		
		
		
	}
