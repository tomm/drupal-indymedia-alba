<?php

function phptemplate_preprocess_node(&$vars) {
  $node = $vars['node'];
  if (count($node->taxonomy)) {
    $vars['terms'] = t('!tags', array('!tags' => $vars['terms']));
  }
  if (theme_get_setting('toggle_node_info_'. $node->type)) {
    $vars['author'] = t('By: !user', array('!user' => theme('username', $node)));
  }
  else {
    $vars['author'] = '';
  }
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb">'. implode(' &#8594; ', $breadcrumb) .'</div>';
  }
}

function phptemplate_username($object) {

  if ($object->uid && $object->name) {
    // Shorten the name when it is too long or it will break many tables.
    if (drupal_strlen($object->name) > 20) {
      $name = drupal_substr($object->name, 0, 15) .'...';
    }
    else {
      $name = $object->name;
    }

    if (user_access('access user profiles')) {
      $output = l($name, 'user/'. $object->uid, array('title' => t('View user profile.')));
    }
    else {
      $output = check_plain($name);
    }
  }
  else if ($object->name) {
    // Sometimes modules display content composed by people who are
    // not registered members of the site (e.g. mailing list or news
    // aggregator modules). This clause enables modules to display
    // the true author of the content.
    if ($object->homepage) {
      $output = l($object->name, $object->homepage, array('class' => 'anonymous', 'title' => t('not verified')));
    }
    else {
      $output = '<span class="anonymous" title="'. t('not verified') .'">'. check_plain($object->name) .'</span>';
    }
  }
  else {
    $output = variable_get('anonymous', t('Anonymous'));
  }

  return $output;
} 

/**
 * Theme an image node teaser
 */
function phptemplate_image_teaser($node) {
 return l(image_display($node, 'thumbnail'), 'node/'. $node->nid, array('class' => 'image'), NULL, NULL, TRUE, TRUE) . $node->teaser;
}
