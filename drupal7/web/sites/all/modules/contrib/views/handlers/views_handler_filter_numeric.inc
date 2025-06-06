<?php

/**
 * @file
 * Definition of views_handler_filter_numeric.
 */

/**
 * Simple filter to handle greater than/less than filters.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_numeric extends views_handler_filter {

  /**
   * Exposed filter options.
   */
  public $always_multiple = TRUE;

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['value'] = array(
      'contains' => array(
        'min' => array('default' => ''),
        'max' => array('default' => ''),
        'value' => array('default' => ''),
      ),
    );

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function operators() {
    $operators = array(
      '<' => array(
        'title' => t('Is less than'),
        'method' => 'op_simple',
        'short' => t('<'),
        'values' => 1,
      ),
      '<=' => array(
        'title' => t('Is less than or equal to'),
        'method' => 'op_simple',
        'short' => t('<='),
        'values' => 1,
      ),
      '=' => array(
        'title' => t('Is equal to'),
        'method' => 'op_simple',
        'short' => t('='),
        'values' => 1,
      ),
      '!=' => array(
        'title' => t('Is not equal to'),
        'method' => 'op_simple',
        'short' => t('!='),
        'values' => 1,
      ),
      '>=' => array(
        'title' => t('Is greater than or equal to'),
        'method' => 'op_simple',
        'short' => t('>='),
        'values' => 1,
      ),
      '>' => array(
        'title' => t('Is greater than'),
        'method' => 'op_simple',
        'short' => t('>'),
        'values' => 1,
      ),
      'between' => array(
        'title' => t('Is between'),
        'method' => 'op_between',
        'short' => t('between'),
        'values' => 2,
      ),
      'not between' => array(
        'title' => t('Is not between'),
        'method' => 'op_between',
        'short' => t('not between'),
        'values' => 2,
      ),
    );

    // If the definition allows for the empty operator, add it.
    if (!empty($this->definition['allow empty'])) {
      $operators += array(
        'empty' => array(
          'title' => t('Is empty (NULL)'),
          'method' => 'op_empty',
          'short' => t('empty'),
          'values' => 0,
        ),
        'not empty' => array(
          'title' => t('Is not empty (NOT NULL)'),
          'method' => 'op_empty',
          'short' => t('not empty'),
          'values' => 0,
        ),
      );
    }

    // Add regexp support for MySQL.
    if (Database::getConnection()->databaseType() == 'mysql') {
      $operators += array(
        'regular_expression' => array(
          'title' => t('Regular expression'),
          'short' => t('regex'),
          'method' => 'op_regex',
          'values' => 1,
        ),
        'not_regular_expression' => array(
          'title' => t('Not regular expression'),
          'short' => t('not regex'),
          'method' => 'op_not_regex',
          'values' => 1,
        ),
      );
    }

    return $operators;
  }

  /**
   * Provide a list of all the numeric operators.
   */
  public function operator_options($which = 'title') {
    $options = array();
    foreach ($this->operators() as $id => $info) {
      $options[$id] = $info[$which];
    }

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function operator_values($values = 1) {
    $options = array();
    foreach ($this->operators() as $id => $info) {
      if ($info['values'] == $values) {
        $options[] = $id;
      }
    }

    return $options;
  }

  /**
   * Provide a simple textfield for equality.
   */
  public function value_form(&$form, &$form_state) {
    $form['value']['#tree'] = TRUE;

    // We have to make some choices when creating this as an exposed
    // filter form. For example, if the operator is locked and thus
    // not rendered, we can't render dependencies; instead we only
    // render the form items we need.
    $which = 'all';
    $limit_operators = !empty($this->options['expose']['limit_operators']) && (count($this->options['expose']['available_operators']) > 0);
    $use_value = FALSE;
    $use_minmax = FALSE;

    if (!empty($form['operator'])) {
      $source = ($form['operator']['#type'] == 'radios') ? 'radio:options[operator]' : 'edit-options-operator';
    }

    if (!empty($form_state['exposed'])) {
      $operator_values_with_1_values = $this->operator_values(1);
      $operator_values_with_2_values = $this->operator_values(2);
      if ($limit_operators) {
        // If limit operators is enabled, check that at least one operator
        // with two values is enabled to display the min max widgets.
        foreach ($operator_values_with_2_values as $operator) {
          if (isset($this->options['expose']['available_operators'][$operator])) {
            $use_minmax = TRUE;
            break;
          }
        }
        // The same for operators with one value.
        foreach ($operator_values_with_1_values as $operator) {
          if (isset($this->options['expose']['available_operators'][$operator])) {
            $use_value = TRUE;
            break;
          }
        }
      }
      else {
        $use_minmax = $use_value = TRUE;
      }
      $identifier = $this->options['expose']['identifier'];

      if (empty($this->options['expose']['use_operator']) || empty($this->options['expose']['operator_id'])) {
        // Exposed and locked.
        $which = in_array($this->operator, $operator_values_with_2_values) ? 'minmax' : 'value';
      }
      else {
        $source = 'edit-' . drupal_html_id($this->options['expose']['operator_id']);
      }
    }
    else {
      $use_minmax = $use_value = TRUE;
    }

    if ($use_value) {
      if ($which == 'all') {
        $form['value']['value'] = array(
          '#type' => 'textfield',
          '#title' => empty($form_state['exposed']) ? t('Value') : '',
          '#size' => 30,
          '#default_value' => $this->value['value'],
          '#dependency' => array($source => $this->operator_values(1)),
        );
        if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier]['value'])) {
          $form_state['input'][$identifier]['value'] = $this->value['value'];
        }
      }
      elseif ($which == 'value') {
        // When exposed we drop the value-value and just do value if
        // the operator is locked.
        $form['value'] = array(
          '#type' => 'textfield',
          '#title' => empty($form_state['exposed']) ? t('Value') : '',
          '#size' => 30,
          '#default_value' => $this->value['value'],
        );
        if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
          $form_state['input'][$identifier] = $this->value['value'];
        }
      }
    }

    if ($which == 'all' || $which == 'minmax') {
      if ($use_minmax) {
        $form['value']['min'] = array(
          '#type' => 'textfield',
          '#title' => empty($form_state['exposed']) ? t('Min') : '',
          '#size' => 30,
          '#default_value' => $this->value['min'],
        );
        $form['value']['max'] = array(
          '#type' => 'textfield',
          '#title' => empty($form_state['exposed']) ? t('And max') : t('And'),
          '#size' => 30,
          '#default_value' => $this->value['max'],
        );

        if ($which == 'all') {
          $dependency = array(
            '#dependency' => array($source => $this->operator_values(2)),
          );

          $form['value']['min'] += $dependency;
          $form['value']['max'] += $dependency;
        }
      }

      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier]['min']) && $use_minmax) {
        $form_state['input'][$identifier]['min'] = $this->value['min'];
      }
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier]['max']) && $use_minmax) {
        $form_state['input'][$identifier]['max'] = $this->value['max'];
      }

      if (!isset($form['value'])) {
        // Ensure there is something in the 'value'.
        $form['value'] = array(
          '#type' => 'value',
          '#value' => NULL,
        );
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";

    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($field);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function op_between($field) {
    if ($this->operator == 'between') {
      $this->query->add_where($this->options['group'], $field, array($this->value['min'], $this->value['max']), 'BETWEEN');
    }
    else {
      $this->query->add_where($this->options['group'], db_or()->condition($field, $this->value['min'], '<=')->condition($field, $this->value['max'], '>='));
    }
  }

  /**
   * {@inheritdoc}
   */
  public function op_simple($field) {
    $this->query->add_where($this->options['group'], $field, $this->value['value'], $this->operator);
  }

  /**
   * {@inheritdoc}
   */
  public function op_empty($field) {
    if ($this->operator == 'empty') {
      $operator = "IS NULL";
    }
    else {
      $operator = "IS NOT NULL";
    }

    $this->query->add_where($this->options['group'], $field, NULL, $operator);
  }

  /**
   * {@inheritdoc}
   */
  public function op_regex($field) {
    $this->query->add_where($this->options['group'], $field, $this->value['value'], 'RLIKE');
  }

  /**
   * {@inheritdoc}
   */
  public function op_not_regex($field) {
    $this->query->add_where($this->options['group'], $field, $this->value['value'], 'NOT RLIKE');
  }

  /**
   * {@inheritdoc}
   */
  public function admin_summary() {
    if ($this->is_a_group()) {
      return t('grouped');
    }
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }

    $options = $this->operator_options('short');
    $output = check_plain($options[$this->operator]);
    if (in_array($this->operator, $this->operator_values(2))) {
      $output .= ' ' . t('@min and @max', array('@min' => $this->value['min'], '@max' => $this->value['max']));
    }
    elseif (in_array($this->operator, $this->operator_values(1))) {
      $output .= ' ' . check_plain($this->value['value']);
    }
    return $output;
  }

  /**
   * Do some minor translation of the exposed input.
   */
  public function accept_exposed_input($input) {
    if (empty($this->options['exposed'])) {
      return TRUE;
    }

    // Rewrite the input value so that it's in the correct format so that the
    // parent gets the right data.
    if (!empty($this->options['expose']['identifier'])) {
      $value = &$input[$this->options['expose']['identifier']];
      if (!is_array($value)) {
        $value = array(
          'value' => $value,
        );
      }
      if (!empty($this->options['expose']['use_operator']) && !empty($this->options['expose']['operator_id']) && isset($input[$this->options['expose']['operator_id']])) {
        if ($input[$this->options['expose']['operator_id']] === 'empty' || $input[$this->options['expose']['operator_id']] === 'not empty') {
          // Parent method will expect a boolean value. We don't ask for a value
          // so we'll force "Yes".
          $value['value'] = 1;
        }
      }
    }

    $rc = parent::accept_exposed_input($input);

    if (empty($this->options['expose']['required'])) {
      // We have to do some of our own checking for non-required filters.
      $info = $this->operators();
      if (!empty($info[$this->operator]['values']) && is_array($value)) {
        switch ($info[$this->operator]['values']) {
          case 1:
            if (array_key_exists('value', $value) && $value['value'] === '') {
              return FALSE;
            }
            break;

          case 2:
            if (array_key_exists('min', $value) && $value['min'] === '' && array_key_exists('max', $value) && $value['max'] === '') {
              return FALSE;
            }
            break;
        }
      }
    }

    return $rc;
  }

}
