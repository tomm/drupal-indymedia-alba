<?php
// $Id: image_captcha.user.inc,v 1.3.2.1 2008/04/06 15:27:27 soxofaan Exp $

/**
 * Implementation of image CAPTCHA for use with the CAPTCHA module
 *
 * Loosely Based on MyCaptcha by Heine Deelstra
 * (http://heine.familiedeelstra.com/mycaptcha-download)
 */

/**
 * menu callback function that generates the CAPTCHA image
 */
function image_captcha_image($seed=NULL) {
  if (!$seed) {
    return;
  }
  // get the code to draw from $_SESSION
  $code = $_SESSION['image_captcha'][$seed];
  // unset the code from $_SESSION to prevent rerendering the CAPTCHA
  unset($_SESSION['image_captcha'][$seed]);
  // only generate an image if there is an code
  if ($code) {
    // generate the image
    $image = @_image_captcha_generate_image($code);
    // check of generation was successful
    if (!$image) {
      watchdog('CAPTCHA', 'Generation of image CAPTCHA failed. Check your image CAPTCHA configuration and especially the used font.', array(), WATCHDOG_ERROR);
      exit();
    }
    // Send the image resource as an image to the client
    drupal_set_header("Content-type: image/jpeg");
    // Following header is needed for Konqueror, which would re-request the image
    // on a mouseover event, which failes because the image can only be generated
    // once. This cache directive forces Konqueror to use cache instead of
    // re-requesting
    drupal_set_header("Cache-Control: max-age=3600, must-revalidate");
    // print the image as jpg to the client
    imagejpeg($image);
    // Clean up
    imagedestroy($image);
    exit();
  }
}


/**
 * small helper function for parsing a hexadecimal color to a RGB tuple
 */
function _image_captcha_hex_to_rgb($hex) {
  // handle #RGB format
  if (strlen($hex) == 4) {
    $hex = $hex[1] . $hex[1] . $hex[2] . $hex[2] . $hex[3] . $hex[3];
  }
  $c = hexdec($hex);
  $rgb = array();
  for ($i = 16; $i >= 0; $i -= 8) {
    $rgb[] = ($c >> $i) & 0xFF;
  }
  return $rgb;
}


/**
 * base function for generating a image CAPTCHA
 */
