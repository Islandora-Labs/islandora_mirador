<?php

/**
 * @file
 * Admin form for the module.
 */

/**
 * Form definition for the admin settings.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_mirador_admin_settings_form(array $form, array &$form_state) {
  $get_default_value = function($name, $default) use($form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
  }
  $form = array(
    'islandora_mirador_url' => array(
      '#type' => 'textfield',
      '#title' => t('Mirador Server URL'),
      '#description' => t('The URL of the Miradora server. Defaults to http://localhost:8000.'),
      '#default_value' => $get_default_value('islandora_mirador_url', 'http://localhost:8000'),
    ),
    'islandora_mirador_layout' => array(
      '#type' => 'textfield',
      '#title' => t('Mirador Layout Style'),
      '#description' => t('The default layout for Mirador panes. Defaults to 1x1'),
      '#default_value' => $get_default_value('islandora_mirador_layout', '1x1'),
    ),
  );
  return system_settings_form($form);
}
