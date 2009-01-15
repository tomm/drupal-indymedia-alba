// $Id: fckeditor.utils.js,v 1.2.2.25 2008/12/12 20:27:03 wwalc Exp $
var fckIsRunning = new Array;
var fckIsLaunching = new Array;
var fckLaunchedTextareaId = new Array;
var fckLaunchedJsId = new Array;
var fckFirstrun = new Array;
var fckIsIE = ( /*@cc_on!@*/false ) ? true : false ;

function Toggle(js_id, textareaID, textTextarea, TextRTE, xss_check)
{
  var eFCKeditorDiv	= document.getElementById( 'fck_' + js_id ) ;
  var teaser = false;
  var teaserCheckbox = false;
  
  for (var i in Drupal.settings.teaser) {
    if (Drupal.settings.teaser[i] == textareaID)
      teaser = i;
      teaserCheckbox = Drupal.settings.teaserCheckbox[i];
  }

  if (!fckIsRunning[js_id])
  {
    if (!fckIsLaunching[js_id])
    {
      //display is set to '' at this stage because of IE 800a025e bug
      if (fckIsIE)
        eFCKeditorDiv.style.display = '' ;
      fckIsLaunching[js_id] = true;
      $(".img_assist-button").hide();
      if (xss_check && $('#' + textareaID).attr('class').indexOf("filterxss2") != -1) {
        $.post(Drupal.settings.basePath + 'index.php?q=fckeditor/xss', {
            text: $('#' + textareaID).val(),
            'filters[]': Drupal.settings.fckeditor_filters[js_id]
          }, 
          function(text) {
            $('#' + textareaID).val(text);
            $('#' + js_id).val(text);
            window[js_id].ReplaceTextarea();
          }
        );
      }
      else {
        eval(js_id + '.ReplaceTextarea();');
      }
      $('#img_assist-link-' + textareaID).hide();
    }
    setTimeout("Toggle('" + js_id + "','" + textareaID + "','" + textTextarea + "','" + TextRTE + "'," + xss_check + ");",1000);
    return ;
  }

  var oEditor ;
  if ( typeof( FCKeditorAPI ) != 'undefined' )
    oEditor = FCKeditorAPI.GetInstance( js_id );

  // Get the _Textarea and _FCKeditor DIVs.
  var eTextarea	= document.getElementById( textareaID );
  var eFCKeditor	= document.getElementById( js_id );
  var text;

  // If the _Textarea DIV is visible, switch to FCKeditor.
  if ( eTextarea.style.display != 'none' )
  {
    if (typeof( imceInitiateFCK ) != 'undefined')
      imceInitiateFCK();
    document.getElementById('switch_' + js_id).innerHTML = textTextarea;

    // Switch the DIVs display.
    eFCKeditorDiv.style.display = '';

    text = eTextarea.value;
    if (teaser && $('input[@class=teaser-button]').attr('value') == Drupal.t('Join summary')) {
      var val = $('#' + teaser).val();
      if (val && val.length) {
        text = val + '<!--break-->' + text;
      }
    }

    // This is a hack for Gecko 1.0.x ... it stops editing when the editor is hidden.
    if (oEditor && !document.all)
    {
      if (oEditor.EditMode == FCK_EDITMODE_WYSIWYG)
      oEditor.MakeEditable() ;
    }

    if ( text.length ) {
      oEditor.SetHTML( text, false);
    }
    eTextarea.style.display = 'none';
    $('#img_assist-link-' + textareaID).hide();
    $(".img_assist-button").hide();

    if (teaser) {
      $('div[@class=teaser-button-wrapper]').hide();
      $('#' + teaser).parent().hide();
      $('#' + teaserCheckbox).parent().show();
    }
  }
  else
  {
    if (fckFirstrun[js_id]) {
      fckFirstrun[js_id] = false;
    }
    if (textareaID == 'edit-body') {
      document.getElementById('switch_' + js_id).innerHTML = TextRTE;
    }

    var text = oEditor.GetHTML(true);
    
    if (teaser) {
      var t = text.indexOf('<!--break-->');
      if (t != -1) {
        $('#' + teaser).val(text.slice(0,t));
        $('#' + textareaID).val(text.slice(t+12));
        $('#' + teaser).parent().show();
        $('#' + teaser).attr('disabled', '');
        if ($('input[@class=teaser-button]').attr('value') != Drupal.t('Join summary')) {
          try {$('input[@class=teaser-button]').click();} catch(e) {$('input[@class=teaser-button]').val(Drupal.t('Join summary'));}
        }
      }
      else {
        $('#' + teaser).attr('disabled', 'disabled');
        if ($('input[@class=teaser-button]').attr('value') != Drupal.t('Split summary at cursor')) {
          try {$('input[@class=teaser-button]').click();} catch(e) {$('input[@class=teaser-button]').val(Drupal.t('Split summary at cursor'));}
        }
        // Set the textarea value to the editor value.
        $('#' + textareaID).val(text);
      }
    }
    else {
      // Set the textarea value to the editor value.
      $('#' + textareaID).val(text);
    }

    $('#img_assist-link-' + textareaID).show();
    $(".img_assist-button").show();
    // Switch the DIVs display.
    eTextarea.style.display = '';
    eFCKeditorDiv.style.display = 'none';
    if (teaser) {
      $('div[@class=teaser-button-wrapper]').show();
    }
  }
}

