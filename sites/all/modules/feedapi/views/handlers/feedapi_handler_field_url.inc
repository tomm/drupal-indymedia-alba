<?php
// $Id: feedapi_handler_field_url.inc,v 1.1.2.1 2009/01/23 13:47:11 alexb Exp $

/**
 * Field handler to provide title links to drupal nodes
 *
 * @ingroup views_field_handlers
 */
class feedapi_handler_field_url extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['display_url'] = array('default' => 'url');
    return $options;
  }

  function query() {
    if ($this->options['display_url'] == 'title') {
      $this->additional_fields['title'] = array('table' => 'node', 'field' => 'title');
    }
    parent::query();
  }

  /**
   * Provide link to the page being visited.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['display_url'] = array(
      '#title' => t('Display'),
      '#type' => 'select',
      '#options' => array(
        'title' => t("Node title as link"),
        'url' => t('URL as link'),
        'text' => t('URL as plain text'),
      ),
      '#default_value' => $this->options['display_url'],
    );
  }

  function render($values) {
    $value = $values->{$this->field_alias};

    if ($this->options['display_url'] == 'title') {
      return l($values->{$this->aliases['title']}, $value);
    }
    if ($this->options['display_url'] == 'url') {
      return l($value, $value);
    }
    return $value;
  }
}
