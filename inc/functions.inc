<?php


function agency_property_geocode($postcode){

	$point = geocoder('google',$postcode);

	$location = array(
		'longitude' => 0.0,
		'latitude' => 0.0,
	);

	if ($point){
		$location['latitude'] = $point->coords[1];
		$location['longitude'] = $point->coords[0];
	}


	return $location;

}


function agency_property_address_string($entity){

	$required = array('address1','address2','address3','towncity','county');

	$variables = get_object_vars($entity);

	$values = array();

	foreach($variables as $key => $value){
		if (in_array($key,$required) && !empty($value)){
			$values[] = $value;
		}
	}

	return implode(', ', $values);

}

function agency_property_load_features($pid){

	$features = array();

	$query = new EntityFieldQuery();

	$query->entityCondition('entity_type', 'agency_feature')
		->propertyCondition('pid',$pid,'=');

	$result = $query->execute();

	if (isset($result['agency_feature'])){
		$feature_nids = array_keys($result['agency_feature']);
		$features = array_values(agency_feature_load_multiple($feature_nids));
	}

	return $features;

}

function agency_property_delete_all_features($pid){

	$query = new EntityFieldQuery();

	$query->entityCondition('entity_type', 'agency_feature')
		->propertyCondition('pid',$pid,'=');

	$result = $query->execute();

	if (isset($result['agency_feature'])){
		$feature_nids = array_keys($result['agency_feature']);
		if (entity_delete_multiple('agency_feature', $feature_nids)){
			return TRUE;
		}
	}

	return FALSE;
}

function agency_property_create_features($features){

	$total = 0;

	foreach($features as $feature){
		$entity = entity_create('agency_feature',$feature);
		if ($entity->save()){
			$total++;
		}
	}

	return $total;

}



function agency_property_categorisation_exists($pid, $vid, $tid){

	$query = new EntityFieldQuery();

	$query->entityCondition('entity_type', 'agency_categorisation')
		->propertyCondition('pid',$pid,'=')
		->propertyCondition('vid',$vid,'=')
		->propertyCondition('tid',$tid,'=');

	$results = $query->execute();

	if (isset($results['agency_categorisation'])){
		return TRUE;
	}

	return FALSE;
}

function agency_property_categorisation_create($pid, $vid, $tid){

		$data = array(
			'pid' => $pid,
			'vid' => $vid,
			'tid' => $tid,
		);

		$categorisation = entity_create('agency_categorisation',$data);

		if ($categorisation){
			if ($categorisation->save()){
				return TRUE;
			}

		}

		return FALSE;
}

function agency_property_image_move_up($mid, $pid){

	$query = new EntityFieldQuery();

	$query->entityCondition('entity_type', 'agency_media')
		->propertyCondition('pid',$pid,'=')
		->propertyCondition('type','image','=');

	$results = $query->execute();

	if (isset($results['agency_media'])){

		$image_nids = array_keys($results['agency_media']);

		$images = entity_load('agency_media',$image_nids);

		$current_position = $images[$mid]->position;

		if ($current_position > 1){

			$desired_position = $images[$mid]->position - 1;

			foreach($images as $image){
				if ($image->position == $desired_position){
					$image->position = $current_position;
					break;
				}
			}

			$images[$mid]->position = $desired_position;

			foreach($images as $image){
				$image->save();
			}

		}

	}

}

function agency_property_image_move_down($mid, $pid){

	$query = new EntityFieldQuery();

	$query->entityCondition('entity_type', 'agency_media')
		->propertyCondition('pid',$pid,'=')
		->propertyCondition('type','image','=');

	$results = $query->execute();

	if (isset($results['agency_media'])){

		$image_nids = array_keys($results['agency_media']);

		$images = entity_load('agency_media',$image_nids);

		$current_position = $images[$mid]->position;

		$max_position = count($images);

		if ($current_position < $max_position){

			$desired_position = $images[$mid]->position + 1;

			foreach($images as $image){
				if ($image->position == $desired_position){
					$image->position = $current_position;
					break;
				}
			}

			$images[$mid]->position = $desired_position;

			foreach($images as $image){
				$image->save();
			}

		}

	}

}

