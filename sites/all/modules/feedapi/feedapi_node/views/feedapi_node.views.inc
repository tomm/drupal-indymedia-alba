<?php
// $Id: feedapi_node.views.inc,v 1.1.2.4 2009/01/23 13:47:10 alexb Exp $

/**
 * @file
 * Add views functionality to FeedAPI Node
 *
 * Offer feed-related arguments
 */

/**
 * Implementation of hook_views_data().
 */
function feedapi_node_views_data() {
  $data['feedapi_node_item']['table']['group'] = t('FeedAPI Item');
  $data['feedapi_node_item']['table']['join']['node'] = array(
    'left_table' => 'feedapi_node_item_feed',
    'left_field' => 'feed_item_nid',
    'field' => 'nid',
  );
  // Item-related fields
  $data['feedapi_node_item']['url'] = array(
    'title' => t('URL'),
    'help' => t('The original URL of this feed item'),
    'field' => array(
      'handler' => 'feedapi_handler_field_url',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['feedapi_node_item']['timestamp'] = array(
    'title' => t('Date'),
    'help' => t('The date of item publish'),
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
  $data['feedapi_node_item']['arrived'] = array(
    'title' => t('Arrived'),
    'help' => t('The date the item was imported'),
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


  $data['feedapi_node_item_feed']['table']['group'] = t('FeedAPI Item');
  $data['feedapi_node_item_feed']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'feed_item_nid',
    ),
  );
  // Argument for filtering a specific feed
  $data['feedapi_node_item_feed']['feed_nid'] = array(
    'title' => t('Parent feed'),
    'help' => t('The feed that generated this node'),
    'relationship' => array(
      'label' => t('Parent feed'),
      'base' => 'node',
      'base field' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  return $data;
}
