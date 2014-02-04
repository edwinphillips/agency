<?php

//Try to login the FTP user
function agency_import_ftp_login($ftp, $config){
	return $ftp->ftp_login($config['username'], $config['password']);
}

//Get a list of files in the remote ftp account
function agency_import_ftp_filelist($ftp, $filter){
	return $ftp->ftp_nlist($filter);
}

//Check that the local import path exists
function agency_import_ftp_import_path_exists($config){
	return is_dir($config['import']);
}

//Create local import path
function agency_import_ftp_create_import_path($config){
	return mkdir($config['import']);
}
	
//Get the temp download and local import paths
function agency_import_ftp_get_paths($config, $file){

	$paths = array(
	  'temp' => file_directory_temp() . '/' . $file,
	  'import' => drupal_realpath($config['import']) . '/' . $file, 
	);

	return $paths;

}

//Download remote file from FTP account
function agency_import_ftp_get_remote_file($ftp, $paths, $file){
	return $ftp->ftp_get($paths['temp'],$file,FTP_BINARY);
}

//Move the downloaded file from temp to import path 
function agency_import_ftp_move_download($paths){
	return rename($paths['temp'], $paths['import']);
}

//Delete the file on the remote FTP server
function agency_import_ftp_delete_remote_file($ftp, $file){
	return $ftp->ftp_delete($file);
}

//Check that the time the file was last written is less than the delay time
function agency_import_ftp_file_check_lastwritten($ftp, $file, $delay){
	return ($ftp->ftp_mdtm($file) < $delay) ? TRUE : FALSE;
}

//Download a file from remote server, move to import directory and delete remote file
function agency_import_ftp_download_file($ftp, $config, $file, $delay){

	if (agency_import_ftp_file_check_lastwritten($ftp, $file, $delay)){
	  
	  $paths = agency_import_ftp_get_paths($config, $file);

    if (agency_import_ftp_get_remote_file($ftp, $paths, $file)){

      watchdog('info','File ' . $file . ' was downloaded from remote FTP account');  
      
      if (agency_import_ftp_move_download($paths)){
        
        watchdog('info','File '. $file .' was moved to private path');  

        if (agency_import_ftp_delete_remote_file($ftp, $file)){
          watchdog('info','File '. $file .' was delete from remote FTP account');
        }

      }

    }

	}else{
	  watchdog('info','Delaying import of '. $file .' from remote FTP');
	} 

}

//Main function calls other functions to download files via FTP
function agency_import_ftp_process($config){

  //Initialize FTP
  $ftp = new Ftp($config['host']);

  //Check ftp initialized
  if ($ftp){
    
    //Try to login to the FTP server
    if (agency_import_ftp_login($ftp, $config)){
      
      //Set the connection to pasv
      $ftp->ftp_pasv(TRUE);
      
      //Get a list of files in the remote ftp account
      $files = agency_import_ftp_filelist($ftp, ".");
      
      //Log the number of files and beginning of sync
      watchdog('info','Beginning FTP sync, there are ' . count($files) . ' files found in the remote FTP account');

      //Set the delay on a file last written time before you are willing to touch it
      $delay = time() - 300;

      //Iterate through the files in the remote ftp account
      foreach($files as $file){

        //Check if the local folder to receive ftp downloaded files exist
        if (!agency_import_ftp_import_path_exists($config)){

          //Local folder doesn't exist, so create it
          agency_import_ftp_create_import_path($config);
        
        }

        //Download by FTP the file 
        agency_import_ftp_download_file($ftp, $config, $file, $delay);

      }

    }else{
      //Log unable to login to FTP
      watchdog('info','Unable to login to remote FTP');
    }

    //Set the last checked ftp variable, used in Cron timings
    variable_set('agency_import_ftp_lastcheck',time());

  }else{

    //Log that unable to create FTP connection
    watchdog('info','Unable to create FTP connection');
  
  }

  //Log that the FTP cron task has finished
  watchdog('info','FTP cron task has finished');

  //Set the ftp connection to null
  $ftp = null;

}