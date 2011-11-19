<?php

function indymedia_alba_preprocess_page(&$vars, $hook) {
  if ( (arg(0) == 'user') AND (arg(2) == 'contact') ) $vars['title'] = 'Contact';
  if ( (arg(0) == 'user') AND (arg(2) == 'edit') ) $vars['title'] = 'Edit Profile';
  if ($hook == 'page') {
    if (arg(0) == 'user') {
      $vars['tabs'] = str_replace('View', 'Articles', $vars['tabs']);
    }
  }
}

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
    if (arg(0) == 'user') {
      $name = $object->name;
    } else {
      if (drupal_strlen($object->name) > 20) {
        $name = drupal_substr($object->name, 0, 15) .'...';
      } else {
        $name = $object->name;
      }
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

/* BITS TO MAKE WYSIWYG IMAGE INSERT WORK FROM UPLOADED ATTACHMENTS */
/**
 * Theme the attachments list.
 *
 * @ingroup themeable
 */
function phptemplate_upload_form_current($form) {
  $header = array('', t('Delete'), t('List'), t('Description'), t('Weight'), t('Insert into text'), t('Size'));
  drupal_add_tabledrag('upload-attachments', 'order', 'sibling', 'upload-weight');

  foreach (element_children($form) as $key) {
    // Add class to group weight fields for drag and drop.
    $form[$key]['weight']['#attributes']['class'] = 'upload-weight';

    $row = array('');
    $row[] = drupal_render($form[$key]['remove']);
    $row[] = drupal_render($form[$key]['list']);
    $row[] = drupal_render($form[$key]['description']);
    $row[] = drupal_render($form[$key]['weight']);
    $row[] = drupal_render($form[$key]['insertbutton']);
    $row[] = drupal_render($form[$key]['size']);
    $rows[] = array('data' => $row, 'class' => 'draggable');
  }
  $output = theme('table', $header, $rows, array('id' => 'upload-attachments'));
  $output .= drupal_render($form);
  return $output;
}

/**
 * Theme the attachment form.
 * Note: required to output prefix/suffix.
 *
 * @ingroup themeable
 */
function phptemplate_upload_form_new($form) {
	// add wysiwyg insert buttons
	foreach (element_children($form['files']) as $key) {
		$filepath = $form['files'][$key]['filepath']['#value'];
		$fid = $form['files'][$key]['fid']['#value'];
		if (image_get_info($filepath)) {
			$thumb_path = 'sites/default/files/imagecache/small_thumb/' . $filepath;
			/* The javascript for this is in imc_alba module */
			$form['files'][$key]['insertbutton'] = array('#type' => 'markup', '#value' => 
			'<button type="button" class="insertimg" id="'.file_create_url($filepath).'">Big</button>'.
			'<button type="button" class="insertthumb" id="'.file_create_url($thumb_path).'">Small</button>'
			);
		}
	}
	drupal_add_tabledrag('upload-attachments', 'order', 'sibling', 'upload-weight');

	$output = drupal_render($form);
	return $output;
}

