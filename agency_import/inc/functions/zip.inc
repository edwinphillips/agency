<?php

//Get a list of zip files in the import path
function agency_import_zip_get_zip_files($config){
	return file_scan_directory($config['import'],'/.*\.zip$/');
}

//Open the zip file
function agency_import_zip_open_zip_file($zip, $file){
	return $zip->open(drupal_realpath($file->uri));	
}

//Extract the contents of the zip file to the import path
function agency_import_zip_extract_zip_file($zip, $config){
	return $zip->extractTo(drupal_realpath($config['path']));
}

//Check that the local import path exists
function agency_import_zip_archive_path_exists($config){
	return is_dir($config['archive']);
}

//Create local archive path
function agency_import_ftp_create_archive_path($config){
	return mkdir($config['archive']);
}

//Set the file paths for the zip file in the import path and the zip file when moved to archive
function agency_import_zip_get_paths($file, $config){

	$paths = array(
	  'private' => drupal_realpath($file->uri),
	  'archive' => drupal_realpath($config['archive']) . '/' . date('dmyHis',time()) . '.zip', 
	);

	return $paths;

}

//Move the zip file to the archive
function agency_import_zip_move_to_archive($paths){
	return rename($paths['private'],$paths['archive']);
}

//Unzip the zip file
function agency_import_zip_unzip($file, $config){

  //Initialize ziparchive
  $zip = new ZipArchive();

  //If the zip was instantiated
  if ($zip){

    //Open the zip file
    if (agency_import_zip_open_zip_file($zip, $file)){
      
      //Log that file was opened
      watchdog('info','Opened the file ' . $file->uri . ' for unzipping');

	    //Get the paths for archiving the zip file
	    $paths = agency_import_zip_get_paths($file, $config);

      //Extract the zip file
      if (agency_import_zip_extract_zip_file($zip, $config)){

      	//Move the uploaded zip file to the archive directory
      	if (agency_import_zip_move_to_archive($paths)){

      		//Log that file was successfully moved
      		watchdog('info', $file->uri . ' file was unzipped successfully and source moved to archive ' . $paths['archive']);

      	}else{

      		//Log that the file was unable to be moved
      		watchdog('error', $file->uri . ' file was unzipped, but not moved to archive ' . $paths['archive']);

      	}

      }

      //Close the zip file
      $zip->close();

    }

  }else{

    //Log that unable to instantiate ziparchive
    watchdog('error','Unable to instantiate ZipArchive');
  
  }

}

//Main function, calls functions to unzip any files found
function agency_import_unzip_process($config){

  //Get a list of zip files in the import path
  $files = agency_import_zip_get_zip_files($config);

  //Check if the archive path exists
  if (!agency_import_zip_archive_path_exists($config)){ 

    //If the archive path doesn't exist, then create it
    if (agency_import_ftp_create_archive_path($config)){

      //Log archive path created
      watchdog('info','Directory ' . $config['archive'] . ' was not found, created');  
    
    }

  }

  //Iterate each file in the import path
  foreach($files as $file){

    //Log that a file was found for unzipping
    watchdog('info', $file->uri . ' file found for unzipping');

    //Process this zip file
    agency_import_zip_unzip($file, $config);

  }

}