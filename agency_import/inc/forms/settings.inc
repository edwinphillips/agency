<?php

function agency_import_settings($form, &$form_state){
	
  $form = array();

  $form['agency_import_path'] = array(
    '#title' => t('Import Path'),
    '#type' => 'textfield',
    '#default_value' => variable_get('agency_import_path', null),
    '#description' => t('Import path for uploaded files'),
    '#required' => TRUE,
  );

  $form['agency_archive_path'] = array(
    '#title' => t('Archive Path'),
    '#type' => 'textfield',
    '#default_value' => variable_get('agency_archive_path', null),
    '#description' => t('Archive path for uploaded files'),
    '#required' => TRUE,
  );  

  $form['agency_public_path'] = array(
    '#title' => t('Public Path'),
    '#type' => 'textfield',
    '#default_value' => variable_get('agency_public_path', null),
    '#description' => t('Public path for uploaded files'),
    '#required' => TRUE,
  );  

  $form['agency_import_lookups'] = array(
    '#title' => t('Field Lookups'),
    '#type' => 'textfield',
    '#default_value' => variable_get('agency_import_lookups', null),
    '#description' => t('The fields to perform database lookups against'),
    '#required' => TRUE,
  );

  $form['agency_import_type'] = array(
    '#title' => t('Feed Type'),
    '#type' => 'select',
    '#default_value' => variable_get('agency_import_type', 'rightmove'),
    '#description' => t('Type of import file'),
    '#options' => array(
    	'rightmove' => 'Rightmove (BLM)',
    ),    
    '#required' => TRUE,
  );

  $form['agency_import_property_delay'] = array(
    '#title' => t('Property Import Delay'),
    '#type' => 'select',
    '#options' => array(
      300 => '5 Minutes',
      21600 => '6 Hours',
      43200 => '12 Hours',
      86400 => '24 Hours',
    ),
    '#default_value' => variable_get('agency_import_property_delay', 21600),
    '#description' => t('How often to check for and import new property'),
    '#required' => TRUE,
  );   

  $form['agency_import_compressed'] = array(
    '#title' => t('Compressed'),
    '#type' => 'select',
    '#default_value' => variable_get('agency_import_compressed', 'no'),
    '#options' => array(
    	'no' => 'No',
    	'yes' => 'Yes',
    ),
    '#description' => t('Import file is in compressed (zipped) format'),
    '#required' => TRUE,
  );  

  $form['agency_import_ftp_host'] = array(
    '#title' => t('FTP Hostname'),
    '#type' => 'textfield',
    '#default_value' => variable_get('agency_import_ftp_host', null),
    '#description' => t('FTP hostname for uploads'),
    '#required' => TRUE,
  );  

  $form['agency_import_ftp_username'] = array(
    '#title' => t('FTP Username'),
    '#type' => 'textfield',
    '#default_value' => variable_get('agency_import_ftp_username', null),
    '#description' => t('FTP username for uploads'),
    '#required' => TRUE,
  );  

  $form['agency_import_ftp_password'] = array(
    '#title' => t('FTP Password'),
    '#type' => 'textfield',
    '#default_value' => variable_get('agency_import_ftp_password', null),
    '#description' => t('FTP password for uploads'),
    '#required' => TRUE,
  );  

  $form['agency_import_ftp_delay'] = array(
    '#title' => t('FTP Delay'),
    '#type' => 'select',
    '#options' => array(
      300 => '5 Minutes',      
      3600 => '1 Hour',
      7200 => '2 Hours',
      14400 => '4 Hours',
    ),
    '#default_value' => variable_get('agency_import_ftp_delay', 3600),
    '#description' => t('How often to check for new FTP uploads'),
    '#required' => TRUE,
  ); 

  return system_settings_form($form);
}