<?php

	class CMyContent extends CContentLogin
	{
		function OnRenderContent(&$arrayItems)
		{
			$arrayConfig = array();
			$arrayConfig['columns'] = 100;    
			$arrayConfig['lines'] = 15;
			$arrayConfig['userspecific'] = true;
			$arrayConfig['showsubmitbuttonsaveandstay'] = true;
			$arrayConfig['defaultcontent'] = $this->GetWebsite()->LoadString('TID_CONTENT_PROJECTDEFAULT');
 
			$this->GetWebsite()->IncludeModule('cms/wikipad');
			$wikipad = new CWikiPadModule(
				$this->GetWebsite(),
				'project',
				$arrayConfig);
			$arrayItems['MODULE_WIKIPAD'] = $wikipad->RenderModule();


			// Parse the wiki-syntax to extract the project tree

			function GetLevelFromWikiLine($strLine)
			{
				$nLineLevel = 0;
				if (FindString($strLine,'===') == 0)
				{
					$nLineLevel = 3;
				}
				else if (FindString($strLine,'==') == 0)
				{
					$nLineLevel = 2;
				}
				else if (FindString($strLine,'=') == 0)
				{
					$nLineLevel = 1;
				}
				if (FindString($strLine,'***') == 0)
				{
					$nLineLevel = 6;
				}
				else if (FindString($strLine,'**') == 0)
				{
					$nLineLevel = 5;
				}
				else if (FindString($strLine,'*') == 0)
				{
					$nLineLevel = 4;
				}
				return $nLineLevel;
			}
			
	
			$strContent = $wikipad->GetData($wikipad->GetDataName());	

			$arrayCurrent = array();
			$nCurrentLevel = 0;

			$arrayNodes = array();
	
			$arrayLines = StringExplode($strContent,"\n",true);
			
			$nID = 0;
			foreach ($arrayLines as $strLine)
			{
				$strLine = StringTrim($strLine);
				$nLevel = GetLevelFromWikiLine($strLine);
				if ($nLevel != -1)
				{
					$arrayNode = array();
					$arrayNode['id'] = ''.$nID++;
					$arrayNode['name'] = StringRemoveCharacters($strLine,'=*');
					$arrayNode['level'] = $nLevel;
					$arrayNodes[] = $arrayNode;
				}								
			}

			// Derive links		

			$arrayLinks = array();
			
			$nCurrentIndex = 0;
			foreach ($arrayNodes as $arrayNode)
			{
				$nLevel = ArrayGetValue($arrayNode,'level');
				if ($nLevel > 0)
				{
					$nIndex = $nCurrentIndex - 1;
					while ($nIndex >= 0)
					{
						$arrayPreviousNode = $arrayNodes[$nIndex];
						$nPreviousLevel = ArrayGetValue($arrayPreviousNode,'level');
						if ($nPreviousLevel < $nLevel)
						{
							$arrayLink = array();
							$arrayLink['source'] = ArrayGetValue($arrayNode,'id');
							$arrayLink['target'] = ArrayGetValue($arrayPreviousNode,'id');
							$arrayLink['strength'] = 0.50; 
							$arrayLinks[] = $arrayLink;
							break;	
						}
						$nIndex--;	
					}
					
					
				}	
				
				$nCurrentIndex++;	
			}
			
			// Render the graph
		
			$arrayData = array();	
			$arrayData['nodes'] = $arrayNodes;
			$arrayData['links'] = $arrayLinks;
			
			$this->IncludeElement('network');
			$network = new CNetworkElement($this->GetWebsite(),'default');

			$arrayItems['PROJECT_GRAPH'] = $network->RenderNetwork($arrayData);


		}
	}
	
