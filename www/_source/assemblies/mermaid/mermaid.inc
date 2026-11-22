<?php

	function ActivateMermaid(CWebsite $website)
	{
		$website->AddJavaScript('mermaid-sdk', 'mermaid-12.3.0.min', false);
		$website->AddJavaScript('mermaid');
		$website->AddStyleSheet('mermaid');

		return true;
	}
