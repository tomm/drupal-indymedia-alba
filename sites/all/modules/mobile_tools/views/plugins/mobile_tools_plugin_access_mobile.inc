<?php

/**
 * Access plugin that provides permission-based access control.
 */
class mobile_tools_plugin_access_mobile extends views_plugin_access {
  /**
   * Check access directly.
   */
  function access($account) {
    return mobile_tools_views_check_access($this->options['mobile_tools'], $this->options['perm'], $this->options['role']);
  }

  function get_access_callback(){
    return array('mobile_tools_views_check_access', array($this->options['mobile_tools'], $this->options['perm'], $this->options['role']));
  }


  /**
   * Display for Views UI.
   */
  function summary_title() {
    $options = array('mobile' => 'Mobile', 'desktop' => 'Desktop');
    if(count($this->options) == 0){
      return t('No device selected');
    }
    elseif (count($this->options['mobile_tools']) > 0) {
      $selected = array();
      foreach($options as $name => $value){
        if($this->options['mobile_tools'][$name]){
          $selected[] = $value;
        }
      }

      $selected = implode(',', $selected);
      return t('Device selected !devices', array('!devices' => "($selected)"));
    }
  }

  /**
   * Define the options for this plugin.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['perm'] = array('default' => 'access content');
    $options['mobile_tools'] = array('default' => 'mobile');
    $options['role'] = array('default' => 0);
    return $options;
  }

  /**
   * Override of options_form().
   */
  function options_form(&$form, &$form_state) {
    // Generate feature options.
    $options = array('mobile' => 'Mobile', 'desktop' => 'Desktop');
    $form['mobile_tools'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Devices that can access this view'),
      '#default_value' => $this->options['mobile_tools'],
      '#options' => $options,
      '#description' => t('Only allow access to this view if the user has access to the selected feature.'),
    );

    // Get list of permissions.
    $perms = array('' => '<'. t('No permission check') .'>');
    foreach (module_list(FALSE, FALSE, TRUE) as $module) {
      if ($permissions = module_invoke($module, 'perm')) {
        $perms[$module] = drupal_map_assoc($permissions);
      }
    }
    $form['perm'] = array(
      '#type' => 'select',
      '#options' => $perms,
      '#title' => t('Permission'),
      '#default_value' => $this->options['perm'],
      '#description' => t('Only users with the selected permission flag will be able to access this display. Note that users with "access all views" can see any view, regardless of other permissions.'),
    );
    // also add role check
    $form['role'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Role'),
      '#default_value' => $this->options['role'],
      '#options' => views_ui_get_roles(),
      '#description' => t('Only the checked roles will be able to access this display. Note that users with "access all views" can see any view, regardless of role.'),
    );
  }

 }