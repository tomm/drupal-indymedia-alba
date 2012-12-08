<?php
/**
 * @file
 * Generate configuration form and save settings.
 */

/**
 * Configuration form for creating mobile user roles.
 */
function mobile_tools_roles_configuration_form() {
  $form['mobile_tools_roles_configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mobile roles configuration'),
    '#collapsible' => TRUE,
    '#description' => t('Mobile Tools Roles provides the ability to create mobile versions of existing user roles. Mobile roles only apply to site visitors who are using a mobile device and take the place of normal site roles. Permissions for mobile roles can be set on the <a href="@permissions">permissions</a> page.', array('@permissions' => '/admin/user/permissions')),
  );
  $form['mobile_tools_roles_configuration']['mobile_tools_enable_roles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable mobile user roles'),
    '#default_value' => variable_get('mobile_tools_enable_roles', 0),
    '#description' => t('When activated, site visitors will get the mobile versions of their normal roles when the site is being mobilized.'),
  );

  if (variable_get('mobile_tools_enable_roles', 0)) {
    $form['mobile_tools_roles'] = array(
      '#type' => 'fieldset',
      '#title' => t('Mobile roles'),
      '#collapsible' => TRUE,
      '#tree' => TRUE,
      '#description' => t('Enable or disable the mobile version of each user role. When no mobile role is created, the user will keep its normal role. The settings can also be configured in the !roles configuration section.', array('!roles' => l('roles', 'admin/user/roles'))),
    );

    // Build a query to select all current roles and role names
    $query = "SELECT r.rid, r.name FROM {role} r";

    // Execute the query
    $result = db_query($query);

    // Get the current Mobile Tools Roles setting
    $roles = variable_get('mobile_tools_roles', array());

    // Process all of the currently available roles
    while ($role = db_fetch_object($result)) {
      // Build a query to count the current number of Mobile Tools Roles relations
      $query = "SELECT COUNT(*) as count FROM {mobile_tools_roles_relations} WHERE mrid = %d";

      // Execute the query
      $count = db_result(db_query($query, $role->rid));

      // Display a checkbox to create a mobile version of each non-mobile role
      if ($count == 0) {
        $form['mobile_tools_roles'][$role->rid] = array(
          '#type' => 'checkbox',
          '#title' => $role->name,
          '#default_value' => isset($roles[$role->rid]) ? $roles[$role->rid] : 0,
          '#description' => t('Enable to create the %role role.', array('%role' => $role->name .' (Mobile)')),
        );
      }
    }

    $form['mobile_tools_roles']['mobile_tools_roles_overview'] = array(
      '#value' => _mobile_tools_roles_overview(),
    );
  }

  // Add a submit function
  $form['#submit'][] = 'mobile_tools_roles_configuration_form_submit';

  return system_settings_form($form);
}

/**
 * Submit function for the mobile_tools_roles_configuration_form().
 */
function mobile_tools_roles_configuration_form_submit($form, &$form_state) {
  // Check if the mobile roles fieldset is available
  if (isset($form_state['values']['mobile_tools_roles'])) {
    // Process each of the roles found in the mobile roles fieldset tree
    foreach ($form_state['values']['mobile_tools_roles'] as $rid => $value) {
      // Get information about the current role ID
      $mobile_role = mobile_tools_roles_get_mobile_role($rid);

      // If a role's checkbox was checked add a mobile version of the role, otherwise
      // delete any mobile versions of the role
      if (!empty($mobile_role) && $value == 0) {
        _mobile_tools_roles_edit_mobile_role('delete', $rid, $mobile_role['mrid']);
      }
      elseif (empty($mobile_role) && $value == 1) {
        _mobile_tools_roles_edit_mobile_role('add', $rid);
      }
    }
  }
}

/**
 * Helper function to display an overview of mobile roles.
 */
function _mobile_tools_roles_overview() {
  // Create a variable to hold the table output
  $output = '';

  // Build a query to select all of the current Mobile Tools Roles relations
  $query = "SELECT * FROM {mobile_tools_roles_relations}";

  // Execute the query
  $result = db_query($query);

  // Create an array to store the table rows
  $rows = array();

  // Process all of the currently available relations
  while ($item = db_fetch_object($result)) {
    // Build a query to select all of the roles which match a given role ID
    $query = "SELECT * FROM {role} WHERE rid = %d";

    // Execute the query for the current relation's role ID
    $result1 = db_query($query, $item->rid);

    // Execute the query for the current relation's mobile role ID
    $result2 = db_query($query, $item->mrid);

    // Retrieve the name of the role associated with the role ID and mobile row ID
    $rows[] = array(l(db_fetch_object($result1)->name, 'admin/user/permissions/' . $item->rid), l(db_fetch_object($result2)->name, 'admin/user/permissions/' . $item->mrid));
  }

  // Create an array of table headers
  $headers = array(t('Original role'), t('Mobile role'));

  // Display the table if there are any rows available
  if (count($rows) != 0) {
    $output = theme('table', $headers, $rows);
  }

  return $output;
}

/**
 * Helper function to save and delete mobile roles.
 *
 * @param $op
 *   The operation to perform.
 *   -delete: Delete the mobile role.
 *   -add: Add the mobile role.  
 * @param $rid
 *   The role ID of the equivalent desktop role.
 * @param $mrid
 *   The role ID of the mobile role. Necessary only when deleting a mobile
 *   role. 
 */
function _mobile_tools_roles_edit_mobile_role($op, $rid, $mrid = '') {
  switch ($op) {
    case 'delete':
      // Delete the mobile role
      db_query("DELETE FROM {role} WHERE rid = %d", $mrid);

      // Delete the mobile role's relation to an equivalent user role
      db_query("DELETE FROM {mobile_tools_roles_relations} WHERE mrid = %d", $mrid);

      // Delete the mobile role's permission
      db_query("DELETE FROM {permission} WHERE rid = %d", $mrid);
    break;
    case 'add':
      // Build a query to find the name of a roll with a certain role ID
      $query = "SELECT name FROM {role} WHERE rid = %d";

      // Execue the query
      $name = db_result(db_query($query, $rid));

      // Build and execute a query to insert a new role
      $result = db_query("INSERT INTO {role} (name) VALUES ('%s')", $name .' (Mobile)');

      // Find the ID of the last insert
      $mrid = db_last_insert_id('role', 'rid');

      // Execute a query to add a relationship between the role ID and the mobile role ID
      db_query("INSERT INTO {mobile_tools_roles_relations} (rid, mrid) VALUES (%d, %d)", $rid, $mrid);

      // Find all of the permissions associated with the role ID
      $perm = db_fetch_object(db_query("SELECT * FROM {permission} WHERE rid = %d", $rid));

      // If the role ID has any associated permissions, add them to the mobile role ID
      if (!empty($perm)) {
        db_query("INSERT INTO {permission} (rid, perm, tid) VALUES (%d, '%s', %d)", $mrid, $perm->perm, $perm->tid);
      }
      break;  
  }
}
