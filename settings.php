<?php
/**
 * @file
 * Main Backdrop CMS configuration file.
 */

 $database_prefix = '';

$config_directories['active'] = './config/active';
$config_directories['staging'] = './config/staging';

// $database_charset = 'utf8mb4';

$settings['update_free_access'] = FALSE;

/**
 * Salt for one-time login links and cancel links, form tokens, etc.
 */
$settings['hash_salt'] = 'RzVpyqba_pSicLnJqO26WFsjsqBkLDFvkytDMGykJ2s';

/**
 * Trusted host configuration (optional but highly recommended).
 */
// $settings['trusted_host_patterns'] = array('^www\.example\.com$');

/** if (defined('PANTHEON_ENVIRONMENT')) {
  if (in_array($_ENV['PANTHEON_ENVIRONMENT'], array('dev', 'test', 'live'))) {
    $settings['trusted_host_patterns'][] = "{$_ENV['PANTHEON_ENVIRONMENT']}-{$_ENV['PANTHEON_SITE_NAME']}.pantheon.io";
    $settings['trusted_host_patterns'][] = "{$_ENV['PANTHEON_ENVIRONMENT']}-{$_ENV['PANTHEON_SITE_NAME']}.pantheonsite.io";
  }
}
*/

/**
 * Base URL (optional).
 */
// $base_url = 'http://www.example.com'; // NO trailing slash!

ini_set('session.gc_probability', 1);
ini_set('session.gc_divisor', 100);
ini_set('session.gc_maxlifetime', 200000);
ini_set('session.cookie_lifetime', 2000000);

// ini_set('pcre.backtrack_limit', 200000);
// ini_set('pcre.recursion_limit', 200000);
// $cookie_domain = '.example.com';
// $settings['maintenance_theme'] = 'bartik';
// $settings['reverse_proxy'] = TRUE;
// $settings['reverse_proxy_addresses'] = array('a.b.c.d', ...);
// $settings['reverse_proxy_header'] = 'HTTP_X_CLUSTER_CLIENT_IP';
// $settings['omit_vary_cookie'] = TRUE;
// $settings['form_cache_expiration'] = 21600;

$settings['404_fast_paths_exclude'] = '/\/(?:styles)|(?:system\/files)\//';
$settings['404_fast_paths'] = '/\.(?:txt|png|gif|jpe?g|css|js|ico|swf|flv|cgi|bat|pl|dll|exe|asp)$/i';
$settings['404_fast_html'] = '<!DOCTYPE html><html><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL "@path" was not found on this server.</p></body></html>';

// fast_404();

// $settings['proxy_server'] = '';
// $settings['proxy_port'] = 8080;
// $settings['proxy_username'] = '';
// $settings['proxy_password'] = '';
// $settings['proxy_user_agent'] = '';
// $settings['proxy_exceptions'] = array('127.0.0.1', 'localhost');
// $settings['allow_authorize_operations'] = FALSE;
// $settings['https'] = TRUE;
$settings['backdrop_drupal_compatibility'] = TRUE;
//$config['system.core']['site_name'] = 'My Backdrop site';
//$config['system.core']['file_temporary_path'] = '/tmp';
//$config['system.core']['block_interest_cohort'] = FALSE;

/**
 * Include a local settings file, if available.
 *
 * To make local development easier, you can add a settings.local.php file that
 * contains settings specific to your local installation, or to any secondary
 * environment (staging, development, etc).
 *
 * Typically used to specify a different database connection information, to
 * disable caching, JavaScript/CSS compression, re-routing of outgoing e-mails,
 * Google Analytics, and other things that should not happen on development and
 * testing sites.
 *
 * This local settings file can be ignored in your Git repository, so that any
 * updates to settings.php can be pulled in without overwriting your local
 * changes.
 *
 * Keep this code block at the end of this file to take full effect.
 */
if (file_exists(__DIR__ . '/settings.local.php')) {
  include __DIR__ . '/settings.local.php';
}

/**
 * Pantheon specific compatibility.
 *
 * Override the database information to pass the correct Database credentials
 * directly from Pantheon to Backdrop.
 */
if (isset($_SERVER['PRESSFLOW_SETTINGS'])) {
  $_SERVER['BACKDROP_SETTINGS'] = $_SERVER['PRESSFLOW_SETTINGS'];
}

// Automatically generated include for settings managed by ddev.
$ddev_settings = __DIR__ . '/settings.ddev.php';
if (getenv('IS_DDEV_PROJECT') == 'true' && is_readable($ddev_settings)) {
  require $ddev_settings;
}
