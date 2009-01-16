<?php
/* $Id: ical.inc,v 1.16 2008/09/06 22:20:28 killes Exp $ */

/**
 * @file
 * API for event import/export in iCalendar format as outlined in Internet Calendaring and Scheduling Core Object Specification
 * http://www.ietf.org/rfc/rfc2445.txt
 *
 * This module is IN DEVELOPMENT and not a finished product
 */

/**
 * Turn an array of events into a valid iCalendar file
 *
 * @param $events
 *   An array of associative arrays where
 *      'start'         => Unix timestamp (GMT) of start time (Required, if no allday_start)
 *      'end'           => Unix timestamp (GMT) of end time (Optional)
 *      'allday_start'  => Start date of all-day event in YYYYMMDD format (Required, if no start)
 *      'allday_end'    => End date of all-day event in YYYYMMDD format (Optional)
 *      'summary'       => Title of event (Text)
 *      'description'   => Description of event (Text)
 *      'location'      => Location of event (Text)
 *      'uid'           => ID of the event for use by calendaring program.  Recommend the url of the node
 *      'url'           => URL of event information
 *
 * @param $calname
 *   Name of the calendar.  Will use site name if none is specified.
 *
 * @return
 *   Text of a iCalendar file
 */
function ical_export($events, $calname = NULL) {
  $output = "BEGIN:VCALENDAR\nVERSION:2.0\n";
  $output .= "METHOD:PUBLISH\n";
  $output .= 'X-WR-CALNAME:'. variable_get('site_name', '') .' | '. ical_escape_text($calname) ."\n";
  $output .= "PRODID:-//strange bird labs//Drupal iCal API//EN\n";
  foreach ($events as $uid => $event) {
    $output .= "BEGIN:VEVENT\n";
    $output .= "DTSTAMP;VALUE=DATE-TIME:". gmdate("Ymd\THis\Z", time()) ."\n";
    if (!$event['has_time']) { // all day event
      $output .= "DTSTART;VALUE=DATE-TIME:" . event_format_date($event['start_utc'], 'custom', "Ymd\THis\Z") ."\n";
      //If allday event, set to day after allday start
      $end_date = event_date_later($event['start'], 1);
      $output .= "DTEND;VALUE=DATE-TIME:" . event_format_date($end_date, 'custom', 'Ymd') ."\n";
    }
    else if (!empty($event['start_utc']) && !empty($event['end_utc'])) {
      $output .= "DTSTART;VALUE=DATE-TIME:". event_format_date($event['start_utc'], 'custom', "Ymd\THis\Z") ."\n";
      $output .= "DTEND;VALUE=DATE-TIME:". event_format_date($event['end_utc'], 'custom', "Ymd\THis\Z") ."\n";
    }
    else if (!empty($event['start_utc'])) {
      $output .= "DTSTART;VALUE=DATE-TIME:". event_format_date($event['start_utc'], 'custom', "Ymd\THis\Z") ."\n";
    }
    $output .= "UID:". ($event['uid'] ? $event['uid'] : $uid) ."\n";
    if (!empty($event['url'])) $output .= "URL;VALUE=URI:" . $event['url'] ."\n";
    if (!empty($event['location'])) $output .= "LOCATION:" . ical_escape_text($event['location']) ."\n";
    $output .= "SUMMARY:" . ical_escape_text($event['summary']) ."\n";
    if (!empty($event['description'])) $output .= "DESCRIPTION:" . ical_escape_text($event['description']) ."\n";
    $output .= "END:VEVENT\n";
  }
  $output .= "END:VCALENDAR\n";
  return $output;
}

/**
 * Escape #text elements for safe iCal use
 *
 * @param $text
 *   Text to escape
 *
 * @return
 *   Escaped text
 *
 */
function ical_escape_text($text) {
  //$text = strip_tags($text);
  $text = str_replace('"', '\"', $text);
  $text = str_replace("\\", "\\\\", $text);
  $text = str_replace(",", "\,", $text);
  $text = str_replace(":", "\:", $text);
  $text = str_replace(";", "\;", $text);
  $text = str_replace("\n", "\n ", $text);
  return $text;
}

