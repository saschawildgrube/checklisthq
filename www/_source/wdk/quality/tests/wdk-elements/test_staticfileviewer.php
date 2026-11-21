<?php

	require_once(GetWDKDir().'wdk_websitesatellite.inc');
	require_once(GetWDKDir().'elements/element_staticfileviewer.inc');

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('Static file viewer literal text');
		}

		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);
			$strSource = "<script>alert('test')</script>\n<?php {TOOLBAR} ?TID_TITLE? & text";
			$strEncoded = CStaticFileViewerElement::EncodeText($strSource);
			if (strpos($strEncoded,'<') !== false || strpos($strEncoded,'{') !== false ||
				strpos($strEncoded,'?') !== false || HtmlDecode($strEncoded) !== $strSource)
			{
				$this->SetResult(false);
			}
		}
	}
