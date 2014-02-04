<?php

class AgencyImportMappingController extends EntityAPIController {}

class AgencyImportMappingUIController extends EntityDefaultUIController {

	public function overviewForm($form, &$form_state){

		$header = array(
			'mid' => array('data' => t('mid'), 'field' => 'mid'),
			'name' => array('data' => t('name'), 'field' => 'name'),
			'data' => array('data' => t('data'), 'field' => 'data'),
			'vocabulary' => array('data' => t('vocabulary')),
			'term' => array('data' => t('term')),
 			'operations' => array('data' => t('operations')),		
		);

    $options = array();

    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', 'agency_mapping');

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

    $mapping_results = !empty($result['agency_mapping']) ? $result['agency_mapping'] : array();
    $mapping_results_array = !empty($mapping_results) ? agency_mapping_load_multiple(array_keys($mapping_results)) : array();

    foreach($mapping_results_array as $mid => $mapping){

    	$vocabulary = taxonomy_vocabulary_load($mapping->vid);

    	$term = taxonomy_term_load($mapping->tid);

    	$options['mapping_id-'.$mid] = array(
    		'mid' => $mid,
    		'name' => $mapping->name,
    		'data' => $mapping->data,
    		'vocabulary' => $vocabulary->name,
    		'term' => $term->name,
    		'operations' => 
	        l(t('View'), 'admin/agency/mapping/' . $mapping->mid) . ' | ' .
	        l(t('Edit'), ADMIN_AGENCY_MAPPING_MANAGE_URI . $mid , array('query' => array('destination' => ADMIN_AGENCY_MAPPING_URI))) . ' | ' .
	        l(t('Delete'), ADMIN_AGENCY_MAPPING_MANAGE_URI . $mid . '/delete', array('attributes' => array('class' => array('mapping-delete-' . $mid), ), 'query' => array('destination' => ADMIN_AGENCY_MAPPING_URI))),
     	);

    }

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
        'delete' => t('Delete selected mapping'),
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
      '#empty' => t('There are no mappings.'),
    );	

		$form['pager'] = array('#theme' => 'pager');

    return $form;	
	}	

  public function overviewFormSubmit($form, &$form_state) {
    $values = $form_state['input'];
    $mapping_ids = array();

    if (!empty($values['entities'])) {
      foreach ($values['entities'] as $index => $value) {
        if (!empty($value)) {
          $mapping_ids[] = str_replace('mapping_id-', '', $value);
        }
      }

      switch ($values['operations']) {
        case 'delete': {
          drupal_goto('admin/agency/mapping/bulk/delete/' . implode('|', $mapping_ids));
          break;
        }
      }
    }

  }		
}