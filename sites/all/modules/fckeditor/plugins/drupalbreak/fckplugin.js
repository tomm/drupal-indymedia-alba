// $Id: fckplugin.js,v 1.2 2007/11/15 16:15:48 wwalc Exp $
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
 * Plugin: add support for <!--break--> tag inside Drupal
 * Source: http://drupal.org/node/81893
 */

var pBreakBorderStyle = "#FF0000 1px dotted" ;

// Define the command.
var FCKDrupalBreak = function( name )
{
	this.Name = name ;
	this.EditMode = FCK.EditMode ;
}

FCKDrupalBreak.prototype.Execute = function()
{
	if ( FCK.EditMode != FCK_EDITMODE_WYSIWYG ) 
		return ;
	
	FCKUndo.SaveUndoStep() ;
		
	switch ( this.Name )
	{
		case 'Break' :
			this.RemoveOldBreaks();
			var e = FCK.EditorDocument.createComment( 'break' ) ;
			oFakeImage = FCK.InsertElement( FCKDocumentProcessor_CreateFakeImage( 'FCK__DrupalBreak', e ) ) ;
			oFakeImage.setAttribute( "_drupalbreak", "true" ) ;
			oFakeImage.style.borderTop = oFakeImage.style.borderBottom = pBreakBorderStyle ;
			this.MoveBreakOutsideElement();
		break;
		default :
		break;
	}	
}

FCKDrupalBreak.prototype.RemoveOldBreaks = function()
{	
	// get all elements in FCK document
	var elements = FCK.EditorDocument.getElementsByTagName( 'img' ) ;

	// check every element for childNodes
	var i = 0;
	var next ;
	while ( element = elements[i++] )
	{
		if ( element.getAttribute( '_drupalbreak' ) == "true" )
		{
			element.parentNode.removeChild( element ) ;
		}
	}
}

FCKDrupalBreak.prototype.MoveBreakOutsideElement = function()
{
  FCK.FixBody();
	// get all elements in FCK document
	var elements = FCK.EditorDocument.getElementsByTagName( 'img' ) ;

	// check every element for childNodes
	var i = 0;
	var next ;
	while ( element = elements[i++] )
	{
		if ( element.getAttribute( '_drupalbreak' ) == "true" )
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

FCKDrupalBreak.prototype.GetState = function()
{
	return ( FCK.EditMode == FCK_EDITMODE_WYSIWYG ? FCK_TRISTATE_OFF : FCK_TRISTATE_DISABLED ) ;
}

// Register the Drupal tag commands.
FCKCommands.RegisterCommand( 'DrupalBreak', new FCKDrupalBreak( 'Break' ) ) ;
// Create the Drupal tag buttons.
var oDrupalItem = new FCKToolbarButton( 'DrupalBreak', 'Summary / body separator', null, FCK_TOOLBARITEM_ICONTEXT, true, true ) ;
oDrupalItem.IconPath = FCKConfig.PluginsPath + 'drupalbreak/drupalbreak.gif';
FCKToolbarItems.RegisterItem( 'DrupalBreak', oDrupalItem ) ;

// after switch in to source mode and back proccess page and insert fake
// image for break again
// Drupal Page Breaks Processor

var FCKDrupalBreaksProcessor = FCKDocumentProcessor.AppendNew() ;
FCKDrupalBreaksProcessor.ProcessDocument = function( document )
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
				if (node.nodeValue == 'break' || PContent == '<!--break-->') {
					var oFakeImage = FCKDocumentProcessor_CreateFakeImage( 'FCK__DrupalBreak', node.cloneNode(true) ) ;
					oFakeImage.setAttribute( "_drupalbreak", "true" ) ;
					oFakeImage.style.borderTop = oFakeImage.style.borderBottom = pBreakBorderStyle ;
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
