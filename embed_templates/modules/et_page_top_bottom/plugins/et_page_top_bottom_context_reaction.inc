<?php

/**
 * @file
 * Contains code for et_page_top_bottom context reaction.
 */

/**
 * Class et_page_top_bottom_context_reaction.
 */
class et_page_top_bottom_context_reaction extends context_reaction {

  /**
   * Provide form to enter embeds.
   *
   * @param $context
   * @return array
   */
  function options_form($context) {
    return array(
      'et_page_top_bottom' => array(
        '#type' => 'select',
        '#options' => $this->getEmbeds(),
        '#title' => t('Page Top or Bottom Embed'),
        '#description' => t('Select the embeds you wish to appear on selected pages.'),
      ),
    );
  }

  /**
   * Capture selected embeds to send to execute function.
   *
   * @param $values
   * @return array
   */
  function options_form_submit($values) {
    return array('et_page_top_bottom' => $values['et_page_top_bottom']);
  }

  /**
   * Add embed code to page.
   *
   * @return array
   */
  function execute(&$page) {
    foreach ($this->get_contexts() as $context) {
      if (!empty($context->reactions[$this->plugin])) {

        $id = $context->reactions[$this->plugin]['et_page_top_bottom'];
        $embed = entity_load_single('embed_templates', $id);

        $data = unserialize($embed->data);
        $type = $embed->type;

        $js = _et_page_top_bottom_get_js($data, $type);

        // Add weight based on position.
        $weight = $data['position'] == 'top' ? -9999 : 9999;

        // Add JS to proper section of $page array.
        $page['page_' . $data['position']]['et_page_top_bottom'][] = array(
          '#markup' => $js,
          '#weight' => $weight,
        );
      }
    }
    return $page;
  }

  /**
   * Get all published embeds and return as select list options.
   *
   * @return array
   */
  function getEmbeds() {
    $options = array();

    // Get published embed names.
    $query_options = array(
      'renderer' => array('page_top_bottom'),
      'status' => array('published'),
    );
    $names = embed_templates_get_embeds($query_options);

    // Return names for select list.
    if ($names) {
      foreach ($names as $name) {
        $options[$name->id] = $name->name;
      }
      return $options;
    }

    // Return no options if no published embeds.
    return $options;
  }
}
