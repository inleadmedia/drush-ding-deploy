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
  $env = drush_get_option('env');
  $profile = drush_get_option('profile-name');
  $newrelic_settings = _newrelic_settings($options['#name'], $env);

  $info_file = $dir . '/profiles/' . $profile . '/' . $profile . '.info';
  $info = _parse_info_file($info_file);

  $newrelic_settings['description'] = 'Deploying ' . $info['version'];
  _newrelic_send_info($newrelic_settings);
}

/**
 * Read settings from newrelic.drushrc.php.
 *
 * @param string $profile
 *   Profile name.
 * @param string $env
 *   Environment.
 *
 * @return array
 *   New Relic settings.
 */
function _newrelic_settings($profile, $env) {
  // Get newrelic settings from alias_path.
  $alias_path = drush_sitealias_alias_path();
  $newrelic_settings = array();
  foreach ($alias_path as $path) {
    $file = realpath($path . '/newrelic.drushrc.php');
    if (file_exists($file)) {
      include $file;
      $newrelic_settings = array_merge($newrelic_settings, $newrelic);
      unset($newrelic);
    }
  }

  $profile = str_replace('@', '', $profile);

  $settings = array(
    'api-key' => $newrelic_settings['api-key'],
    'user' => $newrelic_settings['user'],
  );

  // Site settings.
  $site_profile = str_replace('-' . $env, '', $profile);
  if (!empty($newrelic_settings['profiles'][$site_profile])) {
    $settings = array_merge($settings, $newrelic_settings['profiles'][$site_profile]);
  }

  // Env specific site settings.
  if (!empty($newrelic_settings['profiles'][$profile])) {
    $settings = array_merge($settings, $newrelic_settings['profiles'][$profile]);
  }

  return $settings;
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

  $command = 'curl -H "x-api-key:' . $data['api-key'] . '"';
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
