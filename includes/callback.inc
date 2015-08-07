<?php

/**
 * @file
 * Handles all the callbacks.
 *
 */


/**
 * Fetches page metadata for use in the viewer such as width and height.
 *
 * Prints a JSON object repersenting the metadata.
 * This function exits.
 *
 * @param AbstractObject $object
 *   The book object to generate the metadata from.
 */
function islandora_mirador_shared_canvas_manifest_callback(AbstractObject $object) {
        $manifest = islandora_mirador_create_manifest_from_object($object);

        drupal_json_output($manifest);
        drupal_exit();
}

function islandora_mirador_create_manifest_from_object(AbstractObject $object) {
    module_load_include('inc', 'islandora', 'includes/authtokens');
    module_load_include('inc', 'islandora_mirador', 'includes/Canvas');
    module_load_include('inc', 'islandora_mirador', 'includes/SharedCanvasManifest');

    $metadata_uri = url("islandora/object/{$object->id}/datastream/MODS/view", array(
        'absolute' => TRUE,
        'query' => array(
                'token' => islandora_get_object_token($object->id, 'MODS', 2),
        ),
    ));

    // Generate our parent Manifest
    $manifest = new SharedCanvasManifest($object->id, $object->label, $metadata_uri);

    // Grab the individual pages and add them as a Canvas
    try {
        $pages = $object->repository->ri->sparqlQuery("SELECT *
                FROM <#ri>
                WHERE {
                        ?pid <fedora-rels-ext:isMemberOf> <info:fedora/$object->id> .
                }");
    }
    catch(Exception $e) {
        return "Error while accessing children: $e\n";
    }

    foreach($pages as $page) {

        $page_pid = $page['pid']['value'];

        $page_object = $object->repository->getObject($page_pid);
        $page_uri = url("islandora/object/$page_pid", array(
                'absolute' => TRUE
        ));

        $page_object_rels = $page_object['JP2']->relationships;
        $width_rel = $page_object_rels->get('http://islandora.ca/ontology/relsext#', 'width');
        $height_rel = $page_object_rels->get('http://islandora.ca/ontology/relsext#', 'height');
        $width = $width_rel[0]['object']['value'];
        $height = $height_rel[0]['object']['value'];

        $mimetype = $page_object[$dsid]->mimetype;

        $canvas = new Canvas($page_uri, $page_object->label, $page_uri."/datastream/MODS/view");
        $canvas->setImage($page_uri."/datastream/$dsid/view", $mimetype, $width, $height);
        $manifest->addCanvas($canvas);
    }

    return $manifest->getJson();
}