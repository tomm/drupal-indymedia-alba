<?php
/**
 * Device testing page
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<head>
  <title>Device Tester</title>
  <meta name = "viewport" content = "user-scalable=no, width=device-width, initial-scale=1.0, maximum-scale=1.0" /> 
</head>
<body onload="test()">
<style type="text/css">
.gradient {
   background-image: -webkit-gradient(
    linear,
    left bottom,
    left top,
    color-stop(0.19, rgb(181,181,181)),
    color-stop(0.84, rgb(255,148,148))
  );
  background-image: -moz-linear-gradient(
    center bottom,
    rgb(181,181,181) 19%,
    rgb(255,148,148) 84%
  );
  padding: 10px;
  margin: 10px;
}

.border {
  padding:10px;
  margin: 10px;
  border: 1px solid black;
  -moz-border-radius: 15px;
  -webkit-border-radius: 15px;
}


@font-face {
	font-family:"GothamRoundedBold";
	src: url('/sites/all/themes/tylenol_iphone/css/gothamroundedbold-webfont.eot');
	src: url('/sites/all/themes/tylenol_iphone/css/gothamroundedbold-webfont.woff') format('woff'), url('/sites/all/themes/tylenol_iphone/css/gothamroundedbold-webfont.ttf') format('truetype'), url('/sites/all/themes/tylenol_iphone/css/gothamroundedbold-webfont.svg#webfonti7pUmUrd') format('svg');
}

.fontface {
  font-family:"GothamRoundedBold"; 
}

#js-test {
  margin:10px;
}
</style>
<?php drupal_set_title("device tester"); ?>
<h1>Device Tester</h1>
<p>This page allows you to test device capabilities and report any errors.
The feedback form will automatically collect device characteristics for processing.</p>

<h2>Device Info</h2>
<dl>
  <dt>Phone brand</dt>
  <dd><?php print $device->getCapability('brand_name');?></dd>
  <dt>Model</dt>
  <dd><?php print $device->getCapability('model_name');?></dd>
  <dt>OS</dt>
  <dd><?php print $device->getCapability('device_os'); ?></dd>
  <dt>Browser</dt>
  <dd><?php print $device->getCapability('mobile_browser'); ?></dd>
</dl>

<h2>Device capabilities</h2>
<strong>Device Group: </strong> 

<h3>Screen size</h3>
<strong>Screen width: </strong> <?php print $device->getCapability('resolution_width'); ?>px
<br />
<strong>Screen height: </strong><?php print $device->getCapability('resolution_height'); ?>px

<h3>CSS3 Gradients</h3>
<strong>Support (WURFL): </strong> <?php print $device->getCapability('css_gradient'); ?>
<div class="gradient">
Gradient tester: check if the background has a gradient.
</div>  

<h3>CSS3 Borders</h3>
<strong>Support (WURFL): </strong> <?php print $device->getCapability('css_rounded_corners'); ?>
<div class="border">
Border tester: check if the borders are rounded
</div>
<h3>Javascript support</h3>
<strong> Support (WURFL): </strong><?php print $device->getCapability('ajax_support_javascript'); ?>

<div  id="js-test">If you see this, js is not working on this device</div>
<script>
function test() {
  document.getElementById('js-test').innerHTML = "Javascript is working";
}
</script>

<h3>Font Face</h3>
<strong>Support (WURFL):</strong> <label class="fontface">Check if the font style is different</label>

<h3>Media Queries</h3>
<strong>Support (WURFL):</strong> 
<h2>Feedback Form:</h2> 
<?php print $form; ?>
</body>
</html>
<?php exit(); ?>