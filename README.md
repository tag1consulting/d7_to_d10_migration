# Requirements

[DDEV](https://ddev.com) is required to run this example project. See the [installation guide](https://ddev.readthedocs.io/en/stable/users/install/ddev-installation/) for instructions for your operating system. You can get support via:

* [Discord](https://discord.gg/kDvSFBSZfs)
* The #ddev channel in [Drupal Slack](https://drupal.org/slack)

# Drupal 7 setup

Execute these commands to get a fully populated Drupal 7 site. It will be available at https://migration-drupal7.ddev.site/

```
cd drupal7
ddev start
ddev import-db --file ../assets/drupal7_db.sql.gz
ddev import-files --source ../assets/drupal7_files.tar.gz
ddev restart
ddev launch
ddev drush uli
```

# Drupal 10 setup

Execute these commands to get a Drupal 10 site. It will be available at https://migration-drupal10.ddev.site/

```
cd drupal10
ddev start
ddev composer install
ddev composer si
ddev launch
ddev drush uli
```
