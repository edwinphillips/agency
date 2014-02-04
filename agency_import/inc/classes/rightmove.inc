<?php

class Rightmove{

	private $branch;
	private $identifier;
	private $config;	
	private $paths;
	private $filename;
	private $contents;
	private $header;
	private $definition;
	private $lines;
	private $rows;
	private $files;
	private $categorisations;
	private $features;


	public function __construct($filename){
		$this->filename = $filename;
	}

	//**************************************
	//** METHODS TO SETUP / LOG / CLEANUP **
	//**************************************

	//Set the branch this import is for
	private function setupBranchAndIdentifier(){

		//Split the filename by the underscore
		$parts = explode('_',pathinfo($this->filename,PATHINFO_FILENAME));

		//Check if the split led to an array, there should be 2 parts
		if (is_array($parts)){
				
			//Get the first part of the array, this is the branch id
			$id = reset($parts);

			//If the branch id is numeric (as it should be)
			if (is_numeric($id)){

				//Set the branch to the branch id
				$this->branch = reset($parts);

				//Set the import identifier to the end item in the array, the upload identifier
				$this->identifier = end($parts);
	
			}

		}

	}

	//Load the configuration from Drupal Variables
	private function loadConfig(){
		$this->config = array(
	    'host' => variable_get('agency_import_ftp_host',null),
	    'username' => variable_get('agency_import_ftp_username',null),
	    'password' => variable_get('agency_import_ftp_password',null),
	    'import' => variable_get('agency_import_path', null),
	    'archive' => variable_get('agency_archive_path',null),
	    'public' => variable_get('agency_public_path',null),
	    'compressed' => variable_get('agency_import_compressed','no'),
	    'type' => variable_get('agency_import_type',null),
	    'lookups' => variable_get('agency_import_lookups',null),	
		);	
	}

	//Generate full paths to the relevant directories for this import
	private function generatePaths(){

		$this->paths = array(
			'import' => drupal_realpath($this->config['import']),
			'archive' => $this->buildArchivePath(),
			'public' => $this->config['public'],
		);

	}	

	//Load the contents of the rightmove blm file
	private function loadContents(){
		$this->contents = file_get_contents($this->buildImportFilePath());
	}

	//Called by process method to start the setup
	public function process(){

		//Get the branch and upload identifier
		$this->setupBranchAndIdentifier();

		//Set the branch this import is for
		if (!empty($this->branch) && !empty($this->identifier)){

			//Load the configuration from the drupal admin settings screen
			$this->loadConfig();

			//Generate the paths that will be used during the import
			$this->generatePaths();

			//Load the contents of the file
			$this->loadContents();

			//Process the file contents
			$this->processFile();

			//Cleanup the remaining files / bits
			$this->cleanup();

		}else{

			//Log that unable to set the branch / upload identifier
			$this->log('error','Unable to set the branch and / or identifier for this import');

		}

	}

	//Move the import file to the archive directory
	private function moveImportFileToArchive(){

		//Get the import filename and path
		$import = $this->buildImportFilePath();

		//Get the archive filename and path
		$archive = $this->buildImportFileArchivePath();

		//Save the import file to the archive path
		return rename($import, $archive);
	
	}

	//Zip the contents of the archive folder
	private function zipArchiveFolder(){

		//Create a new zip archive
		$zip = new ZipArchive();

		//Open the zip archive with the path for the archive from the buildZipArchivePath function
		$zip->open($this->buildZipArchivePath(), ZipArchive::CREATE);
		
		//Get the target path to add to the zip
		$target = $this->paths['archive'] . '/*';

		//For each file in the target path add it to the zip
		foreach (glob($target) as $file) {
	    $zip->addFile($file);
		}

		//Close and save the zip
		$zip->close();

	}

