<?php
// $Id: archive.pages.inc,v 1.18.2.8 2010/06/16 18:30:48 rmuilwijk Exp $

/**
 * Fetch nodes for the selected date, or current date if none selected.
 *
 * @param $year
 *   Number of year.
 * @param $month
 *   Number of month.
 * @param $day
 *   Number of day.
 * @return
 *   A string with the themed page.
 */
function archive_page($type = 'all', $year = 0, $month = 0, $day = 0) {
  // Argument validation, should be a valid node type and numeric.
  $node_types = node_get_types();
  if (($type != 'all' && !isset($node_types[$type])) || !is_numeric($year)
      || !is_numeric($month) || $month > 12 || !is_numeric($day) || $day > 31) {
    return MENU_NOT_FOUND;
  }

  // Make sure all values are secure.
  $day = (int) $day;
  $year = (int) $year;
  $month = (int) $month;
  if ($month < 0 || $month > 12) {
    // Ensure that we have a proper array index later.
    $month = 0;
  }
  if (!_archive_validate_type($type)) {
    $type = 'all';
  }

  drupal_set_title(theme('archive_page_title', $type, $year, $month, $day));

  // Check that there're nodes we can display.
  $nodes = db_result(db_query(db_rewrite_sql("SELECT COUNT(1) FROM {node} n WHERE n.status = 1 AND n.type IN ('". implode("', '", variable_get('archive_type_filters', array())) ."')")));
  if (!$nodes && $type == 'all' && $year == 0 && $month == 0 && $day == 0) {
    return t('No content found.');
  }

  drupal_add_css(drupal_get_path('module', 'archive') .'/archive.css');
  $date = _archive_date($type, $year, $month, $day);
  $output = theme('archive_navigation', $type, $date);
  $nodes = variable_get('default_nodes_main', 10);
  $query = _archive_query($type, $date);
  $result = pager_query(db_rewrite_sql(array_shift($query)), $nodes, 0, null, $query);

  $found_rows = false;
  $node_date = 0;
  while ($o = db_fetch_object($result)) {
    $node = node_load($o->nid);
    $node_created = $node->created + $date->tz;
    // Determine which separators are needed
    $separators = array('year'  => 0,
                        'month' => 0,
                        'day'   => 0);
    if (!$year) {
      $created_year = date('Y', $node_created);
      $last_year = date('Y', $node_date);
      if ($created_year != $last_year) {
        $separators['year'] = 1;
      }
    }
    // Print month separaters
    if (!$month) {
      $created_month = date('n', $node_created);
      $last_month = date('n', $node_date);
      if ($created_month != $last_month) {
        $separators['month'] = 1;
      }
    }
    // Print day separaters
    if (!$day) {
      $created_day = date('j', $node_created);
      $last_day = date('j', $node_date);
      if ($created_day != $last_day) {
        $separators['day'] = 1;
      }
    }
    $output .= theme('archive_separator', $node_created, $separators);
    $output .= node_view($node, true);
    $found_rows = true;
    $node_date = $node->created + $date->tz;
  }
  if ($found_rows) {
    $output .= theme('pager', null, $nodes);
  }
  // Handle URLs that are incorrectly typed and try to parse info out of them
  else {
    if ($date->days[$date->day]) {
      drupal_goto(_archive_url($type, $date->year, $date->month, $date->day));
    }
    else if ($date->months[$date->month]) {
      drupal_goto(_archive_url($type, $date->year, $date->month));
    }
    else if ($date->years[$date->year]) {
      drupal_goto(_archive_url($type, $date->year));
    }
    else {
      drupal_goto(_archive_url($type));
    }
  }

  return $output;
}

/**
 * Builds an archive SQL query with its parameters for the specified date.
 *
 * @param $date
 *   A date object obtained from _archive_date().
 * @return
 *   An array of (query, param_start, param_end).
 */
function _archive_query($type, $date) {
  // Confine the display interval to only one day
  if ($date->day) {
    $start = gmmktime(0, 0, 0, $date->month, $date->day, $date->year);
    $end   = gmmktime(0, 0, 0, $date->month, $date->day + 1, $date->year);
  }
  // Confine the display interval to one month
  else if ($date->month) {
    $start = gmmktime(0, 0, 0, $date->month, 1, $date->year);
    $end   = gmmktime(0, 0, 0, $date->month + 1, 1, $date->year);
  }
  // Confine the display interval to one year
  else if ($date->year) {
    $start = gmmktime(0, 0, 0, 1, 1, $date->year);
    $end   = gmmktime(0, 0, 0, 1, 1, $date->year + 1);
  }
  else {
    $start = 0;
    $end = 0;
  }

  // Grab limits on node types if exist
  $final_types = _archive_types_sql_string($type);

  // Allow viewing all nodes, not just nodes by year
  if ($start && $end) {
    return array('SELECT DISTINCT(n.nid), n.type FROM {node} n WHERE n.status = 1 '. $final_types .'AND n.created >= %d AND n.created < %d ORDER BY n.created DESC', $start - $date->tz, $end - $date->tz);
  }
  else {
    return array('SELECT DISTINCT(n.nid), n.type FROM {node} n WHERE n.status = 1 '. $final_types .'ORDER BY n.created DESC');
  }
}

