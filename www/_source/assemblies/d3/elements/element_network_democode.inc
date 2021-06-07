$arrayData = array();

$arrayNodes = array(
	array(
		'id' => '1',
		'label' => 'Root',
		'link' => '',
		'type' => 'darkblue'
	),
	array(
		'id' => '2',
		'label' => 'Branch 1',
		'link' => '',
		'type' => 'blue'
	),
	array(
		'id' => '3',
		'label' => 'Branch 2',
		'link' => '',
		'type' => 'blue'
		),
	array(
		'id' => '4',
		'label' => 'Leave 1.1',
		'link' => '',
		'type' => 'lightblue'
		),
	array(
		'id' => '5',
		'label' => 'Leave 1.2',
		'link' => '',
		'type' => 'lightblue'
		),
	array(
		'id' => '6',
		'label' => 'Leave 2.1',
		'link' => '',
		'type' => 'lightblue'
		),
	array(
		'id' => '7',
		'label' => 'Leave 2.2',
		'link' => '',
		'type' => 'lightblue'
		)		
	);

$arrayLinks = array(
	array(
		'source' => '2',
		'target' => '1',
		'strength' => 1.0
		),
	array(
		'source' => '3',
		'target' => '1',
		'strength' => 1.0
		),
	array(
		'source' => '4',
		'target' => '2',
		'strength' => .75
		),
	array(
		'source' => '5',
		'target' => '2',
		'strength' => .75
		),
	array(
		'source' => '6',
		'target' => '3',
		'strength' => .75
		),
	array(
		'source' => '7',
		'target' => '3',
		'strength' => .75
		)
	);
	
$arrayData['nodes'] = $arrayNodes;
$arrayData['links'] = $arrayLinks;
			
$this->IncludeElement('network');
$network = new CNetworkElement($this->GetWebsite(),'default');
$strOutput = $network->RenderNetwork($arrayData);
