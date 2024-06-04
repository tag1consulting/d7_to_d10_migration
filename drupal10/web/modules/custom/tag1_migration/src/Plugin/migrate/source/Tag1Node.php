<?php

namespace Drupal\tag1_migration\Plugin\migrate\source;

use Drupal\node\Plugin\migrate\source\d7\Node;

// cspell:ignore tnid

/**
 * Drupal 7 all node revisions source, including translation revisions.
 *
 * For available configuration keys, refer to the parent classes.
 *
 * @see \Drupal\migrate\Plugin\migrate\source\SqlBase
 * @see \Drupal\migrate\Plugin\migrate\source\SourcePluginBase
 *
 * @MigrateSource(
 *   id = "tag1_d7_node",
 *   source_module = "node"
 * )
 */
class Tag1Node extends Node {

  /**
   * {@inheritdoc}
   */
  public function query() {
    $query = parent::query();
    $query->condition('n.status', 1);
    return $query;
  }

}
