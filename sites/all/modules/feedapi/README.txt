; $Id: README.txt,v 1.1.2.6.2.8 2009/07/02 16:38:15 aronnovak Exp $

FeedAPI
-------

The FeedAPI module group has several purposes:

1) Provide a basic aggregation module that is able to handle various parsers
   and processors and give the user the ability to change how feed items are
   represented and how incoming feed URLs are parsed.
2) Provide an API for writing parsers and processors. FeedAPI defines an
   exact requirement set for both parsers and processors. For more details,
   see the developer documentation at http://groups.drupal.org/node/5301.
3) Provide a common base for existing aggregator-purpose modules to avoid
   the overlapping of tasks (for example, parsing and downloading).

Known issues:
- Parser SimplePie submodule may be incompatible with other modules which include
  SimplePie (for example Activity Stream)
- Do not use FeedAPI and poormanscron together (http://drupal.org/node/235328)


Sub-modules
-----------

If you only turn on FeedAPI module on the admin/build/modules page,
you won't get the functionality that you expect from an aggregator module.

If you don't want to bother about the fine print here, a good standard configuration
is FeedAPI, FeedAPI Node and SimplePie parser (don't forget to download the SimplePie
package from http://www.simplepie.org).

Here is the purpose of each sub-module:

1) FeedAPI - This is the main module, you should always enable it.
2) FeedAPI Node - If you like the idea of feed items are nodes, you should
   enable this.
   (processor)
4) FeedAPI Inherit - If you use og module and taxonomy modules and you would
   like to pass the information of these modules between the feeds and feed
   items this processor can handle it.
5) Common Syndication Parser - Only enable this if you have a PHP5-powered
   site. If PHP5 is present, this parser has excellent performance.
   (parser)
6) SimplePie Parser - This is the parser for you if your site runs on PHP4.
   In some cases it is better and more established than the
   Common Syndication Parser. Because of the Drupal CVS policy, the
   repository should not contain 3rd party source code, so:
   -----------
   WARNING: before you turn on SimplePie Parser, please download
   SimplePie from http://www.simplepie.org and move simplepie.inc file
   to feedapi/parser_simplepie directory.
   -----------
   (parser)

To get things working you must enable at least one parser and one processor.

Start using the module
----------------------

Feeds are based on content-types. As you enable the processors one
content-type is created for each processors. Likely you want to use these
pre-configured content-types for creating the feeds.
If you would setup more complex processor configuration,
please visit admin/content/types and edit/add the existing content-types. About
the content-type editing: make sure that "Is a feed content type" checkbox is
turned on. Below that checkbox you will find the parsers and processors
configuration.

Create your first feed
----------------------

Visit the node/add page and select the content-type you like. Supply the
URL and hit Submit. Then select Refresh at the feed node, if you want to see the
items.

Developers
----------

parser_simplepie.module contains a stop gap patch that won't be necessary for most
installations (issue http://drupal.org/node/205706). If you would like to use automatic
feed type detection, please remove the lines

// Stop gap for simplefeed.inc version <= 1.1
return "XML feed";

from your parser_simplepie.module and use simplepie > 1.1 version, if you experience
a whitescreen on refreshing feeds.

Author/credits
--------------

* The author of the module is Áron Novák (http://drupal.org/user/61864).
* The module uses SimplePie (www.simplepie.org).
* The module refactored Aggregation module's
  (http://www.drupal.org/project/aggregation) parser.
* Alex Barth (http://drupal.org/user/53995) provided very useful patches.
  (The concept of setting storage and feed = node, plus the feedapi_aggregator,
  are almost entirely his work.)
* Jose Reyero (http://drupal.org/user/4299) refactored the whole cron part
  of the module and he provided performance-booster patches.
* I got lots of useful responses from my mentors
  Ken Rickard, Károly Négyesi (chx), David Norman (deekayen), and Alex Barth.
--
This is a Summer of Code 2007 sponsored project.