function _image_captcha_generate_image($code) {
  // get font
  list($font, $errmsg, $errvar) = _image_captcha_get_font();
  if (!$font) {
    watchdog('CAPTCHA', $errmsg, $errvar, WATCHDOG_ERROR);
    exit();
  }
  // get other settings
  $font_size = (int) variable_get('image_captcha_font_size', 30);
  $character_spacing = (float) variable_get('image_captcha_character_spacing', '1.2');
  $characters = _image_captcha_utf8_split($code);
  $character_quantity = count($characters);
  $width = $character_spacing * $font_size * $character_quantity;
  $height = 2 * $font_size;

  // create image resource
  $image = imagecreatetruecolor($width, $height);
  if (!$image) {
    return FALSE;
  }

  // get background color and paint
  $background_rgb = _image_captcha_hex_to_rgb(variable_get('image_captcha_background_color', '#ffffff'));
  $background_color = imagecolorallocate($image, $background_rgb[0], $background_rgb[1], $background_rgb[2]);
  imagefilledrectangle($image, 0, 0, $width, $height, $background_color);

  // draw text
  if (variable_get('image_captcha_double_vision', 0)) {
    $result = _image_captcha_image_generator_print_string($image, $width, $height, $font, $font_size, $code, TRUE);
    if (!$result) {
      return FALSE;
    }
  }
  $result = _image_captcha_image_generator_print_string($image, $width, $height, $font, $font_size, $code, FALSE);
  if (!$result) {
    return FALSE;
  }

  // add noise
  $noise_colors = array();
  for ($i = 0; $i < 20; $i++) {
    $noise_colors[] = imagecolorallocate($image, mt_rand(0, 255), mt_rand(0, 255), mt_rand(0, 255));
  }
  // Add additional noise.
  if (variable_get('image_captcha_dot_noise', 0)) {
    _image_captcha_image_generator_add_dots($image, $width, $height, $noise_colors);
  }
  if (variable_get('image_captcha_line_noise', 0)) {
    _image_captcha_image_generator_add_lines($image, $width, $height, $noise_colors);
  }

  // Distort the image.
  $distortion_amplitude = .25 * $font_size * variable_get('image_captcha_distortion_amplitude', 6) / 10.0;
  if ($distortion_amplitude > 1) {
    // distortion parameters
    $wavelength_xr = (2+3*lcg_value())*$font_size;
    $wavelength_yr = (2+3*lcg_value())*$font_size;
    $freq_xr = 2 * 3.141592 / $wavelength_xr;
    $freq_yr = 2 * 3.141592 / $wavelength_yr;
    $wavelength_xt = (2+3*lcg_value())*$font_size;
    $wavelength_yt = (2+3*lcg_value())*$font_size;
    $freq_xt = 2 * 3.141592 / $wavelength_xt;
    $freq_yt = 2 * 3.141592 / $wavelength_yt;

    $distorted_image = imagecreatetruecolor($width, $height);
    if (!$distorted_image) {
      return FALSE;
    }

    if (variable_get('image_captcha_bilinair_interpolation', FALSE)) {
      // distortion with bilineair interpolation
      for ($x = 0; $x < $width; $x++) {
        for ($y = 0; $y < $height; $y++) {
          // get distorted sample point in source image
          $r = $distortion_amplitude * sin($x * $freq_xr + $y * $freq_yr);
          $theta = $x * $freq_xt + $y * $freq_yt;
          $sx = $x + $r * cos($theta);
          $sy = $y + $r * sin($theta);
          $sxf = (int)floor($sx);
          $syf = (int)floor($sy);
          if ($sxf < 0 || $syf < 0 || $sxf >= $width - 1 || $syf >= $height - 1) {
            $color = $background_color;
          }
          else {
            // bilineair interpolation: sample at four corners
            $color_00 = imagecolorat($image, $sxf  , $syf  );
            $color_00_r = ($color_00 >> 16) & 0xFF;
            $color_00_g = ($color_00 >> 8) & 0xFF;
            $color_00_b = $color_00 & 0xFF;
            $color_10 = imagecolorat($image, $sxf+1, $syf  );
            $color_10_r = ($color_10 >> 16) & 0xFF;
            $color_10_g = ($color_10 >> 8) & 0xFF;
            $color_10_b = $color_10 & 0xFF;
            $color_01 = imagecolorat($image, $sxf  , $syf+1);
            $color_01_r = ($color_01 >> 16) & 0xFF;
            $color_01_g = ($color_01 >> 8) & 0xFF;
            $color_01_b = $color_01 & 0xFF;
            $color_11 = imagecolorat($image, $sxf+1, $syf+1);
            $color_11_r = ($color_11 >> 16) & 0xFF;
            $color_11_g = ($color_11 >> 8) & 0xFF;
            $color_11_b = $color_11 & 0xFF;
            // interpolation factors
            $u  = $sx - $sxf;
            $v  = $sy - $syf;
            // interpolate
            $r = (int)((1-$v)*((1-$u)*$color_00_r + $u*$color_10_r) + $v*((1-$u)*$color_01_r + $u*$color_11_r));
            $g = (int)((1-$v)*((1-$u)*$color_00_g + $u*$color_10_g) + $v*((1-$u)*$color_01_g + $u*$color_11_g));
            $b = (int)((1-$v)*((1-$u)*$color_00_b + $u*$color_10_b) + $v*((1-$u)*$color_01_b + $u*$color_11_b));
            // build color
            $color = ($r<<16) + ($g<<8) + $b;
          }
          imagesetpixel($distorted_image, $x, $y, $color);
        }
      }
    }
    else {
      // distortion with nearest neighbor interpolation
      for ($x = 0; $x < $width; $x++) {
        for ($y = 0; $y < $height; $y++) {
          // get distorted sample point in source image
          $r = $distortion_amplitude * sin($x * $freq_xr + $y * $freq_yr);
          $theta = $x * $freq_xt + $y * $freq_yt;
          $sx = $x + $r * cos($theta);
          $sy = $y + $r * sin($theta);
          $sxf = (int)floor($sx);
          $syf = (int)floor($sy);
          if ($sxf < 0 || $syf < 0 || $sxf >= $width - 1 || $syf >= $height - 1) {
            $color = $background_color;
          }
          else {
            $color = imagecolorat($image, $sxf, $syf);
          }
          imagesetpixel($distorted_image, $x, $y, $color);
        }
      }
    }
    // release undistorted image
    imagedestroy($image);
    // return distorted image
    return $distorted_image;
  }
  else {
    return $image;
  }
}

