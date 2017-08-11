<?php

/**
 * @file
 * Admin settings.
 */

/**
 * Module system settings form.
 */
function ding_kultunaut_import_settings_form($form, &$form_state, $deleted_id) {
  $form = array(
    '#tree' => TRUE,
  );

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
      '#default_value' => variable_get(
        'ding_kultunaut_import_domains',
        array()
      ),
    );
  }

  $options = array(
    '' => t('Select library'),
  );

  // Library mapping.
  $query = db_select('node', 'n');
  $query->innerJoin('domain_access', 'da', 'n.nid = da.nid');
  $query->innerJoin('domain', 'd', 'da.gid=d.domain_id');
  $query->fields('n', array('nid', 'title'));
  $query->condition('n.type', 'ding_library', '=');
  $query->condition('n.status', 1);

  $options = $query->execute()->fetchAllKeyed();

  $form['ding_kultunaut_library_mapping'] = array(
    '#prefix' => '<div id="ding-kultunaut-import-mapping-wrapper">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
    '#title' => t('Import mappings'),
    '#theme' => 'ding_kultunaut_import_mapping_table',
    '#header' => array(
      t('Library'),
      t('Search String'),
      '',
    ),
  );

  $fields = variable_get('ding_kultunaut_library_mapping', array());

  $form_state['ding_kultunaut_library_mapping'] = $fields;
  if (!isset($form_state['fields_count'])) {
    $form_state['fields_count'] = empty($fields) ? 1 : count($fields);
  }

  $fields_count = $form_state['fields_count'];

  for ($i = 0; $i < $fields_count; $i++) {
    $form['ding_kultunaut_library_mapping'][$i]['library'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => isset($fields[$i]['library']) ? $fields[$i]['library'] : '',
    );

    $form['ding_kultunaut_library_mapping'][$i]['searched_string'] = array(
      '#type' => 'textfield',
      '#default_value' => isset($fields[$i]['searched_string']) ? $fields[$i]['searched_string'] : '',
    );

    $form['ding_kultunaut_library_mapping'][$i]['delete'] = array(
      '#type' => 'submit',
      '#value' => 'Delete row ' . $i,
      '#submit' => array('ding_kultunaut_import_mapping_delete'),
      '#limit_validation_errors' => array(),
    );
  }

  $form['ding_kultunaut_library_mapping']['more'] = array(
    '#type' => 'submit',
    '#value' => t('Add another one'),
    '#submit' => array('ding_kultunaut_import_mapping_submit'),
    '#limit_validation_errors' => array(),
    '#ajax' => array(
      'callback' => 'ding_kultunaut_import_mapping_callback',
      'wrapper' => 'ding-kultunaut-import-mapping-wrapper',
    ),
  );

  return system_settings_form($form);
}

/**
 * Ajax handler for more button.
 */
function ding_kultunaut_import_mapping_callback($form, $form_state) {
  return $form['ding_kultunaut_library_mapping'];
}

/**
 * Submit handler for more button.
 */
function ding_kultunaut_import_mapping_submit($form, &$form_state) {
  $form_state['fields_count']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for row delete.
 */
function ding_kultunaut_import_mapping_delete($form, &$form_state) {
  $items = variable_get('ding_kultunaut_library_mapping', array());

  $deletion_id = array_keys(
    $form_state['values']['ding_kultunaut_library_mapping']
  );
  $deletion_id = array_shift($deletion_id);

  unset($items[$deletion_id]);
  unset($form_state['values']['ding_kultunaut_library_mapping'][$deletion_id]);

  foreach ($items as $item) {
    $new_items[] = $item;
  }
  variable_set('ding_kultunaut_library_mapping', $new_items);

  if ($form_state['fields_count'] > 1) {
    $form_state['fields_count']--;
  }

  drupal_rebuild_form($form['form_id'], $form_state, $form);
}
