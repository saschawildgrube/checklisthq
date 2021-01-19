
	$this->IncludeElement('googlemaps');
	$googlemap = new CGoogleMapsElement($this->GetWebsite(),'default');
	$strOutput = $googlemap->RenderGoogleMaps(51.477904,0,5, 'satellite');
	$strOutput .= $googlemap->RenderGoogleMaps(50.046088,8.574422,10,'terrain');


		
		/*
		"Googleplex, 1600 Amphitheatre Pkwy, Mountain View, CA 94043");
		*/
