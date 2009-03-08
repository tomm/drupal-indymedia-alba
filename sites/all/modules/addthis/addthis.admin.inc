<?php
// $Id: addthis.admin.inc,v 1.1.2.4 2009/02/15 10:02:37 wesku Exp $

/**
 * @file
 * Administration settings for AddThis button integration
 */

/**
 * Administration settings form.
 *
 * @return
 *   The completed form definition.
 */
function addthis_admin_settings() {
  $form = array();

  $form['addthis_general_settings'] = array(
    '#type'  => 'fieldset',
    '#title' => t('General settings'),
  );
  $form['addthis_general_settings']['addthis_username'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Username'),
    '#default_value' => variable_get('addthis_username', 'my-username'),
    '#description'   => t('Your username for <a href="http://www.addthis.com/">addthis.com</a>. Example: my-username'),
  );
  $form['addthis_general_settings']['addthis_display_in_links'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display on node pages'),
    '#default_value' => variable_get('addthis_display_in_links', '0'),
    '#description'   => t('Display an AddThis button always on a node page\'s links section.'),
  );
  $form['addthis_general_settings']['addthis_display_in_teasers'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display in node teasers'),
    '#default_value' => variable_get('addthis_display_in_teasers', '0'),
    '#description'   => t('Display an AddThis button in the node teasers.'),
  );

  $form['addthis_button_settings'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Button image settings'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
  $form['addthis_button_settings']['addthis_image'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Button image url'),
    '#default_value' => variable_get('addthis_image', 'http://s9.addthis.com/button1-share.gif'),
    '#description'   => t('URL for small image in the button. Example: http://s9.addthis.com/button1-share.gif'),
  );
  $form['addthis_button_settings']['addthis_image_width'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Button image width'),
    '#default_value' => variable_get('addthis_image_width', '125'),
    '#description'   => t('Width for button image. Example: 125'),
  );
  $form['addthis_button_settings']['addthis_image_height'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Button image height'),
    '#default_value' => variable_get('addthis_image_height', '16'),
    '#description'   => t('Height for button image. Example: 16'),
  );
  $form['addthis_button_settings']['addthis_image_attributes'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Button image HTML attributes'),
    '#default_value' => variable_get('addthis_image_attributes', 'alt=""'),
    '#description'   => t('Extra HTML attributes for img tag. Example: alt=""'),
  );

  $form['addthis_widget_settings'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Widget settings'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
  $form['addthis_widget_settings']['addthis_dropdown_disabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Disable dropdown'),
    '#default_value' => variable_get('addthis_dropdown_disabled', '0'),
    '#description'   => t('You can disable the dropdown for selecting where to share your link and use a pop-up window instead. This can be useful if the button is for example located next to a flash object.'),
  );
  $form['addthis_widget_settings']['addthis_logo'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Logo url'),
    '#default_value' => variable_get('addthis_logo', 'http://www.addthis.com/images/yourlogo.png'),
    '#description'   => t('The logo to display on the popup window (about 200x50 pixels). The popup window is show when the user selects the \'More\' choice. Example: http://www.your-website.com/img/mylogo.gif'),
  );
  $form['addthis_widget_settings']['addthis_logo_background'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Logo background color'),
    '#default_value' => variable_get('addthis_logo_background', 'EFEFFF'),
    '#description'   => t('The color to use as a background around the logo in the popup. Example: EFEFEF'),
  );
  $form['addthis_widget_settings']['addthis_logo_color'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Logo text color'),
    '#default_value' => variable_get('addthis_logo_color', '666699'),
    '#description'   => t('The color to use for the text next to the logo in the popup. Example: 666666'),
  );
  $form['addthis_widget_settings']['addthis_brand'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Brand text'),
    '#default_value' => variable_get('addthis_brand', 'Your Site'),
    '#description'   => t('The brand name to display in the drop-down (top right). Example: My Website'),
  );
  $form['addthis_widget_settings']['addthis_options'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Options'),
    '#default_value' => variable_get('addthis_options', 'favorites, email, digg, delicious, myspace, facebook, google, live, more'),
    '#description'   => t('A comma-separated ordered list of options to include in the drop-down. Example: favorites, email, digg, delicious, more<br/>Currently supported options:<br/>
	aim, aolfav, ask, backflip, ballhype, blinklist, blogmarks, bluedot, buzz, delicious, digg, diigo, email, facebook, favorites, fark, feedmelinks, friendfeed, furl, google, kaboodle, kirtsy, linkedin, live, magnolia, misterwong, mixx, multiply, myweb, myspace, netvous, newsvine, pownce, propeller, reddit, segnalo, sharedstuff, slashdot, spurl, stumbleupon, stylehive, tailrank, technorati, thisnext, twitter, yardbarker, yahoobkm, more<br />
        The most up-to-date list can be found <a href="!url">here</a>.', array('!url' => url('http://addthis.com/customization.php'))),
  );
  $form['addthis_widget_settings']['addthis_offset_top'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Offset top'),
    '#default_value' => variable_get('addthis_offset_top', '2'),
    '#description'   => t('Vertical offset for the drop-down window (in pixels). Example: 2'),
  );
  $form['addthis_widget_settings']['addthis_offset_left'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Offset top'),
    '#default_value' => variable_get('addthis_offset_left', '2'),
    '#description'   => t('Horizontal offset for the drop-down window (in pixels). Example: 2'),
  );
  return system_settings_form($form);
}