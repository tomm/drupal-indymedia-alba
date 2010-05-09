<?php
// $Id: feedapi.admin.inc,v 1.1.2.2 2009/06/05 13:32:47 aronnovak Exp $

/**
 * @file
 * Administration-related functions for FeedAPI
 */

/**
 * Provide a UI for overviewing the existing feeds
 */
function feedapi_admin_overview() {
  $header = array(
    t('Title'),
    t('New items added per update'),
    t('Update rate'),
    t('Number of items'),
    t('Processing time'),
    t('Commands'),
  );
  $rows = array();
  $result = pager_query("SELECT nid from {feedapi} ORDER BY next_refresh_time DESC", 50, 0, "SELECT count(*) FROM {feedapi}");
  while ($nid = db_fetch_array($result)) {
    $nid = $nid['nid'];
    $node = node_load($nid);
    if (is_object($node)) {
      $commands = array(l(t('Delete'), 'node/'. $node->nid .'/delete', array('query' => 'destination=admin/content/feed')),
        l(t('Remove items'), 'node/'. $node->nid .'/purge', array('query' => 'destination=admin/content/feed')),
        l(t('Refresh'), 'node/'. $node->nid .'/refresh'),
        l(t('Edit'), 'node/'. $node->nid .'/edit'),
      );
      // Fetch statistics for this feed
      foreach (array('download_num', 'new',  'process_time', 'update_times') as $type) {
        $node->feed->statistics[$type] = _feedapi_get_stat($node->nid, $type, TRUE);
      }
      if (count($node->feed->statistics['download_num']) != 0 && count($node->feed->statistics['new']) != 0 && count($node->feed->statistics['process_time']) != 0) {
        $update_rate = _feedapi_update_rate($node->feed->statistics['update_times']);
        $rows[] = array(
          l($node->title, "node/$node->nid"),
          round(array_sum($node->feed->statistics['new']) / count($node->feed->statistics['new']), 2),
          is_numeric($update_rate) ? format_interval($update_rate) : $update_rate,
          round((array_sum($node->feed->statistics['download_num']) / count($node->feed->statistics['download_num'])), 2),
          round((array_sum($node->feed->statistics['process_time']) / count($node->feed->statistics['process_time'])), 2) .' '. t('ms'),
          theme('item_list', $commands),
        );
      }
      else {
        $rows[] = array(
          l($node->title, "node/$node->nid"),
          '', '', t('No enough data for statistics'), '',
          theme('item_list', $commands),
        );
      }
    }
  }
  $output = format_plural(round(FEEDAPI_CRON_STAT_LIFETIME / (24*3600)), "Average over the last day.",
    "Averages over the last @count days."
  );
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', 0, 50);
  return $output;
}

/**
 * Settings: allowed HTML tags, number of feeds refreshed in one round
 */
function feedapi_admin_settings() {
  $form['feedapi_allowed_html_tags'] = array(
    '#type' => 'textfield', '#title' => t('Allowed HTML tags'), '#size' => 80, '#maxlength' => 255,
    '#default_value' => variable_get('feedapi_allowed_html_tags', '<a> <b> <br> <dd> <dl> <dt> <em> <i> <li> <ol> <p> <strong> <u> <ul>'),
    '#description' => t('The list of tags which are allowed in feeds, i.e., which will not be removed by Drupal.')
  );
  $form['feedapi_allow_html_all'] = array(
    '#type' => 'checkbox', '#title' => t('Allow all HTML tags'),
    '#default_value' => variable_get('feedapi_allow_html_all', FALSE),
    '#description' => t('In this case the module does\'t filter any HTML elements from the incoming fields. This checkbox overrides the above list of allowed tags.')
  );
  if (variable_get('feedapi_allow_html_all', FALSE)) {
    $form['feedapi_allowed_html_tags']['#disabled'] = TRUE;
  }
  // Drupal will try to overwrite this value at cron time
  $max_exec = !ini_get('safe_mode') ? 240 : ini_get('max_execution_time');
  $form['feedapi_cron_percentage'] = array(
    '#type' => 'select',
    '#title' => t('Cron time for FeedAPI [%]'),
    '#options' => drupal_map_assoc(array(15, 25, 50, 75)),
    '#default_value' => variable_get('feedapi_cron_percentage', 15),
    '#description' => t('Percentage of maximal PHP execution time (currently !exec seconds). At current settings, the FeedAPI cron process can run for up to !now seconds.',
      array("!exec" => $max_exec, "!now" => (variable_get('feedapi_cron_percentage', 15) / 100) * $max_exec)),
  );
  return system_settings_form($form);
}
