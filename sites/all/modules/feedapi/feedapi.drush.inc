<?php
// $Id: feedapi.drush.inc,v 1.1.2.3 2009/07/02 16:38:15 aronnovak Exp $

/**
 * @file
 * Drush commands for FeedAPI.
 */

/**
 * Implementation of hook_drush_help().
 */
function feedapi_drush_help($section) {
  switch ($section) {
    case 'drush:feedapi create':
      return dt('Creates a feed.');
    case 'drush:feedapi refresh':
      return dt('Refreshes a feed.');
    case 'drush:feedapi delete':
      return dt('Deletes a feed.');
    case 'drush:feedapi parse':
      return dt('Parses a feed and shows you the output.');
    case 'drush:feedapi config':
      return dt('Shows the config of a parser of a processor.');

  }
}

/**
 * Implementation of hook_drush_command().
 */
function feedapi_drush_command() {
  $options['--type'] = 'The node type of the feed. Default value: feed, the built-in content-type of FeedAPI.';
  $items['feedapi create'] = array(
    'callback' => 'feedapi_drush_create',
    'description' => dt('Creates a feed'),
    'options' => $options,
    'arguments' => array(
       'url' => 'A feed URL. Mandatory.',
    ),
  );
  $items['feedapi refresh'] = array(
    'callback' => 'feedapi_drush_refresh',
    'description' => dt('Refreshes a feed'),
    'arguments' => array(
       'feed' => 'A feed URL or node nid. Mandatory.',
    ),
  );
  $items['feedapi parse'] = array(
    'callback' => 'feedapi_drush_parse',
    'description' => dt('Parses a feed and outputs the result to the stdout'),
    'options' => $options,
    'arguments' => array(
       'url' => 'A feed URL. Mandatory.',
       'parser' => 'The name of the parser. Mandatory.',
    ),
  );
  $items['feedapi config'] = array(
    'callback' => 'feedapi_drush_config',
    'description' => dt('Shows the config of a given parser or processor'),
    'options' => $options,
    'arguments' => array(
       'name' => 'The name of the parser or the processor. Mandatory.',
    ),
  );
  return $items;
}

/**
 * Creates a feed
 *
 * @param $url
 */
function feedapi_drush_create($url) {
  $node_template = new stdClass();
  $node_template->type = drush_get_option('type') ? drush_get_option('type') : 'feed';
  feedapi_create_node($node_template, $url);
}

/**
 * Refreshes a feed.
 * When the feed URL is not enough to exactly define a feed, use the nid
 *
 * @param $feed
 *   URL or nid
 */
function feedapi_drush_refresh($feed) {
  $nid = _feedapi_drush_get_nid($feed);
  if (is_numeric($nid)) {
    $node = node_load(array('nid' => $nid));
    feedapi_invoke('refresh', $node->feed, FALSE);
  }
}

/**
 * Invokes the parser and displays the output
 */
function feedapi_drush_parse($url, $parser) {
  $type = drush_get_option('type') ? drush_get_option('type') : 'feed';
  $settings = feedapi_get_settings($type);
  $feed = new stdClass();
  $feed->url = $url;
  print_r(_feedapi_call_parsers($feed, array($parser), $settings));
}

function feedapi_drush_config($name) {
  $type = drush_get_option('type') ? drush_get_option('type') : 'feed';
  $settings = variable_get('feedapi_settings_'. $type, array());
  if (isset($settings['parsers'][$name])) {
    print_r($settings['parsers'][$name]);
  }
  elseif (isset($settings['processors'][$name])) {
    print_r($settings['processors'][$name]);
  }
}

function _feedapi_drush_get_nid($feed) {
  if (valid_url($feed, TRUE)) {
    return db_result(db_query("SELECT nid FROM {feedapi} WHERE url = '%s'", $feed));
  }
  return $feed;
}