	//Delete the archive folder and its content
	private function deleteArchiveFolders(){

		//Get the archive path
		$archive = $this->paths['archive'];

		//Check that the archive path is a directory and iterate through this 
		//directory and any sub directories deleting file and sub directories
		//and then delete the archive directory
		if (is_dir($archive)) { 
		 $objects = scandir($archive); 
		 foreach ($objects as $object) { 
		   if ($object != "." && $object != "..") { 
		     unlink($archive."/".$object); 
		   } 
		 } 
		 reset($objects); 
		 rmdir($archive); 
		} 

	}

	//Cleanup the import by calling the functions to move import file to archive, zip archive and delete
	private function cleanup(){

		//Move the blm file to the archive directory
		$this->moveImportFileToArchive();

		//Zip up the folder 
		$this->zipArchiveFolder();

		//Delete the archive folder
		$this->deleteArchiveFolders();

		//Move any stray uploaded files to public if they already exist in public
		$this->updateUploadedMedia();

	}

	//Delete the branch data that already exists to ensure consistancy
	private function deleteExistingBranchData(){

		//Check for the branch mapping
		$mapping = $this->getCategorisationMapping('BRANCH_ID',$this->branch);

		//The branch mapping was found
		if ($mapping){

			//Get the properties for the branch
			$properties = $this->getExistingPropertiesForBranch($mapping);

			//If there are properties for this branch
			if ($properties){

				//Iterate each property
				foreach($properties as $property){

					//Get the media files for the property
					$files = $this->getMediaForProperty($property->pid);

					//Get the features for the property
					$features = $this->getFeaturesForProperty($property->pid);

					//Get the categorisations for the properties
					$categorisations = $this->getCategorisationsForProperty($property->pid);

					//Check if there are any files for the property
					if ($files){

						//Iterate through each file
						foreach($files as $file){

							//Try to set the managed file for deletion or log that were unable to delete this
							if ($this->setManagedFileForDeletion($file->fid)){
								if (!$file->delete()){
									$this->log('error','Unable to delete media file ' . $file->fid);
								}
							}else{
								$this->log('error','Unable to set managed file ' . $file->fid . ' status for deletion');
							}

						}

					}

					//Check if there are features for the property, if there are delete the features or log that were unable to
					if ($features){
						agency_feature_delete_multiple($features);
					}

					//Check if there are categorisations for the property, if there are delete the categorisations or log that were unable to
					if ($categorisations){
						agency_categorisation_delete_multiple($categorisations);
					}

					//Delete the property entity itself, or log that was unable to delete the property
					$property->delete();

				}

				//Try to see if there are any properties now in the database
				$properties = $this->getExistingPropertiesForBranch($mapping);

				//If there aren't any properties for the branch return that they've all been deleted, otherwise you
				//will return the default of false and the import will fail
				if (!$properties){
					return TRUE;
				}

			}else{	
				
				//Log that there are no properties for the branch as yet, this is ok, return that delete worked
				$this->log('info', 'There are no existing properties for the branch, perhaps a new branch or clean database?');
				
				return TRUE;

			}

		}else{

			//The branch wasn't found in the database, this is bad, it needs a mapping for all sorts, so allow the 
			//default return to succeed and the import will fail
			$this->log('info', 'The branch mapping was not found in the database, failing until one is setup');
		
		}

		return FALSE;

	}

