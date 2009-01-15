The Event Views module makes the event fields available to the Views module, 
and creates default event views that filter events using a drop-down date selector, 
the jscalendar selector, or by arguments in the url. Set up a default event view by 
going to admin/views and select add. Save the default views as-is or make any 
changes you like.

The event_views module will automatically be disabled if either the event module or 
the views module is disabled, since it requires both those modules to work.

You can change the format of the output. The default views display a list of events, 
but you can change it to a table or a teaser list in the Page settings. You can also 
add or remove fields from the view.

You can filter events in several ways. Use filters to pre-select a date range, expose 
those filters to allow the viewer to select a date range, or eliminate the filters and 
give the view year, month, and day arguments to filter the events by the 
url (i.e. YYYY/MM/DD).  

Arguments can be confusing but are very powerful. You can combine arguments in various 
ways, like year/month, year/month/day, or year/week. You can also add content type 
and taxonomy arguments before or after the year/month/day arguments for even more 
granularity. For each argument you add, you need to select a display. For lower-level 
arguments, like the year argument in a year/month combination, select the option to 
provide a summary view. That will display a list of all the months that have events 
in that year. You can then click on the month to see the events for that month.

To view a week calendar, create a view with Year and Week arguments, the go to YYYY/WW
where WW is the week number (1-53) that you want to see.

To theme the exposed filters, put the following in your template.php file. Change
'phptemplate' to your theme name and make whatever other style changes you want.

/**
 *  Example to override standard views handling of the exposed filters to mimic event display
 *  This will change the display only for the calendar view types to show the exposed filters
 *  One on top of the other above the calendar instead of side-by-side in a table
 */
function phptemplate_views_filters($form) {
  
  $view = $form['view']['#value'];

  // changed display only for event_views page types
  if (array_key_exists($view->page_type, event_views_view_types())) {
    
    foreach ($view->exposed_filter as $count => $expose) {
      $form["filter$count"]['#title'] = $expose['label'];
    }
    return form_render($form['q']) . form_render($form);
  
  // standard views explosed filters display for all other types
  } else {
    
    foreach ($view->exposed_filter as $count => $expose) {
      $row[] = form_render($form["op$count"]) . form_render($form["filter$count"]);
      $label[] = $expose['label'];
    }
    $row[] = form_render($form['submit']);
    $label[] = ''; // so the column count is the same.
    return form_render($form['q']) . theme('table', $label, array($row)) . form_render($form);
  }
}




