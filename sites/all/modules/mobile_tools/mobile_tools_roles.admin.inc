<?php
/**
 * @file
 * Mobile Tools Admin pages
 */
 
/**
* Configuration form for creating mobile user roles
*/  
function mobile_tools_roles_configuration_form() {
  $form['mobile_tools_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mobile Tools settings'),
    '#collapsible' => TRUE,
    '#description' => t('The Mobile Tools module allows the creation of a mobile version of each role . these mobile versions of each role will be assigned to the mobile user. In the !url configuration panel, you can assign permissions to the mobile user.', array('!url' => l('permissions', 'admin/user/permissions'))),
  );
  $form['mobile_tools_settings']['mobile_tools_enable_roles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate mobile user roles' ),
    '#description' => t('When activated, mobile users will get the mobile versions of their normal roles when the site is being mobilized.'),
    '#default_value' => variable_get('mobile_tools_enable_roles', 0),
  );
  
  if(variable_get('mobile_tools_enable_roles', 0)){
    $form['mobile_tools_roles'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Mobile roles'),
      '#collapsible' => TRUE,
      '#description' => t('Enable or disable the mobile version of each user role. When no mobile role is created, the user will
      keep its normal role. The settings can also be configured in the !roles configuration section.', array('!roles' => l('roles', 'admin/user/roles'))),
      '#suffix' => mobile_tools_roles_overview()
    );
    $result = db_query("SELECT r.rid, r.name FROM {role} r");
    $roles = variable_get('mobile_tools_roles', array());
    while ($role = db_fetch_object($result)) {
      $query = "SELECT COUNT(*) as count FROM {mobile_tools_roles_relations} WHERE mrid = %d";
      $count = db_result(db_query($query, $role->rid));
      if($count == 0){
        $form['mobile_tools_roles'][$role->rid] = array(
          '#type' => 'checkbox',
          '#truee' => TRUE, 
          '#title' => $role->name,
          '#default_value' => isset($roles[$role->rid]) ? $roles[$role->rid] : 0,
          '#description' => t('Enabling will create the %role role . the name can be changed afterwards in the !roles settings page', array('%role' => $role->name .' (Mobile)', '!roles' =>  l('roles', 'admin/user/roles'))),
        );
      }
    }
  }
  $form['#submit'][] = 'mobile_tools_roles_configuration_form_submit';
  return system_settings_form($form);
}

/**
 * Submit function for the mobile tools / mobile roles configuration page
 *  
 */ 
function mobile_tools_roles_configuration_form_submit($form, &$form_state) {
  if(isset($form_state['values']['mobile_tools_roles'])){
    foreach($form_state['values']['mobile_tools_roles'] as $rid => $value){
      $mobile_role = mobile_tools_roles_get_mobile_role($rid);
      if(!empty($mobile_role) && $value == 0){
        mobile_tools_roles_edit_mobile_role('delete', $rid, $mobile_role['mrid']);
      }
      elseif(empty($mobile_role) && $value == 1){
        mobile_tools_roles_edit_mobile_role('add', $rid);
      }
    }
 }
}