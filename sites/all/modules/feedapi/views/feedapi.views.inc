<?php
// $Id: feedapi.views.inc,v 1.1.2.7 2009/06/05 13:32:48 aronnovak Exp $

/**
 * @file
 * Add views functionality to FeedAPI Node
 *
 * Offer feed-related arguments
 */

/**
 * Implementation of hook_views_data().
 */
function feedapi_views_data() {
  $data['feedapi']['table']['group'] = t('FeedAPI Feed');
  $data['feedapi']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );


  // Feed-related fields
  $data['feedapi']['url'] = array(
    'title' => t('URL'),
    'help' => t('The source RSS/Atom/RDF feed URL.'),
    'field' => array(
      'handler' => 'feedapi_handler_field_url',
    ),
  );
  $data['feedapi']['feed_type'] = array(
    'title' => t('Feed Type'),
    'help' => t('The type of the feed.'),
    'field' => array(
      'click sortable' => TRUE,
     ),
  );
  $data['feedapi']['next_refresh_time'] = array(
    'title' => t('Next refresh'),
    'help' => t('The date when the feed is scheduled to be refreshed earliest.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  $data['feedapi']['link'] = array(
    'real field' => 'link',
    'title' => t('Link'),
    'help' => t('The link to the site.'),
    'field' => array(
      'handler' => 'feedapi_handler_field_url',
    ),
  );
  $data['feedapi']['url_unique'] = array(
    'real field' => 'url',
    'title' => t('Unique URL'),
    'help' => t('The feed that generated this node'),
    'filter' => array(
      'handler' => 'feedapi_handler_filter_url_unique',
    ),
  );
  $data['feedapi']['feed_content_type'] = array(
    'title' => t('Feed content type'),
    'help' => t('Whether or not the content type is used as FeedAPI feed.'),
    'filter' => array(
      'handler' => 'feedapi_handler_filter_feed_content_type',
      'label' => t('Is feed content type'),
    ),
  );

  // links to operate on the node
  $data['feedapi']['purge_node'] = array(
    'field' => array(
      'title' => t('Purge link'),
      'help' => t('Provide a simple link to purge the feed.'),
      'handler' => 'feedapi_handler_field_node_link_purge',
    ),
  );
  $data['feedapi']['refresh_node'] = array(
    'field' => array(
      'title' => t('Refresh link'),
      'help' => t('Provide a simple link to refresh the feed.'),
      'handler' => 'feedapi_handler_field_node_link_refresh',
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function feedapi_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'feedapi') .'/views/handlers',
    ),
    'handlers' => array(
      // field handlers
      'feedapi_handler_field_url' => array(
        'parent' => 'views_handler_field',
      ),
      'feedapi_handler_field_node_link_refresh' => array(
        'parent' => 'views_handler_field_node_link',
      ),
      'feedapi_handler_field_node_link_purge' => array(
        'parent' => 'views_handler_field_node_link',
      ),

      // filter handlers
      'feedapi_handler_filter_url_unique' => array(
        'parent' => 'views_handler_filter',
      ),
      'feedapi_handler_filter_feed_content_type' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
    ),
  );
}