/**
 * Given the location of a valide iCalendar file, will return an array of event information
 *
 * @param $filename
 *   Location (local or remote) of a valid iCalendar file
 *
 * @return
 *   An array of associative arrays where
 *      'start'         => start time as date array
 *      'end'           => end time as date array
 *      'summary'       => Title of event
 *      'description'   => Description of event
 *      'location'      => Location of event
 *      'uid'           => ID of the event in calendaring program
 *      'url'           => URL of event information                                    */
function ical_import($filename) {
  $items = array();
  $ifile = @fopen($filename, "r");
  if ($ifile == FALSE) exit('Invalid input file');
  $nextline = fgets($ifile, 1024);
  if (trim($nextline) != 'BEGIN:VCALENDAR') exit('Invalid calendar file:'. $filename);
  while (!feof($ifile)) {
    $line = $nextline;
    $nextline = fgets($ifile, 1024);
    $nextline = ereg_replace("[\r\n]", "", $nextline);
    while (substr($nextline, 0, 1) == " ") {
      $line = $line . substr($nextline, 1);
      $nextline = fgets($ifile, 1024);
      $nextline = ereg_replace("[\r\n]", "", $nextline);
    }
    $line = trim($line);
    switch ($line) {
      case 'BEGIN:VEVENT':
        unset($start_unixtime, $start_date, $start_time,
          $end_unixtime, $end_date, $end_time,
          $allday_start, $allday_end, $the_duration,
          $uid,
          $summary,
          $description,
          $url,
          $location
              );
        break;
      case 'END:VEVENT':
        if (empty($uid)) {
          $uid = $uid_counter;
          $uid_counter++;
        }
        if (empty($end_unixtime) && isset($the_duration)) {
          $end_unixtime = $start_unixtime + $the_duration;
          $end_time = date('Hi', $end_unixtime);
        }

        $items[$uid] = array('start' => $start_unixtime,
          'end' => $end_unixtime,
          'allday_start' => $allday_start,
          'allday_end' => $allday_end,
          'summary' => $summary,
          'description' => $description,
          'location' => $location,
          'url' => $url,
          'uid' => $uid);
        break;
      default:

        unset($field, $data, $prop_pos, $property);
        ereg("([^:]+):(.*)", $line, $line);
        $field = $line[1];
        $data = $line[2];

        $property = $field;
        $prop_pos = strpos($property, ';');
        if ($prop_pos !== false) $property = substr($property, 0, $prop_pos);
        $property = strtoupper($property);

        switch ($property) {

          case 'DTSTART':
            $zulu_time = false;
            if (substr($data, -1) == 'Z') $zulu_time = true;
            $data = str_replace('T', '', $data);
            $data = str_replace('Z', '', $data);
            $field = str_replace(';VALUE=DATE-TIME', '', $field);
            if ((preg_match("/^DTSTART;VALUE=DATE/i", $field)) || (ereg ('^([0-9]{4})([0-9]{2})([0-9]{2})$', $data))) {
              ereg('([0-9]{4})([0-9]{2})([0-9]{2})', $data, $dtstart_check);
              $allday_start = $data;
              $start_date = $allday_start;
              $start_unixtime = strtotime($data);
            }
            else {
              if (preg_match("/^DTSTART;TZID=/i", $field)) {
                $tz_tmp = explode('=', $field);
                $tz_dtstart = $tz_tmp[1];
                unset($tz_tmp);
              }
              elseif ($zulu_time) {
                $tz_dtstart = 'GMT';
              }
              preg_match('/([0-9]{4})([0-9]{2})([0-9]{2})([0-9]{0,2})([0-9]{0,2})/', $data, $regs);
              $start_date = $regs[1] . $regs[2] . $regs[3];
              $start_time = $regs[4] . $regs[5];
              $start_unixtime = mktime($regs[4], $regs[5], 0, $regs[2], $regs[3], $regs[1]);

              $dlst = date('I', $start_unixtime);
              $server_offset_tmp = _ical_choose_offset($start_unixtime, 'Same as Server');
              if (isset($tz_dtstart)) {
                if ($tz = _ical_tz($tz_dtstart)) {
                  $offset_tmp = date('I', $start_unixtime) ? $tz->offset_dst : $tz->offset;
                }
                else {
                  $offset_tmp = '+0000';
                }
              }
              else if (isset($calendar_tz)) {
                if ($tz = _ical_tz($calendar_tz)) {
                  $offset_tmp = date('I', $start_unixtime) ? $tz->offset_dst : $tz->offset;
                }
                else {
                  $offset_tmp = '+0000';
                }
              }
              else {
                $offset_tmp = $server_offset_tmp;
              }
              $start_unixtime = _ical_calc_time($offset_tmp, $server_offset_tmp, $start_unixtime);
              $start_date = date('Ymd', $start_unixtime);
              $start_time = date('Hi', $start_unixtime);
              unset($server_offset_tmp, $offset_tmp, $tz_dtstart);
            }
            break;
          case 'DTEND':
            $zulu_time = false;
            if (substr($data, -1) == 'Z') $zulu_time = true;
            $data = str_replace('T', '', $data);
            $data = str_replace('Z', '', $data);
            $field = str_replace(';VALUE=DATE-TIME', '', $field);
            if ((preg_match("/^DTEND;VALUE=DATE/i", $field)) || (ereg ('^([0-9]{4})([0-9]{2})([0-9]{2})$', $data))) {
              $allday_end = $data;
              $end_date = $allday_end;
              $end_unixtime = strtotime($data);
            }
            else {
              if (preg_match("/^DTEND;TZID=/i", $field)) {
                $tz_tmp = explode('=', $field);
                $tz_dtend = $tz_tmp[1];
                unset($tz_tmp);
              }
              elseif ($zulu_time) {
                $tz_dtend = 'GMT';
              }
              preg_match('/([0-9]{4})([0-9]{2})([0-9]{2})([0-9]{0,2})([0-9]{0,2})/', $data, $regs);
              $end_date = $regs[1] . $regs[2] . $regs[3];
              $end_time = $regs[4] . $regs[5];
              $end_unixtime = mktime($regs[4], $regs[5], 0, $regs[2], $regs[3], $regs[1]);

              $server_offset_tmp = _ical_choose_offset($end_unixtime, 'Same as Server');
              if (isset($tz_dtend)) {
                if ($tz = _ical_tz($tz_dtend)) {
                  $offset_tmp = date('I', $end_unixtime) ? $tz->offset_dst : $tz->offset;
                }
                else {
                  $offset_tmp = '+0000';
                }
              }
              else if (isset($calendar_tz)) {
                if ($tz = _ical_tz($calendar_tz)) {
                  $offset_tmp = date('I', $end_unixtime) ? $tz->offset_dst : $tz->offset;
                }
                else {
                  $offset_tmp = '+0000';
                }
              }
              else {
                $offset_tmp = $server_offset_tmp;
              }
              $end_unixtime = _ical_calc_time($offset_tmp, $server_offset_tmp, $end_unixtime);
              $end_date = date('Ymd', $end_unixtime);
              $end_time = date('Hi', $end_unixtime);
              unset($server_offset_tmp, $offset_tmp, $tz_dtend);
            }
            break;
          case 'DURATION':
            if (!stristr($field, '=DURATION')) {
              ereg('^P([0-9]{1,2}[W])?([0-9]{1,2}[D])?([T]{0,1})?([0-9]{1,2}[H])?([0-9]{1,2}[M])?([0-9]{1,2}[S])?', $data, $duration);
              $weeks = str_replace('W', '', $duration[1]);
              $days = str_replace('D', '', $duration[2]);
              $hours = str_replace('H', '', $duration[4]);
              $minutes = str_replace('M', '', $duration[5]);
              $seconds = str_replace('S', '', $duration[6]);
              $the_duration = ($weeks * 60 * 60 * 24 * 7) + ($days * 60 * 60 * 24) + ($hours * 60 * 60) + ($minutes * 60) + ($seconds);
            }
            break;
          case 'SUMMARY':
            $summary = $data;
            break;
          case 'DESCRIPTION':
            $description = $data;
            break;
          case 'UID':
            $uid = $data;
            break;
          case 'X-WR-CALNAME':
            $actual_calname = $data;
            break;
          case 'X-WR-TIMEZONE':
            $calendar_tz = $data;
            break;
          case 'LOCATION':
            $location = $data;
            break;
          case 'URL':
            $url = $data;
            break;
        }
    }
  }
  return $items;
}

