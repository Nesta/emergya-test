<?php
/**
 * Page callback for VAR Injector's main admin page.
 */
function maria_import_export_admin_form() {
}

/**
 * Import button callback for the VAR rule listing form.
 */
function maria_import_export_admin_import_button($form, &$form_state) {
  $button = $form_state['clicked_button'];
  $crid = $button['#crid'];
  $form_state['redirect'] = 'admin/maria_import_export/import/'. $crid;
}

/**
 * Export button callback for the VAR rule listing form.
 * Redirects the user to the confirmation form.
 */
function maria_import_export_admin_export_button($form, &$form_state) {
  $button = $form_state['clicked_button'];
  $crid = $button['#crid'];
  $form_state['redirect'] = 'admin/maria_import_export/export/'. $crid;
}

/**
 * Theme function for the VAR Injector admin overview form.
 */
function theme_maria_import_export_admin_form($form) {
  $headers = array(t('Select option:'));
  $rows[] = array(l(t('Import'), 'admin/maria_import_export/import'), l(t('Export'), 'admin/maria_import_export/export'));
  $output .= theme('table', $headers, $rows);
  $output .= drupal_render($form);
  return $output;
}

/**
 * Constructor for the VAR rule import form.
 */
function maria_import_export_import($form_state, $crid = NULL){
 // ....
}

/**
 * Validation callback for the VAR rule import form.
 */
function maria_import_export_import_do_validate($form, &$form_state) {

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
function maria_import_export_import_save($form, &$form_state) {
 // ....
}
/**
 * Constructor for the VAR rule import form.
 */
function maria_import_export_get_variables_to_export($form_state, $crid = NULL) {

  // Getting the modules
 // ....
  $res=db_query("SELECT * FROM {variable} WHERE name = '%s' AND value = '%s'", $form_state['values']['name'], $form_state['values']['value']);
  while ($variable = db_fetch_object($res)) {
    if (isset($form_state['values'][$variable->name])) {
      $form_state['values'][$variable->name]=$variable->name;
	  $form_state['values'][$variable->value]=$variable->value;
    }
  }

  // Get variables from DB
  //....

  $header = array(t('Name'), t('Value'), t('Modules'));

  $form['checkboxes'] = array(
    '#type' => 'tableselect',
   // ....
  );

  $form['buttons']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
    '#submit' => array('maria_import_export_variables_save'),
  );

  return $form;
}

/**
 * Submit button callback for the VAR rule import form.
 */
function maria_import_export_variables_save($form, &$form_state) {
 // ....
}

//....
