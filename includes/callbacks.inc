<?php
/**
 * @file
 * Module callbacks.
 */

/**
 * Mirador manifest callback.
 */
function islandora_mirador_manifest_callback(AbstractObject $object) {
  module_load_include('inc', 'islandora_mirador', 'includes/shared_canvas_builder');
  drupal_json_output(islandora_mirador_get_shared_canvas_manifest($object));
}
