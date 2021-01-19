// GoogleMapsReactComponent
'use strict';

class GoogleMapsReactComponent extends React.Component
{
  constructor(props)
  {
    super(props);
  	this.m_strId = 'googlemaps_'+GetRandomToken(5);  
  }
  
  componentDidMount()
  {
  	this.RenderGoogleMaps(''+this.m_strId, this.props);
  }

  render()
  {
  	var nHeight = 600;   
  	var nWidth = 800;
  	if (this.props['height'] > 0)
  	{
  		nHeight = this.props['height'];
  	}
   	if (this.props['width'] > 0)
  	{
  		nWidth = this.props['width'];
  	}
  	return e(
  		'div', 
  		{
  			id: this.m_strId,
  			style:
  			{
  				height: nHeight,
  				width: nWidth
  			}
  		},
  		null);
  }
  
	RenderGoogleMaps(strSelector,aProps)
	{
			
		var fLat = GetValue(aProps,'lat');
		if (fLat == null)
		{
			fLat = 30;
		}
		var fLong = GetValue(aProps,'long');
		if (fLong == null)
		{
			fLong = 0;
		}
			
		var nZoom = GetValue(aProps,'zoom');
		if (nZoom == null)
		{
			nZoom = 2;
		}
			
		var strType = GetValue(aProps,'type');
		if (strType == null)
		{
			strType = 'terrain';
		}
	
		var map = new google.maps.Map(
			document.getElementById(strSelector),
			{
				zoom: nZoom, 
				center: new google.maps.LatLng(fLat,fLong),  
				mapTypeId: strType,
				mapTypeControl: true,
				streetViewControl: true
			});
	}
  
}

