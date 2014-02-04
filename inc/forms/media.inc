<?php

function agency_media_form($form, &$form_state, $media){
	
	$form = array();

  $form['pid'] = array(
    '#title' => t('Property'),
    '#type' => 'select',
    '#options' => agency_properties_for_select(),
    '#description' => t('Select the property to assign this media item to'),
    '#default_value' => (isset($media) && isset($media->pid)) ? $media->pid : '',        
    '#required' => TRUE,
  );

  $form['type'] = array(
    '#title' => t('Type'),
    '#type' => 'select',
    '#options' => agency_property_media_type_for_select(),
    '#description' => t('Select the type of media this is'),
    '#default_value' => (isset($media) && isset($media->type)) ? $media->type : '',            
    '#required' => TRUE,
  );


  $form['file'] = array(
    '#title' => t('Media File'),
    '#type' => 'managed_file',
    '#description' => t('Choose the file type to upload - only doc,docx,pdf,jpg,png and mp4 are permitted'),
    '#upload_location' => 'public://agency/',
    '#default_value' => (isset($media) && isset($media->fid)) ? $media->fid : '',        
    '#required' => TRUE,
    '#upload_validators' => array(
      'file_validate_extensions' => array('doc docx pdf jpg png mp4'),
    ),
  );

  $form['status'] = array(
    '#title' => t('Status'),
    '#type' => 'select',
    '#options' => agency_media_status_for_select(),
    '#description' => t('Select the website status for this media item'),
    '#default_value' => (isset($media) && isset($media->status)) ? $media->status : '',        
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


function agency_media_form_submit($form,&$form_state){

  $file = file_load($form_state['values']['file']);

  if ($file){

  	$file->status = FILE_STATUS_PERMANENT;

  	if (file_save($file)){

			$media = entity_ui_form_submit_build_entity($form, $form_state);

		  $media->fid = $file->fid;

		  //@TODO
		  $media->expires = null;

		  if ($media->save()){

			  drupal_set_message(t('Media has been created / uploaded'));

			  $form_state['redirect'] = 'admin/agency/media';

		  }

  	}

  }

}