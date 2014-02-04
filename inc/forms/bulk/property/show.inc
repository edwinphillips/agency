<?php

function agency_property_bulk_show($form, &$form_state, $property_ids) {
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

  return confirm_form($form, t('Show all properties?'), 'admin/agency/property', t('Are you sure that you wish to show all the properties listed above on the website?'), t('Show all'), t('Cancel'));
}

/**
 * Implements hook_submit().
 */
function agency_property_bulk_show_submit($form, &$form_state) {

  $property_ids = $form_state['property_ids'];

  $properties = agency_property_load_multiple($property_ids);

  foreach ($properties as $property) {
    $property->status = 1;
    $property->save();
  }

  drupal_set_message(t('Property visibility updated to visible'));
  drupal_goto('admin/agency/property');

}