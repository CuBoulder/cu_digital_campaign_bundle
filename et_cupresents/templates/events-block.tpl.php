<?php
/**
 * @file
 * Template for et_cupresents embed entity.
 */

// Setup variables that can have multiple values.
// Should be done somewhere else as a follow-up ticket.
$query_string = '';

// Deal with tags.
if (!empty($variables['tags'])) {
  $tags = explode(',', $variables['tags']);
  array_walk($tags, function(&$item) {
    $item = trim($item);
  });
  $query_string .= '&filters[tags][]=' . implode('&filters[tags][]=', $tags);
}

// Deal with series.
if (!empty($variables['series'])) {
  $series = explode(',', $variables['series']);
  array_walk($series, function (&$item) {
    $item = trim($item);
  });
  $query_string .= '&filters[series][]=' . implode('&filters[series][]=', $series);
}

// Build last part of query string.
$fields = array('venue', 'from', 'to');

foreach ($fields as $value) {
  if (!empty($variables[$value])) {
    $query_string .= "&filters[$value]=$variables[$value]";
  }
}
?>

<div class="cupresents-event-block-wrapper">
  <script src="//bridge.cupresents.org/rss/production?format=js<?php print check_plain($query_string); ?>&include_custom=<?php print check_plain($variables['include_custom']); ?>"></script>
 </div>

