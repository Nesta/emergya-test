<?php
/**
 * Page callback for VAR Injector's main admin page.
 */
function hp_import_export_admin_form() {
}

/**
 * Import button callback for the VAR rule listing form.
 */
function hp_import_export_admin_import_button($form, &$form_state) {
  $button = $form_state['clicked_button'];
  $crid = $button['#crid'];
  $form_state['redirect'] = 'admin/hp_import_export/import/'. $crid;
}

/**
 * Export button callback for the VAR rule listing form.
 * Redirects the user to the confirmation form.
 */
function hp_import_export_admin_export_button($form, &$form_state) {
  ....
}

/**
 * Theme function for the VAR Injector admin overview form.
 */
function theme_hp_import_export_admin_form($form) {
  $headers = array(t('Select option:'));
  ....
  $output .= theme('table', $headers, $rows);
  $output .= drupal_render($form);
  return $output;
}

/**
 * Constructor for the VAR rule import form.
 */
function hp_import_export_import($form_state, $crid = NULL){
  ....
}

/**
 * Validation callback for the VAR rule import form.
 */
function hp_import_export_import_do_validate($form, &$form_state) {

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
function hp_import_export_import_save($form, &$form_state) {
  ....
}
/**
 * Constructor for the VAR rule import form.
 */
function hp_import_export_get_variables_to_export($form_state, $crid = NULL) {

  // Getting the modules
  ....

  // Get variables from DB
  ....

  $header = array(t('Name'), t('Value'), t('Modules'));

  $form['checkboxes'] = array(
    '#type' => 'tableselect',
    ....
  );

  $form['buttons']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
    '#submit' => array('hp_import_export_variables_save'),
  );

  return $form;
}

/**
 * Submit button callback for the VAR rule import form.
 */
function hp_import_export_variables_save($form, &$form_state) {
  ....
}

....
