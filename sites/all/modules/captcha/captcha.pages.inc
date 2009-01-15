<?php
// $Id: captcha.pages.inc,v 1.7 2009/01/03 22:14:45 soxofaan Exp $

/**
 * Form alter phase of form processing for untrusted users.
 */
function _captcha_form_alter_untrusted_user(&$form, $form_state, $form_id) {

  // Get CAPTCHA type and module for given form_id. Return if no CAPTCHA was set.
  $captcha_point = db_fetch_object(db_query("SELECT module, type FROM {captcha_points} WHERE form_id = '%s'", $form_id));
  if (!$captcha_point || !$captcha_point->type) {
    return;
  }

    // Prevent caching of the page with this CAPTCHA enabled form.
  // This needs to be done even if the CAPTCHA will be skipped later:
  // other untrusted users should not get a cached page when
  // the current untrusted user can skip the current CAPTCHA.
  global $conf;
  $conf['cache'] = FALSE;

  // Retrieve or generate CAPTCHA session ID.
  if (isset($form_state['post']['form_id'])
  && $form_state['post']['form_id'] == $form_id
  && isset($form_state['post']['captcha_sid'])) {
    $captcha_sid = $form_state['post']['captcha_sid'];
  } else {
    // Generate new CAPTCHA session.
    $captcha_sid = _captcha_generate_captcha_session(CAPTCHA_STATUS_UNSOLVED);
  }

  if (_captcha_required_for_user($captcha_sid, $form_id)) {

    // Generate a CAPTCHA and its solution (note that the CAPTCHA session ID
    // is given as thrid argument.
    $captcha = module_invoke($captcha_point->module, 'captcha', 'generate', $captcha_point->type, $captcha_sid);
    if (!$captcha) {
      // The selected module returned nothing, maybe it is disabled or it's wrong, we should watchdog that and then quit.
      watchdog('CAPTCHA',
        'CAPTCHA problem: hook_captcha() of module %module returned nothing when trying to retrieve challenge type %type for form %form_id.',
        array('%type' => $captcha_point->type, '%module' => $captcha_point->module, '%form_id' => $form_id),
        WATCHDOG_ERROR);
      return;
    }

    // Add a CAPTCHA part to the form (depends on value of captcha_description).
    $captcha_description = _captcha_get_description();
    if ($captcha_description) {
      // $captcha_description is not empty: CAPTCHA part is a fieldset with description.
      $form['captcha']['captcha_widgets'] = array(
        '#type' => 'fieldset',
        '#title' => t('CAPTCHA'),
        '#description' => $captcha_description,
        '#attributes' => array('class' => 'captcha'),
      );
    }
    else {
      // $captcha_description is empty: CAPTCHA part is an empty markup form element.
      $form['captcha']['captcha_widgets'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="captcha">',
        '#suffix' => '</div>',
      );
    }

    // Add the form elements of the generated CAPTCHA to the form
    $form['captcha']['captcha_widgets'] = array_merge($form['captcha']['captcha_widgets'], $captcha['form']);


    // Add pre_render callback for additional CAPTCHA processing.
    $form['#pre_render'][] = 'captcha_pre_render_untrusted_user';
    // Add pre_render callback for placement of CAPTCHA formt element (above submit buttons).
    $form['#pre_render'][] = 'captcha_pre_render_place_captcha';

    // Add a validation callback for the CAPTCHA form element.
    // It is put in front of the list of the validation callbacks (if any).
    // This is needed for user login protection, where the login is done in
    // a validation callback (user_login_authenticate_validate), so
    // captcha_validate() needs to run before that.
    $form['#validate'] = array_merge(array('captcha_validate'), (array)($form['#validate']));

  }

  $form['captcha']['captcha_sid'] = array(
    '#type' => 'hidden',
    '#value' => $captcha_sid,
  );

  // Store information for usage in the validation and pre_render phase.
  $form['captcha']['captcha_info'] = array(
    '#type' => 'value',
    '#value' => array(
      'module' => $captcha_point->module,
      'type' => $captcha_point->type,
      'captcha_sid' => $captcha_sid,
      'solution' => $captcha['solution'],
    ),
  );


}

/**
 * Helper function for checking if CAPTCHA is required for user,
 * based on CAPTCHA session ID and user session info.
 */
function _captcha_required_for_user($captcha_sid, $form_id) {
  $captcha_session_status = db_result(db_query("SELECT status FROM {captcha_sessions} WHERE csid = %d", $captcha_sid));

  $captcha_success_form_ids = (array)($_SESSION['captcha_success_form_ids']);
  switch (variable_get('captcha_persistence', CAPTCHA_PERSISTENCE_SHOW_ALWAYS)) {
    case CAPTCHA_PERSISTENCE_SKIP_ONCE_SUCCESSFUL:
      $captcha_persistence_status = (count($captcha_success_form_ids) > 0);
      break;
    case CAPTCHA_PERSISTENCE_SKIP_ONCE_SUCCESSFUL_PER_FORM:
      $captcha_persistence_status = isset($captcha_success_form_ids[$form_id]);
      break;
    default:
      $captcha_persistence_status = FALSE;
  }

  return ($captcha_session_status == CAPTCHA_STATUS_UNSOLVED) && !$captcha_persistence_status;
}

