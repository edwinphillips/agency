<?php

function agency_property_form($form, &$form_state, $property){

	if (isset($property) && isset($property->pid)){
		$form['pid'] = array(
			'#type' => 'hidden',
			'#value' => $property->pid
		);
	}

  $form['identification'] = array(
  	'#type' => 'fieldset',
  	'#title' => 'Identification &amp; Address',
  	'#collapsible' => TRUE,
  	'#collapsed' => FALSE,
  );

  $form['identification']['reference'] = array(
    '#title' => t('Agent Reference'),
    '#type' => 'textfield',
    '#default_value' => (isset($property) && isset($property->reference)) ? $property->reference : '',
    '#description' => t('Enter agent reference for this property'),
    '#required' => TRUE,
  );


  $form['identification']['address1'] = array(
    '#title' => t('Address Line 1'),
    '#type' => 'textfield',
    '#description' => t('First address line for property'),
    '#default_value' => (isset($property) && isset($property->address1)) ? $property->address1 : '',    
    '#required' => TRUE,
  );

  $form['identification']['address2'] = array(
    '#title' => t('Address Line 2'),
    '#type' => 'textfield',
    '#description' => t('Second address line for property'),
    '#default_value' => (isset($property) && isset($property->address2)) ? $property->address2 : '',    
    '#required' => FALSE,
  );  

  $form['identification']['address3'] = array(
    '#title' => t('Address Line 3'),
    '#type' => 'textfield',
    '#description' => t('Third address line for property'),
    '#default_value' => (isset($property) && isset($property->address3)) ? $property->address3 : '',        
    '#required' => FALSE,
  );  

  $form['identification']['towncity'] = array(
    '#title' => t('Town / City'),
    '#type' => 'textfield',
    '#description' => t('Town / City for property'),
    '#default_value' => (isset($property) && isset($property->towncity)) ? $property->towncity : '',        
    '#required' => FALSE,
  );    

  $form['identification']['county'] = array(
    '#title' => t('County'),
    '#type' => 'textfield',
    '#description' => t('County for property'),
    '#default_value' => (isset($property) && isset($property->county)) ? $property->county : '',        
    '#required' => FALSE,
  );      

  $form['identification']['postcode'] = array(
    '#title' => t('Postcode'),
    '#type' => 'textfield',
    '#description' => t('Postcode for property'),
    '#default_value' => (isset($property) && isset($property->postcode)) ? $property->postcode : '',        
    '#required' => TRUE,
  );    

  $form['details'] = array(
  	'#type' => 'fieldset',
  	'#title' => 'Information',
  	'#collapsible' => TRUE,
  	'#collapsed' => TRUE,
  );

  $form['details']['summary'] = array(
    '#title' => t('Summary'),
    '#type' => 'textarea',
    '#description' => t('Summary for property'),
    '#default_value' => (isset($property) && isset($property->summary)) ? $property->summary : '',        
    '#required' => TRUE,
  );

  $form['details']['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#description' => t('Description for property'),
    '#default_value' => (isset($property) && isset($property->description)) ? $property->description : '',        
    '#required' => TRUE,
  );

  $form['details']['bedrooms'] = array(
    '#title' => t('Bedrooms'),
    '#type' => 'textfield',
    '#description' => t('Bedrooms for property'),
    '#default_value' => (isset($property) && isset($property->bedrooms)) ? $property->bedrooms : '',        
    '#required' => TRUE,
  );  

  $form['details']['price'] = array(
    '#title' => t('Price'),
    '#type' => 'textfield',
    '#description' => t('Property Price'),
    '#default_value' => (isset($property) && isset($property->price)) ? $property->price : '',        
    '#required' => TRUE,
  );          

  $form['features'] = array(
  	'#type' => 'fieldset',
  	'#title' => 'Features',
  	'#collapsible' => TRUE,
  	'#collapsed' => TRUE,
  );

  $form['features']['intro'] = array(
  	'#type' => 'markup',
  	'#markup' => 'Please enter upto 10 features for a property that will be displayed as a bulleted list. To remove a feature, just empty the textfield'
  );

  if (isset($property) && isset($property->pid)){
  	$features = agency_property_load_features($property->pid);
	}

  for($i = 0; $i < 10; $i++){

  	$form['features']['feature_'. $i] = array(
  		'#type' => 'textfield',
  		'#title' => 'Feature '. ($i + 1),
  	);

  	if (isset($features) && isset($features[$i])){
  		$form['features']['feature_' . $i]['#default_value'] = $features[$i]->description;
  	}

  }

  //Images
  if (isset($property) && isset($property->pid)){

	  $form['images'] = array(
	  	'#type' => 'fieldset',
	  	'#title' => 'Images',
	  	'#collapsible' => TRUE,
	  	'#collapsed' => TRUE,
	  );

	  $form['images']['table'] = array(
	  	'#type' => 'markup',
	  	'#markup' => agency_property_images_table($property->pid),
	  	'#prefix' => '<div id="agency-images-table">',
	  	'#suffix' => '</div>',
	  );

	  $form['images']['upload'] = array(
	  	'#type' => 'container',
	  	'#prefix' => '<div id="agency-images-upload">',
	  	'#suffix' => '</div>',
	  );

	  $form['images']['upload']['image'] = array(
	  	'#title' => 'Select image to upload',
	  	'#type' => 'managed_file',
	    '#description' => t('Choose the file type to upload - only jpg, png are permitted'),
	    '#upload_location' => 'public://agency/',
	    '#upload_validators' => array(
	      'file_validate_extensions' => array('doc docx pdf jpg png mp4'),
	    ),  	
	  );

	  $form['images']['button'] = array(
	  	'#type' => 'button',
	  	'#value' => 'Save Upload',
	  	'#name' => 'upload-image-button',	  	
	  	'#ajax' => array(
	  		'callback' => 'agency_property_image_upload_callback',
	  		'wrapper' => 'agency-images-table',
	  	)
	  );

	}

	//Media
  if (isset($property) && isset($property->pid)){

	  $form['media'] = array(
	  	'#type' => 'fieldset',
	  	'#title' => 'Media',
	  	'#collapsible' => TRUE,
	  	'#collapsed' => TRUE,
	  );

	  $form['media']['table'] = array(
	  	'#type' => 'markup',
	  	'#markup' => agency_property_media_table($property->pid),
	  	'#prefix' => '<div id="agency-media-table">',
	  	'#suffix' => '</div>',
	  );

	  $form['media']['upload'] = array(
	  	'#type' => 'container',
	  	'#prefix' => '<div id="agency-media-upload">',
	  	'#suffix' => '</div>',
	  );

	  $form['media']['upload']['file'] = array(
	  	'#title' => 'Select file to upload',
	  	'#type' => 'managed_file',
	    '#description' => t('Choose the file type to upload - only doc, docx, pdf, jpg, png and mp4 are permitted'),
	    '#upload_location' => 'public://agency/',
	    '#upload_validators' => array(
	      'file_validate_extensions' => array('doc docx pdf jpg png mp4'),
	    ),  	
	  );

	  $form['media']['type'] = array(
	  	'#type' => 'select',
	  	'#title' => 'File Type',
	  	'#options' => array(
	  		'epc' => 'EPC',
	  		'floorplan' => 'Floorplan',
	  		'brochure' => 'Brochure',
	  		'video' => 'Video'
	  	),
	  	'#required' => FALSE,
	  );

	  $form['media']['button'] = array(
	  	'#type' => 'button',
	  	'#value' => 'Save Upload',
	  	'#name' => 'upload-media-button',
	  	'#ajax' => array(
	  		'callback' => 'agency_property_media_upload_callback',
	  		'wrapper' => 'agency-media-table',
	  	)
	  );

	}

	//Categorisations
  if (isset($property) && isset($property->pid)){

	  $form['categorisations'] = array(
	  	'#type' => 'fieldset',
	  	'#title' => 'Categorisation',
	  	'#collapsible' => TRUE,
	  	'#collapsed' => TRUE,
	  );

	  $form['categorisations']['table'] = array(
	  	'#type' => 'markup',
	  	'#markup' => agency_property_categorisations_table($property->pid),
	  	'#prefix' => '<div id="agency-categorisations-table">',
	  	'#suffix' => '</div>',
	  );

	  $form['categorisations']['vocabulary'] = array(
	  	'#type' => 'select',
	  	'#title' => 'Vocabulary',
	  	'#options' => agency_property_vocabularies_for_select(),
	  	'#required' => FALSE,
	  	'#ajax' => array(
	  		'event' => 'change',
	  		'callback' => 'agency_property_vocabularies_terms_callback',
	  		'wrapper' => 'agency-categorisations-terms',
	  		'method' => 'replace',
	  	),
	  );

	  $form['categorisations']['terms'] = array(
	  	'#type' => 'container',
	  	'#prefix' => '<div id="agency-categorisations-terms">',
	  	'#suffix' => '</div>',
	  );

	  if (isset($form_state['values']['vocabulary'])){

		  $form['categorisations']['terms']['term'] = array(
		  	'#type' => 'select',
		  	'#title' => 'Term',
		  	'#options' => agency_property_vocabulary_terms_for_select($form_state['values']['vocabulary']),
		  	'#required' => FALSE,
		  );

	  }

	  $form['categorisations']['button'] = array(
	  	'#type' => 'button',
	  	'#value' => 'Add Categorisation',
	  	'#name' => 'add-categorisation-button',	  	
	  	'#ajax' => array(
	  		'callback' => 'agency_property_categorisations_add_callback',
	  		'wrapper' => 'agency-categorisations-table',
	  	)
	  );

	}

  $form['website'] = array(
  	'#type' => 'fieldset',
  	'#title' => 'Website',
  	'#collapsible' => TRUE,
  	'#collapsed' => TRUE,
  );

  $form['website']['status'] = array(
    '#title' => t('Status'),
    '#type' => 'select',
    '#options' => agency_property_status_for_select(),
    '#description' => t('Select the website status for this property'),
    '#default_value' => (isset($property) && isset($property->status)) ? $property->status : '',        
    '#required' => TRUE,
  );  

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );  

	return $form;

}

function agency_property_form_submit($form,&$form_state){

	$property = entity_ui_form_submit_build_entity($form, $form_state);

	if ($property){

		$location = agency_property_geocode($form_state['values']['postcode']);

		if ($location){

			$property->longitude = $location['longitude'];
			$property->latitude = $location['latitude'];

			//@TODO
			$property->expires = null;

			if ($property->save()){

				agency_property_delete_all_features($property->pid);

				$features = array();

				for($i = 0; $i < 10; $i++){
					if (isset($form_state['values']['feature_' . $i]) && !empty($form_state['values']['feature_' . $i])){
						$features[] = array(
							'pid' => $property->pid,
							'description' => $form_state['values']['feature_' . $i],
						);
					}
				}

				if ($features){
					agency_property_create_features($features);
				}

			  drupal_set_message(t('Property has been created'));

			  $form_state['redirect'] = 'admin/agency/property/manage/' . $property->pid;

			}

		}

	}

}