/**
 * Returns the different node types that have nodes.
 *
 * @param $date
 *   A date object obtained from _archive_date().
 * @return
 *   An array of node-types to number of posts of that type.
 */
function _archive_node_types($date) {

  $types = variable_get('archive_type_filters', array());

  // Confine the display interval to only one day
  if ($date->day) {
    $start = gmmktime(0, 0, 0, $date->month, $date->day, $date->year);
    $end   = gmmktime(0, 0, 0, $date->month, $date->day + 1, $date->year);
  }
  // Confine the display interval to one month
  else if ($date->month) {
    $start = gmmktime(0, 0, 0, $date->month, 1, $date->year);
    $end   = gmmktime(0, 0, 0, $date->month + 1, 1, $date->year);
  }
  // Confine the display interval to one year
  else if ($date->year) {
    $start = gmmktime(0, 0, 0, 1, 1, $date->year);
    $end   = gmmktime(0, 0, 0, 1, 1, $date->year + 1);
  }
  else {
    $start = 0;
    $end = 0;
  }
  if ($start && $end) {
    $result = db_query(db_rewrite_sql("SELECT t.type, t.name, COUNT(n.nid) AS node_count FROM {node} n INNER JOIN {node_type} t ON t.type = n.type WHERE n.status = 1 AND t.type IN ('". join($types, "', '") ."') AND n.created BETWEEN %d AND %d GROUP BY t.type, t.name, n.created ORDER BY n.created"), $start - $date->tz, $end - $date->tz);
  }
  else {
    $result = db_query(db_rewrite_sql("SELECT t.type, t.name, COUNT(n.nid) AS node_count FROM {node} n INNER JOIN {node_type} t ON t.type = n.type WHERE n.status = 1 AND t.type IN ('". join($types, "', '") ."') GROUP BY t.type, t.name, n.created ORDER BY n.created"));
  }

  $n_types = array();
  while ($row = db_fetch_array($result)) {
    $n_types[$row['type']] = array('count' => $row['node_count'],
                                   'name'  => $row['name']);
  }

  ksort($n_types);
  return $n_types;
}

/**
 * Theme function for the title of the archive page. Changes according to content
 * being viewed
 *
 * @param string $type
 * @param integer $year
 * @param integer $month
 * @param integer $day
 * @return string
 */
function theme_archive_page_title($type, $year, $month, $day) {

  $title = t('Archive');
  $month_names = array('', 'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec');
  if ($day) {
    $title .= ' - '. t($month_names[$month]) .' '. $day .', '. $year;
  }
  else if ($month) {
    $title .= ' - '. t($month_names[$month]) .' '. $year;
  }
  else if ($year) {
    $title .= ' - '. $year;
  }

  if ($type != 'all') {
    $type_name = db_result(db_query("SELECT name FROM {node_type} WHERE type = '%s'", $type));

    $title .= ' - '. ($type_name ? t($type_name) : $type);
  }

  return $title;
}

/**
 * Theme the archive navigation with years, months and dates by default.
 *
 * @ingroup themeable
 */
function theme_archive_navigation($type, $date) {
  $output  = "<div id=\"archive-container\"><dl><dt>". t('Date') ."</dt><dd>\n";
  $output .= theme('archive_navigation_years', $type, $date);
  if (_archive_validate_date($date->year)) {
    $output .= theme('archive_navigation_months', $type, $date);
  }
  if (_archive_validate_date($date->year, $date->month)) {
    $output .= theme('archive_navigation_days', $type, $date);
  }
  $output .= "</dd>";

  // Only display node type filter if more than one node type represented
  if (sizeof(_archive_node_types($date)) > 1) {
    $output .= "<dt>". t('Type') ."</dt><dd>\n";
    $output .= theme('archive_navigation_node_types', $type, $date);
    $output .= "</dd>";
  }
  $output .= "</dl></div>\n";
  return $output;
}

/**
 * Theme the list of years for the archive navigation.
 *
 * @ingroup themeable
 */
function theme_archive_navigation_years($type, $date) {
  $output = "<ul id=\"archive-years\">\n";

  $all_count = 0;
  foreach ($date->years as $year_count) {
    $all_count += $year_count;
  }

  $output .= '<li'. ($date->year?'':' class="selected"') .'>'. l(t('All'), _archive_url($type), array('attributes' => array('title' => format_plural($all_count, '1 post', '@count posts')))) ."</li>\n";
  foreach ($date->years as $year => $year_count) {
    $class = '';
    if ($year == $date->year) {
      $class = ' class="selected"';
    }
    $output .= '<li'. $class .'>'. l($year, _archive_url($type, $year), array('attributes' => array('title' => format_plural($year_count, '1 post', '@count posts')))) ."</li>\n";
  }
  $output .= "</ul>\n";

  return $output;
}

