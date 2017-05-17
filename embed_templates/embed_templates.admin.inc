<?php

/**
 * @file
 * Form functions for embed_templates module.
 */

/**
 * A listing of embed codes.
 *
 * @param array $form
 * @param array $form_state
 * @return array
 */
function embed_templates_overview_form($form, &$form_state) {

  // If set to delete embeds, then return confirmation form.
  if (isset($form_state['storage']['confirm'])) {
    // Hacky way to get action link to not display...I hate myself.
    drupal_add_css('ul.action-links {display: none;}', array('type' => 'inline'));
    return embed_templates_confirm($form, $form_state);
  }

  // Initialize form array.
  $form = array();

  // Set title since it doesn't stick.
  drupal_set_title('Embeds');

  // If search has been performed, then return embeds from the search.
  if (isset($form_state['storage']['search_embeds'])) {
    $embed_templates = $form_state['storage']['search_embeds'];
  } else {
    // Get existing embed codes.
    $embed_templates = embed_templates_get_embeds();
  }

  // Build table only if embed codes exist.
  if (!empty($embed_templates)) {

    // Place embed codes in $form_state for submission processing.
    $form_state['embed_templates'] = $embed_templates;

    // Add search form.
    $form = embed_templates_search_form($form, $form_state);

    // Add table of embeds.
    $form = embed_templates_embeds_table($form, $form_state);
  }
  else {
    drupal_set_message(t('No embed codes exist. You can create one by clicking the action link below.'), 'warning');
  }
  return $form;
}

