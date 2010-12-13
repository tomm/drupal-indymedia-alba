<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html lang="<?php print $language->language ?>" xml:lang="<?php print
$language->language ?>" xmlns="http://www.w3.org/1999/xhtml">

<head profile="http://gmpg.org/xfn/11">
<title><?php print $head_title ?></title>
<?php print $head ?>
<?php print $styles ?>
<?php print $scripts ?>
<!--[if lte IE 6]>
<style>
#header { position: static; }
#wrapper { position: static; }
</style>
<![endif]-->
</head>

<body class="<?php print $body_classes ?>">
<div id="wrapper">

<div id="header"<?php print (count($secondary_links) ? ' class="p2"' : ' class="p1"') ?>>

  <div id="branding">
  <?php if ($logo){ ?>
    <a href="<?php print url(); ?>">
    <img src="<?php print check_url($logo) ?>" alt="<?php print $site_name ?>" id="logo" />
    </a>
  <?php } ?>
  </div>

  <?php if($search_box) { ?>
    <!-- IE hack --><div style="position: static;"></div>
    <?php print $search_box ?>
  <?php } ?>

  <?php if ($header): ?>
  <div id="header-additional">
  <?php print $header ?>
  </div>
  <?php endif; ?>

  <div id="menu">
  <?php if ($primary_links){ ?>
    <?php print theme('links', $primary_links, array('class' => 'links primary_menu')) ?>
  <?php } ?>
  <?php if (count($secondary_links)){ ?>
    <?php print theme('links', $secondary_links, array('class' => 'links secondary_menu')) ?>
  <?php } ?>
  </div>
</div>

<!--[if lte IE 6]> <div class="messages warning">
<a href="http://www.mozilla.com/">
Your web browser is old and insecure.
Please upgrade it so that you can take
advantage of the latest features of Indymedia Scotland</a> </div> <![endif]-->

<?php if (0) /*($breadcrumb && !$is_front)*/{ ?>
<div id="breadcrumb"><?php print $breadcrumb ?></div>
<?php } ?>
<?php if ($messages != ""){ ?>
<div id="message"><?php print $messages ?></div>
<?php } ?>
<?php if ($mission != ""){ ?>
<div id="mission"><span><?php print $mission ?></span></div>
<?php } ?>
<?php if ($title != ""){ ?>
<h2 id="title"><?php print $title ?></h2>
<?php } ?>
<?php if ($help != ""){ ?>
<p id="help"><?php print $help ?></p>
<?php } ?>
<?php if ($tabs != ""){ ?>
<?php print $tabs ?>
<?php } ?>
<div class="content">
<?php print $before_content ?>
<?php print $content ?>
<?php print $after_content ?>
</div>

<div id="footer" class="footer">
<?php if ($footer_message){ ?>
  <?php print $footer_message;?>
<?php } ?>
  <p class="credits">Powered by <a href="http://www.drupal.org">Drupal</a>.</p>
</div>

</div>

<?php print $closure;?>
</body>
</html>
