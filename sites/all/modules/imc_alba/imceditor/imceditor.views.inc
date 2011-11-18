<?php

function imceditor_views_data() {
	$data = array();
	$data['imc_node_moderation']['table']['group'] = t('IMC node moderation');

	$data['imc_node_moderation']['table']['join'] = array(
		'node' => array(
			'left_field' => 'nid',
			'field' => 'nid'
		),
		'users' => array(
			'left_field' => 'uid',
			'field' => 'uid'
		),
	);

	$data['imc_node_moderation']['nid'] = array(
		'title' => t('Node ID'),
		'relationship' => array(
			'base' => 'node',
			'field' => 'nid',
			'handler' => 'views_handler_relationship',
			'label' => 'Node ID msg1',
		),
		'sort' => array(
			'handler' => 'views_handler_sort',
		),
		'filter' => array(
			'handler' => 'views_handler_filter_numeric',
		),
		'argument' => array(
			'handler' => 'views_handler_argument_numeric',
		),
	);

	$data['imc_node_moderation']['status'] = array(
		'title' => t('Status'),
		'help' => t('Describe different values...'),
		'field' => array(
			'handler' => 'views_handler_field',
			'click sortable' => true,
		),
		'sort' => array(
			'handler' => 'views_handler_sort',
		),
		'filter' => array(
			'handler' => 'views_handler_filter_numeric',
		),
		'argument' => array(
			'handler' => 'views_handler_argument_numeric',
		),
	);

	$data['imc_node_moderation']['comment'] = array(
		'title' => t('Comment'),
		'field' => array(
			'handler' => 'views_handler_field',
			'click sortable' => true,
		),
		'sort' => array(
			'handler' => 'views_handler_sort',
		),
		'filter' => array(
			'handler' => 'views_handler_filter_string',
		),
		'argument' => array(
			'handler' => 'views_handler_argument_string',
		),
	);

	$data['imc_node_moderation']['uid'] = array(
		'title' => t('User ID'),
		'help' => t('The user who applied this moderation'),
		'relationship' => array(
			'base' => 'users',
			'field' => 'uid',
			'handler' => 'views_handler_relationship',
			'label' => 'erm msg2',
		),
	);
	
	$data['imc_node_moderation']['timestamp'] = array(
		'title' => t('Timestamp'),
		'field' => array(
			'handler' => 'views_handler_field',
			'click sortable' => true,
		),
		'sort' => array(
			'handler' => 'views_handler_sort',
		),
		'filter' => array(
			'handler' => 'views_handler_filter_numeric',
		),
		'argument' => array(
			'handler' => 'views_handler_argument_numeric',
		),
	);

	return $data;
}