	//This function will move any uploaded media that exists in the public directory with the same
	//name with uploaded media. 
	private function updateUploadedMedia(){

		//Get the path and iterator for that path
		$dir = new DirectoryIterator($this->paths['import']);

		//Iterate through each file in the path
		foreach ($dir as $file) {

			//If the file is not a dot file
	    if (!$file->isDot() && $file->getFilename() !== '.DS_Store') {

	    	//Get the import path and the public file path for the file
	    	$import = $this->buildImportMediaPath($file->getFilename());
	    	$public = $this->buildPublicFullMediaPath($file->getFilename());

	    	//Check if the file exists in the public directory
	    	if ($this->fileExists($public)){

	    		//Try to move the file from the import directory to the public directory
	    		if (rename($import,$public)){
	    			$this->log('info','Updated file ' . $file->getFilename() . ' moved to the public directory');
	    		}else{
	    			$this->log('error','Updated file ' . $file->getFilename() . ' unable to be moved to the public directory');
	    		}

	    	}else{
	    		
	    		//The file doesn't exist in the public directory, so the file is stray and should be deleted
	    		if (unlink($import)){
	    			$this->log('info','Uploaded file ' . $file->getFilename() . ' was deleted from the import directory as did not exist in public');
	    		}else{
	    			$this->log('error','Uploaded file ' . $file->getFilename() . ' was unable to be deleted from the import directory, please manually delete');
	    		}
	    	}
	    }
		}

	}

	//Log a message to Watchdog
	private function log($level, $message){
		watchdog($level, $message);
	}

	//*********************************************
	//** END OF METHODS TO SETUP / LOG / CLEANUP **
	//*********************************************

	//*****************************************
	//** START OF METHODS FOR BUILDING PATHS **
	//*****************************************

	//Build a directory path
	private function buildPath($path){
		return implode('/', $path);
	}

	//Build the path to the archive for this import
	private function buildArchivePath(){
		
		$path = array(
			drupal_realpath($this->config['archive']),
			$this->identifier,
		);

		return $this->buildPath($path);

	}

	//Build the path to the import file
	private function buildImportFilePath(){

		$path = array(
			$this->paths['import'],
			$this->filename,
		);

		return $this->buildPath($path);
	}

	//Build the path where the import file should be in the archive
	private function buildImportFileArchivePath(){

		$path = array(
			$this->paths['archive'],
			$this->filename,
		);

		return $this->buildPath($path);
	}

	//Build the path where the import media file should be 
	private function buildImportMediaPath($filename){

		$path = array(
			$this->paths['import'],
			$filename,
		);

		return $this->buildPath($path);
	}		

	//Build the path where the public media file should be
	private function buildPublicMediaPath($filename){

		$path = array(
			$this->paths['public'],
			$filename,
		);

		return $this->buildPath($path);
	}	

	//Build the path where the public media file should be
	private function buildPublicFullMediaPath($filename){

		$path = array(
			drupal_realpath($this->paths['public']),
			$filename,
		);

		return $this->buildPath($path);
	}	


	//Build the path where the media file should be archived
	private function buildArchiveMediaPath($filename){

		$path = array(
			$this->paths['archive'],
			$filename,
		);
		
		return $this->buildPath($path);
	
	}

	//Build the path where the zip archive should be
	private function buildZipArchivePath(){
		
		$path = array(
			drupal_realpath($this->config['archive']),
			$this->identifier . '.zip',
		);

		return $this->buildPath($path);

	}

	//Create a directory at the path
	private function createPath($path){
		return mkdir($path);
	}

	//Create a path for the archive for this import
	private function createArchivePath(){
		return $this->createPath($this->paths['archive']);
	}

	//Check if a path exists
	private function pathExists($path){
		return is_dir($path);
	}

	//Check if the file exists
	private function fileExists($path){
		return file_exists($path);
	}


	//Check if the archive path exists, if not create it
	private function archivePathExists(){

		if (!$this->pathExists($this->paths['archive'])){
			$this->createArchivePath();
		}

		return $this->pathExists($this->paths['archive']);		

	}

	//***************************************
	//** END OF METHODS FOR BUILDING PATHS **
	//***************************************

	//*****************************
	//** METHODS TO PROCESS FILE **
	//*****************************

