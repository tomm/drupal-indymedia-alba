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

//FCK.Events.AttachEvent( 'OnSelectionChange', FCKImgAssist.Redraw ) ;

var FCKImageAssistProcessor = FCKDocumentProcessor.AppendNew() ;
FCKImageAssistProcessor.ProcessDocument = function( document )
{
  // get all elements in FCK document
  var elements = document.getElementsByTagName( '*' ) ;

  // check every element for childNodes
  var i = 0;
  while (element = elements[i++]) {
    var nodes = element.childNodes;

    var j = 0;
    while (node = nodes[j++]) {
      if (node.nodeName == '#text') {

        var aPlaholders = node.nodeValue.match( /\[img_assist[^\[\]]+\]/g ) ;

        if (aPlaholders) {
          var index = 0;
          for ( var k = 0 ; k < aPlaholders.length ; k++ )
          {
            index = node.nodeValue.indexOf(aPlaholders[k]);
            if (index != -1) {
              var e = FCK.EditorDocument.createTextNode( aPlaholders[k] ) ;
              var oFakeImage = FCKDocumentProcessor_CreateFakeImage( 'FCK__ImgAssist', e ) ;
              oFakeImage.setAttribute( "_fckimgassist", "true" ) ;
              oFakeImage.setAttribute("src", FCKConfig.PluginsPath + "imgassist/imgassist.gif");
              oFakeImage.style.width=72;
              oFakeImage.style.height=50;

              var substr1 = FCK.EditorDocument.createTextNode( node.nodeValue.substring(0, index) ) ;
              var substr2 = FCK.EditorDocument.createTextNode( node.nodeValue.substring(index + aPlaholders[k].length) ) ;

              node.parentNode.insertBefore( substr1, node ) ;
              node.parentNode.insertBefore( oFakeImage, node ) ;
              node.parentNode.insertBefore( substr2, node ) ;

              node.parentNode.removeChild( node ) ;
            }
          }
        }
      }
    }
  }
}

FCKImageAssistProcessor.Redraw = function( document )
{
  FCKImageAssistProcessor.ProcessDocument(FCK.EditorDocument);
}

// Context menu for templates.
FCK.ContextMenu.RegisterListener({
  AddItems : function( contextMenu, tag, tagName )
  {
    if ( tagName == 'IMG' )
    {
      if ( tag.getAttribute( '_fckimgassist' ) )
      {
        contextMenu.AddSeparator() ;
        contextMenu.AddItem( 'Img_Assist', 'Image Assist' ) ;
      }
    }
  }
});

FCKCommands.RegisterCommand( 'Img_Assist', new FCKDialogCommand( 'Img_Assist', 'Image Assist Object', FCKConfig.PluginsPath + 'imgassist/fck_imgassist.html', 400, 330 ) ) ;

FCK.Events.AttachEvent( 'OnSelectionChange', FCKImageAssistProcessor.Redraw ) ;