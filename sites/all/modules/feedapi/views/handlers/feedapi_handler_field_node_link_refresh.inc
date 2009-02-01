<?php
// $Id: feedapi_handler_field_node_link_refresh.inc,v 1.1.2.1 2009/01/23 13:47:11 alexb Exp $
/**
 * Field handler to present a refresh node link.
 */
class feedapi_handler_field_node_link_refresh extends views_handler_field_node_link {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
    $this->additional_fields['uid'] = array('table' => 'node', 'field' => 'uid');
    $this->additional_fields['type'] = array('table' => 'node', 'field' => 'type');
  }

  function render($values) {
    // ensure user has access to edit this node.
    $node = new stdClass();
    $node->nid = $values->{$this->aliases['nid']};
    $node->uid = $values->{$this->aliases['uid']};
    $node->type = $values->{$this->aliases['type']};
    $node->status = 1; // unpublished nodes ignore access control
    if (!_feedapi_op_access($node)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('refresh');
    return l($text, "node/$node->nid/refresh", array('query' => drupal_get_destination()));
  }
}


