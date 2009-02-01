<?php
// $Id: text_captcha.user.inc,v 1.1 2008/01/04 14:29:58 soxofaan Exp $

/**
 * function for generating a random nonsense word of a given number of characters
 */
function _text_captcha_generate_nonsense_word($characters) {
  $vowels = "bcdfghjklmnpqrstvwxyz";
  $consonants = "aeiou";
  $vowel_max = strlen($vowels) - 1;
  $consonant_max = strlen($consonants) - 1;
  $word = '';
  $o = mt_rand(0, 1); // randomly start with vowel or consonants
  for ($i = 0; $i < $characters ; ++$i) {
    if (($i+$o) % 2) {
      $word .= $consonants[mt_rand(0, $consonant_max)];
    }
    else {
      $word .= $vowels[mt_rand(0, $vowel_max)];
    }
  }
  return $word;
}

/**
 * function for generating an array of words
 */
function _text_captcha_generate_words($num) {
  $words = array();
  if (variable_get('text_captcha_words', TEXT_CAPTCHA_GENERATE_NONSENSE_WORDS) == TEXT_CAPTCHA_USER_DEFINED_WORDS) {
    // use user defined words
    $uwords = explode(' ', variable_get('text_captcha_userdefined_words', ''));
    $keys = array_rand($uwords, $num);
    foreach ($keys as $key) {
      $words[] = $uwords[$key];
    }
  }
  else {
    // generate nonsense words
    for ($w=0; $w<$num; ++$w) {
      $words[] = _text_captcha_generate_nonsense_word(mt_rand(3, 7));
    }
  }
  return $words;
}

/**
 * function that returns a textual represention of an ordinal
 */
function _text_captcha_ordinal($n) {
  $ordinalmap = array(1 => t('first'), 2 => t('second'), 3 => t('third'),
    4 => t('fourth'), 5 => t('fifth'), 6 => t('sixth'), 7 => t('seventh'),
    8 => t('eighth'), 9 => t('ninth'), 10 => t('tenth'));
  if (array_key_exists($n, $ordinalmap)) {
    return $ordinalmap[$n];
  }
  else {
    return "{$n}th";
  }
}
