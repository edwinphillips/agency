<?php

//Callback for image being uploaded
function agency_property_image_upload_callback($form, &$form_state){

	$pid = $form_state['values']['pid'];

  $file = file_load($form_state['values']['image']);

  if ($file){

  	if (agency_property_media_create($pid, $file->fid, 'image')){
	  	
	  	$file->status = FILE_STATUS_PERMANENT;

	  	file_save($file);

  	}

  }

  unset($form['images']['upload']['image']['#value']);
  unset($form['images']['upload']['image']['#file']);
  unset($form['images']['upload']['image']['fid']);
  unset($form['images']['upload']['image']['filename']);

  $commands = array();
  $commands[] = ajax_command_replace('#agency-images-table', '<div id="agency-images-table">'. agency_property_images_table($pid) . '</div>');
  $commands[] = ajax_command_replace('#agency-images-upload', '<div id="agency-images-upload">'. drupal_render($form['images']['upload']) . '</div>');

  $page = array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
  ajax_deliver($page);
}

//Callback for image position being changed
function agency_property_image_position_callback($direction, $mid, $pid, $type = 'ajax'){

	if ($direction == 'up'){
		agency_property_image_move_up($mid, $pid);
	}else{
		agency_property_image_move_down($mid, $pid);		
	}

  if ($type == 'ajax') {
    $output = '<div id="agency-images-table">'.agency_property_images_table($pid).'</div>';
    $commands = array();
    $commands[] = ajax_command_replace('#agency-images-table', $output);
    $page = array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
    ajax_deliver($page);
  }
  else {
    $output = agency_property_images_table($pid);
    return $output;
  }	
}

//Callback for image being deleted
function agency_property_image_delete_callback($mid, $pid, $type = 'ajax'){

	agency_property_media_delete($mid);

	agency_property_media_reorder_on_delete($pid, 'image');

  if ($type == 'ajax') {
    $output = '<div id="agency-images-table">'.agency_property_images_table($pid).'</div>';
    $commands = array();
    $commands[] = ajax_command_replace('#agency-images-table', $output);
    $page = array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
    ajax_deliver($page);
  }
  else {
    $output = agency_property_images_table($pid);
    return $output;
  }	
}

//Callback for media being uploaded
function agency_property_media_upload_callback($form, &$form_state){

	$pid = $form_state['values']['pid'];

	$type = $form_state['values']['type'];

  $file = file_load($form_state['values']['file']);

  if ($file){

  	if (agency_property_media_create($pid, $file->fid, $type)){
	  	
	  	$file->status = FILE_STATUS_PERMANENT;

	  	file_save($file);	

  	}

  }

  unset($form['media']['upload']['file']['#value']);
  unset($form['media']['upload']['file']['#file']);
  unset($form['media']['upload']['file']['fid']);
  unset($form['media']['upload']['file']['filename']);

  $commands = array();
  $commands[] = ajax_command_replace('#agency-media-table', '<div id="agency-media-table">'. agency_property_media_table($pid) . '</div>');
  $commands[] = ajax_command_replace('#agency-media-upload', '<div id="agency-media-upload">'. drupal_render($form['media']['upload']['file']) . '</div>');

  $page = array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
  ajax_deliver($page);
}

//Callback for media being deleted
function agency_property_media_delete_callback($mid, $pid, $type = 'ajax'){

	agency_property_media_delete($mid);

	agency_property_media_reorder_on_delete($pid, 'media');

  if ($type == 'ajax') {
    $output = '<div id="agency-media-table">'.agency_property_media_table($pid).'</div>';
    $commands = array();
    $commands[] = ajax_command_replace('#agency-media-table', $output);
    $page = array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
    ajax_deliver($page);
  }
  else {
    $output = agency_property_media_table($pid);
    return $output;
  }	
}

//Callback for media position being changed
function agency_property_media_position_callback($direction, $mid, $pid, $type = 'ajax'){

	if ($direction == 'up'){
		agency_property_media_move_up($mid, $pid);
	}else{
		agency_property_media_move_down($mid, $pid);		
	}

  if ($type == 'ajax') {
    $output = '<div id="agency-media-table">'.agency_property_media_table($pid).'</div>';
    $commands = array();
    $commands[] = ajax_command_replace('#agency-media-table', $output);
    $page = array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
    ajax_deliver($page);
  }
  else {
    $output = agency_property_media_table($pid);
    return $output;
  }	
}

//Callback for categorisation being added
function agency_property_categorisations_add_callback($form, &$form_state){
	
	$pid = $form_state['values']['pid'];
	$vid = $form_state['values']['vocabulary'];
	$tid = $form_state['values']['term'];

	if (!agency_property_categorisation_exists($pid,$vid,$tid)){
		agency_property_categorisation_create($pid, $vid, $tid);
	}

	return '<div id="agency-categorisations-table">'. agency_property_categorisations_table($pid) . '</div>';

}

//Callback for categorisation being deleted
function agency_property_categorisation_delete_callback($cid, $pid, $type = 'ajax'){

	agency_property_categorisation_delete($cid);

  if ($type == 'ajax') {
    $output = '<div id="agency-categorisations-table">'.agency_property_categorisations_table($pid).'</div>';
    $commands = array();
    $commands[] = ajax_command_replace('#agency-categorisations-table', $output);
    $page = array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
    ajax_deliver($page);
  }
  else {
    $output = agency_property_categorisations_table($pid);
    return $output;
  }	
}

//Callback to load terms for vocabulary
function agency_property_vocabularies_terms_callback($form, &$form_state){
	return $form['categorisations']['terms'];
}
