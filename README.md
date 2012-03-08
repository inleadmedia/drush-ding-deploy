DESCRIPTION
===========

This is a standard deployment script for Ding.

Important
---------

You need 

* [Drush](http://drupal.org/project/drush) version >= 4.5
* [Drush_make](http://drupal.org/project/drush_make) version >= 2.3

Setup
-----

We use one generic aliases.drushrc.php file in ~/.drush/:

```php
// Define the customers.
$customers = array(
  'easybib',
  'ringbib',
  'tarnbib',
  'allebib',
  'master',
  'dev-line'
);

// Define the basic options.
$basics = array(
  'uri'                   => 'default',
  'profile-name'          => 'easyprofile',
  'profile-core-version'  => '7.x',
  'profile-url'           => 'git@github.com:inleadmedia/easyprofile.git',
  'remote-host'           => 'app3',
  'remote-user'           => 'ubuntu',
  'path-aliases'          => array(
    '%drush'          => '/usr/local/lib/drush',
    '%drush-script'   => '/usr/share/drush/drush',
  ),
);

$production = array_merge(
  $basics,
  array(
    'env' => 'prod',
  )
);

$staging = array_merge(
  $basics,
  array(
    'env' => 'staging',
  )
);


foreach ($customers as $site) {
  $aliases["$site-prod"] = array_merge(
    $production,
    array(
      'profile-branch' => "$site",
      'root'           => "/var/www/$site.prod",
      'build-path'     => "/var/www/deploy/build/$site",
    )
  );
  $aliases["$site-staging"] = array_merge(
    $staging,
    array(
      'profile-branch' => "$site",
      'root'           => "/var/www/$site.staging",
      'build-path'     => "/var/www/deploy/build/$site",
    )
  );
}
```


Usage
-----

Using an aliases file, you can use the following commands:

`drush ding-deploy-install @prod`

Installs the deploy script in ~/.drush of the remote user.


`drush @prod ding-deploy-setup`

Ensures that the build directory exists, downloads the bootstrap make
file, installs Drupal to the root path if it doesn't exists (unless
overridden with --no-core), and creates a symlink in the profiles
directory to the build directory (unless overridden by --no-symlink).


`drush @prod ding-deploy --code-only`

Deploys to the site. Runs the bootstrap make file and symlinks the new
build into the site. The --code-only is needed when there isn't a
running site yet.


`drush @prod ding-deploy`

Deploys, sets the site offline, makes a database dump and moves the
new build into place. Then it runs updb and additional commands before
setting the site online again. If any of this fails, the entire
deployment is rolled back.


`drush ding-deploy-build @local testbuild`

Builds from the specified profile into
testbuild/profiles/<profile>. This command is not supposed to be used
for deployment, it is used internally by ding-deploy, and is usefull
for creating development sites without using a full build setup.

