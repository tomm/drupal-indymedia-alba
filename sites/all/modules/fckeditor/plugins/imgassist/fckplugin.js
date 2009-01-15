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
          var index = 0;
          while (aPlaholders = node.nodeValue.match( /\[img_assist[^\[\]]+\]/g )) {
            index = node.nodeValue.indexOf(aPlaholders[0]);
            if (index != -1) {
              var oImgAs = FCKTools.ImageAssistDecode ( aPlaholders[0] );

              var substr1 = FCK.EditorDocument.createTextNode( node.nodeValue.substring(0, index) );
              var substr2 = FCK.EditorDocument.createTextNode( node.nodeValue.substring(index + aPlaholders[0].length) );

              node.parentNode.insertBefore( substr1, node ) ;
              node.parentNode.insertBefore( oImgAs, node ) ;
              node.parentNode.insertBefore( substr2, node ) ;

              node.parentNode.removeChild( node ) ;
              if (node) node.nodeValue='';
            }
          }
      }
    }
  }
}

// Open the image_assis dialog on double click.
FCKImageAssistProcessor.OnDoubleClick = function( span ) {
  if ( span.tagName == 'IMG' && span._fckimgassist_fckimgassist ) {
    FCKCommands.GetCommand( 'Img_Assist' ).Execute() ;
  }
}

// We must process the IMG tags, change <img...> to [img_assist...]
FCKXHtml.TagProcessors['img'] = function( node, htmlNode )
{
  if ( htmlNode.getAttribute('nid') ) {
    var IAString = FCKTools.ImageAssistCode ( htmlNode );
    if( IAString )
    node = FCKXHtml.XML.createTextNode(IAString) ;
  }
  else{
    FCKXHtml._AppendChildNodes( node, htmlNode, false ) ;
  }
  return node ;
}

FCKImageAssistProcessor.Redraw = function( document )
{
  FCKImageAssistProcessor.ProcessDocument(FCK.EditorDocument);
}

// Context menu for templates.
FCK.ContextMenu.RegisterListener({
  AddItems : function( contextMenu, tag, tagName )
  {
    if ( tagName == 'IMG' || tagName == 'img') {
      if ( tag.getAttribute( '_fckimgassist' ) ) {
        contextMenu.RemoveAllItems();
        contextMenu.AddItem( 'Img_Assist', 'Image Assist' , FCKConfig.PluginsPath + 'imgassist/imgassist_small.gif') ;
      }
    }
  }
});

FCKTools.ImageAssistDecode = function( IAString )
{
  var IAObj = FCK.EditorDocument.createElement( 'img' ) ;
  var IAarg = IAString.match( /\[img_assist\s*([^\]]*?)\]/ );
  IAarg=IAarg[1].toString().split("|");
  var order = new Array();
  for (var k=0;k<IAarg.length;k++) {
    index2 = IAarg[k].indexOf('=');
    if (index2 != -1) {
      var temp_name = IAarg[k].split("=",1)[0];
      var temp_val  = IAarg[k].slice(index2+1);
      order[k] = temp_name;
      if (temp_name == 'width') {
        if (parseInt(temp_val) == 'NaN') temp_val=72;
        IAObj.style.width = parseInt(temp_val);
      }
      else if (temp_name == 'height') {
        if (parseInt(temp_val) == 'NaN') temp_val=50;
        IAObj.style.height = parseInt(temp_val);
      }
      else {
        IAObj.setAttribute(temp_name, temp_val);
      }
    }
  }
  if (!IAObj.style.width)  IAObj.setAttribute('width', 72);
  if (!IAObj.style.height) IAObj.setAttribute('height', 50);
  IAObj.setAttribute ('order', order.join("|"));
  IAObj.setAttribute ('src', FCKConfig.BasePath +'images/spacer.gif');
  IAObj.setAttribute ('_fckimgassist', 'true');
  IAObj._fckimgassist = true ;
  IAObj.className = 'image_assist';

  return IAObj;
}

FCKTools.ImageAssistCode = function( IAObj )
{
  var IAString;
  if ( IAObj.getAttribute('nid') ) {
    var IAString = '[img_assist'; 
    var orderStr = IAObj.getAttribute('order').toString();
    var order = orderStr.split("|");
    for(var i=1;i<order.length;i++) {
      IAString += '|'+ order[i] +'=';
      if (IAObj.getAttribute(order[i])) {
        IAString += IAObj.getAttribute(order[i]);
      }
      else if (order[i] == 'width') {
        IAString += parseInt(IAObj.style.width);
      }
      else if (order[i] == 'height') {
        IAString += parseInt(IAObj.style.height);
      }
    }
    if ((orderStr.indexOf('width') == -1) && (IAObj.style.width)) {
      IAString += '|width='+ parseInt(IAObj.style.width);
    }
    if ((orderStr.indexOf('height') == -1) && (IAObj.style.height)) {
      IAString += '|height='+ parseInt(IAObj.style.height);
    }
    IAString += ']' ;
  }
  else {
    return false;
  }
  return IAString;
}

FCKCommands.RegisterCommand( 'Img_Assist', new FCKDialogCommand( 'Img_Assist', 'Image Assist Object', FCKConfig.PluginsPath + 'imgassist/fck_imgassist.html', 500, 330 ) ) ;

FCK.Events.AttachEvent( 'OnSelectionChange', FCKImageAssistProcessor.Redraw ) ;

/*
 * Plugin: add image_assist button
 */
var FCKimageAssist = function(){
  this.EditMode = FCK.EditMode;
}

var imgAssistIcon= top.document.getElementById('img_assist-link-' + FCK.Config['TextareaID']);
if (imgAssistIcon){

  FCKimageAssist.prototype.Execute = function(){
    if ( FCK.EditMode != FCK_EDITMODE_WYSIWYG ) {
      return ;
    }
    ia_icon = top.document.getElementById('img_assist-link-' + FCK.Config['TextareaID']);
    ia_href = ia_icon.getAttribute("href");
    top.window.open(ia_href, 'img_assist_link', 'width=600,height=350,scrollbars=yes,status=yes,resizable=yes,toolbar=no,menubar=no');
  }
  // Define the command.
  FCKimageAssist.prototype.GetState = function(){
    return ( FCK.EditMode == FCK_EDITMODE_WYSIWYG ? FCK_TRISTATE_OFF : FCK_TRISTATE_DISABLED ) ;
  }
  FCKCommands.RegisterCommand( 'ImageAssist', new FCKimageAssist() ) ;

  var oIAItem = new FCKToolbarButton( 'ImageAssist', 'Image Assist', null, FCK_TOOLBARITEM_ICONTEXT, true, true ) ;
  oIAItem.IconPath = FCKConfig.PluginsPath + 'imgassist/imgassist_small.gif';
  // Create the ImageAssist buttons.
  FCKToolbarItems.RegisterItem( 'ImageAssist', oIAItem ) ;

  addToolbarElement('ImageAssist', 'Basic', 0);
  addToolbarElement('ImageAssist', 'DrupalBasic', 0);
  addToolbarElement('ImageAssist', 'Default', 4);
  addToolbarElement('ImageAssist', 'DrupalFiltered', 0);
  addToolbarElement('ImageAssist', 'DrupalFull', 0);
}
