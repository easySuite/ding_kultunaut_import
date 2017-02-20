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

  if (module_exists('domain')) {
    $options = array();
    foreach (domain_domains() as $data) {
      // The domain must be valid.
      if ($data['valid'] || user_access('access inactive domains')) {
        // Filter checkboxes but not select lists.
        $options[$data['domain_id']] = check_plain($data['sitename']);
      }
    }

    $form['domain_prefix'] = array(
      '#type' => 'container',
      '#prefix' => '<h2>Domains settings</h2>',
    );

    $form['ding_kultunaut_import_domains'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Publish to'),
      '#options' => $options,
      '#required' => TRUE,
      '#description' => t('Select which affiliates can access this content.'),
      '#default_value' => variable_get('ding_kultunaut_import_domains', array()),
    );
  }

  return system_settings_form($form);
}
