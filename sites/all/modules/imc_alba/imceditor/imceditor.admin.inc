<?php

function imceditor_moderation_operations($status) {
	$ops = array();
	switch ($status) {
		case 'hidden':
			$ops['unhide'] = array(t('Un-Hide'));
			$ops['delete'] = array(t('Delete'));
			break;
		case 'deleted':
			$ops['undelete'] = array(t('Un-delete'));
			break;
		case 'unmoderated':
			$ops['markmoderated'] = array(t('Mark as moderated'));
			$ops['hide'] = array(t('Hide'));
			$ops['delete'] = array(t('Delete'));
			break;
		default:
			$ops['hide'] = array(t('Hide'));
			$ops['delete'] = array(t('Delete'));
			break;
	}
	return $ops;
}

function _imceditor_mod_op_gerund($op) {
	// jesus wept
	switch ($op) {
		case 'hide': return 'hiding';
		case 'unhide': return 'unhiding';
		case 'delete': return 'deleting';
		case 'undelete': return 'undeleting';
		case 'markmoderated': return 'marking as moderated';
		default: return '??BUG??';
	}
}

function imceditor_admin_action_confirm_validate($form, &$form_state) {
	if (!$form_state['values']['reason']) form_set_error('reason');
}
function imceditor_admin_action_confirm_submit($form, &$form_state) {
	if ($form_state['values']['confirm']) {
		switch ($form['#operation']) {
			case 'hide':
				imceditor_hide($form['#nids'], $form_state['values']['reason']);
				drupal_set_message(t('The articles have been hidden.'));
				break;
			case 'unhide':
				imceditor_unhide($form['#nids'], $form_state['values']['reason']);
				drupal_set_message(t('The articles have been un-hidden.'));
				break;
			case 'delete':
				imceditor_delete($form['#nids'], $form_state['values']['reason']);
				drupal_set_message(t('The articles have been deleted.'));
				break;
			case 'undelete':
				imceditor_undelete($form['#nids'], $form_state['values']['reason']);
				drupal_set_message(t('The articles have been un-deleted.'));
				break;
			default:
				drupal_set_message('BUG: Action not yet implemented.');
				break;
		}
		cache_clear_all();
	}
}

function imceditor_admin_action_confirm(&$form_state) {
	$nids = &$form_state['nids'];
	$form = array();

	if (sizeof($nids) == 0) {
		drupal_set_message(t("You did not select any articles."));
		drupal_goto($_GET['q']);
	} else {
		$form['#nids'] = array();
		foreach ($nids as $nid) {
			$form['#nids'][] = $nid;
		}
		$form['#operation'] = $form_state['values']['operation'];
		if ($form['#operation'] == 'markmoderated') {
			imceditor_markmoderated($form['#nids']);
			drupal_set_message(t('The articles have been marked as moderated.'));
			drupal_goto('admin/content/imceditor/unmoderated');
			return;
		}
		$gerund = _imceditor_mod_op_gerund($form['#operation']);
		$form['reason'] = array(
			'#title' => t('Reason for %%gerund_verb%%',
				array('%%gerund_verb%%' => $gerund)),
			'#type' => 'textfield',
			'#required' => TRUE,
		);
		$form = confirm_form($form, t('Specify a reason for %gerund_verb% %num% articles, or cancel this action',
				array('%gerund_verb%' => $gerund, '%action%' => $form['#operation'], '%num%'=>sizeof($nids))),
				'admin/content/imceditor', '', t('Confirm'),
				t('Cancel'));

		$form['#validate'] = array('imceditor_admin_action_confirm_validate');
		$form['#submit'] = array('imceditor_admin_action_confirm_submit');
		return $form;
	}
}

function imceditor_admin_overview_validate($form, $form_state) {
}

function imceditor_admin_overview_submit($form, &$form_state) {
	$form_state['nids'] = array();
	foreach ($form_state['values']['nodes'] as $nid => $state) {
		if ($state) $form_state['nids'][] = $nid;
	}
	$form_state['rebuild'] = TRUE;
}

