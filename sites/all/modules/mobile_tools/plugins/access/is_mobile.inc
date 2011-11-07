<?php

/**
 * @file
 * Plugin to provide access control based on evaluated PHP.
 */

/**
 * Implementation of specially named hook_ctools_access().
 */
function mobile_tools_is_mobile_ctools_access() {
  $args['is_mobile'] = array(
    'title' => t("Mobile Browser"),
    'description' => t('Control access based on mobile browser.'),
    'callback' => 'mobile_tools_is_mobile_ctools_access_check',
    'default' => array('description' => '', 'browser' => ''),
    'settings form' => 'mobile_tools_is_mobile_ctools_access_settings',
    'summary' => 'mobile_tools_is_mobile_ctools_acesss_summary',
    'all contexts' => TRUE,
  );

  return $args;
}

/**
 * Settings form for the 'by perm' access plugin
 *
 * @todo Need a way to provide a list of all available contexts to be used by
 *       the eval-ed PHP.
 */
function mobile_tools_is_mobile_ctools_access_settings(&$form, &$form_state, $conf) {
  $perms = array();

  $options = mobile_tools_device_groups();
  $options['mobile'] = 'Mobile device';
  $form['settings']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Administrative desc'),
    '#default_value' => $conf['description'],
    '#description' => t('A description for this test for administrative purposes.'),
  );
  $form['settings']['mobile_any'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mobile brower'),
    '#default_value' => $conf['mobile_any'],
    '#description' =>  t('Access will be granted if the client browser is any mobile.'),
  );
  $form['settings']['mobile_list'] = array(
    '#type' => 'select',
    '#title' => t('User Agent'),
    '#default_value' => $conf['mobile_list'],
    '#options' => $options,
    '#multiple' => TRUE,
    '#description' =>  t('Access will be granted if the client browser matches the specific groups.'),
  );
}

/**
 * Check for access.
 */
function mobile_tools_is_mobile_ctools_access_check($conf, $contexts) {
  $browser = mobile_tools_is_mobile_device();
  if ($conf['mobile_any'] && $browser['type'] == 'mobile') {
    return TRUE;
  }

  $access = in_array($browser['group'], $conf['mobile_list']);
  if (in_array($browser['type'], $conf['mobile_list']) && $browser['type'] == 'mobile') {
    return TRUE;
  }
  return $access;
}

/**
 * Provide a summary description based upon the checked roles.
 */
function mobile_tools_is_mobile_ctools_acesss_summary($conf, $contexts) {
  return check_plain($conf['description']);
}
