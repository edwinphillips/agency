<?php

function agency_media_bulk_hide($form, &$form_state, $media_ids) {
  $media_ids = explode('|', $media_ids);
  $mediafiles = agency_media_load_multiple($media_ids);

  $form = array();
  $form_state['media_ids'] = $media_ids;

  $variables = array(
    'type' => 'ul',
    'items' => array(),
    'title' => '',
    'attributes' => array(),
  );

  foreach ($mediafiles as $mediafile) {
    $variables['items'][] = $mediafile->mid;
  }

  $form['summary'] = array(
    '#markup' => theme_item_list($variables),
  );

  return confirm_form($form, t('Hide all media?'), 'admin/agency/media', t('Are you sure that you wish to hide all the media listed above on the website?'), t('Hide all'), t('Cancel'));
}

/**
 * Implements hook_submit().
 */
function agency_media_bulk_hide_submit($form, &$form_state) {

  $media_ids = $form_state['media_ids'];

  $mediafiles = agency_media_load_multiple($media_ids);

  foreach ($mediafiles as $mediafile) {
    $mediafile->status = 0;
    $mediafile->save();
  }

  drupal_set_message(t('Media visibility updated to hidden'));
  drupal_goto('admin/agency/media');

}