<?php
/**
 * Views include file
 *
 */

/**
 * Implementation of hook_views_plugin()
 */
function mobile_tools_views_plugins(){
  return array(
    'module' => 'mobile_tools',
    'access' => array(
      'mobile_tools' => array(
        'title' => t('Mobile Access Setting'),
        'help' => t('Provide an access check for the accessing device'),
        'handler' => 'mobile_tools_plugin_access_mobile',
        'path' => drupal_get_path('module', 'mobile_tools') . '/views/plugins',
        'uses options' => TRUE,
      ),
    ),
  );
}