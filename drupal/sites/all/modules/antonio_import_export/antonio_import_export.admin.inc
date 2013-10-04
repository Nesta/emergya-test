<?php
/**
 * Page callback for VAR Injector's main admin page.
 */
function antonio_import_export_admin_form() {

}

/**
 * Import button callback for the VAR rule listing form.
 */
function antonio_import_export_admin_import_button($form, &$form_state) {
  $button = $form_state['clicked_button'];
  $crid = $button['#crid'];
  $form_state['redirect'] = 'admin/antonio_import_export/import/'. $crid;
}

/**
 * Export button callback for the VAR rule listing form.
 * Redirects the user to the confirmation form.
 */
function antonio_import_export_admin_export_button($form, &$form_state) {
  $button = $form_state['clicked_button'];
  $crid = $button['#crid'];
  $form_state['redirect'] = 'admin/antonio_import_export/export/'. $crid;
}

/**
 * Theme function for the VAR Injector admin overview form.
 */
function theme_antonio_import_export_admin_form($form) {
	$headers = array(t('Select option:'));  

	$rows[] = array(
		l('Import', 'admin/antonio_import_export/import'),
		l('Export', 'admin/antonio_import_export/export'),
	);  

	$output .= theme('table', $headers, $rows);
	$output .= drupal_render($form);
	return $output;
}

/**
 * Constructor for the VAR rule import form.
 */
function antonio_import_export_import(&$form_state, $crid = NULL){	

	// Formulario Multipaso

	if (!isset($form_state['storage']['step'])) {
		$form_state['storage']['step'] = 1;
	}

	$default_value = '';
	if (isset($form_state['storage']['values'][$form_state['storage']['step']])) {
		$default_value = $form_state['storage']['values'][$form_state['storage']['step']];
	}

	switch ($form_state['storage']['step']) {
	case 1:
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		$form['load'] = array(
			'#type' => 'file',
			'#title' => t('Upload file to import'),
			'#description' => t('File to import'),
			//'#element_validate' => array('antonio_import_export_import_do_validate'),
		);
		break;
		
	case 2:
	
		// Crea un archivo temporal en el directorio sites/default/files para leer su contenido, y luego es borrado
		$file = file_save_upload('load', file_directory_path(), FILE_EXISTS_REPLACE);
		$f_import = fopen(file_directory_path()."/".$file->filename, "r") or exit("Unable to open file!");	
		
		while(!feof($f_import)){
			$subcadenas = explode(" = ", fgets($f_import));
			$name = $subcadenas[0];
			$value = trim($subcadenas[1]);
			$value = ltrim($value, "\"");
			$value = rtrim($value, "\"");
			
			$options[$name] = array(
				'name' => $name,
				'value' => $value,				
			);			
		}
		fclose($f_import);
		unlink(file_directory_path()."/".$file->filename);
		
		$header = array( 
			'name' => 'Name',
			'value' => 'Value',			
		);	
		
		$form['checkboxes'] = array(
			'#type' => 'tableselect',
			'#header' => $header,
			'#options' => $options,			
			'#empty' => t('No content available.'),
		);
	  break;
	}


	if ($form_state['storage']['step'] > 1) {
		$form['previous'] = array(
		  '#type' => 'submit',
		  '#value' => t('<< Back'),
		);
	}
	if ($form_state['storage']['step'] != 2) {
		$form['next'] = array(
		  '#type' => 'submit',
		  '#value' => t('Upload'),
		);
	}
	else {
		$form['finish'] = array(
		  '#type' => 'submit',
		  '#value' => t('Import'),
		);
	}
	return $form;
}


// Uso está función para el submit del formulario multipaso
function antonio_import_export_import_submit(&$form, &$form_state){	
	$form_state['storage']['values'][$form_state['storage']['step']] = $form_state['values'];

	if ($form_state['clicked_button']['#id'] == 'edit-previous') {
		$form_state['storage']['step']--;
	}
	elseif ($form_state['clicked_button']['#id'] == 'edit-next') {
		$form_state['storage']['step']++;
	}
	elseif ($form_state['clicked_button']['#id'] == 'edit-finish') {
		
		$data_form = array_filter($form_state['values']['checkboxes']);
		foreach ($data_form as $marcada){
			$data = array(
				'name' => $marcada,
				'value' => NULL,  //No se cómo recuperar este valor del formulario :(				
			);	
			drupal_write_record('variable', $data);	
		}
		
		drupal_set_message("Se han importado las variables a la BD, aunque sus valores son NULL...");

		unset($form_state['storage']);
	}
}

/**
 * Validation callback for the VAR rule import form.
 */
function antonio_import_export_import_do_validate($form, &$form_state) {

  $validated = true;
  $loaded = array();
  if ($_FILES['files']['type']['file'] == 'application/octet-stream'){
    $loaded = @parse_ini_file($_FILES['files']['tmp_name']['file']);
    if (sizeof($loaded) == 0) {
      $validated = false;
    }    
  }else{
    $validated = false;
  }
  
  return $validated;
}

/**
 * Submit button callback for the VAR rule import form.
 */
function antonio_import_export_import_save($form, &$form_state) {
	/*Esta función no la uso ya que el proceso de guardado de las variables en la BD lo realizo en la función
	antonio_import_export_import_submit */
}

/**
 * Constructor for the VAR rule import form.
 */
function antonio_import_export_get_variables_to_export($form_state, $crid = NULL) {
    // Getting the modules
  
	// No se cómo determinar que variables pertenecen a cada módulo :(
	$modules = module_list();

	// Get variables from DB
  
	$result = db_query("SELECT * FROM {variable}");	
	
	while( $fila = db_fetch_array($result) ) {		
		$options[$fila['name']] = array(
			'name' => $fila['name'],
			'value' => variable_get($fila['name'], $default),
			'modules' => '???',
		);			
	}	

	$header = array( 
		'name' => 'Name',
		'value' => 'Value',
		'modules' => 'Modules',  //No se cómo determinar que variables pertenecen a cada módulo :(
	);	
	
	$form['checkboxes'] = array(
		'#type' => 'tableselect',
		'#header' => $header,
		'#options' => $options,
		'#empty' => t('No content available.'),
	);

	$form['buttons']['save'] = array(
		'#type' => 'submit',
		'#value' => t('Export'),
		'#submit' => array('antonio_import_export_variables_save'),
	);

	return $form;
}

/**
 * Submit button callback for the VAR rule import form.
 */
function antonio_import_export_variables_save($form, &$form_state) {
	
	$data_form = array_filter($form_state['values']['checkboxes']);
	foreach ($data_form as $marcada){
		//Aquí tampoco se cómo recuperar la key 'value' del formulario, así que la consigo con la función variable_get
		$contenido[$marcada] = $marcada." = "."\"".variable_get($marcada, $default)."\"";
	}	
	
	$data = implode("\n", $contenido);	
	file_save_data($data, 'export.var', FILE_EXISTS_REPLACE);	
	drupal_set_message("El archivo se ha exportado correctamente y se encuentra en el directorio por defecto /sites/default/files/export.var");
	
	return $form;
}
