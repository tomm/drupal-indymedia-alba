/*
* FCKeditor - The text editor for Internet - http://www.fckeditor.net
* Copyright (C) 2003-2007 Frederico Caldeira Knabben
*
* == BEGIN LICENSE ==
*
* Licensed under the terms of any of the following licenses at your
* choice:
*
*  - GNU General Public License Version 2 or later (the "GPL")
*    http://www.gnu.org/licenses/gpl.html
*
*  - GNU Lesser General Public License Version 2.1 or later (the "LGPL")
*    http://www.gnu.org/licenses/lgpl.html
*
*  - Mozilla Public License Version 1.1 or later (the "MPL")
*    http://www.mozilla.org/MPL/MPL-1.1.html
*
* == END LICENSE ==
*
* Plugin to insert "Placeholders" in the editor.
*/

// The object used for all Placeholder operations.


// Register the Drupal tag commands.
FCKCommands.RegisterCommand( 'LinkToMenu', new FCKDialogCommand( 'LinkToMenu', FCKLang.DlgLinkToMenu, FCKConfig.PluginsPath + 'linktomenu/fck_linktomenu.html', 400, 330 ) ) ;

// Create the Drupal tag buttons.
var oLinkNodeMenuItem = new FCKToolbarButton( 'LinkToMenu', 'LinkToMenu', null, null, true, true ) ;
oLinkNodeMenuItem.IconPath = FCKConfig.PluginsPath + 'linktomenu/images/linktomenu.gif';
FCKToolbarItems.RegisterItem( 'LinkToMenu', oLinkNodeMenuItem ) ;
