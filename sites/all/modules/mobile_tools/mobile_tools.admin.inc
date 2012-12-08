<?php

/**
 * @file
 * Generate configuration form and save settings.
 */

/**
 * Configuration form.
 */
function mobile_tools_configuration_form() {
  // Get the base URL of the drupal installation
  global $base_url;

  $form['mobile_tools_site_urls'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site URLs'),
    '#collapsible' => TRUE,
    '#description' => t('Enter a mobile and desktop URL for your website; following the standard practice of naming the mobile site either m.domain.com or www.domain.mobi is recommended. If both URLs are different, site visitors will be redirected to the appropriate site. If both URLs are the same, visitors will not be redirected but <a href="@theme-switching">theme switching</a> will still occur.', array('@theme-switching' => '/admin/settings/mobile-tools/theme-switching')),
  );
  $form['mobile_tools_site_urls']['mobile_tools_mobile_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile URL'),
    '#default_value' => variable_get('mobile_tools_mobile_url', mobile_tools_create_mobile_url($base_url)),
    '#description' => t('Enter the URL of your mobile website.'),
  );
  $form['mobile_tools_site_urls']['mobile_tools_desktop_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Desktop URL'),
    '#default_value' => variable_get('mobile_tools_desktop_url', $base_url),
    '#collapsible' => TRUE,
    '#description' => t('Enter the URL of your desktop website.'),
  );
  $form['mobile_tools_meta_tags'] = array(
    '#type' => 'fieldset',
    '#title' => t('Meta tags'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['mobile_tools_meta_tags']['mobile_tools_add_header'] = array(
   '#type' => 'checkbox',
   '#title' => t('Add mobile meta tags'),
   '#default_value' => variable_get('mobile_tools_add_header', 1),
   '#description' => t('Add mobile-specific meta tags including viewport and HandheldFriendly.'),
  );
  $form['mobile_tools_redirection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Redirection'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['mobile_tools_redirection']['mobile_tools_redirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable automatic redirection of site visitors'),
    '#default_value' => variable_get('mobile_tools_redirect', FALSE),
    '#description' => t('Enable this option to automatically detect site visitors using a mobile device and redirect them to the mobile site.'),
  );
  $form['mobile_tools_redirection']['mobile_tools_cookie_session'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirection cookie lifespan'),
    '#size' => 10,
    '#maxlength' => 10,
    '#default_value' => variable_get('mobile_tools_cookie_session', 3600 * 24 * 30),
    '#description' => t("Site visitors who are automatically redirected to the mobile site can, if they prefer, view the desktop version. A cookie will be saved on the visitor's computer to remember their preference. Enter 0 to switch to session cookies which will automatically forget the visitor's preference once they leave the site."),
    '#field_suffix' => t('Seconds'),
  );
  $form['mobile_tools_redirection']['mobile_tools_redirect_exceptions_type'] = array(
    '#type' => 'radios',
    '#title' => t('Redirect on specific pages'),
    '#default_value' => variable_get('mobile_tools_redirect_exceptions_type', 'not-redirect'),
    '#options' => array(
      'not-redirect' => t('Redirect on every page except the listed pages.'),
      'only-redirect' => t('Redirect on only the listed pages.'),
    ),
  );
  $form['mobile_tools_redirection']['mobile_tools_redirect_exceptions'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => variable_get('mobile_tools_redirect_exceptions', ''),
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );
  $form['mobile_tools_notification'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block message'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('You can create a block with a different message on the mobile site than the desktop site. This can be for example used to create a link back to the deskop or mobile site.'),
  );
  $form['mobile_tools_notification']['mobile_notification'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile message'),
    '#default_value' => variable_get('mobile_notification', 'View full site'),
    '#description' => t('This will create a URL back to the desktop site while maintaining the current path.'),
  );
  $form['mobile_tools_notification']['desktop_notification'] = array(
    '#type' => 'textfield',
    '#title' => t('Desktop message'),
    '#default_value' => variable_get('desktop_notification', 'View mobile site'),
    '#description' => t('This will create a URL back to the mobile site while maintaining the current path.'),
  );
  $form['build_mode'] = array(
    '#type' => 'fieldset',
    '#title' => t('Build mode'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t("Add a mobile build mode to Drupal's default full, teaser, print and RSS build modes."),
  );
  $form['build_mode']['mobile_tools_enable_build_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable a Mobile Tools build mode.'),
    '#default_value' => variable_get('mobile_tools_enable_build_mode', 0),
    '#description' => t('Use modules such as <a href="@build-modes">Build modes</a> or <a href="@display-suite">Display Suite</a> to configure build modes.', array('@build-modes' => 'http://drupal.org/project/buildmodes', '@display-suite' => 'http://drupal.org/project/ds')),
  );
  $form['frontpage'] = array(
    '#type' => 'fieldset',
    '#title' => t('Front page'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Configure unique front page settings for site visitors visiting the mobile site.'),
  );
  $form['frontpage']['site_frontpage_mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('Default mobile front page'),
    '#default_value' => variable_get('site_frontpage_mobile', variable_get('site_frontpage', 'node')),
    '#description' => t('The mobile home page displays content from this relative URL. If unsure, specify "node".'),
  );
  $form['frontpage']['default_nodes_main_mobile'] = array(
    '#type' => 'select',
    '#title' => t('Number of posts on mobile main page'),
    '#default_value' => variable_get('default_nodes_main_mobile', 10),
    '#options' => array(
      1 => 1,
      2 => 2,
      3 => 3,
      4 => 4,
      5 => 5,
      6 => 6,
      7 => 7,
      8 => 8,
      9 => 9,
      10 => 10,
      15 => 15,
      20 => 20,
      25 => 25,
      30 => 30,
    ),
    '#description' => t('The default maximum number of posts to display per page on overview pages such as the mobile main page.'),
  );

  return system_settings_form($form);
}

/**
 * Validation of the system settings form
 */
function mobile_tools_configuration_form_validate($form, &$form_state) {
  $form_state['values']['mobile_tools_mobile_url'] = preg_replace('{/$}', '', $form_state['values']['mobile_tools_mobile_url']);
  $form_state['values']['mobile_tools_desktop_url'] = preg_replace('{/$}', '', $form_state['values']['mobile_tools_desktop_url']);
}

/**
 * Configuration form for configuring the mobile context in the theming system
 */
function mobile_tools_themes_configuration_form() {
  // Get a list of the currently available theme names
  $themes = _mobile_tools_list_theme_names();

  // Display a warning message if no themes are available
  if (count($themes) == 0) {
    $form['mobile_tools_theme_warning'] = array(
      '#value' => t('You must enable one or more themes in order to use theme switching.'),
    );
  }
  else {
    $form['mobile_tools_theme_configuration'] = array(
      '#type' => 'fieldset',
      '#title' => t('Theme configuration'),
      '#collapsible' => TRUE,
      '#description' => t('Display a different theme to mobile users.'),
    );
    $form['mobile_tools_theme_configuration']['mobile-tools-theme-switch'] = array(
      '#type' => 'radios',
      '#title' => t('Theme switch'),
      '#default_value' => variable_get('mobile-tools-theme-switch', 'mobile-tools-no-switch'),
      '#options' => array(
        'mobile-tools-no-switch' => t('No theme switch'),
        'mobile-tools-mobile-device' => t('Switch theme for a mobile device *'),
        'mobile-tools-mobile-url' => t('Switch theme based on the URL'),
      ),
      '#description' => t('Choose how theme switching should be handled. *This is not recommended since using one url for both mobile and desktop site disable the drupal caching.'),
    );
    $form['mobile_tools_theme_configuration']['mobile_tools_theme_name'] = array(
      '#type' => 'select',
      '#title' => t('Mobile theme'),
      '#default_value' => variable_get("mobile_tools_theme_name", FALSE),
      '#options' => $themes,
      '#description' => t('Select a theme to display to mobile devices.'),
    );

    $mobile_detection_module = variable_get('mobile-tools-device-detection', NULL);

    if (isset($mobile_detection_module)) {
      $mobile_groups = module_invoke($mobile_detection_module, 'device_groups_info');

      $form['mobile_tools_group_config'] = array(
        '#type' => 'fieldset',
        '#title' => t('Configure mobile theme per device group'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#description' => t('Display different themes to different groups of devices.'),
      );

      foreach ($mobile_groups as $group => $group_title) {
        $form['mobile_tools_group_config'][$mobile_detection_module . '_' . $group] = array(
          '#type' => 'fieldset',
          '#title' => $group_title,
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
        );
        $form['mobile_tools_group_config'][$mobile_detection_module . '_' . $group][$mobile_detection_module . '_' . $group . '_enable'] = array(
          '#type' => 'checkbox',
          '#title' => t('Enable theme configuration for this device group'),
          '#default_value' => variable_get($mobile_detection_module . '_' . $group . '_enable', ''),
        );
        $form['mobile_tools_group_config'][$mobile_detection_module . '_' . $group][$mobile_detection_module . '_' . $group . '_theme'] = array(
          '#type' => 'select',
          '#title' => t('Mobile theme'),
          '#default_value' => variable_get($mobile_detection_module . '_' . $group . '_theme', FALSE),
          '#options' => $themes,
          '#description' => t('Select a theme to display to mobile devices which are part of this device group.'),
        );
      }
    }
  }

  return system_settings_form($form);
}

/**
 * Helper function to return a list of available themes.
 */
function _mobile_tools_list_theme_names() {
  global $conf;
  $themes = list_themes();
  $list = array();
  foreach ($themes as $key => $value ) {
    if ($value->status == 1 ) {
      $list[] = $key;
    }
  }
  if (count($list) == 0) {
    return array();
  }
  else {
    return array_combine($list, $list);
  }
}

/**
 * Configuration of external modules
 */
function mobile_tools_external_modules_configuration_form() {
  $form['mobile_tools_site_detection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site detection'),
    '#collapsible' => TRUE,
    '#description' => t('Mobile Tools provides an API which allows other modules or themes to determine if a website visitor is currently viewing the desktop or the mobile site. A site detection module must be installed in order to detect the current site type.'),
  );

  // Get a list of modules which provide site detection
  $site_detection = mobile_tools_get_module_names(module_implements('is_mobile_site'));

  // Display a warning message if no site detection modules are available
  if (empty($site_detection)) {
    $form['mobile_tools_site_detection']['mobile-tools-site-detection-warning'] = array(
      '#value' => t('You must enable one or more site detection modules in order to use site detection.'),
    );
  }
  else {
    $form['mobile_tools_site_detection']['mobile-tools-site-type-detection'] = array(
      '#type' => 'radios',
      '#title' => t('Site detection module'),
      '#default_value' => variable_get('mobile-tools-site-type-detection', NULL),
      '#options' => $site_detection,
      '#description' => t('Choose a module to use for site detection.'),
    );
  }

  $form['mobile_tools_device_detection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Device detection'),
    '#collapsible' => TRUE,
    '#description' => t('Mobile Tools provides an API which allows other modules or themes to determine if a website visitor is using a mobile device. A device detection module must be installed in order to detect devices.'),
  );

  // Get a list of modules which provide device detection
  $device_detection = mobile_tools_get_module_names(module_implements('detect_mobile_device'));

  // Display a warning message if no device detection modules are available
  if (empty($device_detection)) {
    $form['mobile_tools_device_detection']['mobile-tools-device-detection-warning'] = array(
      '#value' => t('You must enable one or more device detection modules in order to use device capability detection.'),
    );
  }
  else {
    $form['mobile_tools_device_detection']['mobile-tools-device-detection'] = array(
      '#type' => 'radios',
      '#title' => t('Device detection module'),
      '#default_value' => variable_get('mobile-tools-device-detection', NULL),
      '#options' => $device_detection,
      '#description' => t('Choose a module to use for device detection.'),
    );
  }

  $form['mobile_tools_device_capability_detection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Device capability detection'),
    '#collapsible' => TRUE,
    '#description' => t("Mobile Tools provides an API which allows other modules or themes to determine the capabilities of a website visitor's mobile device. A device capability detection module must be installed in order to detect device capabilities."),
  );

  // Get a list of modules which provide device capability detection
  $device_capability = mobile_tools_get_module_names(module_implements('determine_device_capability'));

  // Display a warning message if no device capability detection modules are available
  if (empty($device_capability)) {
    $form['mobile_tools_device_capability_detection']['mobile-tools-device-capabilities-warning'] = array(
      '#value' => t('You must enable one or more device capability detection modules in order to use device capability detection.'),
    );
  }
  else {
    $form['mobile_tools_device_capability_detection']['mobile-tools-device-capabilities'] = array(
      '#type' => 'radios',
      '#title' => t('Device capability detection'),
      '#default_value' => variable_get('mobile-tools-device-capabilities', NULL),
      '#options' => $device_capability,
      '#description' => t('Choose a module to use for device capability detection.'),
    );
  }

  return system_settings_form($form);
}

/**
 * Return the human readable name of the modules
 */
function mobile_tools_get_module_names($modules) {
  $output = array();

  foreach ($modules as $module_key => $module_value) {
    $query = "SELECT * FROM {system} WHERE type = '%s' AND name = '%s'";
    $result = db_query($query, 'module', $module_value);
    $item = db_fetch_object($result);
    $info = $item->info;
    $info = unserialize($info);
    $output[$module_value] = $info['name'];
  }

  return $output;
}
