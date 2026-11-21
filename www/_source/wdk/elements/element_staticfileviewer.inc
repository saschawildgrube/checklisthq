<?php

	require_once(GetWDKDir()."wdk_element.inc");

	class CStaticFileViewerElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID = "default")
		{
			parent::__construct($website,"staticfileviewer",$strStyleID,false,true);
		}

		static function EncodeText($strText)
		{
			// Keep source tokens literal during subsequent WDK template/resource passes.
			return str_replace(array('{','}','?'),array('&#123;','&#125;','&#63;'),HtmlEncode($strText));
		}

		function RenderStaticFileViewer($strText)
		{
			$this->SetOutputItem("STATICFILEVIEWER_TEXT",self::EncodeText($strText));
			return $this->ReplaceOutputItems($this->GetLayoutHTML());
		}
	}