/**
 * Theme the list of months for the archive navigation.
 *
 * @ingroup themeable
 */
function theme_archive_navigation_months($type, $date) {
  $output = "<ul id=\"archive-months\">\n";

  $all_count = 0;
  foreach ($date->months as $month) {
    $all_count += $month;
  }

  $output .= '<li'. ($date->month?'':' class="selected"') .'>'. l(t('All'), _archive_url($type, $date->year), array('attributes' => array('title' => format_plural($all_count, '1 post', '@count posts')))) ."</li>\n";
  $curr_month = date('n', time());
  $curr_year = date('Y', time());
  foreach (range(1, 12) as $month) {
    $posts = !empty($date->months[$month]) ? $date->months[$month] : 0;
    $class = '';
    if ($month == $date->month) {
      $class = ' class="selected"';
    }
    else if ($curr_year == $date->year && $month > $curr_month) {
      $class = ' class="future"';
    }
    $month_names = array('', 'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec');
    $output .= "<li$class>". ($posts > 0 ? l(t($month_names[$month]), _archive_url($type, $date->year, $month), array('attributes' => array('title' => format_plural($posts, "1 post", "@count posts")))) : t($month_names[$month])) ."</li>\n";
  }
  $output .= "</ul>\n";
  return $output;
}

/**
 * Theme the list of days for the archive navigation.
 *
 * @ingroup themeable
 */
function theme_archive_navigation_days($type, $date) {
  $output = "<ul id=\"archive-days\">\n";

  $all_count = 0;
  foreach ($date->days as $day) {
    $all_count += $day;
  }

  $output .= '<li'. ($date->day?'':' class="selected"') .'>'. l(t('All'), _archive_url($type, $date->year, $date->month), array('attributes' => array('title' => format_plural($all_count, '1 post', '@count posts')))) ."</li>\n";
  $curr_month = date('n', time());
  $curr_year = date('Y', time());
  $curr_day = date('j', time());

  $day_stop = gmdate('t', gmmktime(0, 0, 0, $date->month, 1, $date->year));
  for ($day = 1; $day <= $day_stop; $day++) {
    $posts = array_key_exists($day, $date->days) ? $date->days[$day] : 0;
    $class = '';
    if ($day == $date->day) {
      $class = ' class="selected"';
    }
    else if ($curr_year == $date->year && $curr_month == $date->month && $day > $curr_day) {
      $class = ' class="future"';
    }
    $output .= "<li$class>". ($posts ? l($day, _archive_url($type, $date->year, $date->month, $day), array('attributes' => array("title" => format_plural($posts, "1 post", "@count posts")))) : $day) ."</li>\n";
  }
  $output .= "</ul>\n";
  return $output;
}

/**
 * Theme the list of node types for the archives.
 *
 * @ingroup themeable
 */
function theme_archive_navigation_node_types($type, $date) {

  $output = "<ul id=\"archive-node_types\">\n";
  $types_count = _archive_node_types($date);

  $all_count = 0;
  foreach ($types_count as $t) {
    $all_count += $t['count'];
  }

  $output .= '<li'. ($type && $type != 'all'?'':' class="selected"') .'>'. l(t('All'), _archive_url('all', $date->year, $date->month, $date->day), array('attributes' => array('title' => format_plural($all_count, '1 post', '@count posts')))) ."</li>\n";
  foreach ($types_count as $ft_key => $ft_value) {
    if (!$ft_value['count']) {
      continue;
    }
    $class = ($ft_key == $type ? ' class="selected"' : '');
    $name = $ft_value['name'];
    if ($types_count[$ft_key]['count'] > 0) {
      $output .= "<li$class>". l($name, _archive_url($ft_key, $date->year, $date->month, $date->day), array('attributes' => array("title" => format_plural($types_count[$ft_key]['count'], "1 post", "@count posts")))) ."</li>\n";
    }
    else {
      $output .= "<li$class>$name</li>\n";
    }
  }
  $output .= "</ul>\n";
  return $output;
}

/**
 * Theme the date separators between nodes of different year/month/day.
 *
 * @param $date_created
 *   A UNIX timestamp.
 * @param $separators
 *   An array with 'year', 'month', and 'day' keys. A value of 1 for any
 *   of those keys means a transition for that unit of time.
 *   Ex. array('year' => 0, 'month' => 1, 'day' => 1)
 *       ^  Means the month has transitioned
 *
 * @ingroup themeable
 */
function theme_archive_separator($date_created, $separators) {
  $date_sep = '';
  if ($separators['year'] && $separators['month'] && $separators['day']) {
    $date_sep = format_date($date_created, 'custom', 'F jS, Y');
  }
  else if ($separators['month'] && $separators['day']) {
    $date_sep = format_date($date_created, 'custom', 'F jS');
  }
  else if ($separators['day']) {
    $date_sep = format_date($date_created, 'custom', 'F jS');
  }

  return '<h3>'. $date_sep .'</h3>';
}
