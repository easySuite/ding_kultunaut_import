<?php
/**
 * @file
 * Admin settings.
 */

/**
 * Module system settings form.
 */
function ding_kultunaut_import_settings_form($form, &$form_state) {
  $form = array();

  $form['ding_kultunaut_import_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Feed URL'),
    '#description' => t('URL to Kultunaut feed XML.'),
    '#maxlength' => 256,
    '#default_value' => variable_get('ding_kultunaut_import_url', ''),
  );

  return system_settings_form($form);
}
