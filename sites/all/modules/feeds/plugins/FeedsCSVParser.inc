<?php
// $Id: FeedsCSVParser.inc,v 1.11 2010/07/06 17:09:43 alexb Exp $

/**
 * Parses a given file as a CSV file.
 */
class FeedsCSVParser extends FeedsParser {

  /**
   * Implementation of FeedsParser::parse().
   */
  public function parse(FeedsImportBatch $batch, FeedsSource $source) {

    // Parse.
    feeds_include_library('ParserCSV.inc', 'ParserCSV');
    $iterator = new ParserCSVIterator(realpath($batch->getFilePath()));
    $source_config = $source->getConfigFor($this);
    $parser = new ParserCSV();
    $delimiter = $source_config['delimiter'] == 'TAB' ? "\t" : $source_config['delimiter'];
    $parser->setDelimiter($delimiter);
    $parser->setSkipFirstLine(FALSE);
    $rows = $parser->parse($iterator);
    unset($parser);

    // Apply titles in lower case.
    // @todo Push this functionality into ParserCSV.
    $header = array_shift($rows);
    foreach ($header as $i => $title) {
      $header[$i] = trim(drupal_strtolower($title)); // Use lower case only.
    }
    $result_rows = array();
    foreach ($rows as $i => $row) {
      $result_row = array();
      foreach ($row as $j => $col) {
        $result_row[$header[$j]] = $col;
      }
      $result_rows[$i] = $result_row;
    }
    unset($rows);

    // Populate batch.
    $batch->setItems($result_rows);
  }

  /**
   * Override parent::getSourceElement() to use only lower keys.
   */
  public function getSourceElement($item, $element_key) {
    $element_key = drupal_strtolower($element_key);
    return isset($item[$element_key]) ? $item[$element_key] : '';
  }

  /**
   * Define defaults.
   */
  public function sourceDefaults() {
    return array(
      'delimiter' => $this->config['delimiter'],
    );
  }

  /**
   * Source form.
   *
   * Show mapping configuration as a guidance for import form users.
   */
  public function sourceForm($source_config) {
    $form = array();
    $form['#weight'] = -10;

    $mappings = feeds_importer($this->id)->processor->config['mappings'];
    $sources = $uniques = array();
    foreach ($mappings as $mapping) {
      $sources[] = $mapping['source'];
      if ($mapping['unique']) {
        $uniques[] = $mapping['source'];
      }
    }

    $items = array(
      t('Import !csv_files with one or more of these columns: !columns.', array('!csv_files' => l(t('CSV files'), 'http://en.wikipedia.org/wiki/Comma-separated_values'), '!columns' => implode(', ', $sources))),
      format_plural(count($uniques), t('Column <strong>!column</strong> is mandatory and considered unique: only one item per !column value will be created.', array('!column' => implode(', ', $uniques))), t('Columns <strong>!columns</strong> are mandatory and values in these columns are considered unique: only one entry per value in one of these column will be created.', array('!columns' => implode(', ', $uniques)))),
    );
    $form['help']['#value'] = '<div class="help">'. theme('item_list', $items) .'</div>';

    $form['delimiter'] = array(
      '#type' => 'select',
      '#title' => t('Delimiter'),
      '#description' => t('The character that delimits fields in the CSV file.'),
      '#options'  => array(
        ',' => ',',
        ';' => ';',
        'TAB' => 'TAB',
      ),
      '#default_value' => isset($source_config['delimiter']) ? $source_config['delimiter'] : ',',
    );
    return $form;
  }

  /**
   * Define default configuration.
   */
  public function configDefaults() {
    return array('delimiter' => ',');
  }

  /**
   * Build configuration form.
   */
  public function configForm(&$form_state) {
    $form = array();
    $form['delimiter'] = array(
      '#type' => 'select',
      '#title' => t('Default delimiter'),
      '#description' => t('Default field delimiter.'),
      '#options' => array(
        ',' => ',',
        ';' => ';',
        'TAB' => 'TAB',
      ),
      '#default_value' => $this->config['delimiter'],
    );
    return $form;
  }
}
