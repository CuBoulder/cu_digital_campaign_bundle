<?php

/**
 * @file
 * API hooks for the Embed Templates module.
 */

/**
 * Add your custom embed types.
 *
 * These array definitions are split up into forms and how the data is rendered.
 *
 * array $custom_embed
 *   - label: The human-readable name of your embed type.
 *   - form_callback: This callback generates the form elements for embed creation.
 *   - submission_callback: This callback fires when the embed form is saved.
 *       Useful for doing things before an entity is created/deleted.
 *   - renderer: Machine name of the renderer to use when displaying template.
 *   - path: The path of the module where the template file is located.
 *   - custom_template: You need to enter the relative path to your template file from the module root.
 *
 * @return array
 */
function hook_embed_templates_types() {
  $custom_embeds = array();

  $custom_embeds['my_pixel'] = array(
    'label' => 'My Tracking Pixel',
    'form_callback' => 'my_pixel_form',
    'submission_callback' => 'my_pixel_form_submit',
    'renderer' => 'block',
    'path' => drupal_get_path('module', 'my_module'),
    'template' => 'templates/tracking-pixel-custom-service',
  );

  return $custom_embeds;
}
