<?PHP
/**
*  Implementation of hook_views_data().
*  Expose event fields and filters to the views module.
*/
function event_views_views_data() 
{
   $data['event']['table']['group'] = t('Event');
   $data['event']['table']['join'] = array(
     'node' => array(
       'left_field' => 'nid',
       'field' => 'nid',
     ),
   );

   // Node ID field.
   $data['event']['nid'] = array(
     'title' => t('Event Data'),
     'help' => t('Event data linked to the node'),
     'relationship' => array(
       'base' => 'node',
       'field' => 'nid',
       'handler' => 'views_handler_relationship',
       'label' => t('Event Node'),
     ),
   );

   $data['event']['timezone'] = array(
     'title' => t('timezone'),
     'help' => t('Just a numeric field.'),
     'field' => array(
       'handler' => 'views_handler_field_numeric',
       'click sortable' => TRUE,
      ),
     'filter' => array(
       'handler' => 'views_handler_filter_numeric',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
   );

   $data['event']['unix_event_start'] = array(
     'title' => t('Event Start'),
     'help' => t('Event Start Date and Time'),
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
      'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
   );

   $data['event']['unix_event_end'] = array(
     'title' => t('Event End'),
     'help' => t('Event End Date and Time'),
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
      'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
   );

   $data['event']['start_in_dst'] = array(
     'title' => t('Start in DST'),
     'help' => t('Start in Daily Savings Time?'),
     'field' => array(
       'handler' => 'views_handler_field_boolean',
       'click sortable' => TRUE,
      ),
     'filter' => array(
       'handler' => 'views_handler_filter_boolean_operator',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
      'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        ),
   );

   $data['event']['end_in_dst'] = array(
     'title' => t('End in DST'),
     'help' => t('End in Daily Savings Time?'),
     'field' => array(
       'handler' => 'views_handler_field_boolean',
       'click sortable' => TRUE,
      ),
     'filter' => array(
       'handler' => 'views_handler_filter_boolean_operator',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
      'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        ),
   );

   $data['event']['has_time'] = array(
     'title' => t('Has Time'),
     'help' => t('Event starts at a certain time?'),
     'field' => array(
       'handler' => 'views_handler_field_boolean',
       'click sortable' => TRUE,
      ),
     'filter' => array(
       'handler' => 'views_handler_filter_boolean_operator',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
      'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        ),
   );


   $data['event']['has_end_date'] = array(
     'title' => t('Has End Date'),
     'help' => t('Event has an end date?'),
     'field' => array(
       'handler' => 'views_handler_field_boolean',
       'click sortable' => TRUE,
      ),
     'filter' => array(
       'handler' => 'views_handler_filter_boolean_operator',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
      'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        ),
   );

   $data['event_timezones']['table']['group']  = t('Event');
    $data['event_timezones']['table']['join']['node'] = array(
        'left_table' => 'event',
        'left_field' => 'timezone',
        'field' => 'timezone',
    );

    $data['event_timezones']['name'] = array(
        'title' => t('Timezone Name'),
        'help' => t('This is the timezone name for the event.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
   return $data;
}