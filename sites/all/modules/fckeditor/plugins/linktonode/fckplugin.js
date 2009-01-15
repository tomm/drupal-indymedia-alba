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
FCKCommands.RegisterCommand( 'LinkToNode', new FCKDialogCommand( 'LinkToNode', FCKLang.DlgLinkToNode, FCKConfig.PluginsPath + 'linktonode/fck_linktonode.html', 400, 330 ) ) ;

// Create the Drupal tag buttons.
var oLinkNodeItem = new FCKToolbarButton( 'LinkToNode', 'LinkToNode', null, null, true, true ) ;
oLinkNodeItem.IconPath = FCKConfig.PluginsPath + 'linktonode/images/linktonode.gif';
FCKToolbarItems.RegisterItem( 'LinkToNode', oLinkNodeItem ) ;

FCK.ContextMenu.RegisterListener({
	AddItems : function( menu, tag, tagName )
	{
		var bInsideLink = ( tagName == 'A' || FCKSelection.HasAncestorNode( 'A' ) ) ;

		if ( bInsideLink || FCK.GetNamedCommandState( 'Unlink' ) != FCK_TRISTATE_DISABLED )
		{
			// Go up to the anchor to test its properties
			var oLink = FCKSelection.MoveToAncestorNode( 'A' ) ;
			var bIsAnchor = ( oLink && oLink.name.length > 0 && oLink.href.length == 0 ) ;
			// If it isn't a link then don't add the Link context menu
			if ( bIsAnchor )
			return ;

			// Get the actual Link href.
			var sHRef = oLink.getAttribute( '_fcksavedurl' ) ;
			if ( sHRef == null )
				sHRef = oLink.getAttribute( 'href' , 2 ) || '' ;
			
			if (sHRef.indexOf(FCKConfig.DrupalPath)==0 || sHRef.indexOf("internal:")==0) 
			{
				menu.RemoveAllItems() ;
				
				menu.AddItem( 'Cut'		, FCKLang.Cut	, 7, FCKCommands.GetCommand( 'Cut' ).GetState() == FCK_TRISTATE_DISABLED ) ;
				menu.AddItem( 'Copy'	, FCKLang.Copy	, 8, FCKCommands.GetCommand( 'Copy' ).GetState() == FCK_TRISTATE_DISABLED ) ;
				menu.AddItem( 'Paste'	, FCKLang.Paste	, 9, FCKCommands.GetCommand( 'Paste' ).GetState() == FCK_TRISTATE_DISABLED ) ;				
				menu.AddSeparator() ;
				if ( bInsideLink )
					menu.AddItem( 'LinkToNode', FCKLang.EditLink		, FCKConfig.PluginsPath + 'linktonode/images/linktonode.gif' ) ;
				menu.AddItem( 'Unlink'	, FCKLang.RemoveLink	, FCKConfig.PluginsPath + 'linktonode/images/linktonode.gif' ) ;
			}
		}
	}
});