function agency_property_media_move_up($mid, $pid){

	$query = new EntityFieldQuery();

	$query->entityCondition('entity_type', 'agency_media')
		->propertyCondition('pid',$pid,'=')
		->propertyCondition('type','image','<>');

	$results = $query->execute();

	if (isset($results['agency_media'])){

		$mediafile_nids = array_keys($results['agency_media']);

		$mediafiles = entity_load('agency_media',$mediafile_nids);

		$current_position = $mediafiles[$mid]->position;

		if ($current_position > 1){

			$desired_position = $mediafiles[$mid]->position - 1;

			foreach($mediafiles as $mediafile){
				if ($mediafile->position == $desired_position){
					$mediafile->position = $current_position;
					break;
				}
			}

			$mediafiles[$mid]->position = $desired_position;

			foreach($mediafiles as $mediafile){
				$mediafile->save();
			}

		}

	}

}

function agency_property_media_move_down($mid, $pid){

	$query = new EntityFieldQuery();

	$query->entityCondition('entity_type', 'agency_media')
		->propertyCondition('pid',$pid,'=')
		->propertyCondition('type','image','<>');

	$results = $query->execute();

	if (isset($results['agency_media'])){

		$mediafile_nids = array_keys($results['agency_media']);

		$mediafiles = entity_load('agency_media',$mediafile_nids);

		$current_position = $mediafiles[$mid]->position;
		
		$max_position = count($mediafiles);

		if ($current_position < $max_position){

			$desired_position = $mediafiles[$mid]->position + 1;

			foreach($mediafiles as $mediafile){
				if ($mediafile->position == $desired_position){
					$mediafile->position = $current_position;
					break;
				}
			}

			$mediafiles[$mid]->position = $desired_position;

			foreach($mediafiles as $mediafile){
				$mediafile->save();
			}

		}

	}

}

function agency_property_image_next_position($pid){

	$query = new EntityFieldQuery();

	$query->entityCondition('entity_type', 'agency_media')
		->propertyCondition('pid',$pid,'=')
		->propertyCondition('type','image','=');

	$results = $query->execute();

	if (isset($results['agency_media'])){
		return count($results['agency_media']) + 1;
	}else{
		return 1;
	}

}

function agency_property_media_next_position($pid){

	$query = new EntityFieldQuery();

	$query->entityCondition('entity_type', 'agency_media')
		->propertyCondition('pid',$pid,'=')
		->propertyCondition('type','image','<>');

	$results = $query->execute();

	if (isset($results['agency_media'])){
		return count($results['agency_media']) + 1;
	}else{
		return 1;
	}

}


function agency_property_media_create($pid, $fid, $type){

		//@TODO
		$data = array(
			'pid' => $pid,
			'fid' => $fid,
			'type' => $type,
		);

		if ($type == 'image'){
			$data['position'] = agency_property_image_next_position($pid);
		}else{
			$data['position'] = agency_property_media_next_position($pid);
		}

		$media = entity_create('agency_media',$data);

		if ($media){
			if ($media->save()){
				return TRUE;
			}

		}

		return FALSE;
}

function agency_property_categorisation_delete($cid){

	return entity_delete('agency_categorisation', $cid);

}

function agency_property_media_reorder_on_delete($pid, $type){

	$query = new EntityFieldQuery();

	$query->entityCondition('entity_type', 'agency_media')->propertyCondition('pid',$pid,'=');

	if ($type == 'image'){
		$query->propertyCondition('type','image','=');
	}else{
		$query->propertyCondition('type','image','<>');
	}

	$results = $query->execute();

	if (isset($results['agency_media'])){

		$mediafile_nids = array_keys($results['agency_media']);

		$mediafiles = entity_load('agency_media',$mediafile_nids);

		$i = 1;

		foreach($mediafiles as $mediafile){

			$mediafile->position = $i;

			$mediafile->save();

			$i++;

		}

	}

}

function agency_property_media_delete($mid){

	$media = agency_media_load($mid);

	if ($media){

	  $file = file_load($media->fid);

	  if ($file){

	  	$file->status = FILE_STATUS_TEMPORARY;
	  	
	  	if (file_save($file)){
	  		
				return entity_delete('agency_media', $mid);
						  	
	  	}

	  }

	}

	return FALSE;

}