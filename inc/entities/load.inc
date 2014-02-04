<?php

function agency_property_load($id){
  $property = entity_load('agency_property', array($id));
  return array_pop($property);
}

function agency_property_load_multiple($ids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('agency_property', $ids, $conditions, $reset);
}

function agency_media_load($id){
  $property = entity_load('agency_media', array($id));
  return array_pop($property);
}

function agency_media_load_multiple($ids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('agency_media', $ids, $conditions, $reset);
}

function agency_feature_load($id){
  $feature = entity_load('agency_feature', array($id));
  return array_pop($feature);
}

function agency_feature_load_multiple($ids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('agency_feature', $ids, $conditions, $reset);
}

function agency_mapping_load($id){
  $mapping = entity_load('agency_mapping', array($id));
  return array_pop($mapping);
}

function agency_mapping_load_multiple($ids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('agency_mapping', $ids, $conditions, $reset);
}

function agency_categorisation_load($id){
  $mapping = entity_load('agency_categorisation', array($id));
  return array_pop($mapping);
}

function agency_categorisation_load_multiple($ids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('agency_categorisation', $ids, $conditions, $reset);
}