function _image_captcha_image_generator_add_lines(&$image, $width, $height, $colors) {
  $line_quantity = $width * $height/200.0 * ((int) variable_get('image_captcha_noise_level', 5)) / 10.0;
  for ($i = 0; $i <  $line_quantity; $i++) {
    imageline($image, mt_rand(0, $width), mt_rand(0, $height), mt_rand(0, $width), mt_rand(0, $height), $colors[array_rand($colors)]);
  }
}

function _image_captcha_image_generator_add_dots(&$image, $width, $height, $colors) {
  $noise_quantity = ($width * $height)/2.0 * ((int) variable_get('image_captcha_noise_level', 5)) / 10.0;
  for ($i = 0; $i < $noise_quantity; $i++ ) {
    imagesetpixel($image, mt_rand(0, $width), mt_rand(0, $height), $colors[array_rand($colors)]);
  }
}

 /**
 * Helper function for drawing text on the image
 * 
 * @param $background_mode if the text is for the background of the double vision mode
 */
function _image_captcha_image_generator_print_string(&$image, $width, $height, $font, $font_size, $text, $background_mode=FALSE) {
  // get characters
  $characters = _image_captcha_utf8_split($text);
  $character_quantity = count($characters);
  // get total width
  if ($font == 'BUILTIN') {
    $character_width = imagefontwidth(5);
    $character_height = imagefontheight(5);
    $textwidth = $character_quantity * $character_width;
  }
  else {
    $bbox = imagettfbbox($font_size, 0, realpath($font), $text);
    if (!$bbox) {
      return FALSE;
    }
    $textwidth = $bbox[2] - $bbox[0];
  }
  // calculate spacing
  $spacing = ($width - $textwidth) / ($character_quantity + 1);
  // character jittering
  $jittering_x = .3 * $font_size;
  $jittering_y = .3 * $font_size;
  
  // get colors
  $background_rgb = _image_captcha_hex_to_rgb(variable_get('image_captcha_background_color', '#ffffff'));
  $foreground_rgb = _image_captcha_hex_to_rgb(variable_get('image_captcha_foreground_color', '#000000'));
  // correct foreground for background mode (double vision)
  if ($background_mode) {
    for ($i=0; $i<3; $i++) {
      $foreground_rgb[$i] = 0.75 * $background_rgb[$i] + 0.25 * $foreground_rgb[$i];
    }
  }
  $background_color = imagecolorallocate($image, $background_rgb[0], $background_rgb[1], $background_rgb[2]);
  $foreground_color = imagecolorallocate($image, $foreground_rgb[0], $foreground_rgb[1], $foreground_rgb[2]);
  // precalculate the value ranges for color randomness
  $foreground_randomness = (int)(variable_get('image_captcha_foreground_color_randomness', 100));
  if ($foreground_randomness) {
    if ($background_mode) {
      $foreground_randomness *= .25;
    }
    $foreground_color_range = array();
    for ($i=0; $i<3; $i++) {
      $foreground_color_range[$i] = array(max(0, $foreground_rgb[$i] - $foreground_randomness), min(255, $foreground_rgb[$i] + $foreground_randomness));
    }
  }

  // set default text color
  $color = $foreground_color;

  // start cursor
  $x = $spacing;
  foreach ($characters as $character) {
    // get character dimensions
    if ($font != 'BUILTIN') {
      $bbox = imagettfbbox($font_size, 0, realpath($font), $character);
      $character_width = $bbox[2] - $bbox[0];
      $character_height = $bbox[5] - $bbox[3];
    }
    // calculate y position
    $y = .5 * ($height - $character_height);
    
    // set text color in case of randomness
    if ($foreground_randomness) {
      $color = imagecolorallocate($image,
        mt_rand($foreground_color_range[0][0], $foreground_color_range[0][1]),
        mt_rand($foreground_color_range[1][0], $foreground_color_range[1][1]),
        mt_rand($foreground_color_range[2][0], $foreground_color_range[2][1])
      );
    }
    
    // add jitter to position
    $pos_x = $x + mt_rand(-$jittering_x, $jittering_x);
    $pos_y = $y + mt_rand(-$jittering_y, $jittering_y);
    // draw character
    if ($font == 'BUILTIN') {
      imagestring($image, 5, $pos_x, $pos_y, $character, $color);
    }
    else {
      imagettftext($image, $font_size, 0, $pos_x, $pos_y, $color, realpath($font), $character);
    }
    // shift cursor
    $x += $character_width + $spacing;
  }
  // return a sign of success
  return TRUE;
}