function ical_help($path, $arg) {
  switch ($path) {
  case 'admin/modules#description':
    return t('iCalendar API for Events Modules');
    break;
  }
}

function _ical_tz($tz) {
  foreach (event_get_timezones() as $delta => $zone) {
    if ($tz == $zone['timezone']);
    return (object) $zone;
  }
}

function _ical_choose_offset($time, $timezone) {
  if (!isset($timezone)) $timezone = '';
  switch ($timezone) {
  case '':
    $offset = 'none';
    break;
  case 'Same as Server':
    $offset = date('O', $time);
    break;
  default:
    if ($tz = _ical_tz($timezone)) {
      $offset = date('I', $time) ? $tz->offset_dst : $tz->offset;
    }
    else {
      $offset = '+0000';
    }
  }
  return $offset;
}

function _ical_calc_time($have, $want, $time) {
  if ($have == 'none' || $want == 'none') return $time;
  $have_secs = _ical_calc_offset($have);
  $want_secs = _ical_calc_offset($want);
  $diff = $want_secs - $have_secs;
  $time += $diff;
  return $time;
}

function _ical_calc_offset($offset_str) {
  $sign = substr($offset_str, 0, 1);
  $hours = substr($offset_str, 1, 2);
  $mins = substr($offset_str, 3, 2);
  $secs = ((int)$hours * 3600) + ((int)$mins * 60);
  if ($sign == '-') $secs = 0 - $secs;
  return $secs;
}

