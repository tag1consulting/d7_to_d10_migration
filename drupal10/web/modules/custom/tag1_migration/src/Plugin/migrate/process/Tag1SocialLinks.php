<?php

namespace Drupal\tag1_migration\Plugin\migrate\process;

use Drupal\migrate\MigrateExecutableInterface;
use Drupal\migrate\ProcessPluginBase;
use Drupal\migrate\Row;

/**
 *
 * @MigrateProcessPlugin(
 *   id = "tag1_social_links"
 * )
 */
class Tag1SocialLinks extends ProcessPluginBase {

  /**
   * {@inheritdoc}
   */
  public function transform($value, MigrateExecutableInterface $migrate_executable, Row $row, $destination_property) {
    $social_links = [];
    $result = [];

    foreach ($value as $urlField) {
      if (isset($urlField[0]['value'])) {
        $social_links[]  = $urlField[0]['value'];
      }
    }

    $patterns = [
      'twitter' => '/^(http(s)?\:\/\/)?(www\.)?(twitter|x)\.com\//',
      'linkedin' => '/^(http(s)?\:\/\/)?(www\.)?linkedin\.com\//',
      'drupal' => '/^(http(s)?\:\/\/)?(www\.)?drupal\.org\/u\//',
    ];

    foreach ($social_links as $social_link) {
      foreach ($patterns as $schema_column => $pattern) {
        if (preg_match($pattern, $social_link)) {
          $result[] = [
            'social' => $schema_column,
            'link' => preg_replace($pattern, '', $social_link),
          ];
        }
      }
    }

    return $result;
  }
}
