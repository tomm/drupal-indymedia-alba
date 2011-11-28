<?php

/**
 * @file
 * Theme functions used by Service Links.
 */

/**
 * Build a single link following the style rules.
 */
function theme_service_links_build_link($text, $url = array(), $image = NULL, $nodelink = FALSE, $style, $attributes = array()) {
  if ($nodelink) {
    $query = isset($url[1]) ? $url[1] : NULL;

    switch ($style) {
      case SERVICE_LINKS_STYLE_TEXT:
        $link = array(
          'title' => $text,
          'href' => $url[0],
          'query' => $query,
          'attributes' => $attributes,
        );
        break;
      case SERVICE_LINKS_STYLE_IMAGE:
        $link = array(
          'title' => theme('image', service_links_expand_path($image), $text),
          'href' => $url[0],
          'query' => $query,
          'attributes' => $attributes,
          'html' => TRUE,
        );
        break;
      case SERVICE_LINKS_STYLE_IMAGE_AND_TEXT:
        $link = array(
          'title' => theme('image', service_links_expand_path($image), $text) .' '. $text,
          'href' => $url[0],
          'query' => $query,
          'attributes' => $attributes,
          'html' => TRUE,
        );
        break;
     case SERVICE_LINKS_STYLE_EMPTY:
       $link = array(
         'title' => '<span style="display:none;">' . $text . '</span>',
         'href' => $url[0],
         'query' => $query,
         'attributes' => $attributes,
         'html' => TRUE,
       );
       break;
     default:
       $link = theme($style, $text, $url, $image, $nodelink, $attributes);
    }
  }
  else {
    $attributes = array('attributes' => $attributes);
    if (isset($url[1])) {
      $attributes['query'] = $url[1];
    }

    switch ($style) {
      case SERVICE_LINKS_STYLE_TEXT:
        $link = l($text, $url[0], $attributes);
        break;
      case SERVICE_LINKS_STYLE_IMAGE:
        $attributes = array_merge($attributes, array('html' => TRUE));
        $link = l(theme('image', service_links_expand_path($image), $text), $url[0], $attributes);
        break;
      case SERVICE_LINKS_STYLE_IMAGE_AND_TEXT:
        $attributes = array_merge($attributes, array('html' => TRUE));
        $link = l(theme('image', service_links_expand_path($image), $text) .' '. $text, $url[0], $attributes);
        break;
      case SERVICE_LINKS_STYLE_EMPTY:
        $attributes = array_merge($attributes, array('html' => TRUE));
        $link = l('<span style="display:none;">' . $text . '</span>', $url[0], $attributes);
        break;
      case SERVICE_LINKS_STYLE_FISHEYE:
        $attributes['attributes']['class'] = isset($attributes['attributes']['class']) ? $attributes['attributes']['class'] .' fisheyeItem' : 'fisheyeItem' ;
        $attributes = array_merge($attributes, array('html' => TRUE));
        $link = l(theme('image', service_links_expand_path($image, 'fisheye', 'icons'), $text, NULL, NULL, FALSE) .'<span>'. $text .'</span>', $url[0], $attributes);
        break;
      default:
        $link = theme($style, $text, $url, $image, $nodelink, $attributes);
    }
  }

  return $link;
}

/**
 * Format the items shown in the node.
 */
function theme_service_links_node_format($links, $label = NULL) {
  if (isset($label) && !empty($label)) {
    return '<div class="service-links"><div class="service-label">'. t('@label', array('@label' => $label)) .' </div>'. theme('links', $links) .'</div>';
  }
  else {
    return '<div class="service-links">'. theme('links', $links) .'</div>';
  }
}

/**
 * Format the items shown in one of the provided block.
 */
function theme_service_links_block_format($items, $style = SERVICE_LINKS_STYLE_IMAGE_AND_TEXT) {
  if (empty($items)) {
    return;
  }

  switch ($style) {
    case SERVICE_LINKS_STYLE_IMAGE:
      $output = implode(' ', $items);
      break;
    default:
      $output = theme('item_list', array_values($items));
      break;
  }

  return '<div class="service-links">' . $output . '</div>';
}

/**
 * Format the items shown in the Fisheye block.
 */
function theme_service_links_fisheye_format($items) {
  drupal_add_js(service_links_expand_path('interface.js', 'javascript'));
  drupal_add_js(service_links_expand_path('service_links_fisheye.js', 'javascript'));
  drupal_add_css(service_links_expand_path('service_links_fisheye.css', 'css'));

  return "<div class=\"fisheye\"><div class=\"fisheyeContainer\">\r\n". implode("\r\n", $items) ."\r\n</div></div>";
}