/**
 * Callback to generate search part of embed overview page.
 *
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function embed_templates_search_form($form, $form_state) {
  // Add search fieldset.
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => 'Search',
  );

  // Search by embed name.
  $form['search']['name'] = array(
    '#title' => t('Embed Name'),
    '#description' => t('Search for embed entity by name.'),
    '#type' => 'textfield',
    '#default_value' => isset($form_state['storage']['search_name']) ? $form_state['storage']['search_name'] : '',
  );

  // Search by path.
  $form['search']['path'] = array(
    '#title' => t('Embed Path'),
    '#description' => t('Search for embed entity by path alias.'),
    '#type' => 'textfield',
    '#default_value' => isset($form_state['storage']['search_path']) ? $form_state['storage']['search_path'] : '',
  );

  // Search by renderer.
  $renderer_options = embed_templates_get_renderers();
  // Have to filter for duplicates.
  $renderer_options = array_flip($renderer_options);
  // Make values human readable.
  array_walk($renderer_options, function(&$value, $key) {$value = ucwords(str_replace('_', ' ', $key));});
  // Add "any" key.
  $renderer_options = array('any' => '-Any-') + $renderer_options;

  $form['search']['renderer'] = array(
    '#title' => t('Embed Renderer'),
    '#description' => t('Search for embed entity by renderer.'),
    '#type' => 'select',
    '#options' => $renderer_options,
    '#default_value' => isset($form_state['storage']['search_renderer']) ? $form_state['storage']['search_renderer'] : '',
  );

  // Search by type.
  $types_options = array_flip(array_keys(embed_templates_get_embed_types()));
  // Make values human readable.
  array_walk($types_options, function(&$value, $key) {$value = ucwords(str_replace('_', ' ', $key));});
  // Add "any" key.
  $types_options = array('any' => '-Any-') + $types_options;

  $form['search']['type'] = array(
    '#title' => t('Embed Type'),
    '#description' => t('Search for embed entity by type.'),
    '#type' => 'select',
    '#options' => $types_options,
    '#default_value' => isset($form_state['storage']['search_type']) ? $form_state['storage']['search_type'] : '',
  );

  // Search by status.
  $form['search']['status'] = array(
    '#title' => t('Embed Status'),
    '#description' => t('Search for embed entity by status.'),
    '#type' => 'select',
    '#options' => array('any' => '-Any-', 'unpublished' => 'Unpublished', 'published' => 'Published'),
    '#default_value' => isset($form_state['storage']['search_status']) ? $form_state['storage']['search_status'] : '',
  );


  $form['search']['search'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#submit' => array('embed_templates_overview_form_submit'),
  );

  // Add container for operations functionality.
  $form['operations'] = array(
    '#type' => 'fieldset',
    '#title' => 'Operations',
  );

  // Add select list for choosing operations.
  $select_options = array(
    'unpublish' => 'Unpublish Embed Codes',
    'publish' => 'Publish Embed Codes',
    'delete' => 'Delete Embed Codes',
  );

  $form['operations']['select'] = array(
    '#type' => 'select',
    '#options' => $select_options,
    '#default_value' => $select_options['unpublish'],
  );

  // Add submit button for operations.
  $form['operations']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Callback to generate table of embeds.
 *
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function embed_templates_embeds_table($form, $form_state) {
  // Add table for listing embed codes.
  $headers = array(
    'id' => t('ID'),
    'name' => t('Name'),
    'renderer' => t('Renderer'),
    'type' => t('Type'),
    'path' => t('Path'),
    'status' => t('Status'),
    'edit' => t('Edit'),
  );

  $rows = array();
  foreach ($form_state['embed_templates'] as $result) {
    $status = ucwords($result->status);
    $edit_link = l(t('Edit'), 'admin/content/embeds/' . $result->type . '/' . $result->id . '/edit');
    $path_link = $result->path == 'N/A' ? 'N/A' : l(drupal_get_path_alias($result->path), $result->path);

    $rows[] = array(
      'id' => $result->id,
      'name' => $result->name,
      'renderer' => $result->renderer,
      'type' => $result->type,
      'path' => $path_link,
      'status' => $status,
      'edit' => $edit_link,
    );
  }

  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $headers,
    '#options' => $rows,
    '#suffix' => empty($rows) ? 'No embed codes have been created.' : '',
    '#weight' => 10,
  );

  $form['pager'] = array(
    '#theme' => 'pager',
    '#weight' => 11,
  );

  return $form;
}

/**
 * Form submission for Embed Code overview form.
 *
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function embed_templates_overview_form_submit(&$form, &$form_state) {

  // If input for search, then pass to search function.
  if ($form_state['values']['op'] == 'Search') {
    $name = !empty($form_state['values']['name']) ? $form_state['values']['name'] : NULL;
    $path = !empty($form_state['values']['path']) ? $form_state['values']['path'] : NULL;
    $renderer = !empty($form_state['values']['renderer']) ? $form_state['values']['renderer'] : NULL;
    $type = !empty($form_state['values']['type']) ? $form_state['values']['type'] : NULL;
    $status = !empty($form_state['values']['status']) ? $form_state['values']['status'] : NULL;

    $options = array(
      'name' => $name,
      'path' => $path,
      'renderer' => $renderer,
      'type' => $type,
      'status' => $status,
    );

    // Betting on !empty() working for NULL values in array.
    if (!empty($options)) {
      return embed_templates_form_search($form_state, $options);
    } else {
      drupal_set_message('No search terms entered.', 'error');
      return '';
    }
  }

  // Make sure to make the user confirm they want to proceed with bulk operation.
  if (!isset($form_state['storage']['confirm'])) {
    // @todo Don't put in whole form_state array since it adds recursively.
    $form_state['storage']['form_state'] = $form_state;
    $form_state['storage']['form'] = $form;
    // This will cause the form to be rebuilt returning to the confirm part of the form.
    $form_state['storage']['confirm'] = TRUE;
    $form_state['rebuild'] = TRUE;
    return $form_state;
  }

  // Perform bulk operation on entities stored in $form_state from confirmation function.
  switch ($form_state['values']['op']) {
    case 'Unpublish':
      foreach ($form_state['embed_templates'] as $pixel) {
        $pixel->status = 'unpublished';
        // Call submission callback in case embeds need to do something for deletion.
        if ($submission_callback = embed_templates_get_submission_callback($pixel->type)) {
          call_user_func($submission_callback, $pixel, 'save');
        }
        entity_save('embed_templates', $pixel);
      }
      break;
    case 'Publish':
      foreach ($form_state['embed_templates'] as $pixel) {
        $pixel->status = 'published';
        // Call submission callback in case embeds need to do something for deletion.
        if ($submission_callback = embed_templates_get_submission_callback($pixel->type)) {
          call_user_func($submission_callback, $pixel, 'save');
        }
        entity_save('embed_templates', $pixel);
      }
      break;
    case 'Delete':
      foreach ($form_state['embed_templates'] as $pixel) {
        // Call submission callback in case embeds need to do something for deletion.
        if ($submission_callback = embed_templates_get_submission_callback($pixel->type)) {
          call_user_func($submission_callback, $pixel, 'delete');
        }
        entity_delete('embed_templates', $pixel->id);
      }
      break;
  }
}

/**
 * Callback for searching for embeds from overview page.
 *
 * @param $form_state
 * @param null $name
 * @param null $path
 * @return mixed
 * @internal param $form
 */
