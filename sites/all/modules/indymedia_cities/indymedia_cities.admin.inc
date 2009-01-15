<?php
// $Id: indymedia_cities.admin.inc,v 1.1.2.3 2008/05/27 16:44:28 mfb Exp $

/**
 * @file
 * Admin functions for Indymedia cities module.
 */

/**
 * Configuration options for Indymedia cities list.
 */
function indymedia_cities_settings() {
  $form = array();
  $languages = drupal_map_assoc(array_keys(indymedia_cities_languages()));
  $form['indymedia_cities_languages'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Languages'),
    '#options' => $languages,
    '#description' => t('The cities list is available in several languages.  Choose which of the available languages to use on this site.'),
    '#default_value' => variable_get('indymedia_cities_languages', array('en')),
  );
  $form['indymedia_cities_refresh'] = array(
    '#type' => 'select',
    '#title' => t('Refresh rate'),
    '#options' => drupal_map_assoc(array(0, 86400, 172800, 345600, 604800, 1209600, 2419200), 'format_interval'),
    '#description' => t('Select how often to refresh the cities list. Note, the cities list will only be refreshed when cron (cron.php) runs.'),
    '#default_value' => variable_get('indymedia_cities_refresh', 24 * 60 * 60),
  );
  $form['collapse'] = array(
    '#type' => 'fieldset',
    '#title' => t('User interface settings'),
    '#collapsible' => TRUE,
  );
  $disabled = !function_exists('simplexml_load_string');
  $form['collapse']['indymedia_cities_collapsible'] = array(
    '#type' => 'radios',
    '#options' => array(0 => t('Normal'), 1 => t('Collapsible'), 2 => t('Accordion'), 3 => t('Treeview')),
    '#title' => t('List type'),
    '#description' => t('The cities list can be formatted as a normal, collapsible, accordion, or treeview list. Accordion requires the <a href="http://drupal.org/project/jquery_ui">jQuery UI module</a>.  Treeview requires the <a href="http://drupal.org/project/jquery_treeview">jQuery Treeview module</a>.'),
    '#default_value' => variable_get('indymedia_cities_collapsible', 0),
    '#disabled' => $disabled,
  );
  $form['collapse']['indymedia_cities_collapsed'] = array(
    '#type' => 'radios',
    '#options' => array(0 => t('Expanded'), 1 => t('Collapsed')),
    '#title' => t('Collapsible list options'),
    '#description' => t('If the cities list is formatted as a collapsible list, the sections can default to expanded or collapsed.'),
    '#default_value' => variable_get('indymedia_cities_collapsed', 0),
    '#disabled' => $disabled,
  );
  $form['collapse']['indymedia_cities_accordion_event'] = array(
    '#type' => 'radios',
    '#options' => array('click' => t('Click'), 'mouseover' => t('Mouseover')),
    '#title' => t('Accordion event'),
    '#description' => t('The event on which to trigger the accordion.'),
    '#default_value' => variable_get('indymedia_cities_accordion_event', 'click'),
    '#disabled' => !module_exists('jquery_ui'),
  );
  $jquery_treeview_js = '$(function() { $("ul.indymedia_cities-treeview").treeview({ animated: "fast", collapsed: true, unique: true }); });';
  $form['collapse']['jquery_treeview_js'] = array(
    '#type' => 'item',
    '#title' => t('Treeview initialization'),
    '#description' => t('<div>If you are using the treeview list, an initialization javascript such as the following should be configured on the jQuery Treeview settings page: <code>%jquery_treeview_js</code></div>', array('%jquery_treeview_js' => $jquery_treeview_js)),
    '#disabled' => !module_exists('jquery_treeview'),
  );
  
  return system_settings_form($form);
}
