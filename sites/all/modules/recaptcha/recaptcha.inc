<?php
/* $Id: recaptcha.inc,v 1.1.2.4.2.1 2008/01/26 17:24:07 robloach Exp $ */

/**
 * @file
 * Code required by both reCAPTCHA and reCAPTCHA Mailhide modules
 */

/**
 * Let the user know that the reCAPTCHA PHP library is not installed
 */   
function _recaptcha_library_not_found() {
  global $calledalready;
  if(!$calledalready) {
    $calledalready = TRUE;
    drupal_set_message(t('The <a href="@url" target="_blank">reCAPTCHA PHP library</a> was not found. Please install it into %recaptchadir.', array('@url' => 'http://recaptcha.net/plugins/php/index.html', '%recaptchadir' => drupal_get_path('module', 'recaptcha') . '/recaptcha')), 'error');
    if(!function_exists('recaptcha_get_signup_url')) {
      function recaptcha_get_signup_url($domain, $appname) {
        return 'http://recaptcha.net/api/getkey?domain=' . urlencode($domain) . '&appname=' . urlencode($appname);
      }
    }
    if(!function_exists('recaptcha_get_html')) {
      function recaptcha_get_html($pubkey, $error = null, $use_ssl = false) {
        return NULL;
      }
    }
    if(!function_exists('recaptcha_check_answer')) {
      function recaptcha_check_answer($privkey, $remoteip, $challenge, $response) {
        return NULL;
      }
    }
    if(!function_exists('recaptcha_mailhide_html')) {
      function recaptcha_mailhide_html($mailhide_pubkey, $mailhide_privkey, $email) {
        return $email;
      }
    }
  }
} // function _recaptcha_library_not_found()
