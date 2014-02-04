<?php

function agency_mapping_form($form, &$form_state, $mapping){
	
	$form['name'] = array(
		'#type' => 'select',
		'#title' => 'Name',
		'#description' => 'The name of the import field',
		'#options' => agency_import_keys_for_select(),	
  	'#required' => TRUE,
	);

	$form['data'] = array(
		'#type' => 'textfield',
		'#title' => 'Data',
		'#description' => 'The data to match against',
	  '#required' => TRUE,		
	);

  $form['vid'] = array(
  	'#type' => 'select',
  	'#title' => 'Vocabulary',
  	'#description' => 'The vocabulary to lookup the mapped value in',
  	'#options' => agency_import_vocabularies_for_select(),
	  '#required' => TRUE,
  	'#ajax' => array(
  		'event' => 'change',
  		'callback' => 'agency_import_mapping_vocabularies_terms_callback',
  		'wrapper' => 'agency-import-mapping-terms',
  		'method' => 'replace',
  	),
  );

  $form['agency_import_mapping_terms'] = array(
  	'#type' => 'container',
  	'#prefix' => '<div id="agency-import-mapping-terms">',
  	'#suffix' => '</div>',
  );  

	if (isset($form_state['values']['vid'])){

	  $form['agency_import_mapping_terms']['tid'] = array(
	  	'#type' => 'select',
	  	'#title' => 'Term',
	  	'#options' => agency_import_vocabulary_terms_for_select($form_state['values']['vid']),
	  	'#required' => TRUE,
	  );	

	}

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );  

	return $form;

}

/*function agency_mapping_form_validate($form, &$form_state){
	if ($form_state['values']['name'] == 0){
		form_set_error('name', t('Name of the import field is required'));	
	}
	if ($form_state['values']['vid'] == 0){
		form_set_error('name', t('Vocabulary is required'));	
	}
}*/

//Callback to load terms for vocabulary
function agency_import_mapping_vocabularies_terms_callback($form, &$form_state){
	return $form['agency_import_mapping_terms'];
}

function agency_mapping_form_submit($form,&$form_state){

	$mapping = entity_ui_form_submit_build_entity($form, $form_state);

	if ($mapping){

		if ($mapping->save()){

			  drupal_set_message(t('Mapping has been created'));

			  $form_state['redirect'] = 'admin/agency/mapping';

		}

	}

}