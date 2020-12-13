<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$arrayItems["MODULE_LOGIN"] = $this->GetWebsite()->m_moduleLogin->RenderModule("form");
		}
	}
