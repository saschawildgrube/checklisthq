<?php


	class CWikiPadModule extends CModule
	{
		function __construct(
			CWebSite $website,
			$strWikiPageID = 'default',
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				'cms/wikipad',
				$strWikiPageID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$bResult = parent::OnInit();
			$this->IncludeElement('wiki');
			$this->IncludeElement('tabnavigation');
			
			if ($this->GetConfigIsUserSpecific() == true)
			{
				if ($this->GetWebsite()->GetAuthUserID() == '')
				{
					return false;	
				}
			}
			
			return $bResult;
		}
		
		

		function OnRenderModule($strState)
		{
			if ($this->IsViewOnly() == true)
			{
				$strState = 'view';	
			}
			
			
			switch ($strState)
			{
				case 'submit':
				{
					return $this->StateSubmit();	
				}
				case 'edit':
				{
					return $this->StateEdit();	
				}
				case 'editsection':
					return $this->StateEditSection();
				case 'submitsection':
					return $this->StateSubmitSection();
				case 'view':
				default:	
				{
					return $this->StateView();
				}
			}
		}

		function IsViewOnly()
		{
			return $this->GetModuleConfigBoolean('viewonly');	
		}
		
		function PrepareTabNav($strState)
		{
			if ($this->IsViewOnly() == true)
			{
				$this->SetOutputItem('TABNAV','');
				return;
			}
			
			$tabnavigationDef = new CTabNavigationDefinition();

			if ($this->IsToolVisible('view') == true)
			{
				$arrayParams = array();
	   		$arrayParams[$this->GetModuleVarNameState()] = 'view';
	   		$tabnavigationDef->AddTab(
	      		'view',
	      		'?TID_WIKIPAD_TAB_VIEW?',
	      		$this->GetWebsite()->MakeURL($arrayParams));
			}

			if ($this->IsToolVisible('edit') == true)
			{

				$arrayParams = array();
	   		$arrayParams[$this->GetModuleVarNameState()] = 'edit';
	   		$tabnavigationDef->AddTab(
	      		'edit',
	      		'?TID_WIKIPAD_TAB_EDIT?',
	      		$this->GetWebsite()->MakeURL($arrayParams));
			}

			$tabnav = new CTabNavigationElement(
			   $this->GetWebsite(),
   			'default');

			$this->SetOutputItem('TABNAV',
				$tabnav->RenderTabNavigation(
					$tabnavigationDef,
					$strState));	
		}
		
		function OnIsToolVisible($strToolID)
		{
			return true;	
		}
		
		
		function StateView()
		{
			$this->Trace('StateView');
			
			$this->PrepareTabNav('view');


			$strContent = $this->GetData($this->GetDataName());
			if ($strContent === false)
			{
				$this->SetErrorMessageCode('WIKIPAD_GET');
				$this->SetOutputItem('WIKI','');
				$strContent = '';
			}

			$wiki = new CWikiElement($this->GetWebSite(),'default');
			$this->ConfigureWikiSectionEditing($wiki,$strContent);
			$this->SetOutputItem(
				'WIKI',
				$wiki->RenderWiki(
					$strContent));
	
			return 'view';
		}
		
		
		// Hash stored source before adding view-only controls.
		function ConfigureWikiSectionEditing($wiki,$strContent)
		{
			if (!$this->IsViewOnly() && $this->IsToolVisible('edit'))
			{
				$nSection = 0;
				$strHash = MakeHash($strContent);
				$wiki->SetSectionRenderer(function ($strHtml,$strType) use (&$nSection,$strHash)
				{
					$strURL = $this->GetWebsite()->MakeURL(array(
						$this->GetModuleVarNameState() => 'editsection',
						$this->GetModuleVarName('section') => (string)$nSection++,
						$this->GetModuleVarName('sectionhash') => $strHash),false);
					if ($strType == 'separator')
					{
						$strHtml = '<div style="flex: 1;">'.$strHtml.'</div>';
					}
					return '<div class="wikipad-section" style="display: flex; align-items: baseline; flex-wrap: wrap; column-gap: 0.4em;">'.
						$strHtml.' <a style="margin-left: auto; font-size: inherit; font-weight: normal;" href="'.
						HtmlEncode($strURL).'">?TID_WIKIPAD_TAB_EDIT?</a></div>';
				});
			}
		}

		function StateEdit()
		{
			$this->Trace('StateEdit');

			$strContent = $this->GetModuleVarInsecure('content');
			if ($strContent == '')
			{
				$strContent = $this->GetData($this->GetDataName());
				if ($strContent == false)
				{
					$strContent = '';
				}
			}

			return $this->RenderEditForm($strContent);
		}

		function RenderEditForm($strContent,$strSubmitState = 'submit',$arrayParams = array())
		{
			$this->m_formDef = new CFormDefinition();
			$arrayParams[$this->GetModuleVarNameState()] = $strSubmitState;
			$this->m_formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			$nLines = intval($this->GetModuleConfig('lines'));
			if ($nLines == 0)
			{
				$nLines = 30;
			}
			$nColumns = $this->GetModuleConfig('columns');
			if ($nColumns == 0)
			{
				$nColumns = 70;
			}

			$strContent = ReplaceString($strContent,'<code>','<CODE>');
			$strContent = ReplaceString($strContent,'</code>','</CODE>');

			$bShowSubmitButtonSaveAndStay = $this->GetModuleConfig('showsubmitbuttonsaveandstay');  

			$bShowSubmitButtonOnTop = $this->GetModuleConfig('showsubmitbuttonontop');
			if ($bShowSubmitButtonOnTop == true)
			{
				$this->m_formDef->AddControlButtonSubmit('?TID_WIKIPAD_BUTTON_SAVE?');
				if ($bShowSubmitButtonSaveAndStay == true)
				{
					$this->m_formDef->AddControlButtonSubmit('?TID_WIKIPAD_BUTTON_SAVE_AND_STAY?','SUBMIT-STAY','stay');
				}
			}
			$this->m_formDef->AddControlTextArea('CONTENT',$this->GetModuleVarName('content'),'',$strContent,$nColumns,$nLines); 
			$this->m_formDef->AddControlButtonSubmit('?TID_WIKIPAD_BUTTON_SAVE?','SUBMIT-2');
			if ($bShowSubmitButtonSaveAndStay == true)
			{
				$this->m_formDef->AddControlButtonSubmit('?TID_WIKIPAD_BUTTON_SAVE_AND_STAY?','SUBMIT-STAY-2','stay');
			}
	
			$form = new CFormElement($this->GetWebsite(),'onecolumn');
			
			$this->SetOutputItem(
				'FORM',
				$form->RenderForm(
					$this->m_formDef,
					array()));
					
			$this->PrepareTabNav('edit');
	
			return 'edit';
		}

		function MaskWikiLiteralSections($strContent)
		{
			$mask = function ($match) { return preg_replace('/[^\r\n]/',' ',$match[0]); };
			// The renderer extracts syntax-highlight blocks before processing pre blocks.
			$strContent = preg_replace_callback('/<(syntaxhighlight(?:ing)?)\b[^>]*>.*?<\/\1\s*>/is',$mask,$strContent);
			return preg_replace_callback('/<pre>.*?(?:<\/pre>|$)/s',$mask,$strContent);
		}

		function GetSectionBoundaryType($strLine)
		{
			$strLine = trim($strLine);
			if (substr($strLine,0,4) == '----')
			{
				return 'separator';
			}
			// ParseHeaders accepts the highest matching level, capped at six.
			if (substr($strLine,0,1) == '=' && substr($strLine,-1) == '=')
			{
				return 'heading';
			}
			return '';
		}

		// Each boundary starts an independent section, ending at the next boundary.
		function GetEditableSections($strContent)
		{
			$sections = array();
			$nOffset = 0;
			foreach (explode("\n",$this->MaskWikiLiteralSections($strContent)) as $strLine)
			{
				$strType = $this->GetSectionBoundaryType($strLine);
				if ($strType != '')
				{
					$nPrevious = count($sections) - 1;
					if ($nPrevious >= 0)
					{
						$sections[$nPrevious]['length'] = $nOffset - $sections[$nPrevious]['start'];
					}
					$sections[] = array('start' => $nOffset,'length' => strlen($strContent) - $nOffset,'type' => $strType);
				}
				$nOffset += strlen($strLine) + 1;
			}
			return $sections;
		}

		function StateEditSection($bSubmitted = false)
		{
			if ($this->IsViewOnly() || !$this->IsToolVisible('edit'))
			{
				return $this->StateView();
			}
			$strPage = $this->GetData($this->GetDataName());
			$strSection = $this->GetModuleVar('section');
			$sections = $this->GetEditableSections($strPage === false ? '' : $strPage);
			$strRequestedHash = $this->GetModuleVar('sectionhash');
			if (!$bSubmitted && (!ctype_digit((string)$strSection) || !isset($sections[(int)$strSection])
				|| $strPage === false || ($this->IsModuleParam('sectionhash') && $strRequestedHash != ''
					&& $strRequestedHash !== MakeHash($strPage))))
			{
				$this->SetErrorMessageCode('SECTION');
				return $this->StateView();
			}
			$strHash = $bSubmitted ? $strRequestedHash : MakeHash($strPage);
			$strContent = $bSubmitted ? $this->GetModuleVarInsecure('content') :
				substr($strPage,$sections[(int)$strSection]['start'],$sections[(int)$strSection]['length']);
			// Keep a rejected draft in its form without leaking it to whole-page editing.
			$this->SetModuleVar('content','');
			return $this->RenderEditForm($strContent,'submitsection',array(
				$this->GetModuleVarName('section') => HtmlEncode($strSection),
				$this->GetModuleVarName('sectionhash') => HtmlEncode($strHash)));
		}

		function StateSubmitSection()
		{
			if ($this->IsViewOnly() || !$this->IsToolVisible('edit'))
			{
				return $this->StateView();
			}
			$strPage = $this->GetData($this->GetDataName());
			if ($strPage === false)
			{
				$this->SetErrorMessageCode('WIKIPAD_GET');
				return $this->StateEditSection(true);
			}
			$strSection = $this->GetModuleVar('section');
			$sections = $this->GetEditableSections($strPage);
			if (!$this->IsModuleParam('section') || !$this->IsModuleParam('sectionhash') || !$this->IsModuleParam('content')
				|| !ctype_digit((string)$strSection) || !isset($sections[(int)$strSection])
				|| $this->GetModuleVar('sectionhash') !== MakeHash($strPage))
			{
				$this->SetErrorMessageCode('SECTION');
				return $this->StateEditSection(true);
			}
			$section = $sections[(int)$strSection];
			$strContent = $this->GetModuleVarInsecure('content');
			$strContent = ReplaceString($strContent,'<CODE>','<code>');
			$strContent = ReplaceString($strContent,'</CODE>','</code>');
			$strContent = ReplaceString($strContent,u("—"),'--');
			$strSuffix = substr($strPage,$section['start'] + $section['length']);
			if ($strSuffix !== '' && $strContent !== '' && substr($strContent,-1) !== "\n")
			{
				$strContent .= "\n";
			}
			$strNewPage = substr($strPage,0,$section['start']).$strContent.$strSuffix;
			if (!$this->SetData($this->GetDataName(),$strNewPage))
			{
				$this->SetErrorMessageCode('WIKIPAD_SET');
				return $this->StateEditSection(true);
			}
			// Stay only if the edited content still starts with a real boundary.
			$newSections = $this->GetEditableSections($strNewPage);
			$bStay = $this->GetVar('submit') == 'stay' && $strContent !== ''
				&& isset($newSections[(int)$strSection])
				&& $newSections[(int)$strSection]['start'] == $section['start']
				&& $this->GetSectionBoundaryType(strtok($this->MaskWikiLiteralSections($strContent),"\n")) != '';
			$arrayParams = array(
				$this->GetModuleVarNameState() => $bStay ? 'editsection' : 'view',
				$this->GetModuleVarName('section') => $bStay ? $strSection : '',
				$this->GetModuleVarName('sectionhash') => '',
				$this->GetModuleVarName('content') => '',
				$this->GetModuleVarName('action') => '');
			$this->SetModuleVar('content','');
			$this->SetModuleVar('sectionhash','');
			$this->GetWebsite()->Redirect($this->GetWebsite()->MakeURL($arrayParams));
			return $bStay ? $this->StateEditSection() : $this->StateView();
		}

		function StateSubmit()
		{
			$this->Trace('StateSubmit');
			
			$this->Trace($this->GetAllModuleVars());
			
			$strContent = $this->GetModuleVarInsecure('content');
			
			$strContent = ReplaceString($strContent,'<CODE>','<code>');
			$strContent = ReplaceString($strContent,'</CODE>','</code>');
			
			$strContent = ReplaceString($strContent,u("—"),'--'); 
			
			$this->SetState('');
		
			if ($this->SetData($this->GetDataName(),$strContent) == false)
			{
				$this->SetErrorMessageCode('WIKIPAD_SET');
				return $this->StateEdit();
			}
			$this->SetModuleVar('content','');
			
			$strSubmitValue = $this->GetVar('submit');
			
			if ($strSubmitValue == 'stay')
			{
				$strState = $this->StateEdit();
			}
			else
			{
				$strState = $this->StateView();
			}
			$this->RedirectAfterModification($strState);
			return $strState;
		}

		function RedirectAfterModification($strState = 'view')
		{
			// Reload a read-only request after saving, never the modifying request.
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = $strState;
			$arrayParams[$this->GetModuleVarName('action')] = '';
			$this->GetWebsite()->Redirect($this->GetWebsite()->MakeURL($arrayParams));
		}
		
		
		function GetConfigIsUserSpecific()
		{
			return $this->GetModuleConfigBoolean('userspecific');	
		}
		
		function GetDataName()
		{
			return 'wikipad_'.$this->GetInstanceID();	
		}



		function GetData($strDataName)
		{
			$strDefaultContent = $this->GetModuleConfig('defaultcontent');
			if ($this->GetConfigIsUserSpecific())
			{
				return $this->GetWebsite()->GetAuthUserData($strDataName,$strDefaultContent);
			}
			else
			{
				return $this->GetWebsite()->GetData('','',$strDataName,$strDefaultContent);
			}
		}

		function SetData($strDataName,$strContent)
		{
			if ($this->GetConfigIsUserSpecific())
			{
				return $this->GetWebsite()->SetAuthUserData($strDataName,$strContent);
			}
			else
			{
				return $this->GetWebsite()->SetData('','',$strDataName,$strContent);
			}
			
		}

	}