	//Process the header of the rightmove blm file
	private function processFileHeader(){

	  $section = trim(substr($this->contents, strpos($this->contents,'#HEADER#')+8, strpos($this->contents,'#DEFINITION#')-8));
	  
	  $lines = explode("\n", $section);

	  $this->header = array(
	  	'eof' => '^',
	  	'eor' => '|',
	  	'count' => 0,
	  );

	  foreach ($lines as $line) {

	    // get end of field character
	    if (strstr($line,"EOF")) {
	      $replacements = array("EOF", " ", ":", "'", "\n", "\r");
	      $this->header['eof'] = str_replace($replacements, "", $line);
	    }

	    // get end of record character
	    if (strstr($line,"EOR")) {
	      $replacements = array("EOR", " ", ":", "'", "\n", "\r");
	      $this->header['eor'] = str_replace($replacements, "", $line);
	    }

	    // get property count
	    if (strstr($line,"Property Count")) {
	      $replacements = array("Property Count", " ", ":", "'", "\n", "\r");
	      $this->header['count'] = (int)str_replace($replacements, "", $line);
	    }

	  }

	}

	//Process the definitions section of the rightmove blm file
	private function processFileDefinitions(){

		$length = strpos($this->contents, $this->header['eor'], strpos($this->contents,'#DEFINITION#'))-strpos($this->contents,'#DEFINITION#')-12;

		$definition = trim(substr($this->contents, strpos($this->contents, '#DEFINITION#')+12, $length));

		$fields = explode($this->header['eof'], $definition);

		array_pop($fields);

		$this->definitions = $fields;

	}

	//Process the data section of the rightmove blm file to produce the lines of the file
	private function processFileData(){

	  $length = strpos($this->contents, '#END#')-strpos($this->contents, '#DATA#')-6;

	  $lines = trim(substr($this->contents, strpos($this->contents, '#DATA#')+6, $length)); 

	  $lines = explode($this->header['eor'], $lines);

		array_pop($lines);

	  $this->lines = $lines;

	}

	//Call the functions to process the header and then the definitions and data content
	private function processFileContent(){

		$this->processFileHeader();

		if (intval($this->header['count']) > 0){
			
			$this->processFileDefinitions();

			if (count($this->definitions) > 0){
				$this->processFileData();
			}else{
				$this->log('error','No definition fields were found for file ' . $this->filename);
			}

		}else{
			$this->log('error','No properties to import for the file ' . $this->filename);
		}

	}

	//Process each line of the import file and create a row for it
	private function processLines(){

		foreach($this->lines as $line){
			
			$data = explode($this->header['eof'], trim($line));
			
			array_pop($data);

			$row = array();

			for($i = 0; $i < count($data); $i++){
				$row[$this->definitions[$i]] = $data[$i];
			}

			$this->rows[] = $row;

		}

	}

	//Process the file
	private function processFile(){

		//Check to see that the import file has some content
		if ($this->contents !== FALSE){

			//Check to see if the path to the archive directory exists, and if not create
			if ($this->archivePathExists()){

				//Process the file contents
				$this->processFileContent();

				//Check to see if the import file has lines
				if (count($this->lines) > 0){
					
					//Process each line, converting to a row of fields
					$this->processLines();

					//If rows were created
					if (count($this->rows) > 0){

						//Try to delete all properties belonging to this branch which were previously uploaded
						if ($this->deleteExistingBranchData()){

							//Iterate through each row of data
							foreach($this->rows as $row){

								//Try to create the property from the row of data
								if ($property = $this->createProperty($row)){

									//Log that the property was successfully created
									$this->log('info','Created property with reference ' . $property->reference);

									//For each media item for this property add it to an array for processing shortly
									$this->addPropertyMediaFiles($row, $property);

									//For each categorisation for this property add it to an array for processing shortly
									$this->addCategorisations($row, $property);

									//For each feature for this property add it to an array for processing shortly.
									$this->addFeatures($row, $property);

								}else{

									//Log that unable to create property
									$this->log('error','Unable to create property with reference ' . $property->reference);

								}

							}

							//Process the media files for all properties
							$this->processMediaFiles();

							//Process the features for all properties
							$this->processFeatures();

							//Process the categorisations for all properties
							$this->processCategorisations();

						}else{

							//Log that the system was unable to delete the existing data for this branch
							$this->log('error','Unable to delete the existing data for this branch, it might not exist or an error occurred');
						
						}

					}else{

						//Log that there were no rows for this file
						$this->log('error', 'There were no rows found for the file ' . $this->filename);
					}

				}else{

					//Log that no lines were found for this file
					$this->log('error', 'There were no lines found for the file ' . $this->filename);
				}

			}else{

				//Log that the system was unable to create the archive path for this file
				$this->log('info','Unable to create archive path ' . $this->paths['archive']);
			}

		}else{

			//Log that no content was found in the blm file to import
			$this->log('error','No content found in file ' . $this->filename);
		}

	}	

