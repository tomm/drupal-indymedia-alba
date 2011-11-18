<?php if ($page) { ?>
	<?php if ($terms) { ?>
		<div class="taxonomy">Forum: <?php print $terms ?></div>
	<?php } ?>
<?php } ?>
<?php /* this is a total fucking hack. in the forums when it is not
 the first page we don't want the original article shown, to give
 a more forummmy flow to things. hack hack hack hack TODO BAD BAD TODO WARNING
 */
 	if (isset($_GET['page']) && (intval($_GET['page']) != 0)) return
?>

<div id="node-<?php print $nid ?>" class="comment forum-comment firstpost">
<div class="node teaser nodecomment">
<?php print $picture ?>
<?php if ($page) { ?>
	<h2 class="title"><?php print $title ?></h2>
	<?php if ($submitted) { ?>
		<div class="submitted"><?php print $submitted ?></div>
	<?php } ?>
<?php } else { ?>
	<h2 class="title"><a href="<?php print $node_url ?>"><?php print $title ?></a></h2>
	<div class="submitted"><?php print $submitted ?></div>
<?php } ?>
<div class="content"><?php print $content ?></div>
<?php if ($links && $page == 0){ ?>
	<div class="links">&raquo; <?php print $links ?></div>
<?php } ?>
<?php if ($page) { ?>
	<?php if ($links){ ?>
		<div class="links">&raquo; <?php print $links ?></div>
	<?php } ?>
<?php } ?>
</div>
</div>
