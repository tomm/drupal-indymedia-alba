<?php
// $Id: feedapi.opml.inc,v 1.1.2.2 2009/06/04 16:46:57 aronnovak Exp $

/**
 * @file
 * OPML import and export for FeedAPI
 */

/**
 * OPML Feed import form, also allows setting defaults to be applied to each feed
 */
function feedapi_import_opml($form_state) {
  $form['opml'] = array(
    '#type' => 'file',
    '#title' => t('OPML File'),
    '#size' => 50,
    '#description' => t('Upload an OPML file containing a list of newsfeeds to be imported.'),
  );
  $form['feed_type'] = array(
    '#type' => 'select',
    '#title' => t('Feed Type'),
    '#description' => t("The type of feed you would like to associate this import with."),
    '#options' => feedapi_get_types(),
    '#required' => TRUE,
  );
  $form['override_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use TITLE attribute of OPML entries as feed title'),
    '#description' => t('If checked feed title will be overriden with the information from OPML file'),
  );
  $form['override_body'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use TEXT attribute of OPML entries as feed description'),
    '#description' => t('If checked feed description will be overriden with the information from OPML file'),
  );
  $form['#attributes']['enctype'] = 'multipart/form-data';
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  if (module_exists('og')) {
    og_form_add_og_audience($form, $form_state);
  }
  return $form;
}

/**
 * Handle the submission of the OPML import form
 */
function feedapi_import_opml_submit($form, &$form_state) {
  $file = file_save_upload('opml');
  if ($file = file($file->filepath)) {
    $file = implode('', $file);

    if ($count = _feedapi_import_opml($file, $form_state['values'])) {
      drupal_set_message(t('Successfuly imported %count feeds from OPML', array('%count' => $count)));
    }
    else {
      drupal_set_message(t('Feed list could not be imported. Please check that this is a valid OPML file.'), 'error');
    }
  }
  else {
    drupal_set_message(t('Data could not be retrieved, invalid or empty file.'), 'error');
  }
  $form_state['redirect'] = 'admin/content/feed';
}


/**
 * Generates an OPML representation of all feeds.
 */
function feedapi_export_opml() {
  $result = db_query(db_rewrite_sql('SELECT n.title, f.url FROM {feedapi} f INNER JOIN {node} n ON f.nid = n.nid ORDER BY n.title ASC'));
  $feeds = array();
  while ($feed = db_fetch_object($result)) {
    $feeds[] = $feed;
  }
  return theme('feedapi_export_opml', $feeds);
}

/**
 * Theme the OPML feed output.
 *
 * @param $feeds
 *   An array of the feeds to theme.
 * @ingroup themeable
 */
function theme_feedapi_export_opml($feeds) {
  drupal_set_header('Content-Type: text/xml; charset=utf-8');

  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<opml version=\"1.1\">\n";
  $output .= "<head>\n";

  $output .= '<title>'. check_plain(variable_get('site_name', 'Drupal')) ."</title>\n";
  $output .= '<dateModified>'. gmdate('r') ."</dateModified>\n";
  $output .= "</head>\n";
  $output .= "<body>\n";
  foreach ($feeds as $feed) {
    $output .= '<outline text="'. check_plain($feed->title) .'" xmlUrl="'. check_url($feed->url) ."\" />\n";
  }
  $output .= "</body>\n";
  $output .= "</opml>\n";
  print $output;
}

/**
 * Imports from OPML XML file.
 */
function _feedapi_import_opml($opml, $args = array()) {
  $feeds = array();
  $count = 0;
  $parser = drupal_xml_parser_create($opml);

  //Some OPML Files don't have the xml tag, which causes parsing to fail. Hence, using the appended version as a fallback parse
  if (xml_parse_into_struct($parser, $opml, $vals, $index) || xml_parse_into_struct($parser, '<?xml version="1.0"?>'. $opml, $vals, $index)) {
    foreach ($vals as $entry) {
      if ($entry['tag'] == 'OUTLINE') {
        $feeds[] = $entry['attributes'];
      }
    }

    foreach ($feeds as $feed) {
      if (strlen($feed['XMLURL']) > 1) {
        // check if feed url is already in the list
        $dupe = db_result(db_query("SELECT nid FROM {feedapi} WHERE url = '%s'", $feed['XMLURL']));

        // If the feed is not already in the list, add it
        if ($dupe === FALSE) {
          // Generate a feed structure
          $node = new stdClass();
          $node->type = $args['feed_type'];
          if ($args['override_title'] && isset($feed['TITLE'])) {
            $node->title = $feed['TITLE'];
          }
          if ($args['override_body'] && isset($feed['TEXT'])) {
            $node->body = $feed['TEXT'];
          }
          $node->og_groups = $args['og_groups'];
          $node->og_public = (int) $args['og_public'];
          $node = feedapi_create_node($node, $feed['XMLURL']);
          $count++;
        }
      }
    }
  }
  return $count;
}
