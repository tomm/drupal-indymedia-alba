<?php
// $Id: similarterms.tpl.php,v 1.6 2008/07/12 23:18:55 rmiddle Exp $

/**
 * @file
 * simterms.tpl.ph
 * Theme implementation to display a list of related content.
 *
 * Available variables:
 * - $display_options:
 *    'title_only' => 'Display titles only',
 *    'teaser' => 'Display titles and teaser',
 * - $items: the list.
 */
if ($items) {
$items_ls = array();
  if ($display_options == 'title_only') {
    foreach ($items as $node) {
      $items_ls[] = l($node->title, 'node/'. $node->nid);
    }
    print theme('item_list', $items_ls);
  }
  if ($display_options == 'teaser') {
    foreach ($items as $node) {
      print '<li>'. l($node->title, 'node/'. $node->nid);
      print ' - '. $node->teaser;
      print "</li>\n";
    }
  }
}