	//************************************
	//** END OF METHODS TO PROCESS FILE **
	//************************************

	//********************************
	//** START OF GEOCODING METHODS **
	//********************************

	//Method to geocode a postcode
	function geocode($postcode){

		//$point = geocoder('google',$postcode);

		$location = array(
			'longitude' => 0.0,
			'latitude' => 0.0,
		);

		//if ($point){
		//	$location['latitude'] = $point->coords[1];
		//	$location['longitude'] = $point->coords[0];
		//}


		return $location;

	}

	//********************************
	//** END OF GEOCODING METHODS **
	//********************************

	//***********************************
	//** START OF METHODS FOR PROPERTY **
	//***********************************


	//Get all existing properties for the branch this file represents
	private function getExistingPropertiesForBranch($mapping){

		$query = new EntityFieldQuery();

		$query->entityCondition('entity_type', 'agency_categorisation')
			->propertyCondition('vid',$mapping->vid,'=')
			->propertyCondition('tid',$mapping->tid,'=');

		$result = $query->execute();

		if (isset($result['agency_categorisation'])){

			$categorisation_nids = array_keys($result['agency_categorisation']);

			$categorisations = agency_categorisation_load_multiple($categorisation_nids);

			if ($categorisations){

				$property_nids = array();

				foreach($categorisations as $categorisation){
					$property_nids[] = $categorisation->pid;
				}

				return agency_property_load_multiple($property_nids);

			}

		}

		return FALSE;

	}

	//Check that the property exists by its id
	private function propertyExists($pid){

		$query = new EntityFieldQuery();

		$query->entityCondition('entity_type', 'agency_property')
			->propertyCondition('pid',$pid,'=');

		$results = $query->execute();

		if (isset($results['agency_property'])){
			return TRUE;
		}

		return FALSE;

	}

	//Create a property
	private function createProperty($row){

		global $user;

		$data = array(
			'uid' => $user->uid,
			'reference' => $row['AGENT_REF'],
			'address1' => $row['ADDRESS_1'],
			'address2' => $row['ADDRESS_2'],
			'address3' => (isset($row['ADDRESS_3'])) ? $row['ADDRESS_3'] : null,
			'towncity' => $row['TOWN'],
			'postcode' => $row['POSTCODE1'] .  ' ' . $row['POSTCODE2'],
			'summary' => mb_convert_encoding($row['SUMMARY'],'latin1'),
			'description' => mb_convert_encoding($row['DESCRIPTION'],'latin1'),
			'bedrooms' => $row['BEDROOMS'],
			'price' => $row['PRICE'],
			'status' => 1,
		);

		$location = $this->geocode($data['postcode']);

		$data['latitude'] = $location['latitude'];
		$data['longitude'] = $location['longitude'];

		if ($property = entity_create('agency_property',$data)){
			if (entity_save('agency_property',$property)){
				return $property;
			}
		}

		return FALSE;

	}

	//*********************************
	//** END OF METHODS FOR PROPERTY **
	//*********************************	

	//*****************************************
	//** START OF METHODS FOR MEDIA PROPERTY **
	//*****************************************

