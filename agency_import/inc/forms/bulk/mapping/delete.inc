<?php

function agency_mapping_bulk_delete($form, &$form_state, $ids) {
  $ids = explode('|', $ids);
  $items = agency_mapping_load_multiple($ids);

  $form = array();
  $form_state['ids'] = $ids;

  $variables = array(
    'type' => 'ul',
    'items' => array(),
    'title' => '',
    'attributes' => array(),
  );

  foreach ($items as $item) {
    $variables['items'][] = $item->name . ' (' . $item->data . ')';
  }

  $form['summary'] = array(
    '#markup' => theme_item_list($variables),
  );

  return confirm_form($form, t('Delete all mappings?'), 'admin/agency/mapping', t('Are you sure that you wish to delete all the mapping files listed above - this action <u>CANNOT</u> be reversed?'), t('Delete all'), t('Cancel'));
}

/**
 * Implements hook_submit().
 */
function agency_mapping_bulk_delete_submit($form, &$form_state) {

  $ids = $form_state['ids'];

	agency_mapping_delete_multiple($ids);

  drupal_set_message(t('Mappings deleted'));

  drupal_goto('admin/agency/mapping');

}