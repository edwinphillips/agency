<?php

function agency_property_images_table($pid){

	$header = array('image','filename','type','position','operation');

	$rows = array();

	$query = new EntityFieldQuery();

	$query->entityCondition('entity_type', 'agency_media')
		->propertyCondition('pid',$pid,'=')
		->propertyCondition('type','image','=')
		->propertyOrderBy('position','asc');

	$result = $query->execute();

	if (isset($result['agency_media'])){

		$image_nids = array_keys($result['agency_media']);

		$images = entity_load('agency_media',$image_nids);

		foreach($images as $image){
	
		  $file = file_load($image->fid);

		  if ($file){

				$delete = l(t('Delete'), 'admin/agency/agency_property_image_delete_callback/'.$image->mid.'/'.$pid.'/nojs/', array('attributes' => array('class' => array('use-ajax'))));
				$up = l(t('Up'), 'admin/agency/agency_property_image_position_callback/up/'.$image->mid.'/'.$pid.'/nojs/', array('attributes' => array('class' => array('use-ajax'))));
				$down = l(t('Down'), 'admin/agency/agency_property_image_position_callback/down/'.$image->mid.'/'.$pid.'/nojs/', array('attributes' => array('class' => array('use-ajax'))));

				$rows[] = array('<img src="'.image_style_url('thumbnail',$file->uri).'">', $file->filename, $image->type, $image->position, $up . ' | '. $down . ' | ' . $delete);

			}

		}

	}else{
		$rows = array(
			array(
				'fid' => array(
					'data' => t('No images have been added for this property yet'),
					'colspan' => 5
				)
			)
		);
	}

	$table = array(
	    'header' => $header, 
	    'rows' => $rows,
	    'caption' => '',
	    'colgroups' => array(),
	    'sticky' => array(),
	    'empty' => array(),
	    'attributes' => array('width' => '100%')
	);

	return theme_table($table);

}

function agency_property_categorisations_table($pid){

	$header = array('vocabulary','term','operation');
	
	$rows = array();

	$query = new EntityFieldQuery();

	$query->entityCondition('entity_type', 'agency_categorisation')
		->propertyCondition('pid',$pid,'=');

	$result = $query->execute();

	if (isset($result['agency_categorisation'])){

		$categorisation_nids = array_keys($result['agency_categorisation']);

		$categorisations = entity_load('agency_categorisation',$categorisation_nids);

		foreach($categorisations as $categorisation){
			
			$vocabulary = taxonomy_vocabulary_load($categorisation->vid);
			
			$term = taxonomy_term_load($categorisation->tid);

			$link = l(t('Delete'), 'admin/agency/agency_property_categorisation_delete_callback/'.$categorisation->cid.'/'.$pid.'/nojs/', array('attributes' => array('class' => array('use-ajax'))));

			if ($vocabulary && $term){
				$rows[] = array($vocabulary->name, $term->name,$link);
			}

		}

	}else{
		$rows = array(
			array(
				'vocabulary' => array(
					'data' => t('No categorisations have been added for this property yet'),
					'colspan' => 3
				)
			)
		);
	}

	$table = array(
	    'header' => $header, 
	    'rows' => $rows,
	    'caption' => '',
	    'colgroups' => array(),
	    'sticky' => array(),
	    'empty' => array(),
	    'attributes' => array('width' => '100%')
	);

	return theme_table($table);

}

function agency_property_media_table($pid){

	$header = array('filename','type','position','operation');

	$rows = array();

	$query = new EntityFieldQuery();

	$query->entityCondition('entity_type', 'agency_media')
		->propertyCondition('pid',$pid,'=')
		->propertyCondition('type','image','<>')
		->propertyOrderBy('position','asc');

	$result = $query->execute();

	if (isset($result['agency_media'])){

		$mediafile_nids = array_keys($result['agency_media']);

		$mediafiles = entity_load('agency_media',$mediafile_nids);

		foreach($mediafiles as $mediafile){
	
		  $file = file_load($mediafile->fid);

		  if ($file){

				$delete = l(t('Delete'), 'admin/agency/agency_property_media_delete_callback/'.$mediafile->mid.'/'.$pid.'/nojs/', array('attributes' => array('class' => array('use-ajax'))));
				$up = l(t('Up'), 'admin/agency/agency_property_media_position_callback/up/'.$mediafile->mid.'/'.$pid.'/nojs/', array('attributes' => array('class' => array('use-ajax'))));
				$down = l(t('Down'), 'admin/agency/agency_property_media_position_callback/down/'.$mediafile->mid.'/'.$pid.'/nojs/', array('attributes' => array('class' => array('use-ajax'))));

				$rows[] = array($file->filename, $mediafile->type, $mediafile->position, $up . ' | ' . $down .' | ' . $delete);

			}

		}

	}else{
		$rows = array(
			array(
				'fid' => array(
					'data' => t('No media files have been added for this property yet'),
					'colspan' => 4
				)
			)
		);
	}

	$table = array(
	    'header' => $header, 
	    'rows' => $rows,
	    'caption' => '',
	    'colgroups' => array(),
	    'sticky' => array(),
	    'empty' => array(),
	    'attributes' => array('width' => '100%')
	);

	return theme_table($table);

}