<?php

function agency_import_rightmove_files($config){
	return file_scan_directory($config['import'],'/.*\.BLM$/');
}

function agency_import_rightmove_process_files($config) {

	$files = agency_import_rightmove_files($config);

	if (count($files) > 0){

		watchdog('info','Beginning Rightmove import, there are ' . count($files) . ' file(s) to import');

		foreach($files as $file){
			
      $rightmove = new Rightmove($file->filename);

      $rightmove->process();

		}

	}else{
		watchdog('info','There are no rightmove files to import');
	}

}

function agency_import_rightmove_process($config) {

	agency_import_rightmove_process_files($config);

}