/**
 * Implementation of form #validate.
 */
function captcha_validate($form, &$form_state) {
  // Get answer and preprocess if needed
  $captcha_response = $form_state['values']['captcha_response'];
  $captcha_info = $form_state['values']['captcha_info'];
  if ($captcha_info['preprocess']) {
    $captcha_response = module_invoke($captcha_info['module'], 'captcha', 'preprocess', $captcha_info['type'], $captcha_response);
  }
  $form_id = $captcha_info['form_id'];
  $form_id = $form['form_id']['#value'];
  // We use $form_state['clicked_button']['#post']['csid']
  // here instead of $form_state['values']['csid'], because the latter
  // contains the csid of the new form, while the former contains
  // the csid of the posted form.
  $csid = $form_state['clicked_button']['#post']['captcha_sid'];

  $solution = db_result(db_query('SELECT solution FROM {captcha_sessions} WHERE csid = %d AND status = %d', $csid, CAPTCHA_STATUS_UNSOLVED));

  if ($solution === FALSE) {
    // Unknown challenge_id.
    form_set_error('captcha', t('CAPTCHA test failed (unknown csid).'));
  }
  else {
    // Check answer.
    if ($captcha_response == $solution) {
      // Correct answer.
      $_SESSION['captcha_success_form_ids'][$form_id] = $form_id;
      // Record success.
      db_query("UPDATE {captcha_sessions} SET status=%d, attempts=attempts+1 WHERE csid=%d", CAPTCHA_STATUS_SOLVED, $csid);
    }
    else {
      // Wrong answer.
      db_query("UPDATE {captcha_sessions} SET attempts=attempts+1 WHERE csid=%d", $csid);
      // set form error
      form_set_error('captcha_response', t('The answer you entered for the CAPTCHA was not correct.'));
      // update wrong response counter
      variable_set('captcha_wrong_response_counter', variable_get('captcha_wrong_response_counter', 0) + 1);
      // log to watchdog if needed
      if (variable_get('captcha_log_wrong_responses', FALSE)) {
        watchdog('CAPTCHA',
          '%form_id post blocked by CAPTCHA module: challenge "%challenge" (by module "%module"), user answered "%response", but the solution was "%solution".',
          array('%form_id' => $form_id,
            '%response' => $captcha_response, '%solution' => $solution,
            '%challenge' => $captcha_info['type'], '%module' => $captcha_info['module'],
          ),
          WATCHDOG_NOTICE);
      }
      // If CAPTCHA was on a login form: stop validating, quit the current request
      // and forward to the current page (like a reload) to prevent loging in.
      // We do that because the log in procedure, which happens after
      // captcha_validate(), does not check error conditions of extra form
      // elements like the CAPTCHA.
      if ($form_id == 'user_login' || $form_id == 'user_login_block') {
        drupal_goto($_GET['q']);
      }
    }
  }
}

/**
 * Implementation of form #pre_render.
 */
function captcha_pre_render_untrusted_user($form) {

  $form_id = $form['form_id']['#value'];
  $captcha_info = $form['captcha']['captcha_info']['#value'];
  $captcha_sid = (int)($captcha_info['captcha_sid']);

  if (_captcha_required_for_user($captcha_sid, $form_id)) {
    // Update captcha_sessions table.
    _captcha_update_captcha_session($captcha_sid, $captcha_info['solution']);
  }
  else {
    // Remove CAPTCHA widgets from form.
    unset($form['captcha']['captcha_widgets']);
  }

  // Empty the value of the captcha_response form item before rendering
  $form['captcha']['captcha_widgets']['captcha_response']['#value'] = '';
  return $form;
}

/**
 * Pre_render function to place the CAPTCHA form element just above the last submit button
 */
function captcha_pre_render_place_captcha($form) {
  // search the weights of the buttons in the form
  $button_weights = array();
  foreach (element_children($form) as $key) {
    if ($key == 'buttons' || isset($form[$key]['#type']) && ($form[$key]['#type'] == 'submit' || $form[$key]['#type'] == 'button')) {
      $button_weights[] = $form[$key]['#weight'];
    }
  }
  if ($button_weights) {
    // set the weight of the CAPTCHA element a tiny bit smaller than the lightest button weight
    // (note that the default resolution of #weight values is 1/1000 (see drupal/includes/form.inc))
    $first_button_weight = min($button_weights);
    $form['captcha']['#weight'] = $first_button_weight - 0.5/1000.0;
    // make sure the form gets sorted before rendering
    unset($form['#sorted']);
  }
  return $form;
}
