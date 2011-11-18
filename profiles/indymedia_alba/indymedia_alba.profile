<?php
// $Id: indymedia_alba.profile,v 1.17 2009/01/15 11:28:50 tomm Exp $

/**
 * Return an array of the modules to be enabled when this profile is installed.
 *
 * @return
 *   An array of modules to enable.
 */
function indymedia_alba_profile_modules() {
  return array('color', 'help', 'menu', 'taxonomy', 'dblog', 'aggregator',
  		'translation', 'search', 'upload', 'path',
		
		'imc_alba', 'nodecomment', 'article', 'indymedia_cities', 'dadamigrate', 'imceditor',
		'imcviews', 'nodextradata',
		
		'fckeditor');
}

/**
 * Return a description of the profile for the initial installation screen.
 *
 * @return
 *   An array with keys 'name' and 'description' describing this profile,
 *   and optional 'language' to override the language selection for
 *   language-specific profiles.
 */
function indymedia_alba_profile_details() {
  return array(
    'name' => 'Indymedia Alba',
    'description' => 'A basic Indymedia site setup.'
  );
}

/**
 * Return a list of tasks that this profile supports.
 *
 * @return
 *   A keyed array of tasks the profile will perform during
 *   the final stage. The keys of the array will be used internally,
 *   while the values will be displayed to the user in the installer
 *   task list.
 */
function indymedia_alba_profile_task_list() {
}

function _indymedia_alba_add_page($title, $teaser, $body) {
	$node = (object)array(
		'type' => 'page', 'title' => $title,
		'status' => 1, 'created' => time(), 'changed' => time(), 'promote' => 0,
		'uid' => 1, 'comment' => 0, 'format' => 2, // unfiltered HTML
	);
	$node->teaser = $teaser;
	$node->body = ($body ? $body : $teaser);
	node_save($node);
	path_set_alias('node/'.$node->nid, strtolower($title));
	return $node->nid;
}

function _indymedia_alba_add_infodocs() {
	$pages = array(t('About us') => "The Independent Media Center is a network of collectively run media outlets for the creation of radical, accurate, and passionate tellings of the truth. We work out of a love and inspiration for people who continue to work for a better world, despite corporate media's distortions and unwillingness to cover the efforts to free humanity.",
			t('Contact information') => 'to do',
			t('Editorial Policy') => 'to do',
			t('Getting Involved') => 'to do',
			t('Meetings') => 'to do',
			t('Donations') => 'to do');

	$nids = array();
	foreach ($pages as $title => $body) {
		$nids[$title] = _indymedia_alba_add_page($title, $body, '');
	}
	$teaser = t('You should put your mission statement, links to editorial guidelines, contact details, etc here.');
	$body = $teaser;
	foreach ($nids as $title => $nid) {
		$body .= "\n\n".l($title, 'node/'.$nid);
	}
	$info_nid = _indymedia_alba_add_page(t('Information'), $teaser, $body);

	$link = array('menu_name' => 'primary-links', 'weight' => 3, 'link_path' => 'node/'.$info_nid, 'link_title' => t('Information'));
	menu_link_save(&$link);
}

function _indymedia_alba_stuff_terms(&$terms, $vid) {
	$tids = array();
	foreach ($terms as $name => $desc) {
		$term = array(
			'name' => $name, 'vid' => $vid,
			'description' => $desc,
		);
		taxonomy_save_term($term);
		$tids[] = $term['tid'];
	}
	return $tids;
}

/**
 * Perform any final installation tasks for this profile.
 *
 * The installer goes through the profile-select -> locale-select
 * -> requirements -> database -> profile-install-batch
 * -> locale-initial-batch -> configure -> locale-remaining-batch
 * -> finished -> done tasks, in this order, if you don't implement
 * this function in your profile.
 *
 * Important: Any temporary variables should be removed using
 * variable_del() before advancing to the 'profile-finished' phase.
 *
 * @param $task
 *   The current $task of the install system. When hook_profile_tasks()
 *   is first called, this is 'profile'.
 * @param $url
 *   Complete URL to be used for a link or form action on a custom page,
 *   if providing any, to allow the user to proceed with the installation.
 *
 * @return
 *   An optional HTML string to display to the user. Only used if you
 *   modify the $task, otherwise discarded.
 */
