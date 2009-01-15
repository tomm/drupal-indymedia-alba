$Id: README.txt,v 1.16.2.5 2008/06/19 11:10:09 wwalc Exp $

Overview
--------
This module allows Drupal to replace textarea fields with the
FCKeditor.
This HTML text editor brings many of the powerful functions of known
desktop editors like Word to the web. It's relatively lightweight and
doesn't require any kind of installation on the client computer.

Required components
-------------------
To use FCKeditor in Drupal, you will need to download the FCKeditor
http://www.fckeditor.net/

More information and licence
----------------------------
FCKeditor - The text editor for internet
Copyright (C) 2003-2008 Frederico Caldeira Knabben

Licensed under the terms of the GNU Lesser General Public License:
    http://www.opensource.org/licenses/lgpl-license.php

For further information visit:
    http://www.fckeditor.net/
    http://drupal.fckeditor.net

Requirements
------------
  - Drupal 6.x
  - PHP 4.3.0 or greater
  - FCKeditor 2.3.x or greater (http://www.fckeditor.net/)

Configuration
-------------------
Note: this instruction assumes that you install FCKeditor in 
      sites/all/modules directory (recommended).

   1. Unzip the files in the sites/all/modules directory. It should now 
      contain a fckeditor directory.
   2. Download FCKeditor from http://www.fckeditor.net/download. Unzip the 
      contents of the fckeditor directory in the 
      sites/all/modules/fckeditor/fckeditor directory.
   3. Enable the module as usual from Drupal's admin pages.
   4. Grant permissions for use of FCKeditor in 
      "Administer > User Management > Permissions"
      Note: to enable the file browser, read also the 
            "How to enable the file browser" section.
   5. Under "Administer > Site configuration > FCKeditor", adjust 
      the fckeditor profiles. In each profile you can choose which textareas 
      will be replaced by FCKeditor, select default toolbar and configure 
      some more advanced settings.
   6. For the Rich Text Editing to work you also need to configure your filters 
      for the users that may access Rich Text Editing. 
      Either grant those users Full HTML access or use the following:
      <a> <p> <span> <div> <h1> <h2> <h3> <h4> <h5> <h6> <img> <map> <area> <hr> 
      <br> <br /> <ul> <ol> <li> <dl> <dt> <dd> <table> <tr> <td> <em> <b> <u> <i> <strong> 
      <font> <del> <ins> <sub> <sup> <quote> <blockquote> <pre> <address> <code> 
      <cite> <embed> <object> <strike> <caption>.
   7. To have a better control over line breaks, you may disable Line break converter 
      in the chosen filter (recommended).
   8. Modify the fckeditor.config.js file to custom your needs (optional).
      You may copy the needed configuration lines from the default FCKeditor 
      configuration settings (sites/all/modules/fckeditor/fckeditor/fckconfig.js), 
      the lines in fckeditor.config.js will override most settings.

Installation troubleshooting
----------------------------
If your FCKeditor does not show you must check if all files are 
extracted correctly. 
The directory sites/all/modules/fckeditor/fckeditor/ should have the 
following files:
   - fckeditor.js
   - fckconfig.js
   - fckstyles.xml
   - fcktemplates.xml 
and a directory named editor.

The correct directory structure is as follows:
    modules
       fckeditor
          fckeditor.module
          fckeditor
             _samples
             editor
             COPY_HERE.txt
             fckconfig.js
             ...

If you're still having problems, scroll down to the "Help & Contribution" section.
             
Plugins: Teaser break and Pagebreak
-----------------------------------
By default, FCKeditor module comes with two plugins that can handle 
teaser break (<!--break-->) and pagebreak (<!--pagebreak-->). 
You can enable any (or even both) of them.

   1. Open sites/all/modules/fckeditor/fckeditor.config.js and 
      uncomment these three lines:

            FCKConfig.PluginsPath = '../../plugins/' ;
            FCKConfig.Plugins.Add( 'drupalbreak' ) ;
            FCKConfig.Plugins.Add( 'drupalpagebreak' ) ;
            

   2. The second step is to add buttons to the toolbar (in the same file). 
      The button names are: DrupalBreak, DrupalPageBreak. 
      For example if you have a toolbar with an array of buttons defined 
      as follows:

      ['Image','Flash','Table','Rule','SpecialChar']

      simply add those two buttons at the end of array:

      ['Image','Flash','Table','Rule','SpecialChar', 'DrupalBreak', 'DrupalPageBreak']

      (remember about single quotes).

Uploading images and files
--------------------------

There are three ways of uploading files: By using the built-in file browser, 
by using a module like IMCE or using the core upload module.

How to enable the file browser
------------------------------
The editor gives the end user the flexibility to create a custom file browser 
that can be integrated on it. 
The included file browser allows users to view the content of a specific 
directory on the server and add new content to that directory 
(create folders and upload files).

   1. To enable file browsing you need to edit the connector configuration file
      in your fckeditor module directory, the file should be in:

          sites/all/modules/fckeditor/fckeditor/editor/filemanager/connectors/php/config.php
          (FCKeditor 2.5+)

          or

          sites/all/modules/fckeditor/fckeditor/editor/filemanager/browser/default/connectors/php/config.php
          and
          sites/all/modules/fckeditor/fckeditor/editor/filemanager/upload/php/config.php
          (FCKeditor 2.3.x - 2.4.x)

      In this file(s) you will need to enable the file browser by adding one 
      line that includes file with the special authentication function for 
      Drupal (filemanager.config.php). Add this code:

          require_once "../../../../../filemanager.config.php";
          (FCKeditor 2.5+)

      or

          require_once "D:\\xampp\\htdocs\\drupal\\sites\\all\\modules\\fckeditor\\filemanager.config.php"
          (FCKeditor 2.3.x - 2.4.x)

      straight below this line:

          $Config['UserFilesAbsolutePath'] = '' ;

      The config.php file also holds some other important settings, please 
      take a look at it and adjust it to your needs (optional).
      
   2. As of Drupal 5.2, additional step is required: locate file named 
      settings.php inside your drupal directory (usually sites/default/settings.php) 
      and set $cookie_domain variable to the appropiate domain 
      (remember to uncomment that line). If you not do this, FCKeditor will 
      claim that file browser is disabled
      
   3. Enabling file uploads is a security risk. That's why you have to grant a 
      separate permission to enable the file browser to certain groups.
      In "Administer > User Management > Permissions" assign the
      "allow fckeditor file uploads" permissions.
      
   4. Lastly, adjust the File browser settings for each profile.

Modules: Image Assist
---------------------
Image Assist can be integrated with FCKeditor. 
To do this, simply copy the modules/fckeditor/img_assist_fckeditor.js file to modules/img_assist/img_assist_fckeditor.js.
   
Help & Contribution
-------------------
If you are looking for more information, have any troubles in configuration or if 
you found an issue, please visit the official project page:
  http://drupal.org/project/fckeditor

Having problems? Take a look at list of common problems when installing FCKeditor:
  http://drupal.fckeditor.net/troubleshooting

How to tune up FCKeditor to your theme:
  http://drupal.fckeditor.net/tricks
  
We would like to encourage you to join our team if you can help in any way.
If you can translate FCKeditor module, please use fckeditor.pot file as a template
(located in "po" directory) and send us the translated file so that we could attach it.
Any help is appreciated.
     
Credits
-------
 - FCKeditor for Drupal Core functionality originally written by:
     Frederico Caldeira Knabben
     Jorge Tite (LatPro Inc.)

 - FCKeditor for Drupal 5.x originally written by:
     Ontwerpwerk (www.ontwerpwerk.nl)
 
 - FCKeditor for Drupal 5.x is currently maintained by FCKeditor team.
     http://www.fckeditor.net/

 - FCKeditor - The text editor for internet
     Copyright (C) 2003-2006 Frederico Caldeira Knabben
     http://www.fckeditor.net/
