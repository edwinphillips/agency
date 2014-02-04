<?php

function agency_properties_for_select(){
	
	$data = array();

	$query = new EntityFieldQuery();

	$query->entityCondition('entity_type', 'agency_property');

	$result = $query->execute();

	if (isset($result['agency_property'])){
		$property_nids = array_keys($result['agency_property']);
		$properties = entity_load('agency_property',$property_nids);

		foreach($properties as $property){
			$data[$property->pid] = $property->address1 . ' - ' . $property->towncity . ' - ' . $property->postcode;
		}
	}

	return $data;

}

function agency_property_media_type_for_select(){

	$data = array(
		'image' => 'Image',
		'plan' => 'Floor Plan',
		'brochure' => 'Brochure',
		'epc' => 'EPC'
	);

	return $data;

}

function agency_media_status_for_select(){

	$data = array(
		1 => 'Visible',
		0 => 'Hidden',
	);

	return $data;

}

function agency_property_status_for_select(){

	$data = array(
		1 => 'Visible',
		0 => 'Hidden',
	);

	return $data;

}

function agency_property_vocabularies_for_select(){

	$vocabularies = taxonomy_get_vocabularies();

	$options = array();

	$options[0] = '-- Please choose --';

	foreach($vocabularies as $vocabulary){
		$options[$vocabulary->vid] = $vocabulary->name;
	}

	return $options;
}

function agency_property_vocabulary_terms_for_select($vid = 0){

	$terms = taxonomy_get_tree($vid);

	$options = array();

	$options[0] = '-- Please choose --';

	foreach($terms as $term){
		$options[$term->tid] = $term->name;
	}

	return $options;

}