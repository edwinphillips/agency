<?php

function agency_import_check_delay($last, $delay){
	if (!$last || $last < $delay){
		return TRUE;
	}
	return FALSE;
}

function agency_import_get_config(){

  $config = array(
    'host' => variable_get('agency_import_ftp_host',null),
    'username' => variable_get('agency_import_ftp_username',null),
    'password' => variable_get('agency_import_ftp_password',null),
    'import' => variable_get('agency_import_path', null),
    'archive' => variable_get('agency_archive_path',null),
    'public' => variable_get('agency_public_path',null),
    'compressed' => variable_get('agency_import_compressed','no'),
    'type' => variable_get('agency_import_type',null),
    'lookups' => variable_get('agency_import_lookups',null),
  );

  return $config;

}

function agency_import_check_has_required($config){

	$required = array('host','username','password','import','archive','public','compressed','type','lookups');

	if (count($config) == count($required)){
		return TRUE;
	}

	return FALSE;

}

function agency_import_get_queues(){

  $queues = array();

  $queues[] = array(
    'name'=> 'ftp',
    'queue' => DrupalQueue::get('agency_import_ftp'),
  );

  $queues[] = array(
    'name' => 'property',
    'queue' => DrupalQueue::get('agency_import_property'),
  );

  return $queues;

}

function agency_import_keys_for_select(){

  $options = array();

  $type = variable_get('agency_import_type', 'rightmove');

  switch($type){

    case 'rightmove':

      $options['BRANCH_ID'] = 'Branch (BRANCH_ID)';
      $options['STATUS_ID'] = 'Status (STATUS_ID)';
      $options['PRICE_QUALIFIER'] = 'Price Qualifier (PRICE_QUALIFIER)';
      $options['PROP_SUB_ID'] = 'Property Sub Identifier (PROP_SUB_ID)';
      $options['LET_TYPE_ID'] = 'Let Type (LET_TYPE_ID)';
      $options['LET_FURN_ID'] = 'Let Furnished (LET_FURN_ID)';
      $options['LET_RENT_FREQUENCY'] = 'Let Frequency (LET_RENT_FREQUENCY)';
      $options['TENURE_TYPE_ID'] = 'Tenure (TENURE_TYPE_ID)';
      $options['TRANS_TYPE_ID'] = 'Transaction Type (TRANS_TYPE_ID)';

      break;

    default:

      break;

  }

  return $options;

}

function agency_import_vocabularies_for_select(){

  $vocabularies = taxonomy_get_vocabularies();

  $options = array();

  foreach($vocabularies as $vocabulary){
    $options[$vocabulary->vid] = $vocabulary->name;
  }

  return $options;
}

function agency_import_vocabulary_terms_for_select($vid = 0){

  $terms = taxonomy_get_tree($vid);

  $options = array();

  foreach($terms as $term){
    $options[$term->tid] = $term->name;
  }

  return $options;

}