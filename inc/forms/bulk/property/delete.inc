<?php

function agency_property_bulk_delete($form, &$form_state, $property_ids) {
  $property_ids = explode('|', $property_ids);
  $properties = agency_property_load_multiple($property_ids);

  $form = array();
  $form_state['property_ids'] = $property_ids;

  $variables = array(
    'type' => 'ul',
    'items' => array(),
    'title' => '',
    'attributes' => array(),
  );

  foreach ($properties as $property) {
    $variables['items'][] = agency_property_address_string($property);
  }

  $form['summary'] = array(
    '#markup' => theme_item_list($variables),
  );

  return confirm_form($form, t('Delete all properties?'), 'admin/agency/property', t('Are you sure that you wish to delete all the properties listed above - this action <u>CANNOT</u> be reversed?'), t('Delete all'), t('Cancel'));
}

/**
 * Implements hook_submit().
 */
function agency_property_bulk_delete_submit($form, &$form_state) {
  $property_ids = $form_state['property_ids'];
  agency_property_delete_multiple($property_ids);

  drupal_set_message(t('Properties deleted'));
  drupal_goto('admin/agency/property');
}