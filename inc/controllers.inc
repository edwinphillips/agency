<?php

class AgencyPropertyController extends EntityAPIController {

  public function save($entity, DatabaseTransaction $transaction = NULL) {

    if (isset($entity->is_new)) {
      $entity->created = time();
    }

    $entity->updated = time();

    return parent::save($entity, $transaction);
  }

}

class AgencyMediaController extends EntityAPIController {

  public function save($entity, DatabaseTransaction $transaction = NULL) {

    if (isset($entity->is_new)) {
      $entity->created = time();
    }

    $entity->updated = time();

    return parent::save($entity, $transaction);
  }

}

class AgencyCategorisationController extends EntityAPIController {}

class AgencyFeatureController extends EntityAPIController {}

class AgencyPropertyUIController extends EntityDefaultUIController {

	public function overviewForm($form, &$form_state){

		$header = array(
			'pid' => array('data' => t('pid'), 'field' => 'pid'),
			'reference' => array('data' => t('reference'), 'field' => 'reference'),
			'address1' => array('data' => t('address1')),
			'postcode' => array('data' => t('postcode'), 'field' => 'postcode'),
			'status' => array('data' => t('status'), 'field' => 'status'),
 			'operations' => array('data' => t('operations'), 'field' => 'operations'),		
		);

    $options = array();

    $search_reference = !empty($_GET['reference']) ? $_GET['reference'] : NULL;    

    $search_postcode = !empty($_GET['postcode']) ? $_GET['postcode'] : NULL;    

    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', 'agency_property');

    if (!empty($search_reference)) {
    	$query->propertyCondition('reference', $search_reference, '=');
    }

    if (!empty($search_postcode)) {
    	$query->propertyCondition('postcode', $search_postcode, '=');
    }

    // Check for sort order and sort key.
    if (!empty($_GET['sort']) && !empty($_GET['order'])) {
      $sort = strtoupper($_GET['sort']);
      $order = strtolower($_GET['order']);
      $order = str_replace(' ', '_', $order);
      if ($order != 'operations') {
        $query->propertyOrderBy($order, $sort);
      }
    }    

    $query->pager(TOTAL_ITEMS_PER_PAGE);    

    $result = $query->execute();

    $property_results = !empty($result['agency_property']) ? $result['agency_property'] : array();
    $property_results_array = !empty($property_results) ? agency_property_load_multiple(array_keys($property_results)) : array();

    foreach($property_results_array as $pid => $property){

    	$options['property_id-'.$pid] = array(
    		'pid' => $pid,
    		'reference' => $property->reference,
    		'address1' => agency_property_address_string($property),
    		'postcode' => $property->postcode,
    		'status' => ($property->status == 1) ? 'Visible' : 'Hidden',
    		'operations' => 
	        l(t('View'), 'admin/agency/property/' . $property->pid) . ' | ' .
	        l(t('Edit'), ADMIN_AGENCY_PROPERTY_MANAGE_URI . $pid , array('query' => array('destination' => ADMIN_AGENCY_PROPERTY_URI))) . ' | ' .
	        l(t('Delete'), ADMIN_AGENCY_PROPERTY_MANAGE_URI . $pid . '/delete', array('attributes' => array('class' => array('property-delete-' . $property->pid), ), 'query' => array('destination' => ADMIN_AGENCY_PROPERTY_URI))),
     	);

    }

    $form['search'] = array(
      '#type' => 'fieldset',
      '#title' => t('Basic Search'),
      '#collapsible' => TRUE,
    );

    if (!empty($search_reference) || !empty($search_postcode)){
    	$form['search']['#collapsed'] = FALSE;
    }else{
    	$form['search']['#collapsed'] = TRUE;
    }    

    $form['search']['search_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Search'),
    );

    if (!empty($search_reference)){
    	$form['search']['search_text']['#default_value'] = $search_reference;
    }else if (!empty($search_postcode)){
    	$form['search']['search_text']['#default_value'] = $search_postcode;
    }else{
    	$form['search']['search_text']['#default_value'] = '';
    }

    $form['search']['search_type'] = array(
      '#type' => 'select',
      '#title' => t('Search Type'),
      '#options' => array('reference' => 'Reference', 'postcode' => 'Postcode'),
      '#default_value' => !empty($search_postcode) ? 'postcode' : 'reference',
    );

    $form['search']['search_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
    );

    $form['bulk_operations'] = array(
      '#type' => 'fieldset',
      '#title' => t('Bulk Operations'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['bulk_operations']['operations'] = array(
      '#type' => 'select',
      '#options' => array(
        0 => t('Select a bulk operation'),
        'delete' => t('Delete selected property'),
        'hide' => t('Hide selected property'),
        'show' => t('Show selected property'),
      ),
    );

    $form['bulk_operations']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );

    $form['entities'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#attributes' => array('class' => array('entity-sort-table')),
      '#empty' => t('There are no properties.'),
    );	

		$form['pager'] = array('#theme' => 'pager');

    return $form;	
	}