function indymedia_alba_profile_tasks(&$task, $url) {
	// Insert default user-defined node types into the database. For a complete
	// list of available node type attributes, refer to the node type API
	// documentation at: http://api.drupal.org/api/HEAD/function/hook_node_info.
	$types = array(
		array(
			'type' => 'page',
			'name' => st('Page'),
			'module' => 'node',
			'description' => st("A <em>page</em>, similar in form to an <em>article</em>, is a simple method for creating and displaying information that rarely changes, such as an \"About us\" section of a website. By default, a <em>page</em> entry does not allow visitor comments and is not featured on the site's initial home page."),
			'custom' => TRUE,
			'modified' => TRUE,
			'locked' => FALSE,
			'help' => '',
			'min_word_count' => '',
		),
		/*array(
			'type' => 'story',
			'name' => st('Story'),
			'module' => 'node',
			'description' => st("A <em>story</em>, similar in form to a <em>page</em>, is ideal for creating and displaying content that informs or engages website visitors. Press releases, site announcements, and informal blog-like entries may all be created with a <em>story</em> entry. By default, a <em>story</em> entry is automatically featured on the site's initial home page, and provides the ability to post comments."),
			'custom' => TRUE,
			'modified' => TRUE,
			'locked' => FALSE,
			'help' => '',
			'min_word_count' => '',
		),*/
	);

	foreach ($types as $type) {
		$type = (object) _node_type_set_defaults($type);
		node_type_save($type);
	}

	// Default page to not be promoted and have comments disabled.
	variable_set('node_options_page', array('status', 'revision'));
	variable_set('comment_page', COMMENT_NODE_DISABLED);

	// if the indymedia_alba theme exists then use it
	$theme = "garland";
	if (db_result(db_query("SELECT COUNT(*) FROM {system} WHERE type = 'theme' AND name = 'indymedia_alba'"))) {
		$theme_settings = variable_get('theme_settings', array());
		$theme_settings['toggle_node_info_page'] = FALSE;
		variable_set('theme_settings', $theme_settings);
		variable_set('theme_indymedia_alba_settings', array('toggle_name' => 0));

		system_theme_data();
		system_initialize_theme_blocks('indymedia_alba');
		db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' and name = 'indymedia_alba'");
		variable_set('theme_default', 'indymedia_alba');
		$theme = 'indymedia_alba';
	}
	db_query("INSERT INTO {blocks} (module, delta, theme, status, weight, region) VALUES ('imcviews','0','$theme',1,10,'right')");
	db_query("INSERT INTO {blocks} (module, delta, theme, status, weight, region) VALUES ('imcviews','1','$theme',1,11,'right')");
	db_query("INSERT INTO {blocks} (module, delta, theme, status, weight, region) VALUES ('indymedia_cities','en','$theme',1,10,'left')");
	db_query("INSERT INTO {blocks} (module, delta, theme, status, weight, region) VALUES ('imceditor','0','$theme',1,9,'left')");
	// article comments order age ascending
	variable_set('comment_default_order_article', '2');
	variable_set('comment_hidden_viewable_article', '1');
	variable_set('comment_anonymous_article', '1');

	// menu_link_save's behaviour is very odd.
	$model_link = array('options' => array("attributes" => array('title'=>'')));
	$link = $model_link + array('plid'=>0,'mlid'=>0, 'weight' => 1,
			'link_path' => 'admin/content/imceditor/unmoderated', 'link_title' => t('Moderate Content'));
	menu_link_save($link);
	$link = $model_link + array('menu_name' => 'primary-links', 'weight' => 0, 'link_path' => '<front>', 'link_title' => t('Front Page'));
	menu_link_save($link);
	$link = $model_link + array('menu_name' => 'primary-links', 'weight' => 1, 'link_path' => 'newswire', 'link_title' => t('Newswire'));
	menu_link_save($link);
	$link = $model_link + array('menu_name' => 'primary-links', 'weight' => 2, 'link_path' => 'node/add/article', 'link_title' => t('Publish an article'));
	menu_link_save($link);
	$link = $model_link + array('menu_name' => 'primary-links', 'weight' => 3, 'link_path' => 'node/add/event', 'link_title' => t('Announce an event'));
	menu_link_save($link);
	// Update the menu router information.
	menu_rebuild();

	_indymedia_alba_add_infodocs();

	db_query("INSERT INTO {role} VALUES (3,'editor')");
	db_query("INSERT INTO {role} VALUES (4,'admin')");

	db_query("DELETE FROM {permission}");
	db_query("INSERT INTO {permission} VALUES (3,1,'create article content, access fckeditor, allow fckeditor file uploads, access content, create event content, view revisions, create nodecomment content, search content, use advanced search, upload files, view uploaded files, access user profiles',0)");
	db_query("INSERT INTO {permission} VALUES (4,2,'create article content, edit own article content, access fckeditor, allow fckeditor file uploads, access content, create event content, edit own event content, view revisions, create nodecomment content, edit own nodecomment content, search content, use advanced search, translate content, upload files, view uploaded files, access user profiles, change own username',0)");
	db_query("INSERT INTO {permission} VALUES (5,3,'create article content, edit any article content, edit own article content, access fckeditor, allow fckeditor file uploads, moderate content, access content, create event content, create page content, edit any event content, edit any page content, edit own event content, edit own page content, revert revisions, view revisions, create nodecomment content, edit own nodecomment content, search content, use advanced search, translate content, upload files, view uploaded files, access user profiles, change own username',0)");
	db_query("INSERT INTO {permission} VALUES (6,4,'create article content, edit any article content, edit own article content, administer blocks, use PHP for block visibility, access fckeditor, administer fckeditor, allow fckeditor file uploads, administer filters, administer imceditor, moderate content, administer languages, translate interface, administer menu, administer imcviews, access content, administer content types, create event content, create page content, delete any page content, delete own page content, delete revisions, edit any event content, edit any page content, edit own event content, edit own page content, revert revisions, view revisions, create nodecomment content, edit own nodecomment content, administer search, search content, use advanced search, access administration pages, access site reports, administer actions, administer files, administer site configuration, select different theme, administer taxonomy, translate content, upload files, view uploaded files, access user profiles, administer permissions, administer users, change own username',0)");
	
	variable_set('upload_uploadsize_default','2');
	variable_set('upload_usersize_default','10000');
	variable_set('upload_uploadsize_1','2');
	variable_set('upload_usersize_1','10000');
	variable_set('upload_uploadsize_2','2');
	variable_set('upload_usersize_2','10000');
	variable_set('upload_uploadsize_4','2');
	variable_set('upload_usersize_4','10000');
	variable_set('upload_uploadsize_3','2');
	variable_set('upload_usersize_3','10000');

	// filters
	variable_set('allowed_html_1', '<img> <br> <p> <div> <span> <b> <i> <a> <em> <strong> <cite> <code> <ul> <ol> <li> <dl> <dt> <dd>');

	$vocab = array(
                'nodes' => array('article' => 1),
                'name' => 'Category',
                'relations' => 1,
                'hierarchy' => 1,
                'multiple' => 1,
                'required' => 1,
                'tags' => 0,
        );
        taxonomy_save_vocabulary($vocab);
	
	$terms = array('Housing' => "Articles and information about housing issues, tenants' rights, homelessness, etc.",
		'Protest Activity' => "News and media related to protests in the world.",
		'Globalization' => "Anti-globalization activities have taken the forefront in recent years. The IndyMedia sites were originally conceived to promote coverage of anti-globalization protests, and in this category, we offer our perspectives on globalization in words and images.",
		'Environment' => "Environment and climate-related news, activities and ideas in support of a greener world.",
		'Workplace Struggle' => "Strike, struggle for workers' rights and labor issues.",
		'Miscellaneous' => "Catch-all for topics that don't fit into other categories.",
		'Gender' => "Women, Men and feminism",
		'Sexuality' => "Identity? Lesbian, Gay, Bi, Trans, Straight",
		'Animal Rights' => "Information concerning animal rights movements, actions, and protests.",
		'Education' => "News and activities surrounding education.",
		'Civil & Human Rights' => "News and activities concerning loss of civil liberties, human rights violations, Constitutional violations, etc.",
		'Drugs' => "News on the War on Drugs, marijuana legalization, pharmaceutical companies.",
		'Elections & Legislation' => "News on election results, campaigns, pending legislation, the democratic process.",
		'Media' => "Media activism, media critiques.",
		'Repression' => "Police brutality, corruption, use of deadly force, legal, prison activism, information on political prisoners.",
		'Crime & Police' => "Information related to crime, police brutality, corruption, use of deadly force.",
		'Urban Development' => "Issues of urban development, gentrification, renewal policies, etc.",
		'Peace' => "Peace/Anti-war activity",
	);
	_indymedia_alba_stuff_terms(&$terms, $vocab['vid']);

	$vocab = array(
                'nodes' => array('article' => 1),
                'name' => 'Section',
                'relations' => 1,
                'hierarchy' => 1,
                'multiple' => 0,
                'required' => 1,
                'tags' => 0,
        );
        taxonomy_save_vocabulary($vocab);

	$terms = array('News' => '',
	               'Commentary' => '',
		       'Announcements' => '',
		       'Reviews' => '',
		       'Interviews' => '',
		       'Other Press' => 'Links to interesting articles by other media outlets.');
	$tids = _indymedia_alba_stuff_terms(&$terms, $vocab['vid']);
	
	db_query("INSERT INTO {menu_custom} VALUES ('menu-section','Section','')");
	db_query("INSERT INTO {blocks} (module, delta, theme, status, weight, region, pages, cache) VALUES ('menu','menu-section','indymedia_alba',1,8,'left','',-1)");

	$i=0;
	foreach ($terms as $name => $poo) {
		$link = array('menu_name' => 'menu-section', 'weight' => $i+10, 'link_path' => 'taxonomy/term/'.$tids[$i], 'link_title' => $name);
		menu_link_save(&$link);
		$i++;
	}

	// to help the dadamigrate script
	variable_set('dada_section_vid', $vocab['vid']);
	variable_set('dada_section_News', $tids[0]);
	variable_set('dada_section_Commentary', $tids[1]);
	variable_set('dada_section_Announcement', $tids[2]);
	variable_set('dada_section_Review', $tids[3]);
	variable_set('dada_section_Interview', $tids[4]);
	variable_set('dada_section_Otherpress', $tids[5]);
	
	variable_set('date_format_medium',"D, d/m/Y - H:i");
	variable_set('date_format_short_custom',"m/d/Y - H:i");
	variable_set('date_format_short',"d/m/Y - H:i");
	variable_set('configurable_timezones', 0);
	variable_set('date_format_long',"l, j F, Y - H:i");

	variable_set('site_frontpage', 'front');
	// allow users to be created without email verification
	variable_set('user_email_verification', 0);

	// allow editor moderation of some node types
	variable_set('imceditor_moderate_type_article', 1);
	variable_set('imceditor_moderate_type_nodecomment', 1);
	
	// front page categories
	$cats = array();
	$cats[] = array('tid' => 21, 'color' => 'darkcyan', 'style' => 'summaries', 'num' => 6, 'title' => '', 'weight'=>-1);
	$cats[] = array('tid' => 19, 'color' => '#606', 'style' => 'titles', 'num' => 1, 'title' => 'Other recent features', 'weight'=>0);
	$cats[] = array('tid' => 20, 'color' => '#660', 'style' => 'summaries', 'num' => 6, 'title' => '', 'weight'=>1);
	$cats[] = array('tid' => 23, 'color' => 'green', 'style' => 'summaries', 'num' => 6, 'title' => '', 'weight'=>2);
	$cats[] = array('tid' => 22, 'color' => 'darkred', 'style' => 'summaries', 'num' => 6, 'title' => '', 'weight'=>3);
	variable_set('imcviews_front_sections', $cats);

	// enable author field for comments and articles
	variable_set('nodextradata_author_article', 1);
	variable_set('nodextradata_author_nodecomment', 1);
}

/**
 * Implementation of hook_form_alter().
 *
 * Allows the profile to alter the site-configuration form. This is
 * called through custom invocation, so $form_state is not populated.
 */
function indymedia_alba_form_alter(&$form, $form_state, $form_id) {
  if ($form_id == 'install_configure') {
    // Set default for site name field.
    $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
  }
}