function CreateToggle(elId, jsId, fckeditorOn)
{
  var ta = document.getElementById(elId);
  var ta2 = document.getElementById('fck_' + jsId);

  if (!ta || !ta2)
    return ;

  ta2.value = ta.value;
  ta.parentNode.insertBefore(ta2, ta);
  if (fckeditorOn) {
    ta.style.display = 'none';
    $('#img_assist-link-' + elId).hide();
  }
  else
    ta2.style.display = 'none';
}

function doFCKeditorSave(){
  DoFCKeditorTeaserStuff();
  return true; //continue submitting
}  

function DoFCKeditorTeaserStuff()
{
    for( var i = 0 ; i < fckLaunchedJsId.length ; i++ ) {
      if ( document.getElementById( fckLaunchedTextareaId[i] ).style.display == 'none' )
      {
        var text = FCKeditorAPI.GetInstance( fckLaunchedJsId[i] ).GetXHTML(true);
        var teaser = false;
  
        for (var k in Drupal.settings.teaser) {
          if (Drupal.settings.teaser[k] == fckLaunchedTextareaId[i])
            teaser = k;
        }
        
        if (teaser) {
          var t = text.indexOf('<!--break-->');
          if (t != -1) {
            $('#' + teaser).val(text.slice(0,t));
            document.getElementById( fckLaunchedTextareaId[i] ).value = text.slice(t+12);
          }
          else {
            $('#' + teaser).val('');
            $('#' + teaser).attr('disabled', 'disabled');
            document.getElementById( fckLaunchedTextareaId[i] ).value = text;
            if ($('input[@class=teaser-button]').attr('value') == Drupal.t('Join summary')) {
              try {$('input[@class=teaser-button]').click();} catch(e) {$('input[@class=teaser-button]').val(Drupal.t('Join summary'));}
            }
          }
        }
        else {
          document.getElementById( fckLaunchedTextareaId[i] ).value = text;
        }
      }
    }
}
// The FCKeditor_OnComplete function is a special function called everytime an
// editor instance is completely loaded and available for API interactions.
function FCKeditor_OnComplete( editorInstance )
{
  fckIsRunning[editorInstance.Name] = true ;
  fckLaunchedTextareaId.push(editorInstance.Config['TextareaID']) ;
  fckLaunchedJsId.push(editorInstance.Name) ;
  fckFirstrun[editorInstance.Name] = true;

  // Enable the switch button. It is disabled at startup, waiting the editor to be loaded.
  var oElem = document.getElementById('switch_' + editorInstance.Name);
  if (oElem != null) {
    oElem.style.display = '';
  }
  
  // If the textarea isn't visible update the content from the editor.
  $(editorInstance.LinkedField.form).submit(DoFCKeditorTeaserStuff);

  editorInstance.Events.AttachEvent( 'OnAfterLinkedFieldUpdate', DoFCKeditorTeaserStuff ) ;
  
  var teaser = false;
  var teaserCheckbox = false;
  
  for (var k in Drupal.settings.teaser) {
    if (Drupal.settings.teaser[k] == editorInstance.Config['TextareaID']) {
      teaser = k;
      teaserCheckbox = Drupal.settings.teaserCheckbox[k];
    }
  }
  
  if (teaser) {
    $('#' + teaser).attr('disabled', '');
    $('div[@class=teaser-button-wrapper]').hide();
    $('#' + teaser).parent().hide();
    $('#' + teaserCheckbox).parent().show();
  }

  //Img_Assist integration
  IntegrateWithImgAssist();

  // Upload module image insert into fckeditor wants this
  window.oooFckName = editorInstance.Name;
  window.oooFCKeditorAPI = FCKeditorAPI;
}

function FCKeditorReplaceTextarea(textarea_id, oFCKeditor, xss_check)
{
  if ($('#' + oFCKeditor.Config['TextareaID']).length === 0) {
    return;
  }
  $(".img_assist-button").hide();
  if (xss_check && $('#' + oFCKeditor.Config['TextareaID']).attr('class').indexOf("filterxss") != -1) {
    $.post(Drupal.settings.basePath + 'index.php?q=fckeditor/xss', {
      text: $('#' + textarea_id).val(),
      'filters[]': Drupal.settings.fckeditor_filters[textarea_id]
      }, 
      function(text) {
        $('#' + textarea_id).val(text);
        oFCKeditor.ReplaceTextarea();
      }
    );
  }
  else {
    oFCKeditor.ReplaceTextarea();
  }
}

function IntegrateWithImgAssist()
{
  var link = document.getElementsByTagName("a");
  for (var i = 0; i < link.length; i++) {
    cl = link[i].className;
    if ( cl == "img_assist-link") {
      link[i].href = link[i].href.replace("/load/textarea", "/load/fckeditor");
    }
  }
}
