<?php

function agency_media_bulk_show($form, &$form_state, $media_ids) {
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

  return confirm_form($form, t('Show all media?'), 'admin/agency/media', t('Are you sure that you wish to show all the media listed above on the website?'), t('Show all'), t('Cancel'));
}

/**
 * Implements hook_submit().
 */
function agency_media_bulk_show_submit($form, &$form_state) {

  $media_ids = $form_state['media_ids'];

  $mediafiles = agency_media_load_multiple($media_ids);

  foreach ($mediafiles as $mediafile) {
    $mediafile->status = 1;
    $mediafile->save();
  }

  drupal_set_message(t('Media visibility updated to visible'));
  drupal_goto('admin/agency/media');

}