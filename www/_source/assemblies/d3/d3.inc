<?php

	function ActivateD3v7(CWebsite $website)
	{
		$website->AddJavaScript('d3v7','d3-7.9.2.min');

		return true;
	}
