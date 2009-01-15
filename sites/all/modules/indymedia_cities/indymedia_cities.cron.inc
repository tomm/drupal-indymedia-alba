<?php
// $Id: indymedia_cities.cron.inc,v 1.1.2.1 2008/01/26 03:50:06 mfb Exp $

/**
 * @file
 * Cron functions for Indymedia cities module.
 */

/**
 * Cycle through languages and update if expired.
 */
function indymedia_cities_refresh() {
  $refresh = variable_get('indymedia_cities_refresh', 86400);
  $languages = variable_get('indymedia_cities_languages', array('en' => 1));
  foreach (indymedia_cities_languages() as $language => $uri) {
    if (isset($languages[$language]) && $languages[$language] && variable_get('indymedia_cities_checked_'. $language, 0) + $refresh < time()) {
      $modified = variable_get('indymedia_cities_modified_'. $language, 0);
      indymedia_cities_update($uri, $modified, $language);
      variable_set('indymedia_cities_checked_'. $language, time());
    }
  }
}

/**
 * Gets URI and updates list if required.
 */
function indymedia_cities_update($uri, $modified, $language) {
  if (function_exists('simplexml_load_string')) {
    $type = 'xml';
  }
  else {
    $type = 'inc';
  }
  $uri .= $type;
 
  $headers = array();
  if ($modified) {
    $headers['If-Modified-Since'] = gmdate('D, d M Y H:i:s', $modified) .' GMT'; 
  }

  $result = drupal_http_request($uri, $headers);

  switch ($result->code) {
    case 304:
      watchdog('cron', 'Cities list at %uri not modified since last update.', array('%uri' => $uri));
      break;
    case 301:
      // deal with permenent redirections? indymedia_log note?
    case 200:
    case 302:
    case 307:
      if (isset($result->headers['Last-Modified'])) {
        variable_set('indymedia_cities_modified_'. $language, strtotime($result->headers['Last-Modified']));
      }
      if ($type == 'xml') {
        if ($xml = simplexml_load_string($result->data)) {
          variable_set('indymedia_cities_list_'. $language, $result->data);
          watchdog('cron', 'Cities list updated from %uri.', array('%uri' => $uri));
        }
        else {
          watchdog('cron', 'Error reading XML from %uri.', array('%uri' => $uri), WATCHDOG_WARNING);
        }
      }
      else {
        variable_set('indymedia_cities_list_'. $language, $result->data);
        watchdog('cron', 'Cities list updated from %uri.', array('%uri' => $uri));
      }
      break;
    default:
      watchdog('cron', 'Received error code %error from %uri.', array('%uri' => $uri, '%error' => $result->code .' '. $result->error), WATCHDOG_WARNING);
  }
}
