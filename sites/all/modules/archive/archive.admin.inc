<?php
// $Id: archive.admin.inc,v 1.6.2.1 2010/06/16 18:00:01 rmuilwijk Exp $

/**
 * Form building callback for the archive settings page.
 */
function archive_admin_settings() {
  $types = node_get_types();
  $final_types = array();
  foreach ($types as $key => $value) {
    $final_types[$key] = $value->name;
  }

  $form['archive_type_filters'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Content types available in archive'),
    '#default_value' => variable_get('archive_type_filters', array()),
    '#options'       => $final_types,
    '#description'   => t('Posts of these types will be displayed in the archive.')
  );

  return system_settings_form($form);
}
