// $Id: fckplugin.js,v 1.2.2.2 2008/11/19 12:00:08 wwalc Exp $
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
 * Plugin: add support for <!--pagebreak--> tag inside Drupal
 * Source: http://drupal.org/node/81893
 */

// Define the command.
var FCKDrupalPageBreak = function( name )
{
	this.Name = name ;
	this.EditMode = FCK.EditMode;
}

FCKDrupalPageBreak.prototype.Execute = function()
{
	if ( FCK.EditMode != FCK_EDITMODE_WYSIWYG ) 
		return ;
	
	FCKUndo.SaveUndoStep() ;
		
	switch ( this.Name )
	{
		case 'Break' :
			var e = FCK.EditorDocument.createComment( 'pagebreak' ) ;
			var oFakeImage = FCK.InsertElement( FCKDocumentProcessor_CreateFakeImage( 'FCK__PageBreak', e ) ) ;
			oFakeImage.setAttribute( "_drupalpagebreak", "true" ) ;
			this.MoveBreakOutsideElement();
		break;
		default :
		break;
	}	
}

FCKDrupalPageBreak.prototype.MoveBreakOutsideElement = function()
{
  FCK.FixBody();
	// get all elements in FCK document
	var elements = FCK.EditorDocument.getElementsByTagName( 'img' ) ;

	// check every element for childNodes
	var i = 0;
	var next ;
	while ( element = elements[i++] )
	{
		if ( element.getAttribute( '_drupalpagebreak' ) == "true" )
		{
			while( ( next = element.parentNode.nodeName.toLowerCase() ) != 'body' ) 
			{
				//if we are inside p or div, close immediately this tag, insert break tag, 
				//create new element and move remaining siblings to the next element
				if ( ( next == 'div' || next == 'p' ) && ( element.parentNode.parentNode.nodeName.toLowerCase() == 'body' ) )
				{
					var oParent = element.parentNode ;
					var oDiv = FCK.EditorDocument.createElement( next.toUpperCase() ) ;
					var sibling ;

					while( sibling = element.nextSibling )
						oDiv.appendChild( sibling ) ;

					if ( oDiv.childNodes.length )
					{
						if ( oParent.nextSibling )
							FCK.EditorDocument.body.insertBefore( oDiv, oParent.nextSibling ) ;
						else
							FCK.EditorDocument.body.appendChild( oDiv ) ;
					}

					if ( element.parentNode.nextSibling )
						element.parentNode.parentNode.insertBefore( element, element.parentNode.nextSibling ) ;
					else
						element.parentNode.parentNode.appendChild( element ) ;
						
					if ( !oParent.childNodes.length )
						FCK.EditorDocument.body.removeChild( oParent ) ;
					
					//if we put pagebreak next to another pagrebreak, remove it
					if ( element.nextSibling && element.nextSibling.getAttribute( '_drupalpagebreak' ) == "true")
						element.parentNode.removeChild( element.nextSibling ) ;
						
					//we must be sure the bogus node is available to make cursor blinking
					if ( FCKBrowserInfo.IsGeckoLike )
						FCKTools.AppendBogusBr( oParent ) ;
						
					break ;
				}
				else
				{
					if ( element.parentNode.nextSibling )
						element.parentNode.parentNode.insertBefore( element, element.parentNode.nextSibling ) ;
					else
						element.parentNode.parentNode.appendChild( element ) ;
				}				
			}			
		}
	}	
}

FCKDrupalPageBreak.prototype.GetState = function()
{
	return ( FCK.EditMode == FCK_EDITMODE_WYSIWYG ? FCK_TRISTATE_OFF : FCK_TRISTATE_DISABLED ) ;
}

// Register the Drupal tag commands.
FCKCommands.RegisterCommand( 'DrupalPageBreak', new FCKDrupalPageBreak( 'Break' ) ) ;
// Create the Drupal tag buttons.
var oDrupalItem = new FCKToolbarButton( 'DrupalPageBreak', FCKLang.DrupalPageBreakTitle, FCKLang.DrupalPageBreakTooltip, FCK_TOOLBARITEM_ICONTEXT, true, true ) ;
oDrupalItem.IconPath = FCKConfig.PluginsPath + 'drupalpagebreak/drupalpagebreak.gif';
FCKToolbarItems.RegisterItem( 'DrupalPageBreak', oDrupalItem ) ;

// after switch in to source mode and back proccess page and insert fake
// image for break again
// Drupal Page Breaks Processor

var FCKDrupalPageBreaksProcessor = FCKDocumentProcessor.AppendNew() ;
FCKDrupalPageBreaksProcessor.ProcessDocument = function( document )
{
	// get all elements in FCK document
	var elements = document.getElementsByTagName( '*' ) ;

	// check every element for childNodes
	var i = 0;
	while (element = elements[i++]) {
		var nodes = element.childNodes;

		var j = 0;
		while (node = nodes[j++]) {
			if (node.nodeName == '#comment') {
				var re = /\{\d+\}/ ;
				var PContent;
				if (re.test(node.nodeValue))
					PContent = FCKConfig.ProtectedSource.Revert('<!--' + node.nodeValue + '-->', false);
										
				if (node.nodeValue == 'pagebreak' || PContent == '<!--pagebreak-->') {					
					var oFakeImage = FCKDocumentProcessor_CreateFakeImage( 'FCK__PageBreak', node.cloneNode(true) ) ;
					oFakeImage.setAttribute( "_drupalpagebreak", "true" ) ;
					node.parentNode.insertBefore( oFakeImage, node ) ;
					node.parentNode.removeChild( node ) ;						
				}
			}
		}
	}
	FCKDrupalBreak.prototype.MoveBreakOutsideElement();
}

if ( !FCK.Config.ProtectedSource._RevertOld )
	FCK.Config.ProtectedSource._RevertOld = FCK.Config.ProtectedSource.Revert ;

FCK.Config.ProtectedSource.Revert = function( html, clearBin )
{
	// Call the original code.
	var result = FCK.Config.ProtectedSource._RevertOld ( html, clearBin ) ;
	
	if ( typeof FCKDrupalPageBreak !="undefined" && typeof FCKDrupalBreak !="undefined" )
		var re = /<(p|div)>((?:<!--pagebreak-->|<!--break-->)+)<\/\1>/gi ;
	else if ( typeof FCKDrupalBreak !="undefined" )
		var re = /<(p|div)>(<!--break-->)+<\/\1>/gi ;
	else if ( typeof FCKDrupalPageBreak !="undefined" )
		var re = /<(p|div)>(<!--pagebreak-->)+<\/\1>/gi ;
		
	result = result.replace( re, '$2' );
	return result ;
}