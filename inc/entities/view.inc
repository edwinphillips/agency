<?php

function agency_property_view_entity($property) {
  $entity = entity_view('agency_property', array($property->pid => $property));
  return $entity;
}

function agency_media_view_entity($media) {
  $entity = entity_view('agency_media', array($media->mid => $media));
  return $entity;
}
