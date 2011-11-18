<?php
// $Id: indymedia_cities.theme.inc,v 1.1.2.8 2009/06/08 22:51:45 mfb Exp $

/**
 * @file
 * Themeable functions for Indymedia cities module.
 */

/**
 * Given a SimpleXML object, render the HTML cities list.
 *
 * In many cases, you should be able to theme your cities list via CSS
 * without overriding this function.  For example, to render the block
 * as a lower-case, justified paragraph in the footer, you could try:
 *   #footer {
 *     text-align: justify;
 *   }
 *   #footer .block-indymedia_cities {
 *     text-transform: lowercase;
 *   }
 *   #footer .block-indymedia_cities ul, #footer .block-indymedia_cities li, #footer .block-indymedia_cities div {
 *     padding: 0;
 *     margin: 0;
 *     display: inline;
 *   }
 *   #footer .block-indymedia_cities li:after, #footer .block-indymedia_cities li:before {
 *     content: " ";
 *   }
 */
function theme_indymedia_cities($xml) {
  $list = indymedia_cities_parse($xml);

  // treeview list
  if (variable_get('indymedia_cities_collapsible', 0) == 3) {
    return theme('treeview', $list, NULL, 'ul', array('class' => 'indymedia_cities-treeview'));
  }

  // accordion list
  if (variable_get('indymedia_cities_collapsible', 0) == 2) {
    foreach ($list as $key => $section) {
      if (is_array($section)) {
        $list[$key]['data'] = '<a href="#" class="indymedia_cities-header">'. $list[$key]['data'] .'</a>';
      }
    }
    return theme('item_list', $list, NULL, 'ul', array('class' => 'indymedia_cities-accordion'));
  }

  // collapsible list
  elseif (variable_get('indymedia_cities_collapsible', 0)) {
    $output = '';
    foreach ($list as $section) {
      if (is_array($section)) {
        $fieldset = array(
          '#title' => $section['data'],
          '#collapsible' => TRUE,
          '#collapsed' => variable_get('indymedia_cities_collapsed', 0),
          '#value' => theme('item_list', $section['children']),
        );
        $output .= theme('fieldset', $fieldset);
      }
      else {
        $output .= theme('item_list', array($section));
      }
    }
    return $output;
  }

  // list only
  else {
    return theme('item_list', $list);
  }
}

/**
 * Cities list XML parser.
 */
function indymedia_cities_parse($xml) {
  $types = array();
  $regions = array();
  $list = array();
  $type_id = 0;
  $region_id = 0;
  $site_id = 0;

  // Set up the various types of IMC sites.
  foreach ($xml->dict->key as $type) {
    if ($type_id > 0) {
      $types[$type_id]['data'] = strval($type);
      $types[$type_id]['children'] = array();
    } 
    $type_id++;
  }

  // Set up the regions
  foreach ($xml->dict->array[0]->dict->key as $region) {
    if ($region_id > 0) {
      $regions[$region_id]['data'] = strval($region);
      $regions[$region_id]['children'] = array();
    } 
    $region_id++;
  }

  // Load all the "normal" local IMC sites.
  $region_id = 0;
  foreach ($xml->dict->array[0]->dict->array as $region) {
    $site_id = 0;
    if ($region_id == 0) {
      $list[] = l(strval($region->dict->key[$site_id]), strval($region->dict->string[$site_id]));
    }
    else {
      foreach ($region->dict->key as $site) {
        $regions[$region_id]['children'][] = l(strval($site), strval($region->dict->string[$site_id]));
        $site_id++;
      }
    }
    $region_id++;
  }

  // Process sites.
  indymedia_cities_render_type($xml, $types, 1);

  // Project sites.
  indymedia_cities_render_type($xml, $types, 2);

  // Regional sites.
  $type_id = 3;
  $site_id = 0;
  foreach ($xml->dict->array[$type_id]->dict->array as $site) {
    $types[$type_id]['children'][] = l(strval($site->dict->key), strval($site->dict->string));
    $site_id++;
  }

  // Topical sites.
  indymedia_cities_render_type($xml, $types, 4);

  $list = array_merge($list, $regions, $types);
  return $list;
}

/**
 * Helper function for parsing IMC types.
 */
function indymedia_cities_render_type($xml, &$types, $type_id) {
  $site_id = 0;
  foreach ($xml->dict->array[$type_id]->dict->array->dict->key as $site) {
    $types[$type_id]['children'][] = l(strval($site), strval($xml->dict->array[$type_id]->dict->array->dict->string[$site_id]));
    $site_id++;
  }
}

/**
 * Return a themed treeview.
 *
 * @param $items
 *   An array of items to be displayed in the list. If an item is a string,
 *   then it is used as is. If an item is an array, then the "data" element of
 *   the array is used as the contents of the list item. If an item is an array
 *   with a "children" element, those children are displayed in a nested list.
 *   All other elements are treated as attributes of the list item element.
 * @param $title
 *   The title of the list.
 * @param $attributes
 *   The attributes applied to the list element.
 * @param $type
 *   The type of list to return (e.g. "ul", "ol")
 * @return
 *   A string containing the list output.
 */
function theme_treeview($items = array(), $title = NULL, $type = 'ul', $attributes = NULL) {
  $output = '';
  if (isset($title)) {
    $output .= '<h3>'. $title .'</h3>';
  }

  if (!empty($items)) {
    $output .= "<$type". drupal_attributes($attributes) .'>';
    $num_items = count($items);
    foreach ($items as $i => $item) {
      $attributes = array();
      $children = array();
      if (is_array($item)) {
        foreach ($item as $key => $value) {
          if ($key == 'data') {
            $data = '<span>'. $value .'</span>';
          }
          elseif ($key == 'children') {
            $children = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $data = $item;
      }
      if (count($children) > 0) {
        $data .= theme_treeview($children, NULL, $type, $attributes); // Render nested list
      }
      if ($i == 0) {
        $attributes['class'] = empty($attributes['class']) ? 'first' : ($attributes['class'] .' first');
      }
      if ($i == $num_items - 1) {
        $attributes['class'] = empty($attributes['class']) ? 'last' : ($attributes['class'] .' last');
      }
      $output .= '<li'. drupal_attributes($attributes) .'>'. $data ."</li>\n";
    }
    $output .= "</$type>";
  }
  return $output;
}
