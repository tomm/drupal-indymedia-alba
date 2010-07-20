<div style="clear:both;" id="node-<?php print $nid ?>" class="node<?php print ($sticky && $page == 0) ? " sticky" : ""; ?><?php print ($page == 0) ? " teaser" : " "; ?><?php print ' ' . ($node->type); ?>">
<?php print $picture ?>
<?php if ($page) { ?>
	<?php if ($submitted) { ?>
		<div class="submitted"><?php print $submitted ?></div>
	<?php } ?>
<?php } else { ?>
	<h2 class="title"><a href="<?php print $node_url ?>"><?php print $title ?></a></h2>
<?php } ?>
<?php if ($page) { ?>
	<div class="content"><?php print $content ?></div>
	<div class="meta">
	<?php if ($links){ ?>
		<div class="links">&raquo; <?php print $links ?></div>
	<?php } ?>
	</div>
<?php } else { ?>
	<div class="content">
		<?php if (reset($node->files)->filepath) { ?>
			<div class="node-teaser-img"><a href="<?php print $node_url ?>"><?php print theme('imagecache', variable_get('imcviews_teaser_imagecache_preset',''), reset($node->files)->filepath); ?></a></div>
		<? } ?>
		<?php print check_markup($content, variable_get('imcviews_teaser_filter',0), FALSE); ?>
	</div>
<?php } ?>
</div>
