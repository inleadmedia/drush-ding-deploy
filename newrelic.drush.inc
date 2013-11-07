<?php
/**
 * @file
 * Implements sending deployment information to New Relic after ding-deploy.
 */

/**
 * Implements hook_ding_deploy_complete().
 */
function newrelic_ding_deploy_complete($dir) {
  $options = drush_get_merged_options();
  if (!empty($options['newrelic'])) {
    $newrelic_settings = unserialize($options['newrelic']);
  }
  else {
    $alias = drush_sitealias_get_record($options['#name']);
    $newrelic_settings = $alias['newrelic'];
  }

  if (!empty($options['profile-tag'])) {
    $version = $options['profile-tag'];
  }
  else {
    $profile = drush_get_option('profile-name');
    $info_file = $dir . '/profiles/' . $profile . '/' . $profile . '.info';
    $info = _parse_info_file($info_file);
    $version = $info['version'];
  }

  $newrelic_settings['description'] = 'Deploying ' . $version;

  _newrelic_send_info($newrelic_settings);
}

/**
 * Send deployment info to New Relic.
 *
 * @param array $data
 *   Data to be sent. See Events->Deployments->Instructions on New Relic site.
 */
function _newrelic_send_info($data) {
  if (empty($data['app_name']) && empty($data['application_id'])) {
    return;
  }

  $command = '/usr/bin/curl -H "x-api-key:' . $data['api-key'] . '"';
  $command .= ' -d "deployment[description]=' . $data['description'] . '"';

  if (!empty($data['application_id'])) {
    $command .= ' -d "deployment[application_id]=' . $data['application_id'] . '"';
  }
  elseif (!empty($data['app_name'])) {
    $command .= ' -d "deployment[app_name]=' . $data['app_name'] . '"';
  }

  if (!empty($data['revision'])) {
    $command .= ' -d "deployment[revision]=' . $data['revision'] . '"';
  }
  if (!empty($data['changelog'])) {
    $command .= ' -d "deployment[changelog]=' . $data['changelog'] . '"';
  }
  if (!empty($data['user'])) {
    $command .= ' -d "deployment[user]=' . $data['user'] . '"';
  }

  $command .= ' https://rpm.newrelic.com/deployments.xml';

  drush_shell_exec($command);
}
