<?php

// Core settings.

$settings['migrate_source_connection'] = 'migrate';
$settings['migrate_source_version'] = '7';
$settings['migrate_file_public_path'] = 'http://ddev-migration-drupal7-web';
$settings['migrate_file_private_path'] = '';

$settings['migrate_node_migrate_type_classic'] = TRUE;

// Migrate skip fields settings.

$settings['migrate_skip_fields_by_name'] = [
  'node:speaker:*',
  'node:sponsor:*',
  'node:swag:*',
];

$settings['migrate_skip_fields_by_type'] = [
  'youtube',
];

