<div class="ok">
	<?php
	$u = user_load(array('uid' => $prop['uid']));
	$username = ($u ? $u->name : "{unknown}");
	print sprintf(t('This article has been proposed by %s as a feature (it needs %d votes). '), $username, $prop['votes_needed']);
	if ($prop['comment']) {
		print t('Comment: ').$prop['comment'];
	}
	?>
</div>
<?php foreach ($blocks as $block) { ?>
	<div class="warning">
		<?php
		$u = user_load(array('uid' => $block['uid']));
		$username = ($u ? $u->name : "{unknown}");
		print sprintf(t('Feature proposal blocked by %s, stating: %s'), $username, $block['comment']);
		/*if ($prop['comment']) {
			print t('with comment: ').$prop['comment'];
		}*/
		?>
	</div>
<?php } ?>