	//Get all the media in the database for a particular property
	function getMediaForProperty($pid){

		$query = new EntityFieldQuery();

		$query->entityCondition('entity_type', 'agency_media')
			->propertyCondition('pid',$pid,'=');

		$result = $query->execute();

		if (isset($result['agency_media'])){

			$media_nids = array_keys($result['agency_media']);

			return agency_media_load_multiple($media_nids);

		}

		return FALSE;

	}

	//Get the number (at the end of the media field)
	function getMediaNumber($parts){
		return end($parts);
	}

	//Get the type of media this is
	function getMediaType($parts){

		$type = strtolower($parts[1]);
		
		$id = $this->getMediaNumber($parts);

		switch($type){
			case 'image':
				if (intval($id) === 60){
					$type = 'epc';
				}
				break;
			case 'floor':
				$type = 'floorplan';
				break;
			case 'document':
				if (intval($id) !== 50){
					$type = 'brochure';
				}else{
					$type = 'epc';
				}
				break;
		}

		return $type;

	}

	//Get the key for the field holding the caption for the media 
	function getMediaCaptionKey($parts){

		$key = array_slice($parts, 0, -1);

		$key[] = 'TEXT';

		$key[] = $this->getMediaNumber($parts);

		return implode('_', $key);

	}

	//Get the media caption
	function getMediaCaption($fields, $parts){
		
		$key = $this->getMediaCaptionKey($parts);

		if (isset($fields[$key])){
			return $fields[$key];
		}

		return FALSE;

	}	

	//Add media files to files array
	private function addPropertyMediaFiles($row, $property){

		foreach($row as $key => $value){

			if (stripos($key, 'MEDIA') !== FALSE && stripos($key, 'TEXT') === FALSE){

				if (strlen($value) > 0){

					$parts = explode('_', $key);

					$this->files[] = array(
						'pid' => $property->pid,
						'filename' => $value,
						'type' => $this->getMediaType($parts),
						'caption' => $this->getMediaCaption($row, $parts),
					);

				}

			}

		}

	}

