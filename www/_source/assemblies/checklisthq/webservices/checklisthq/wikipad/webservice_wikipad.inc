<?php

	require_once(GetWDKDir().'wdk_webservice.inc');
		
  class CWikipadWebService extends CWebService 
	{
			
		function __construct(
			$strSourceDir,
			$strServiceDir,
			$arrayConfig,
			$arrayParams)
		{
			$arrayMeta = array();
			$arrayMeta['version'] = '0.1'; 
			$arrayMeta['author'] = 'Sascha Wildgrube';
			
			parent::__construct(
				'checklisthq/wikipad',
				$strSourceDir,
				$strServiceDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				array());
		}
		
		function OnInitParameterDefinition(&$paramDef)
		{
			return parent::OnInitParameterDefinition($paramDef);
		}
		
		function OnInitCommandDefinition(&$commandDef)
		{
			$commandDef->AddCommandDefinition('recurringtasks',array());
			return parent::OnInitCommandDefinition($commandDef);
		}
		
		function OnInit()
		{
			return parent::OnInit();
		}		
		
		function OnCommand($strCommand)
		{
			switch ($strCommand)
			{
			case 'recurringtasks':
				$this->CommandRecurringTasks();
				return;

			default:
				parent::OnCommand($strCommand);
				return;
			}
		}

		function CommandInstall()
		{
			$this->Trace("CommandInstall");
			
			parent::CommandInstall();
			if ($this->GetError() != "")
			{
				return;	
			}

			// Set up clean up scheduler job
			$arrayParams = array();
			$arrayParams["command"] = "recurringtasks";
			$this->Install_AddScheduledJob(
				"WikipadRecurringTasks",
				$arrayParams,
				"0",
				"4",
				"*",
				"*",
				"*",
				60);
			
			return;	
		}	

		function CommandRecurringTasks()
		{
			$this->Trace('CommandRecurringTasks');

			$webservice = new CWebServiceConsumerWebApplication($this);
			$nOffset = -1;
			$nMaxOffset = 0;
			do
			{
				$nOffset++;
				
				$this->Trace('Iterating over all wikipad_recurring data items with offset: '.$nOffset);
				
				$arrayParams = array();
				$arrayParams['command'] = 'list';
				$arrayParams['blocksize'] = '1';    
				$arrayParams['offset'] = ''.$nOffset;    
				$arrayParams['filter1'] = 'data_name';
				$arrayParams['filter1_operator'] = '=';
				$arrayParams['filter1_value'] = 'wikipad_recurring';
				$webservice->ConsumeWebService('system/data',$arrayParams);
				if ($webservice->GetError() != '')
				{
					$this->SetError($webservice->GetError());
					return;
				} 
				else 
				{
					$nMaxOffset = $webservice->GetResultValue('MAXOFFSET');
					$arrayRecurringTasksDataItems = $webservice->GetResultValueInsecure('LIST'); 
					
					$this->Trace('Iterating over returned recurring tasks data items: '.ArrayCount($arrayRecurringTasksDataItems));
					
					foreach ($arrayRecurringTasksDataItems as $arrayRecurringTaskDataItem)
					{
						$strUserID = ArrayGetValue($arrayRecurringTaskDataItem,'DATA_CONTEXT_ID');

						$this->Trace('Checking recurring tasks of user: '.$strUserID);
						if ($strUserID != '')
						{
							$strRecurringTasks = ArrayGetValue($arrayRecurringTaskDataItem,'DATA_CONTENT');
							//$strRecurringTasks = StringURLDecode($strRecurringTasks);
							$arrayRecurringTasks = $this->ParseRecurringTasks($strRecurringTasks);
							$this->Trace($arrayRecurringTasks);
						
							if (ArrayCount($arrayRecurringTasks) > 0)
							{
								//$arrayNewTasks = $this->GetMatchingRecurringTasks($arrayRecurringTasks,'2020-12-01 00:00:00');   

								$arrayNewTasks = $this->GetMatchingRecurringTasks($arrayRecurringTasks,RenderDateTimeNow());

								if (ArrayCount($arrayNewTasks) > 0)
								{
									$this->Trace('There are tasks to be added!');
									$this->Trace($arrayNewTasks);
									
				
									$arrayParams = array();
									$arrayParams['command'] = 'get';
									$arrayParams['data_context_type'] = 'USER';
									$arrayParams['data_context_id'] = $strUserID;
									$arrayParams['data_name'] = 'wikipad_tasks';
									$webservice->ConsumeWebService('system/data',$arrayParams);
									if ($webservice->GetError() == '')
									{
										$this->Trace('The user actually has a task data item.');
										$strTasks = $webservice->GetResultValueInsecure('DATA','DATA_CONTENT');
										$this->Trace('Old Task content: '.$strTasks);
										//$strTasks = StringURLDecode($strTasks);
										$this->Trace('Search for "&": '.FindString($strTasks,'&'));

										$strNewTasks = '';
										foreach ($arrayNewTasks as $strNewTask)
										{
											$this->Trace('New task: '.$strNewTask);
											if (FindString($strTasks,$strNewTask) == -1)
											{
												$strNewTasks .= $strNewTask . "\n";
											}
										}

										if ($strNewTasks != '')
										{
											if (FindString($strTasks,'=NEXT ACTION=') == -1)
											{
												$strTasks = $strNewTasks . $strTasks;
											}
											else
											{
												$strTasks = ReplaceString($strTasks,'=NEXT ACTION=',"=NEXT ACTION=\n".$strNewTasks);
											}											
										}
										
										$this->Trace('New Task content:');
										$this->Trace($strTasks);
										
										$this->Trace('Search for "&": '.FindString($strTasks,'&'));
										
										$arrayParams = array();
										$arrayParams['command'] = 'set';
										$arrayParams['data_context_type'] = 'USER';
										$arrayParams['data_context_id'] = $strUserID;
										$arrayParams['data_name'] = 'wikipad_tasks';
										$arrayParams['data_content'] = $strTasks;
										$webservice->ConsumeWebService('system/data',$arrayParams);
										if ($webservice->GetError() != '')
										{
											$this->Trace('Error updating data items: '.$webservice->GetError());
											
										}
									}
								}
							}
						}
					}
				}
				$this->Trace('Max offset: '.$nMaxOffset);
			}
			while ($nOffset < $nMaxOffset);
			return;	
		}
		
		function GetSupportedHeaders()
		{
			$arraySupportedHeaders = array(
				'DAILY',
				
				'MONDAY',
				'TUESDAY',
				'WEDNESDAY',
				'THURSDAY',
				'FRIDAY',
				'SATURDAY',
				'SUNDAY',
				
				'MONTHLY',
				
				'QUARTERLY',
				
				'JANUARY',
				'FEBRUARY',
				'MARCH',
				'APRIL',
				'MAY',
				'JUNE',
				'JULY',
				'AUGUST',
				'SEPTEMBER',
				'OCTOBER',
				'NOVEMBER',
				'DECEMBER');

			return $arraySupportedHeaders;			
		}
		
		function ParseRecurringTasks($strRecurringTasks)
		{
			$arraySupportedHeaders = $this->GetSupportedHeaders();

			$arrayRecurringTasks = array();
			 
			$arrayLines = StringExplode($strRecurringTasks,"\n",true);      
			
			$strHeader = '';
			foreach ($arrayLines as $strLine)
			{
				$this->Trace('ParseRecurringTasks: '.$strLine);
				if (StringSection($strLine,0,1) == '=')
				{
					$strHeader = '';
					foreach ($arraySupportedHeaders as $strSupportedHeader)
					{
						if (FindStringIgnoreCase($strLine,'='.$strSupportedHeader.'=') == 0)
						{
							$strHeader = $strSupportedHeader;
							continue;
						}	
					}
				}
				else
				{
					if (StringSection($strLine,0,1) == '*')
					{
						if ($strHeader != '')
						{
							$arrayRecurringTasks[$strHeader][] = $strLine;
						}
					}	
				}
			}	
			
			return $arrayRecurringTasks;
		}
		
		function GetTimeDayBegin($time)
		{
			return GetTime(RenderDateTime($time));
		}
		
		
		function GetMatchingRecurringTasks($arrayRecurringTasks,$strDateTimeNow)
		{
			$this->Trace('GetMatchingRecurringTasks: datetime: '.$strDateTimeNow);
			$arrayTasks = array();
			
			$timeNow = $this->GetTimeDayBegin($strDateTimeNow);
			$arrayTimeNow = ParseDateTime(RenderDateTime($timeNow));
			$this->Trace('ParseDateTime');
			$this->Trace($arrayTimeNow);
			$nDay = ArrayGetValue($arrayTimeNow,'DAY');
			$nMonth = ArrayGetValue($arrayTimeNow,'MONTH');
			$arraySupportedHeaders = $this->GetSupportedHeaders();
			

			// Daily
			$arrayTasks = ArrayAppend($arrayTasks,ArrayGetValue($arrayRecurringTasks,'DAILY'));
			$this->Trace('GetMatchingRecurringTasks: After daily:');
			$this->Trace($arrayTasks);
						
			// Weekly
			$nWeekDay = ArrayGetValue($arrayTimeNow,'WEEKDAY');
			$this->Trace('Weekyday: '.$nWeekDay);
			$strWeekdayHeader = $arraySupportedHeaders[$nWeekDay+0];  
			$this->Trace('Weekday header: '.$strWeekdayHeader);
			$arrayTasks = ArrayAppend($arrayTasks,ArrayGetValue($arrayRecurringTasks,$strWeekdayHeader));
			$this->Trace('GetMatchingRecurringTasks: After weekly:');
			$this->Trace($arrayTasks);	
						
			// Monthly
			if ($nDay == 1)
			{
				$arrayTasks = ArrayAppend($arrayTasks,ArrayGetValue($arrayRecurringTasks,'MONTHLY'));
			}
			$this->Trace('GetMatchingRecurringTasks: After monthly:');
			$this->Trace($arrayTasks);	
			
			// Quarterly
			if ($nDay == 1)
			{
				if (($nMonth-1 % 3) == 0)
				{
					$arrayTasks = ArrayAppend($arrayTasks,ArrayGetValue($arrayRecurringTasks,'QUARTERLY'));
					
				}
			}
			$this->Trace('GetMatchingRecurringTasks: After quarterly:');
			$this->Trace($arrayTasks);	

			
			// Annualy
			if ($nDay == 1)
			{
				$nMonth = ArrayGetValue($arrayTimeNow,'MONTH');
				$strMonthHeader = $arraySupportedHeaders[$nMonth+9];
				$this->Trace('Month header: '.$strMonthHeader);
				$arrayTasks = ArrayAppend($arrayTasks,ArrayGetValue($arrayRecurringTasks,$strMonthHeader));
			}
			$this->Trace('GetMatchingRecurringTasks: After annualy:');
			$this->Trace($arrayTasks);	

			
			return $arrayTasks;
			
		}
		
	}
	

