<?php

function agency_property_bulk_hide($form, &$form_state, $property_ids) {
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

  return confirm_form($form, t('Hide all properties?'), 'admin/agency/property', t('Are you sure that you wish to hide all the properties listed above on the website?'), t('Hide all'), t('Cancel'));
}

/**
 * Implements hook_submit().
 */
function agency_property_bulk_hide_submit($form, &$form_state) {

  $property_ids = $form_state['property_ids'];

  $properties = agency_property_load_multiple($property_ids);

  foreach ($properties as $property) {
    $property->status = 0;
    $property->save();
  }

  drupal_set_message(t('Property visibility updated to hidden'));
  drupal_goto('admin/agency/property');

}