function embed_templates_form_search(&$form_state, $options = array()) {

  // Setup initial query.
  $query = db_select('embed_templates', 'tp')
    ->fields('tp');

  // Need to combine name and path search into one.
  if ($options['name'] != NULL) {
    $or = db_or();
    $or->condition('tp.name', '%' . db_like($options['name']) . '%', 'LIKE');
  }

  // Search by path.
  if ($options['path'] != NULL && $options['path'] != false) {
    // Always add search for path in embed_templates table. Unthemed Pages will
    // never have an entry in the alias table.
    if (!isset($or)) {
      $or = db_or();
    }
    $or->condition('tp.path', '%' . db_like($options['path']) . '%', 'LIKE');

    // Finding system paths is a little more difficult.
    // May need to add conditions for other alias tables.
    $path_results = db_select('url_alias', 'al')
      ->fields('al', array('source'))
      ->condition('al.alias', '%' . db_like($options['path']) . '%', 'LIKE')
      ->execute()
      ->fetchCol();

    if (!empty($path_results)) {
      // If no name is passed in, we have to create the $or object.
      foreach ($path_results as $system_path) {
        $or->condition('tp.path', '%' . db_like($system_path) . '%', 'LIKE');
      }
    }
  }

  // Add search by renderer.
  if ($options['renderer'] != NULL && $options['renderer'] != 'any') {
    $query->condition('tp.renderer', $options['renderer'], '=');
  }

  // Add search by type.
  if ($options['type'] != NULL && $options['type'] != 'any') {
    $query->condition('tp.type', $options['type'], '=');
  }

  // Add search by status.
  if ($options['status'] != NULL && $options['status'] != 'any') {
    $query->condition('tp.status', $options['status'], '=');
  }

  // Only add conditions if the $or object is set.
  if (isset($or)) {
    $query->condition($or);
  }
  $results = $query->execute()->fetchAllAssoc('id');

  // Load embeds to send back to form.
  if ($results) {
    $embed_templates = entity_load('embed_templates', array_keys($results));
    $form_state['storage']['search_embeds'] = $embed_templates;
  } else {
    // Set message if there are no results.
    $form_state['storage']['search_embeds'] =  embed_templates_get_embeds();
    drupal_set_message(t('No results matched your search. All embeds are shown.'), 'error');
  }

  $form_state['storage']['search_path'] = $options['path'];
  $form_state['storage']['search_name'] = $options['name'];
  $form_state['storage']['search_renderer'] = $options['renderer'];
  $form_state['storage']['search_type'] = $options['type'];
  $form_state['storage']['search_status'] = $options['status'];
  $form_state['rebuild'] = TRUE;
  return $form_state;
}

/**
 * Form for adding embeds.
 *
 * @param $form
 * @param $form_state
 * @param null $embed_templates_id
 *
 * @return array
 */
