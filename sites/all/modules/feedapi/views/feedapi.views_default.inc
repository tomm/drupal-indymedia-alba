<?php
// $Id: feedapi.views_default.inc,v 1.1.2.3 2009/06/05 13:32:48 aronnovak Exp $
/**
 * @file
 * FeedAPI Views - Default Views
 */

/**
 * Implementation of hook_views_default_views().
 */
function feedapi_views_default_views() {
  // feeds admin overview
  $view = new view;
  $view->name = 'feed_admin';
  $view->description = 'Administer feeds. Enable this view to replace the static listing on admin/content/feed with a configurable views based listing.';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Title',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'next_refresh_time' => array(
      'label' => 'Scheduled refresh',
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'next_refresh_time',
      'table' => 'feedapi',
      'field' => 'next_refresh_time',
      'relationship' => 'none',
    ),
    'delete_node' => array(
      'label' => 'Commands',
      'text' => 'Delete',
      'exclude' => 0,
      'id' => 'delete_node',
      'table' => 'node',
      'field' => 'delete_node',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'purge_node' => array(
      'label' => 'Purge',
      'text' => 'Remove items',
      'exclude' => 0,
      'id' => 'purge_node',
      'table' => 'feedapi',
      'field' => 'purge_node',
      'relationship' => 'none',
    ),
    'refresh_node' => array(
      'label' => 'Refresh',
      'text' => 'Refresh',
      'exclude' => 0,
      'id' => 'refresh_node',
      'table' => 'feedapi',
      'field' => 'refresh_node',
      'relationship' => 'none',
    ),
    'edit_node' => array(
      'label' => 'Edit link',
      'text' => 'Edit',
      'exclude' => 0,
      'id' => 'edit_node',
      'table' => 'node',
      'field' => 'edit_node',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'next_refresh_time' => array(
      'order' => 'DESC',
      'granularity' => 'second',
      'id' => 'next_refresh_time',
      'table' => 'feedapi',
      'field' => 'next_refresh_time',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'feed_content_type' => array(
      'operator' => '=',
      'value' => 1,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'feed_content_type',
      'table' => 'feedapi',
      'field' => 'feed_content_type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'administer feedapi',
  ));
  $handler->override_option('title', 'Feeds');
  $handler->override_option('header_format', '1');
  $handler->override_option('header_empty', 0);
  $handler->override_option('items_per_page', 20);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'title' => 'title',
      'next_refresh_time' => 'next_refresh_time',
      'delete_node' => 'delete_node',
      'purge_node' => 'delete_node',
      'refresh_node' => 'delete_node',
      'edit_node' => 'delete_node',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'next_refresh_time' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'delete_node' => array(
        'separator' => ' ',
      ),
      'purge_node' => array(
        'separator' => '',
      ),
      'refresh_node' => array(
        'separator' => '',
      ),
      'edit_node' => array(
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'admin/content/feed');
  $handler->override_option('menu', array(
    'type' => 'normal',
    'title' => 'Feeds',
    'description' => 'View, edit and delete feeds that your site aggregates.',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'weight' => 0,
  ));
  $views[$view->name] = $view;
  return $views;
  return $views;
}


