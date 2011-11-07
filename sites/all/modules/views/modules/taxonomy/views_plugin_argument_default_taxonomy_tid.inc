<?php
/**
 * @file
 *   Taxonomy tid default argument.
 */

class views_plugin_argument_default_taxonomy_tid extends views_plugin_argument_default {
  function option_definition() {
    $options = parent::option_definition();

    $options['term_page'] = array('default' => TRUE);
    $options['node'] = array('default' => FALSE);
    $options['limit'] = array('default' => FALSE);
    $options['vids'] = array('default' => array());

    return $options;
  }

  function argument_form(&$form, &$form_state) {
    $form['term_page'] = array(
      '#type' => 'checkbox',
      '#title' => t('Load default argument from term page'),
      '#default_value' => $this->argument->options['term_page'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array(
        'radio:options[default_action]' => array('default'),
        'radio:options[default_argument_type]' => array($this->id)
      ),
      '#dependency_count' => 2,
    );
    $form['node'] = array(
      '#type' => 'checkbox',
      '#title' => t('Load default argument from node page. Good for related taxonomy blocks.'),
      '#default_value' => $this->argument->options['node'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array(
        'radio:options[default_action]' => array('default'),
        'radio:options[default_argument_type]' => array($this->id)
      ),
      '#dependency_count' => 2,
    );

    $form['limit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Limit terms by vocabulary'),
      '#default_value'=> $this->argument->options['limit'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array(
        'radio:options[default_action]' => array('default'),
        'radio:options[default_argument_type]' => array($this->id),
        'edit-options-node' => array(1)
      ),
      '#dependency_count' => 3,
    );

    $options = array();
    $vocabularies = taxonomy_get_vocabularies();
    foreach ($vocabularies as $voc) {
      $options[$voc->vid] = check_plain($voc->name);
    }

    $form['vids'] = array(
      '#prefix' => '<div><div id="edit-options-vids">',
      '#suffix' => '</div></div>',
      '#type' => 'checkboxes',
      '#title' => t('Vocabularies'),
      '#description' => t('If you wish to limit terms for specific vocabularies, check them; if none are checked, all terms will be included.'),
      '#options' => $options,
      '#default_value' => isset($this->argument->options['vids']) ? $this->argument->options['vids'] : array(),
      '#process' => array('expand_checkboxes', 'views_process_dependency'),
      '#dependency' => array(
        'radio:options[default_action]' => array('default'),
        'radio:options[default_argument_type]' => array($this->id),
        'edit-options-node' => array(1),
        'edit-options-limit' => array(1)
      ),
      '#dependency_count' => 4,
    );
  }

  function options_submit(&$form, &$form_state, &$options) {
    // Clear checkbox values.
    $options['vids'] = array_filter($options['vids']);
  }

  function get_argument() {
    // Load default argument from taxonomy page.
    if (!empty($this->argument->options['term_page'])) {
      if (arg(0) == 'taxonomy' && arg(1) == 'term' && is_numeric(arg(2))) {
        return arg(2);
      }
    }
    // Load default argument from node.
    if (!empty($this->argument->options['node'])) {
      foreach (range(1, 3) as $i) {
        $node = menu_get_object('node', $i);
        if (!empty($node)) {
          break;
        }
      }
      // Just check, if a node could be detected.
      if ($node) {
        if (!empty($this->argument->options['limit'])) {
          $tids = array();
          // Filter by vid.
          foreach ($node->taxonomy as $tid => $term) {
            if (!empty($this->argument->options['vids'][$term->vid])) {
              $tids[] = $tid;
            }
          }
          return implode(",", $tids);
        }
        // Return all tids.
        else {
          return implode(",", array_keys($node->taxonomy));
        }
      }
    }
  }
}