  public function overviewFormSubmit($form, &$form_state) {
    $values = $form_state['input'];
    $property_ids = array();

    if (!empty($values['entities'])) {
      foreach ($values['entities'] as $index => $value) {
        if (!empty($value)) {
          $property_ids[] = str_replace('property_id-', '', $value);
        }
      }

      switch ($values['operations']) {
        case 'delete': {
          drupal_goto('admin/agency/property/bulk/delete/' . implode('|', $property_ids));
          break;
        }
       	case 'hide': {
       		drupal_goto('admin/agency/property/bulk/hide/' . implode('|', $property_ids));
       		break;
       	}
       	case 'show': {
       		drupal_goto('admin/agency/property/bulk/show/' . implode('|', $property_ids));
       		break;
       	}
      }
    }

    switch($values['search_type']){
    	case 'reference':{
    		drupal_goto('admin/agency/property', array('query' => array('reference' => $values['search_text'])));
    		break;
    	}
    	case 'postcode':{
	      drupal_goto('admin/agency/property', array('query' => array('postcode' => $values['search_text'])));    		
    		break;
    	}
    }

  }	

}

class AgencyMediaUIController extends EntityDefaultUIController {

	public function overviewForm($form, &$form_state){

		$header = array(
			'mid' => array('data' => t('mid'), 'field' => 'mid'),
			'filename' => array('data' => t('filename')),
			'property' => array('data' => t('property')),
			'type' => array('data' => t('type'), 'field' => 'type'),
			'status' => array('data'=> t('status'), 'field' => 'status'),
			'operations' => array('data' => t('operations'), 'field' => 'operations'),		
		);

    $options = array();

    $search_property = !empty($_GET['property']) ? $_GET['property'] : NULL;    

    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', 'agency_media');

    if (!empty($search_property)) {
    	$query->propertyCondition('pid', $search_property, '=');
    }

    // Check for sort order and sort key.
    if (!empty($_GET['sort']) && !empty($_GET['order'])) {
      $sort = strtoupper($_GET['sort']);
      $order = strtolower($_GET['order']);
      $order = str_replace(' ', '_', $order);
      if ($order != 'operations') {
        $query->propertyOrderBy($order, $sort);
      }
    }    

    $query->pager(TOTAL_ITEMS_PER_PAGE);    

    $result = $query->execute();

    $media_results = !empty($result['agency_media']) ? $result['agency_media'] : array();
    $media_results_array = !empty($media_results) ? agency_media_load_multiple(array_keys($media_results)) : array();

    foreach($media_results_array as $mid => $media){

    	$file = file_load($media->fid);

    	$property = agency_property_load($media->pid);

    	$options['media_id-'.$mid] = array(
    		'mid' => $media->mid,
    		'filename' => $file->filename,
    		'property' => agency_property_address_string($property),
    		'type' => ucfirst($media->type),
    		'status' => ($media->status == 1) ? 'Visible' : 'Hidden',
    		'operations' => 
	        l(t('View'), 'admin/agency/media/' . $mid) . ' | ' .
	        l(t('Edit'), ADMIN_AGENCY_MEDIA_MANAGE_URI . $mid , array('query' => array('destination' => ADMIN_AGENCY_MEDIA_URI))) . ' | ' .
	        l(t('Delete'), ADMIN_AGENCY_MEDIA_MANAGE_URI . $mid . '/delete', array('attributes' => array('class' => array('media-delete-' . $mid), ), 'query' => array('destination' => ADMIN_AGENCY_MEDIA_URI))),
     	);

    }

    $form['search'] = array(
      '#type' => 'fieldset',
      '#title' => t('Basic Search'),
      '#collapsible' => TRUE,
    );

    if (!empty($search_property)){
    	$form['search']['#collapsed'] = FALSE;
    }else{
    	$form['search']['#collapsed'] = TRUE;
    }    

    $form['search']['search_property'] = array(
    	'#type' => 'select',
    	'#options' => agency_properties_for_select(),
    );

    if (!empty($search_property)){
    	$form['search']['search_property']['#default_value'] = $search_property;
    }else{
    	$form['search']['search_property']['#default_value'] = '';
    }
 
    $form['search']['search_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
    ); 
 
    $form['bulk_operations'] = array(
      '#type' => 'fieldset',
      '#title' => t('Bulk Operations'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['bulk_operations']['operations'] = array(
      '#type' => 'select',
      '#options' => array(
        0 => t('Select a bulk operation'),
        'delete' => t('Delete selected media'),
        'hide' => t('Hide selected media'),
        'show' => t('Show selected media'),
      ),
    );

    $form['bulk_operations']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );

    $form['entities'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#attributes' => array('class' => array('entity-sort-table')),
      '#empty' => t('There are no media items.'),
    );	

		$form['pager'] = array('#theme' => 'pager');

    return $form;	

	}

  public function overviewFormSubmit($form, &$form_state) {
    $values = $form_state['input'];
    $media_ids = array();

    if (!empty($values['entities'])) {
      foreach ($values['entities'] as $index => $value) {
        if (!empty($value)) {
          $media_ids[] = str_replace('media_id-', '', $value);
        }
      }

      switch ($values['operations']) {
        case 'delete': {
          drupal_goto('admin/agency/media/bulk/delete/' . implode('|', $media_ids));
          break;
        }
       	case 'hide': {
       		drupal_goto('admin/agency/media/bulk/hide/' . implode('|', $media_ids));
       		break;
       	}
       	case 'show': {
       		drupal_goto('admin/agency/media/bulk/show/' . implode('|', $media_ids));
       		break;
       	}
      }
    }

		drupal_goto('admin/agency/media', array('query' => array('property' => $values['search_property'])));

  }	

}

class AgencyCategorisationUIController extends EntityDefaultUIController {}

class AgencyFeatureUIController extends EntityDefaultUIController {}