function embed_templates_form($form, &$form_state, $embed_templates_id = NULL) {
  // If set to delete embeds, then return that form.
  if (isset($form_state['storage']['confirm'])) {
    return embed_templates_confirm($form, $form_state);
  }

  // Initialize default form variables.
  $form = array();
  $label = $path = '';
  $status = 'published';
  $data = array();
  $embed_templates = NULL;
  $id = NULL;

  // Load embed if it exists.
  if ($embed_templates_id != NULL && $embed_templates = entity_load_single('embed_templates', $embed_templates_id)) {
    // Grab existing values to place in form.
    $label = $embed_templates->name;
    $path = $embed_templates->path;
    $status = $embed_templates->status;
    $type = $embed_templates->type;
    $data = unserialize($embed_templates->data);
    $id = $embed_templates_id;

    // Store embed in form for form submission processing.
    $form_state['embed_templates'] = $embed_templates;

    // @todo Deal with embed templates entity not loading?
  } else {
    $path_parts = explode('/', current_path());
    $type = array_pop($path_parts);
  }

  $form['embed'] = array(
    '#type' => 'fieldset',
    '#title' => t('Embed'),
    '#weight' => -10,
  );

  $form['embed']['label'] = array(
    '#title' => t('Label'),
    '#description' => t('Administrative label used in overview screen.'),
    '#type' => 'textfield',
    '#default_value' => $label,
    '#required' => TRUE,
  );

  // Search field for existing paths.
  $form['embed']['path'] = array(
    '#title' => t('Path'),
    '#description' => 'The page you will add the embed to. To add embeds to multiple pages, please leave blank and use the Context module.',
    '#type' => 'textfield',
    '#autocomplete_path' => 'titles/autocomplete',
    '#default_value' => $path,
  );

  // Get all status options.
  $publish_options = array(
    'published' => 'Published',
    'unpublished' => 'Unpublished',
  );

  // Add any custom statuses.

  // Add options for publishing.
  $form['publish_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Publishing Options'),
    '#weight' => 11,
  );
  $form['publish_settings']['status'] = array(
    '#title' => t('Status'),
    '#type' => 'select',
    '#options' => $publish_options,
    '#default_value' => $status,
  );

  // Add options for save and cancel.
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('embed_code_form_cancel'),
    '#limit_validation_errors' => array(),
  );

  // Add delete button only if editing an embed.
  if (isset($embed_templates)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#submit' => array('embed_templates_form_submit'),
      '#value' => t('Delete'),
    );
  }

  // Get the specific parts of the embed form.
  if (!isset($form['confirm'])) {
    call_user_func_array(embed_templates_get_form_callback($type), array(&$form, &$form_state, $id, $data));
  }

  return $form;
}

