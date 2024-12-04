<?php

// Core settings.

$settings['migrate_source_connection'] = 'migrate';
$settings['migrate_source_version'] = '7';
$settings['migrate_file_public_path'] = 'http://ddev-migration-drupal7-web';
$settings['migrate_file_private_path'] = '';

$settings['migrate_node_migrate_type_classic'] = TRUE;

// Migrate skip fields settings.

$settings['migrate_skip_fields_source_version'] = '7';

$settings['migrate_skip_fields_by_name'] = [
  'node:speaker:*', // Changed to user entity.
  'node:sponsor:*', // Changed to taxonomy term entity.
  'node:swag:*', // Content type no longer needed.
  '*:*:field_image', // Changed to media reference field.
];

$settings['migrate_skip_fields_by_type'] = [
  'youtube', // Changed to media reference field.
];
