README.txt
==========
The purpose of this module is to provide de Drupal developer with some tools that assist in making a site mobile.

Main functionalities are:

 - theme switching based on mobile device or device group
 - User notification (notify users of existence of mobile or desktop site)
 - User redirection (mobile user to the mobile site) 
 - Cookies to remember users preference (viewing mobile or desktop site)
 - Mobile permissions [experimental]
 - managing the number of displayed posts on the frontpage
 - using third party modules for device detection


CACHING
=======
Device detection and redirection works fine when caching is enabled. However if you choose to do theme switching based on device, caching will bypass the theme switching and get your pages out of cache:
e.g.: First user visits the site with his iphone and gets the iphone theme. The theme get's stored in cache, but the second user with a simple feature phone will also get the iphone theme.

Solution: Open the mobile_tools_cache.inc file and read the documentation!


AGGRESSIVE CACHING
==================
The Mobile Tools module does not support device detection when you use aggressive caching. 
Aggressive cachig bypasses any Drupal loading and by consequence the device detection system.

Recommended is to try user agent detection in your caching server and use a similar algorithm than 
the one used by Mobile Tools. 


AUTHOR/MAINTAINER
======================
Tom Deryckere
http://twitter.com/twom
http://www.mobiledrupal.com