/**
 * Submit function for adding or editing an embed.
 *
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function embed_templates_form_submit(&$form, &$form_state) {

  // If the user has chosen the delete operation, then try to delete embed.
  // Otherwise, go about trying to save embed information.
  if ($form_state['input']['op'] == 'Delete') {
    // If the user hasn't confirmed, they want to delete items, send them to confirm form.
    if (!isset($form_state['storage']['confirm'])) {
      // @todo Don't have recursive $form_state. Probably only store part of the array I need.
      $form_state['storage']['form_state'] = $form_state;
      $form_state['storage']['form'] = $form;
      // This will cause the form to be rebuilt returning to the confirm part of the form.
      $form_state['storage']['confirm'] = TRUE;
      $form_state['rebuild'] = TRUE;
      return $form_state;
    }
    else {
      // Call submission callback in case embeds need to do something for deletion.
      if ($submission_callback = embed_templates_get_submission_callback($form_state['embed_templates']->type)) {
        call_user_func($submission_callback, $form_state['embed_templates'], 'delete');
      }

      // Finally delete embed and redirect to the embed codes overview form.
      entity_delete('embed_templates', $form_state['embed_templates']->id);
      $form_state['redirect'] = 'admin/content/embeds';
    }
  }
  else {

    // Take type form URL or embed entity.
    if (isset($form_state['embed_templates'])) {
      $type = $form_state['embed_templates']->type;
      $renderer = $form_state['embed_templates']->renderer;
    } else {
      $path_parts = explode('/', current_path());
      $type = array_pop($path_parts);
      $renderer = embed_templates_get_renderer($type);
    }

    // Build data array for serialization by grabbing form values for embed type.
    $data = array();
    $options = array();

    foreach ($form['options'] as $key => $value) {
      if (strpos($key, '#') === FALSE) {
        $options[] = $key;
      }
    }

    // @todo Deal with possibility of form elements with same keys. Input values are overwritten if they have the same keys.
    foreach ($form_state['values'] as $key => $value) {
      if (in_array($key, $options)) {
        $data[$key] = $value;
      }
    }

    // Account for embed with no paths. Used for Context and Beans.
    $path = !empty($form_state['values']['path']) ? $form_state['values']['path'] : 'N/A';

    // If editing an embed, add data to existing embed object.
    // Otherwise, make a new embed entity.
    if (isset($form_state['embed_templates']->id)) {
      $embed_templates = $form_state['embed_templates'];
      $embed_templates->name = $form_state['values']['label'];
      $embed_templates->path = $path;
      $embed_templates->type = $type;
      $embed_templates->renderer = $renderer;
      $embed_templates->status = $form_state['values']['status'];
      $embed_templates->data = serialize($data);
    }
    else {
      $values = array(
        'name' => $form_state['values']['label'],
        'path' => $path,
        'type' => $type,
        'renderer' => $renderer,
        'status' => $form_state['values']['status'],
        'data' => serialize($data),
      );
      $embed_templates = entity_create('embed_templates', $values);
    }

    // Call submission callback in case embeds need to do something for creation/save.
    if ($submission_callback = embed_templates_get_submission_callback($type)) {
      call_user_func($submission_callback, $embed_templates, 'save');
    }

    // @todo make sure that the type + name is unique and send form error if not. Probably best to do in validation function.
    $success = $embed_templates->save();

    // Redirect to overview page if successful.
    // Otherwise, show error to user.
    if ($success) {
      $form_state['redirect'] = 'admin/content/embeds';
    }
    else {
      drupal_set_message('Embed code could not be saved. Please try again.', 'error');
    }
  }
}

/**
 * Callback for canceling addition of embeds.
 *
 * @param $form
 * @param $form_state
 */
function embed_code_form_cancel($form, &$form_state) {
  $form_state['redirect'] = 'admin/content/embeds';
}

/**
 * Form callback for confirming operations performed on embed entities.
 *
 * @param $form
 * @param $form_state
 *
 * @return array|mixed|null
 */
function embed_templates_confirm(&$form, &$form_state) {
  // Initialize default variables.
  $path = 'admin/content/embeds';
  $form = array();
  // The operation key differs depending on whether coming from edit or overview form.
  $op = isset($form_state['input']['select']) ? ucfirst($form_state['input']['select']) : ucfirst($form_state['input']['op']);

  // Make sure there are embed codes to operation on.
  if (isset($form_state['embed_templates'])) {
    // If only one embed code...
    if (isset($form_state['embed_templates']->name)) {
      $embed_names = array($form_state['embed_templates']->name);
    }
    else {
      // For multiple pixels, we need to match array key positions with embed IDs.
      $table_options = $form_state['storage']['form']['table']['#options'];
      $selected_options = $form_state['storage']['form_state']['input']['table'];

      // Loop through selected rows and add pixel name.
      $embed_names = array();
      foreach ($selected_options as $key => $option) {
        if ($option != NULL) {
          $embed_names[] = $table_options[$key]['name'];
        }
        else {
          // Replace embeds in form_state array with selected embed codes.
          unset($form_state['embed_templates'][$table_options[$key]['id']]);
        }
      }
    }

    // Build confirm form that is sent back to user.
    $list = theme_item_list(array(
      'items' => $embed_names,
      'title' => NULL,
      'type' => 'ul',
      'attributes' => array()
    ));

    return confirm_form(
      $form,
      t('Do you really want to !op the following embeds?', array('!op' => $op)),
      $path,
      $list,
      $op,
      'Cancel'
    );
  }

  // Account for the case where somehow no embed entities made it to the confirm function.
  return drupal_set_message(t('No embeds selected for operations. Please take appropriate action from the !link.', array('!link' => l('Embeds overview page', $path))), 'error');
}
