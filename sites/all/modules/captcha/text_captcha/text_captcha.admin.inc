<?php
// $Id: text_captcha.admin.inc,v 1.1 2008/01/04 14:29:58 soxofaan Exp $

/**
 * Administration form
 */
function text_captcha_settings_form() {
  $form = array();

  // radio buttons for selecting the kind of words to use
  $form['text_captcha_words'] = array(
    '#type' => 'radios',
    '#title' => t('Kind of words to use in the phrase'),
    '#options' => array(
      TEXT_CAPTCHA_GENERATE_NONSENSE_WORDS => t('Generate nonsense random words.'),
      TEXT_CAPTCHA_USER_DEFINED_WORDS => t('Use user defined words.')
    ),
    '#default_value' => variable_get('text_captcha_words', TEXT_CAPTCHA_GENERATE_NONSENSE_WORDS),
  );
  // textarea for user defined words
  $form['text_captcha_userdefined_words'] = array(
    '#type' => 'textarea',
    '#title' => t('User defined words'),
    '#default_value' => variable_get('text_captcha_userdefined_words', ''),
    '#description' => t('Enter a bunch of space separated words (at least @min).', array('@min' => TEXT_CAPTCHA_USER_DEFINED_WORD_MINIMUM)),
  );
  // textfield for the number of words in the CAPTCHA phrase
  $form['text_captcha_word_quantity'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of words in the phrase'),
    '#default_value' => (int) variable_get('text_captcha_word_quantity', 5),
    '#size' => 2,
    '#maxlength' => 2,
  );

  $form['#validate'][] = 'text_captcha_settings_form_validate';
  return system_settings_form($form);
}

/**
 * Validate function of the administration form
 */
function text_captcha_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['text_captcha_words'] == TEXT_CAPTCHA_USER_DEFINED_WORDS) {
    // check if there are at minimum TEXT_CAPTCHA_USER_DEFINED_WORD_MINIMUM user defined words
    if (count(explode(' ', $form_state['values']['text_captcha_userdefined_words'])) < TEXT_CAPTCHA_USER_DEFINED_WORD_MINIMUM) {
      form_set_error('text_captcha_userdefined_words',
        t('You need to enter at least @min words if you want to use user defined words.',
          array('@min' => TEXT_CAPTCHA_USER_DEFINED_WORD_MINIMUM)
        )
      );
    }
  }
  // chech text_captcha_word_quantity
  $word_quantity = (int) $form_state['values']['text_captcha_word_quantity'];
  if ($word_quantity < 4 || $word_quantity > 10) {
    form_set_error('text_captcha_word_quantity', t('Number of words in the phrase should be between 4 and 10.'));
  }
}
