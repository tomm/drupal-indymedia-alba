<div id="node-<?php print $nid ?>" class="node<?php print ($sticky && $page == 0) ? " sticky" : ""; ?><?php print ($page == 0) ? " teaser" : " "; ?><?php print ' ' . ($node->type); ?>">
<?php print $picture ?>
<?php if ($page) { ?>
	<?php if ($terms) { ?>
		<div class="taxonomy"><?php print $terms ?></div>
	<?php } ?>
	<?php if ($submitted) { ?>
		<div class="submitted"><?php print $submitted ?></div>
	<?php } ?>
<?php } else { ?>
	<h2 class="title"><?php print $title ?></h2>
	<div class="submitted"><?php print $submitted ?></div>
<?php } ?>
<div class="content"><?php print $content ?></div>
<?php if ($links && $page == 0){ ?>
	<div class="links">&raquo; <?php print $links ?></div>
<?php } ?>
<?php if ($page) { ?>
	<div class="meta">
	<?php if ($links){ ?>
		<div class="links">&raquo; <?php print $links ?></div>
	<?php } ?>
	</div>
<?php } ?>
</div>
