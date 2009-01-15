<div class="block block-<? print $block->module?>" id="block-<?php print $block->module . "-" . $block->delta ?>">
<?php if ($block->subject) { ?>
<h2 class="title"><span><?php print $block->subject ?></span></h2>
<?php } ?>
  <div class="content"><?php print $block->content ?></div>
</div>

