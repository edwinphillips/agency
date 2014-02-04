<?php

function agency_property_delete($id){
  return entity_get_controller('agency_property')->delete(array($id));
}

function agency_property_delete_multiple(array $ids) {
  return entity_get_controller('agency_property')->delete($ids);
}

function agency_media_delete($id){
  return entity_get_controller('agency_media')->delete(array($id));
}

function agency_media_delete_multiple(array $ids) {
  return entity_get_controller('agency_media')->delete($ids);
}

function agency_feature_delete($id){
  return entity_get_controller('agency_feature')->delete(array($id));
}

function agency_feature_delete_multiple(array $ids) {
  return entity_get_controller('agency_feature')->delete($ids);
}

function agency_mapping_delete($id) {
  return entity_get_controller('agency_mapping')->delete(array($id));
}

function agency_mapping_delete_multiple(array $ids) {
  return entity_get_controller('agency_mapping')->delete($ids);
}

function agency_categorisation_delete($id) {
  return entity_get_controller('agency_categorisation')->delete(array($id));
}

function agency_categorisation_delete_multiple(array $ids) {
  return entity_get_controller('agency_categorisation')->delete($ids);
}
