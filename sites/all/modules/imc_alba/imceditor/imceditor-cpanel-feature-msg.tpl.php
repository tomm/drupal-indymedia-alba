<?php
if ($node->promote) {
	print t("This article is already a feature.");
} else if (!$prop || !$prop['active']) {
	// can propose a feature
} else if ($prop['user_voted']) {
	if ($prop['user_voted_for']) {
		print t("You have cast your vote in favour");
	} else if ($prop['user_voted_block']) {
		print t("You have blocked this feature proposal");
	}
} else {
	$u = user_load(array('uid' => $prop['uid']));
	$username = ($u ? $u->name : "{unknown}");
	print sprintf(t('This article has been proposed by %s as a feature (it needs %d votes to be promoted to the front page). '), $username, $prop['votes_needed']);
}
?>
