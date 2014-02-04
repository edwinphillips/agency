<?php

function agency_media_bulk_delete($form, &$form_state, $ids) {
  $ids = explode('|', $ids);
  $items = agency_media_load_multiple($ids);

  $form = array();
  $form_state['ids'] = $ids;

  $variables = array(
    'type' => 'ul',
    'items' => array(),
    'title' => '',
    'attributes' => array(),
  );

  foreach ($items as $item) {
    $variables['items'][] = $item->mid;
  }

  $form['summary'] = array(
    '#markup' => theme_item_list($variables),
  );

  return confirm_form($form, t('Delete all media?'), 'admin/agency/media', t('Are you sure that you wish to delete all the media files listed above - this action <u>CANNOT</u> be reversed?'), t('Delete all'), t('Cancel'));
}

/**
 * Implements hook_submit().
 */
function agency_media_bulk_delete_submit($form, &$form_state) {

  $ids = $form_state['ids'];

  foreach($ids as $id){
    agency_property_media_delete($id);
  }

  drupal_set_message(t('Media deleted'));
  drupal_goto('admin/agency/media');
}