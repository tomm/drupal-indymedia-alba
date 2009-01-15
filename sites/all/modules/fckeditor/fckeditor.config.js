// $Id: fckeditor.config.js,v 1.5.2.5 2008/06/20 12:24:14 wwalc Exp $

/*
 Define as many toolbars as you need, you can change toolbar names
 DrupalBasic will be forced on some smaller textareas (if enabled)
 if you change the name of DrupalBasic, you have to update 
 FCKEDITOR_FORCE_SIMPLE_TOOLBAR_NAME in fckeditor.module
 */

//uncomment these three lines to enable teaser break and page break plugins
//remember to add 'DrupalBreak' and 'DrupalPageBreak' buttons to the toolbar
FCKConfig.PluginsPath = '../../plugins/' ;
FCKConfig.Plugins.Add( 'drupalbreak' ) ;
FCKConfig.Plugins.Add( 'imgassist' ) ;
//FCKConfig.Plugins.Add( 'drupalpagebreak' ) ;

/*
 This toolbar is dedicated to users with "Full HTML" access 
 some of commands used here (like 'FontName') use inline styles,
 which unfortunately are stripped by "Filtered HTML" filter
 */
FCKConfig.ToolbarSets["DrupalFull"] = [
['Source'],
['Cut','Copy','Paste','PasteText','PasteWord'],
['Undo','Redo','-','Find','Replace','-','SelectAll','RemoveFormat'],
['Bold','Italic','Underline','StrikeThrough','-','Subscript','Superscript'],
['OrderedList','UnorderedList','-','Outdent','Indent'],
//as of FCKeditor 2.5 you can use also 'Blockquote' button
//['OrderedList','UnorderedList','-','Outdent','Indent','Blockquote'],
['JustifyLeft','JustifyCenter','JustifyRight'],
['Link','Unlink','Anchor'],
['Image','Flash','Table','Rule','SpecialChar','DrupalBreak'],
//uncomment this line to enable teaser break and page break buttons
//remember to load appropriate plugins with FCKConfig.Plugins.Add command a couple of lines below
//['Image','Flash','Table','Rule','SpecialChar','DrupalBreak','DrupalPageBreak'],
'/',
['FontFormat','FontName','FontSize'],
['TextColor','BGColor']
] ;

FCKConfig.ToolbarSets["DrupalBasic"] = [
['Bold','Italic','-','OrderedList','UnorderedList','-','Link','Unlink', 'Image','DrupalBreak']
] ;

//This toolbar should work fine with "Filtered HTML" filter
FCKConfig.ToolbarSets["DrupalFiltered"] = [
['Source'],
['Cut','Copy','Paste','PasteText','PasteWord'],
['Undo','Redo','-','Find','Replace','-','SelectAll','RemoveFormat'],
//as of FCKeditor 2.5 you can use also 'Blockquote' button
//['OrderedList','UnorderedList','-','Outdent','Indent','Blockquote'],
['Link','Unlink','Anchor'],
['Image','Flash','Table','Rule','Smiley','SpecialChar'],
'/',
['FontFormat'],
['Bold','Italic','Underline','StrikeThrough','-','Subscript','Superscript'],
['OrderedList','UnorderedList','-','Outdent','Indent'],
['JustifyLeft','JustifyCenter','JustifyRight','DrupalBreak'],
] ;

//as of FCKeditor 2.5 ShowBlocks command is available
//remove this code if you don't need ShowBlocks buttons
if ( FCK.GetData ) {
  var len ;
  var ts = FCKConfig.ToolbarSets ;
  if (ts["DrupalFull"]) {
    len = ts["DrupalFull"].length ;
    if (!ts["DrupalFull"][len-1] && ts["DrupalFull"][len-2])
      len--;
    if (ts["DrupalFull"][len-1])
      FCKConfig.ToolbarSets["DrupalFull"][len-1].push('ShowBlocks') ;
  }
  if (ts["DrupalFiltered"]) {
    len = ts["DrupalFiltered"].length ;
    if (!ts["DrupalFiltered"][len-1] && ts["DrupalFiltered"][len-2])
      len--;
    if (ts["DrupalFiltered"][len-1])
      FCKConfig.ToolbarSets["DrupalFiltered"][len-1].push('ShowBlocks') ;
  }
}
// Protect PHP code tags (<?...?>) so FCKeditor will not break them when
// switching from Source to WYSIWYG.
// Uncommenting this line doesn't mean the user will not be able to type PHP
// code in the source. This kind of prevention must be done in the server side
// (as does Drupal), so just leave this line as is.
FCKConfig.ProtectedSource.Add( /<\?[\s\S]*?\?>/g ) ;	// PHP style server side code

var _FileBrowserLanguage	= 'php' ;
var _QuickUploadLanguage	= 'php' ;

// This overrides the IndentLength/IndentUnit settings.
FCKConfig.IndentClasses = ['rteindent1','rteindent2','rteindent3','rteindent4'] ;

// [ Left, Center, Right, Justified ]
FCKConfig.JustifyClasses = ['rteleft','rtecenter','rteright','rtejustify'] ;