/**
 *  escape ical separators in quoted-printable encoded code
 */
function ical_quoted_printable_escaped($string) {
  $replace = array(";" => "\;", ":" => "\:");
  return strtr(ical_quoted_printable_encode($string), $replace);
}

/**
 *  encode text using quoted-printable standard
 */
function ical_quoted_printable_encode($text, $line_max = 76) {
  $hex       = array('0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F');
  $lines     = preg_split("/(?:\r\n|\r|\n)/", $text);
  $eol       = "\r\n";
  $linebreak = "=0D=0A";
  $escape    = "=";
  $output    = "";

  for ($x = 0; $x < count($lines); $x++) {
    $line     = $lines[$x];
    $line_len = strlen($line);
    $newline  = "";
    for ($i = 0; $i < $line_len; $i++) {
    $c   = substr($line, $i, 1);
    $dec = ord($c);
    // convert space at end of line
    if ( ($dec == 32) && ($i == ($line_len - 1)) ) {
      $c = $escape ."20";
    }
    // convert tab and special chars
    elseif ( ($dec == 61) || ($dec < 32 ) || ($dec > 126) ) {
      $h2 = floor($dec/16);
      $h1 = floor($dec%16);
      $c  = $escape . $hex["$h2"] . $hex["$h1"];
    }
    // see if new output line is needed
    if ( (strlen($newline) + strlen($c)) >= $line_max ) {
      $output .= $newline . $escape . $eol;
      $newline = "";
    }
    $newline .= $c;
    }
    $output .= $newline;

    // skip last line feed
    if ($x < count($lines) - 1) $output .= $linebreak;
  }
  return trim($output);
}
