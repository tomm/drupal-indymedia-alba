This is replacement for the Drupal archive.module which was available in
Drupal core in Drupal 4.7.x and previous versions. It provides a simple
calendar block that shows the current month with links to the archive for 
days that have posts. It also allows browsing of all content by date or 
node-type, so if you have a linked list users can browse just those nodes.

Installation:
  Install module like normal
  Visit admin/settings/archive and configure which node types you want
    browseable (MUST DO)
  Add link to /archive somewhere on your site. 

Alternatives:
  Weekly archive module: http://drupal.org/project/week
  Views module (much more general): http://drupal.org/project/views
                                    http://drupal.org/node/52037 

Originally by CodeMonkeyX

--------------------------------------------------------------------------------
*Developer* notes 
 
Access permissions:
 - The archive module provides no permissions on its own. Users in
   roles with "access content" permissions may view and select
   content from the /archive page.

Timezone handling:


 - Drupal sets $node->created to time() on node creation and storage
 - Then on display it invokes format_date() to get a date display,
   which adds a timezone to the timestamp and then uses gmdate()
 => This means that "time() + timezone" is used as the displayed date

 - Archive module sets today to time() + timezone to conform
 - SQL injected timestamps should have timezone subtracted, since
   we need to move the window on the queried nodes "back" to get
   nodes for the date the user expects to get
 - Displayed timestamps should have timezone added to them
 - Archive module uses "gm" functions, so that the server timezone
   is not added upon the used timestamps
--------------------------------------------------------------------------------