	//Get the next available position in the heirachy of media for this property
	function getMediaNextPosition($pid){

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

	//Get the next available position in the hierachy of images for this property
	function getImageNextPosition($pid){

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

	//Create a managed file in the file_managed database and move the file source to the archive
	function createManagedFile($filename){

		$import = $this->buildImportMediaPath($filename);

		$public = $this->buildPublicMediaPath($filename);

		$archive = $this->buildArchiveMediaPath($filename);

		if (file_exists($import)){
			
			$handle = fopen($import,'r');

			if ($handle){

				$file = file_save_data($handle,$public);

				fclose($handle);

				if ($file){

					if (!rename($import, $archive)){
						$this->log('error','Unable to move ' . $filename . ' to archive path');
					}

					return $file;

				}

			}

		}

		return FALSE;

	}

	//Set a managed file in drupal for deletion
	function setManagedFileForDeletion($fid){
		
		$file = file_load($fid);

		if ($file){
			$file->status = FILE_STATUS_TEMPORARY;
			if (file_save($file)){
				return TRUE;
			}
		}

		return FALSE;

	}


	//Iterate the media files in the files array, creating a managed file and agency_media entity for each
	private function processMediaFiles(){

		foreach($this->files as $file){

			if ($this->propertyExists($file['pid'])){

				$managed = $this->createManagedFile($file['filename']);

				if ($managed){

					$data = array(
						'pid' => $file['pid'],
						'fid' => $managed->fid,
						'type' => $file['type'],
						'status' => 1,
					);

					if ($file['type'] === 'image'){
						$data['position'] = $this->getImageNextPosition($file['pid']);
					}else{
						$data['position'] = $this->getMediaNextPosition($file['pid']);
					}

					if ($media = entity_create('agency_media',$data)){
						$media->save();
					}

				}

			}else{
				$this->log('error','Unable to create media entity, property '. $file['pid'] .'does not exist');
			}

		}

	}

	//******************************
	//** END OF METHODS FOR MEDIA **
	//******************************	

	//******************************************
	//** START OF METHODS FOR CATEGORISATIONS **
	//******************************************

	//Get the categorisations added to a property
	function getCategorisationsForProperty($pid){

		$query = new EntityFieldQuery();

		$query->entityCondition('entity_type', 'agency_categorisation')
			->propertyCondition('pid',$pid,'=');

		$result = $query->execute();

		if (isset($result['agency_categorisation'])){

			$categorisation_nids = array_keys($result['agency_categorisation']);

			return $categorisation_nids;

		}

		return FALSE;

	}

	//Create the categorisation for the property
	function createCategorisation($data){

		if ($categorisation = entity_create('agency_categorisation', $data)){
			return $categorisation->save();
		}

		return FALSE;

	}

	//Get the categorisation mapping
	function getCategorisationMapping($name, $data){

		$query = new EntityFieldQuery();

		$query->entityCondition('entity_type', 'agency_mapping')
			->propertyCondition('name',$name,'=')
			->propertyCondition('data',$data,'=');

		$results = $query->execute();

		if (isset($results['agency_mapping'])){

			$mapping_nids = array_keys($results['agency_mapping']);

			$mapping_nid = reset($mapping_nids);

			return agency_mapping_load($mapping_nid);			
		
		}

		return FALSE;

	}

	//Iterate the fields that have categorisation mapping and add a categorisation to the categorisations array
	function addCategorisations($fields, $property){

		global $user;

		foreach($fields as $key => $value){;

			if (stripos($this->config['lookups'], $key) !== FALSE && strlen($value) > 0){

				$mapping = $this->getCategorisationMapping($key, $value);

				if ($mapping){

					$this->categorisations[] = array(
						'uid' => $user->uid,
						'pid' => $property->pid,
						'vid' => $mapping->vid,
						'tid' => $mapping->tid,
					);
				}

			}

		}

	}

	//Iterate through the categorisaitons array and create an agency_categorisation for each
	private function processCategorisations(){

		foreach($this->categorisations as $categorisation){
			if ($this->propertyExists($categorisation['pid'])){
				$this->createCategorisation($categorisation);
			}else{
				$this->log('error','Unable to create categorisation entity, property '. $categorisation['pid'] .'does not exist');				
			}		
		}

	}	

	//****************************************
	//** END OF METHODS FOR CATEGORISATIONS **
	//****************************************	

	//***********************************
	//** START OF METHODS FOR FEATURES **
	//***********************************	

	//Get the features for a property
	function getFeaturesForProperty($pid){

		$query = new EntityFieldQuery();

		$query->entityCondition('entity_type', 'agency_feature')
			->propertyCondition('pid',$pid,'=');

		$result = $query->execute();

		if (isset($result['agency_feature'])){

			$feature_nids = array_keys($result['agency_feature']);

			return $feature_nids;

		}

		return FALSE;

	}

	//Create a property feature
	function createFeature($data){

		if ($feature = entity_create('agency_feature',$data)){
			return $feature->save();
		}

		return FALSE;

	}


	//Iterate the fields that are features and add a feature to the features array
	function addFeatures($fields, $property){

		foreach($fields as $key => $value){

			if (stripos($key,'FEATURE') !== FALSE){

				if (strlen($value) > 0){

					$this->features[] = array(
						'pid' => $property->pid,
						'description' => mb_convert_encoding($value, 'latin1'),
					);

				}

			}

		}

	}

	//Iterate through the features array and create an agency_feature entity for each
	private function processFeatures(){

		foreach($this->features as $feature){
			if ($this->propertyExists($feature['pid'])){
				$this->createFeature($feature);
			}else{
				$this->log('error','Unable to create feature entity, property '. $feature['pid'] .'does not exist');								
			}
		}

	}	

	//*********************************
	//** END OF METHODS FOR FEATURES **
	//*********************************		
}