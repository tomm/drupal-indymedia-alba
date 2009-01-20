<?php

function edemocracy_is_user_a_candidate($nid, $uid)
{
	return db_result(db_query("SELECT COUNT(*) FROM {edemocracy_poll_option} " .
		"WHERE nid=%d AND uid=%d", $nid, $uid));
}

function edemocracy_add_user_candidate($nid, $uid, $description)
{
	if (db_result(db_query("SELECT candidatetype FROM {edemocracy_poll} WHERE nid=%d", $nid))
		!= POLL_CANDIDATETYPE_USER) {
		throw new Exception('Cannot add user candidate to this poll type');
	}
	db_query("INSERT INTO {edemocracy_poll_option} (nid, uid, description) VALUES (%d, %d, '%s')",
			$nid, $uid, $description);
}

function edemocracy_add_poll_proposition($nid, $name, $description)
{
	if (db_result(db_query("SELECT candidatetype FROM {edemocracy_poll} WHERE nid=%d", $nid))
		!= POLL_CANDIDATETYPE_PROPOSITION) {
		throw new Exception('Cannot add propositions to this poll type');
	}
	db_query("INSERT INTO {edemocracy_poll_option} (nid, uid, name, description) VALUES (%d, 0, '%s', '%s')",
			$nid, $name, $description);
}

function edemocracy_has_user_voted($nid, $uid) {
	return db_result(db_query("SELECT COUNT(*) FROM {edemocracy_poll_vote} " .
		"WHERE nid=%d AND uid=%d", $nid, $uid));
}

/*
 * Votes should be assoc array of optionid => rating
 */
function edemocracy_register_votes($nid, $uid, $votes) {
	if (edemocracy_has_user_voted($nid, $uid)) {
		throw new Exception("User has already voted.");
	}
	foreach ($votes as $optionid => $rating) {
		$exists = db_result(db_query("SELECT COUNT(*) FROM {edemocracy_poll_vote} " .
			"WHERE nid=%d AND uid=%d AND optionid=%d", $nid, $uid, $optionid));
		if ($exists) {
			throw new Exception("Candidate $optionid does not exist for poll $nid.");
		}
		db_query("INSERT INTO {edemocracy_poll_vote} (nid, uid, optionid, vote) VALUES (%d, %d, %d, %d)",
				$nid, $uid, $optionid, $rating);
	}
}

function edemocracy_count_votes($nid) {
	$r = db_query("SELECT optionid FROM {edemocracy_poll_option} WHERE nid=%d", $nid);
	while ($a = db_fetch_array($r)) {
		$votes = db_result(db_query("SELECT SUM(vote) FROM {edemocracy_poll_vote} ".
			"WHERE nid=%d AND optionid=%d", $nid, $a['optionid']));
		db_query("UPDATE {edemocracy_poll_option} SET finalvotes=%d WHERE nid=%d AND optionid=%d",
				$votes, $nid, $a['optionid']);
	}
	$num_winners = db_result(db_query("SELECT numberofwinners FROM {edemocracy_poll} WHERE nid=%d", $nid));
	db_query("UPDATE {edemocracy_poll_option} SET winner=2 WHERE nid=%d ORDER BY finalvotes DESC LIMIT %d", $nid, $num_winners);
	$min_winning_score = db_result(db_query("SELECT MIN(finalvotes) FROM {edemocracy_poll_option} WHERE winner=2 AND nid=%d", $nid));
	drupal_set_message("Lowest winner ".$min_winning_score);
	if (db_result(db_query("SELECT COUNT(*) FROM {edemocracy_poll_option} WHERE winner=0 AND nid=%d and finalvotes=%d", $nid, $min_winning_score))) {
		// tie. mark those with this num as tied
		db_query("UPDATE {edemocracy_poll_option} SET winner=1 WHERE nid=%d AND finalvotes=%d", $nid, $min_winning_score);
	}
}

/*
 * returns array of optionid => name
 */
function edemocracy_get_winners(&$node) {
	$winners = array();
	$r = db_query("SELECT optionid,name,uid FROM {edemocracy_poll_option} WHERE nid=%d AND winner=2 ORDER BY finalvotes DESC", $node->nid);
	while ($a = db_fetch_array($r)) {
		if ($node->edemocracy_poll_candidatetype == POLL_CANDIDATETYPE_USER) {
			$u = user_load($a['uid']);
			$winners[$a['optionid']] = $u->name;
		} else {
			$winners[$a['optionid']] = $a['name'];
		}
	}
	return $winners;
}
