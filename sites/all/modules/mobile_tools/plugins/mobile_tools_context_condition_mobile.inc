<?php
/**
 * @file
 * Integration with the Context module
 */
class mobile_tools_context_condition_mobile extends context_condition {
  function condition_values() {
    return array_merge(array('mobile' => 'Mobile', 'desktop' => 'Desktop'), mobile_tools_device_groups());
  }

  function execute($value) {
    foreach ($this->get_contexts($value) as $context) {
      $this->condition_met($context, $value);
    }
  }
}
