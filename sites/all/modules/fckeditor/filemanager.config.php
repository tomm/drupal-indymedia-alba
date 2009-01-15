<?php

// $Id: filemanager.config.php,v 1.2.2.4 2008/05/23 11:12:10 wwalc Exp $
/**
 * FCKeditor - The text editor for Internet - http://www.fckeditor.net
 * Copyright (C) 2003-2008 Frederico Caldeira Knabben
 *
 * == BEGIN LICENSE ==
 *
 * Licensed under the terms of any of the following licenses at your
 * choice:
 *
 *  - GNU General Public License Version 2 or later (the "GPL")
 *    http://www.gnu.org/licenses/gpl.html
 *
 *  - GNU Lesser General Public License Version 2.1 or later (the "LGPL")
 *    http://www.gnu.org/licenses/lgpl.html
 *
 *  - Mozilla Public License Version 1.1 or later (the "MPL")
 *    http://www.mozilla.org/MPL/MPL-1.1.html
 *
 * == END LICENSE ==
 *
 * @file
 * FCKeditor Module for Drupal 5.x
 *
 * This file is required by FCKeditor module if you want to enable built-in file management functionality
 *
 * This useful script does the following:
 * - authenticate users that are allowed to use file browser
 * - redefine the $Config['UserFilesPath'] and $Config['UserFilesAbsolutePath'] according to the values set in FCKeditor profile
 */

$fck_user_files_path = "";
$fck_user_files_absolute_path = "";

function CheckAuthentication()
{
  static $authenticated;

  if (!isset($authenticated)) {
    $result = false;

    if (!empty($_SERVER['SCRIPT_FILENAME'])) {
      $drupal_path = dirname(dirname(dirname(dirname($_SERVER['SCRIPT_FILENAME']))));
      if(!file_exists($drupal_path . "/includes/bootstrap.inc")) {
        $drupal_path = dirname(dirname(dirname($_SERVER['SCRIPT_FILENAME'])));
        $depth = 2;
        do {
          $drupal_path = dirname($drupal_path);
          $depth ++;          
        }
        while(!($bootstrapFileFound = file_exists($drupal_path . "/includes/bootstrap.inc")) && $depth<10);
      }
    }
    if (!isset($bootstrapFileFound) || !$bootstrapFileFound) {
      $drupal_path = "../../../";
      if(!file_exists($drupal_path . "/includes/bootstrap.inc")) {
        $drupal_path = "../..";
        do {
          $drupal_path .= "/..";
          $depth = substr_count($drupal_path, "..");
        }
        while(!($bootstrapFileFound = file_exists($drupal_path . "/includes/bootstrap.inc")) && $depth<10);
      }
    }
    if (!isset($bootstrapFileFound) || $bootstrapFileFound) {
      $fck_cwd = getcwd();
      chdir($drupal_path);
      require_once "./includes/bootstrap.inc";
      drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
      $authenticated = user_access("allow fckeditor file uploads");
      if (isset($_SESSION['FCKeditor']['UserFilesPath'], $_SESSION['FCKeditor']['UserFilesAbsolutePath'])) {
        $GLOBALS['fck_user_files_path'] = $_SESSION['FCKeditor']['UserFilesPath'];
        $GLOBALS['fck_user_files_absolute_path'] = $_SESSION['FCKeditor']['UserFilesAbsolutePath'];
      }
      chdir($fck_cwd);
    }
  }

  return $authenticated;
}

/**
 * Note:
 * Although in FCKeditor 2.5 $Config['Enabled'] is not used anymore, 
 * CheckAuthentication() must be called once to initialize session
 * before sending any content
 * Static $authenticated variable is being assigned, so 
 * application performance is not affected
 */
$Config['Enabled'] = CheckAuthentication();

if (!empty($fck_user_files_path)) {
  $Config['UserFilesPath'] = $fck_user_files_path;
  $Config['UserFilesAbsolutePath'] = $fck_user_files_absolute_path;
}
else {
  // Nothing in session? Shouldn't happen... anyway let's try to upload it in the (almost) right place
  // Path to user files relative to the document root.
  $Config['UserFilesPath'] = strtr(base_path(), array(
  "/modules/fckeditor/fckeditor/editor/filemanager/connectors/php" => "",
  "/modules/fckeditor/fckeditor/editor/filemanager/browser/default/connectors/php" => "",
  "/modules/fckeditor/fckeditor/editor/filemanager/upload/php" => "",
  )) . file_directory_path() . "/";
}
