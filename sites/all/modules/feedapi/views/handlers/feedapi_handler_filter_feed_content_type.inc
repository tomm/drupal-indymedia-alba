<?php
// $Id: feedapi_handler_filter_feed_content_type.inc,v 1.1.2.1 2009/01/23 13:47:11 alexb Exp $

/**
 * Filter handler for filtering nodes by whether or not they're feed-enabled.
 */
class feedapi_handler_filter_feed_content_type extends views_handler_filter_boolean_operator {
  function query() {
    $types = node_get_types();
    $feedapi_types = array();
    foreach ($types as $type => $name) {
      if (feedapi_enabled_type($type)) {
        $feedapi_types[] = '"'. $type .'"';
      }
    }
    $this->query->add_where(0, 'node.type '. (empty($this->value) ? 'NOT IN' : 'IN') .'('. implode(', ', $feedapi_types) .')');
  }
}