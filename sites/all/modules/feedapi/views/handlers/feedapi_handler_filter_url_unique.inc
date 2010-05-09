<?php
// $Id: feedapi_handler_filter_url_unique.inc,v 1.1.2.2 2009/07/02 16:38:17 aronnovak Exp $

/**
 * Filter handler to filter out multiple URLs
 *
 * @ingroup views_filter_handlers
 */
class feedapi_handler_filter_url_unique extends views_handler_filter {

  function query() {
    $this->query->add_where(0, 'node.nid IN (SELECT nid FROM feedapi GROUP BY url)');
  }

}
