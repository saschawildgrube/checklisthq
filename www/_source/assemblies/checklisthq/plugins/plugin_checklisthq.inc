<?php

	//require_once(GetAssemblyDir('checklisthq').'xxx.inc');
	
	class CChecklistHQPlugin extends CWebsitePlugin
	{
		private $m_nContext;
		function __construct(
			$website)
		{
			$this->m_nContextID = 1;   
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
			$this->m_nContextID = GetIntegerValue($nContextID);
		}
		
		function GetContextID()
		{
			return $this->m_nContextID;
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
			$this->Trace("OnCheckCondition($strCondition)");  
			$nPos = FindString($strCondition,'context');
			if ($nPos != -1)
			{
				$nContextID = GetIntegerValue(StringSection($strCondition,7+$nPos));
				$this->Trace("OnCheckCondition($strCondition): ".$nContextID);
				if ($this->GetContextID() == $nContextID)
				{
					return true;
				}
			}
			return parent::OnCheckCondition($strCondition);	
		}		
		
		
		
	}
