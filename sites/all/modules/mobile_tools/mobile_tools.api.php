<?php

/**
 * @file
 * Hooks provided by the Mobile Tools module.
 *
 * Modules and themes may implement any of the available hooks to interact with
 * mobile device detection and properties.
 */

/**
 * Determine whether the user is viewing the desktop or the mobile site.
 *
 * This hook can be used to implement custom sit type detection. Custom
 * detection methods will be displayed and made selectable on the Mobile Tools
 * administration page.
 */
function hook_is_mobile_site() {
  // Assume that the device is viewing the desktop site until we can prove
  // otherwise
  $type = 'desktop';

  // Implement some sort of algorithm to determine whether the user is viewing
  // the desktop or the mobile site
  $current_page_type = custom_detection_algorithm();

  if ($current_page_type == 'mobile_page') {
    $type = 'mobile';
  }

  return $type;
}

/**
 * Group similar devices together.
 *
 * This hook can be used to implement custom device groups. Certain mobile
 * options are able to be configured per device group. The available Device
 * groups will be displayed and made selectable wherever an appropriate
 * configuration option is found.
 */
function hook_device_groups_info() {
  return array(
    'apple' => 'Apple',
    'microsoft' => 'Microsoft',
    'google' => 'Google',
    'rim' => 'RIM',
  );
}

/**
 * Detect mobile devices.
 *
 * This hook can be used to implement custom mobile device detection. Custom
 * detection methods will be displayed and made selectable on the Mobile Tools
 * administration page.
 */
function hook_detect_mobile_device() {
  // Assume the device is a desktop until we can prove otherwise
  $type = 'Desktop';
  $group = '';

  // Implement some sort of algorithm to determine the properties of the device
  $device_properties = custom_detection_algorithm();

  // Examine the properties of the device to determine if it is mobile and if
  // belongs to a certain 'family' of devices
  if ($device_properties['is_mobile'] == TRUE) {
    $device_type = 'mobile';
  }
  if ($device_properties['device_family'] == 'iphone' || $device_properties['device_family'] == 'ipad') {
    $group = 'apple';
  }

  return array(
    'type' => $type,
    'group' => $group,
  );
}

/**
 * Determine device capabilities.
 *
 * This hook can be used to implement custom device capability detection.
 * Custom detection methods will be displayed and made selectable on the Mobile
 * Tools administration page.
 */
function hook_determine_device_capability($capability) {
  // Implement some sort of algorithm to determine the device's support for the
  // requested capability
  $device_properties = custom_detection_algorithm();

  // Return the value of the device's capability
  return $device_properties[$capability];
}