function imceditor_admin_overview($form_state) {
	
	if (isset($form_state['values']['operation'])) {
		return imceditor_admin_action_confirm($form_state);
	}

	// /admin/content/imceditor/$visibility/$nodetype
	$nodetype = arg(4);
	$visibility = arg(3);

	$form['options'] = array(
		'#type' => 'fieldset',
		'#title' => t('Update options'),
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
	);
	$options = array();
	foreach (imceditor_moderation_operations($visibility) as $key => $value) {
		$options[$key] = $value[0];
	}
	$form['options']['operation'] = array('#type' => 'select', '#options'
			=> $options, '#default_value' => reset($options));
	$form['options']['submit'] = array('#type' => 'submit', '#value' => t('Update'));
	
	$form['header'] = array('#type' => 'value', '#value' => array(
		theme('table_select_header_cell'),
		array('data' => t('Title'), 'field' => 'title'),
		array('data' => t('Author'), 'field' => 'name'),
		array('data' => t('Type'), 'field' => 'type'),
		array('data' => t('Time'), 'field' => 'created', 'sort' => 'desc'),
	));
	$filter = '';
	switch ($visibility) {
		case 'unmoderated': $filter = ' n.moderate = 0 '; break;
		case 'deleted': $filter = sprintf(" imc_mod.status = %d ", IMCEDITOR_NODE_STATUS_DELETED); break;
		case 'hidden': $filter = sprintf(" ( imc_mod.status = %d OR ( n.status = 0 AND imc_mod.status = NULL)) ", IMCEDITOR_NODE_STATUS_HIDDEN); break;
		case 'promoted': $filter = ' n.status = 1 AND n.promote = 1 '; break;
		default: $filter = " n.status = 1 "; break;
	}
	if ($nodetype) $filter .= sprintf(" AND n.type = '%s' ", db_escape_string($nodetype));
			//(SELECT * FROM {node} n LEFT OUTER JOIN {imc_node_moderation} mod ON mod.nid = n.nid) n
	$sql = sprintf('SELECT n.nid, n.type, n.title, n.uid, u.name, n.created, imc_mod.status
			FROM {node} n
			LEFT OUTER JOIN {users} u
			ON u.uid = n.uid
			LEFT OUTER JOIN {imc_node_moderation} imc_mod
			ON imc_mod.nid = n.nid
			WHERE %s ORDER BY n.created DESC', $filter);
	$result = pager_query($sql, 50, 0);
	while ($crud = db_fetch_object($result)) {
		$nodes[$crud->nid] = '';
		$form['title'][$crud->nid] = array('#value' => l($crud->title, 'node/'.$crud->nid));
		$form['type'][$crud->nid] = array('#value' => $crud->type);
		$form['name'][$crud->nid] = array('#value' => l($crud->name, 'user/'.$crud->uid));
		$form['created'][$crud->nid] = array('#value' => format_date($crud->created, 'small'));
	}

	$form['nodes'] = array('#type' => 'checkboxes', '#options' => isset($nodes) ? $nodes : array());
	$form['pager'] = array('#value' => theme('pager', NULL, 50, 0));
	
	
	return $form;
}

function theme_imceditor_admin_overview($form) {
	$output = drupal_render($form['page_title']);
	$output .= drupal_render($form['options']);
	if (isset($form['title']) && is_array($form['title'])) {
		foreach (element_children($form['title']) as $key) {
			$row = array();
			$row[] = drupal_render($form['nodes'][$key]);
			$row[] = drupal_render($form['title'][$key]);
			$row[] = drupal_render($form['name'][$key]);
			$row[] = drupal_render($form['type'][$key]);
			$row[] = drupal_render($form['created'][$key]);
			$rows[] = $row;
		}
	}
	else {
		$rows[] = array(array('data' => t('No content available.'), 'colspan' => '6'));
	}

	$output .= theme('table', $form['header']['#value'], $rows);
	if ($form['pager']['#value']) {
		$output .= drupal_render($form['pager']);
	}
	$output .= drupal_render($form);
	return $output;
}

