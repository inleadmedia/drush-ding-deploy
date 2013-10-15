<?php
/**
 * This file contains settings for posting deployment information to
 * New Relic server.
 * It must be placed in same folder as aliases.drushrc.php.
 */

$newrelic = array(
  // Global settings. Will be overwritten by site settings.
  'api-key' => 'your api key', // API key from New Relic.
  'user' => 'global deploy user', // User's full name in New Relic.

  'profiles' => array(
    'artesis' => array( // Same as in ding-deploy. General site settings.
      'app_name' => 'Site: Test 1', // App name in New Relic.
      'application_id' => '123', // App name in New Relic.
      'user' => 'site deploy user' // User's full name in New Relic.
    ),
    'artesis-prod' => array( // Production specific settings, will overwrite general ones.
      'app_name' => 'Site: Test 1 (prod)',
      'user' => 'env deploy user',
    ),
    'artesis-staging' => array( // Production specific settings, will overwrite general ones.
      'app_name' => 'Site: Test 1 (staging)',
      'application_id' => '234',
    ),
  ),
);
