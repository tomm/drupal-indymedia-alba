<?php

function edemocracy_vote_form_submit($form_id, $form_state) {
	global $user;
	$nid = $form_state['values']['nid'];
	
	if (edemocracy_has_user_voted($nid, $user->uid)) {
		drupal_set_message('You have already voted in this poll.');
		drupal_goto('/node/' . $nid);
		return;
	}

	$yourvotes = array();
	$r = db_query("SELECT * FROM {edemocracy_poll_option} WHERE nid=%d", $nid);
	while ($a = db_fetch_array($r)) {
		$yourvotes[$a['optionid']] = intval($form_state['values']['vote-' . $a['optionid']]);
	}
	edemocracy_register_votes($nid, $user->uid, $yourvotes);
	drupal_goto("/node/$nid");
}

function edemocracy_vote_form($form_state, $nid)
{
	global $user;
	if (edemocracy_has_user_voted($nid, $user->uid)) {
		drupal_set_message('You have already voted in this poll.');
		drupal_goto('/node/' . intval($nid));
		return;
	}

	$node = node_load($nid);

	
	$form = array();
	$form['nid'] = array('#value' => $node->nid, '#type' => 'hidden');

	if ($node->edemocracy_poll_candidatetype == POLL_CANDIDATETYPE_USER) {
		$form['header'] = array('#type' => 'value', '#value' => array(
			array('data' => t('Your vote')),
			array('data' => t('Name')),
			array('data' => t('Statement')),
		));

		$r = db_query("SELECT * FROM {edemocracy_poll_option} WHERE nid=%d", $nid);
		while ($a = db_fetch_array($r)) {
			$u = user_load($a['uid']);
			$oid = 'vote-' . $a['optionid'];
			$form['vote'][$oid] = array('#type' => 'select',
				'#default_value' => 0,
				'#options' => array(0=>'',1=>'1',2=>'2',3=>'3',4=>'4',5=>'5',
						6=>'6',7=>'7',8=>'8',9=>'9',10=>'10'),
			);
			$form['name'][$oid] = array('#value' => $u->name);
			$form['description'][$oid] = array('#value' => $a['description']);
		}
	} else {
		$form['header'] = array('#type' => 'value', '#value' => array(
			array('data' => t('Your vote')),
			array('data' => t('Name')),
			array('data' => t('Description')),
		));

		$r = db_query("SELECT * FROM {edemocracy_poll_option} WHERE nid=%d", $nid);
		while ($a = db_fetch_array($r)) {
			$u = user_load($a['uid']);
			$oid = 'vote-' . $a['optionid'];
			$form['vote'][$oid] = array('#type' => 'select',
				'#default_value' => 0,
				'#options' => array(0=>'',1=>'1',2=>'2',3=>'3',4=>'4',5=>'5',
						6=>'6',7=>'7',8=>'8',9=>'9',10=>'10'),
			);
			$form['name'][$oid] = array('#value' => $a['name']);
			$form['description'][$oid] = array('#value' => $a['description']);
		}
	}
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Cast your vote',
		'#weight' => 10,
	);
	$form['node_teaser'] = array('#value' => node_view($node, true), '#weight' => -10);
	return $form;

}

function theme_edemocracy_vote_form($form) {
	$output = drupal_render($form['node_teaser']);
	if (isset($form['name']) && is_array($form['name'])) {
		foreach (element_children($form['name']) as $key) {
			$row = array();
			$row[] = drupal_render($form['vote'][$key]);
			$row[] = drupal_render($form['name'][$key]);
			$row[] = drupal_render($form['description'][$key]);
			$rows[] = $row;
		} 
	} else {
		$rows[] = array(array('data' => t('No candidates available.'), 'colspan' => '2'));
	}
	$output .= theme('table', $form['header']['#value'], $rows);
	$output .= drupal_render($form);
	return $output;
}

