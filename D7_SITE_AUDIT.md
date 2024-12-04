# Drupal 7 site audit

## Modules and themes

```
ddev drush pm:list --fields=package,project,name,type,status,version --format=csv | sort > d7_projects.csv
```

## Content types

```sql
SELECT
    nt.module AS provider_module,
    nt.type AS machine_name,
    nt.name,
    nt.description,
    COUNT(n.nid) AS node_count,
    COUNT(CASE WHEN n.status = 0 THEN 1 ELSE NULL END) AS unpublished_count
FROM node_type AS nt
LEFT JOIN node AS n ON n.type = nt.type
GROUP BY nt.type
ORDER BY node_count DESC;
```

## Taxonomy terms

```sql
SELECT
    tv.module,
    tv.vid,
    tv.machine_name,
    tv.name,
    tv.description,
    COUNT(td.tid) AS term_count
FROM taxonomy_vocabulary AS tv
LEFT JOIN taxonomy_term_data AS td ON td.vid = tv.vid
GROUP BY tv.vid
ORDER BY term_count DESC;
```

## Field instances

```sql
SELECT
    fci.entity_type,
    fci.bundle,
    fci.field_name,
    fc.type,
    fc.module
FROM field_config_instance AS fci
INNER JOIN field_config AS fc ON fc.id = fci.field_id
ORDER BY
    entity_type ASC,
    bundle ASC,
    field_name ASC;
```

## Views

One record per view

```sql
SELECT
    vv.base_table,
    vv.name AS machine_name,
    vv.human_name AS label,
    vv.description
FROM views_view AS vv
ORDER BY
    base_table ASC,
    machine_name ASC;
```

One record per view display

```sql
SELECT
    vv.base_table,
    vv.name AS machine_name,
    vv.human_name AS label,
    vv.description,
    vd.display_plugin,
    vd.display_title,
    vd.position AS display_position
FROM views_view AS vv
INNER JOIN views_display AS vd ON vd.vid = vv.vid
ORDER BY
    vv.base_table ASC,
    vv.name ASC,
    vd.position ASC;
```

## Field collections

```sql
SELECT DISTINCT
    field_name AS field_collection
FROM field_collection_item
ORDER BY field_name ASC;
```

## Webform submissions

```sql
SELECT
    n.title AS node_title,
    n.nid AS node_id,
    COUNT(ws.nid) AS submission_count,
    n.status AS node_status,
    w.status AS webform_status
FROM webform AS w
INNER JOIN node AS n ON n.nid = w.nid
LEFT JOIN webform_submissions AS ws ON ws.nid = w.nid
GROUP BY n.nid, n.title, n.status, w.status
ORDER BY submission_count DESC;
```

## Menus

```sql
SELECT
    COUNT(mc.menu_name) AS link_count,
    mc.menu_name AS machine_name,
    mc.title AS name,
    mc.description
FROM menu_custom AS mc
LEFT JOIN menu_links AS ml ON ml.menu_name = mc.menu_name
GROUP BY mc.menu_name
ORDER BY link_count DESC;
```

## Roles

```sql
SELECT
    rid AS role_id,
    name
FROM role
ORDER BY rid ASC;
```

## Files

Filemimes

```sql
SELECT
    COUNT(*) AS mime_count,
    filemime
FROM file_managed
GROUP BY filemime
ORDER BY mime_count DESC;
```

File schemas

```sql
SELECT DISTINCT
    SUBSTRING_INDEX(uri,'://', 1) AS _schema
FROM file_managed;
```

File types when using the [File Entity](https://www.drupal.org/project/file_entity) module

```sql
SELECT DISTINCT
    type
FROM file_managed;
```

## Text formats

Filter formats

```sql
SELECT
    format,
    name
FROM filter_format;
```

Filters

```sql
SELECT DISTINCT
    module,
    name AS filter_name
FROM filter
ORDER BY
    module ASC,
    name ASC;
```

## Image styles

```sql
SELECT
    ims.label AS style_label,
    ims.name AS style_name,
    ime.name AS effect_name,
    ime.weight AS effect_weight
FROM image_styles AS ims
INNER JOIN image_effects AS ime ON ime.isid = ims.isid
ORDER BY
    style_label ASC,
    effect_weight ASC;
```
