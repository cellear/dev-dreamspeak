<?php


/**
 * @file
 * General file handling code for Backup and Migrate.
 */

define('BACKUP_MIGRATE_FILENAME_MAXLENGTH', 255);

/**
 * Add a file to the temporary files list for deletion when we're done.
 */
function backup_migrate_temp_files_add($filepath = NULL) {
  static $files = array();
  if (!$filepath) {
    return $files;
  }
  else {
    $files[] = $filepath;
  }
}

/**
 * Delete all temporary files.
 */
function _backup_migrate_temp_files_delete() {
  if (config_get('backup_migrate.settings','backup_migrate_cleanup_temp_files')) {
    // Delete the temp files created during this run.
    foreach (backup_migrate_temp_files_add() as $file) {
      if (file_exists($file) && is_writable($file)) {
        _backup_migrate_temp_files_delete_file($file);
      }
    }

    // Delete temp files abandoned for 6 or more hours.
    $dir = file_directory_temp();
    $expire = time() - config_get('backup_migrate.settings','backup_migrate_cleanup_time');
    if (file_exists($dir) && is_dir($dir) && is_readable($dir) && $handle = opendir($dir)) {
      while (FALSE !== ($file = @readdir($handle))) {
        // Delete 'backup_migrate_' files in the temp directory that are older than the expire time.
        // We should only attempt to delete writable files to prevent errors in shared environments.
        // This could still cause issues in shared environments with poorly configured file permissions.
        if (strpos($file, 'backup_migrate_') === 0 && is_writable("$dir/$file") && @filectime("$dir/$file") < $expire) {
          _backup_migrate_temp_files_delete_file("$dir/$file");
        }
      }
      closedir($handle);
    }
  }
}

/**
 * Delete a temporary file or folder
 */
function _backup_migrate_temp_files_delete_file($file) {
  if (file_exists($file) && (is_writable($file) || is_link($file))) {
    if (!is_link($file) && is_dir($file) && is_readable($file) && $handle = opendir($file)) {
      $dir = $file;
      while (FALSE !== ($file = @readdir($handle))) {
        if ($file != '..' && $file != '.') {
          _backup_migrate_temp_files_delete_file("$dir/$file");
        }
      }
      closedir($handle);
      rmdir($dir);
    }
    else {
      unlink($file);
    }
  }
}

/**
 * Move files recursively.
 */
function _backup_migrate_move_files($from, $to) {
  if (is_readable($from)) {
    if (is_dir($from) && is_readable($from) && $handle = opendir($from)) {
      if (!file_exists($to)) {
        mkdir($to);
      }
      while (FALSE !== ($file = @readdir($handle))) {
        if ($file != '..' && $file != '.') {
          _backup_migrate_move_files("$from/$file", "$to/$file");
        }
      }
    }
    else {
      rename($from, $to);
    }
  }
  return FALSE;
}



/**
 * Create a temporary directory.
 */
function backup_migrate_temp_directory() {
  $tmp = realpath(file_directory_temp());
  // Check the writability of the temp directory.
  if (!is_writable(realpath(file_directory_temp()))) {
    _backup_migrate_message('Your temporary directory %tmp is not writable. Backup and migrate needs to be able to create temporary files.', array('%tmp' => $tmp), 'error');
  }

  // Use a full path so that the files can be deleted during the shutdown function if needed.
  $file = $tmp .'/'. uniqid('backup_migrate_');
  mkdir($file);
  backup_migrate_temp_files_add($file);
  return $file;
}

/**
 * Return a list of backup filetypes.
 */
function _backup_migrate_filetypes() {
  require_once dirname(__FILE__) . '/filters.inc';

  $out = backup_migrate_filters_file_types();

  foreach ($out as $key => $info) {
    $out[$key]['id'] = empty($info['id']) ? $key : $info['id'];
  }
  return $out;
}

/**
 * Adjust the length of a filename to allow for a string to be appended,
 * staying within the maximum filename limit.
 */
function _backup_migrate_filename_append_prepare($filename, $append_str) {
  $max_name_len = BACKUP_MIGRATE_FILENAME_MAXLENGTH - backdrop_strlen($append_str);
  if (backdrop_strlen($filename) > $max_name_len) {
    $filename = backdrop_substr($filename, 0, $max_name_len);
  }
  return $filename;
}

/**
 * Construct a filename using token and some cleaning.
 */
function _backup_migrate_construct_filename($settings) {
  // Get the raw filename from the settings.
  $filename = $settings->filename;

  // Replace any tokens
  $filename = token_replace($filename);

  // Remove illegal characters
  $filename = _backup_migrate_clean_filename($filename);

  // Generate a timestamp if needed.
  $timestamp = '';
  if ($settings->append_timestamp == 1 && $settings->timestamp_format) {
    $timestamp = format_date(time(), 'custom', $settings->timestamp_format);
  }

  // Trim to length if needed to allow the timestamp to fit into the max filename.
  $filename = _backup_migrate_filename_append_prepare($filename, $timestamp);
  $filename .= '-' . $timestamp;
  $filename = trim($filename, '-');

  // If there is no filename, then just call it untitled.
  if (backdrop_strlen($filename) == 0) {
    $filename = 'untitled';
  }
  return $filename;
}

/**
 * Construct a default filename using the site's name.
 */
function _backup_migrate_default_filename() {
  return '[site:name]';
}

/**
 * Clean up a filename to remove unsafe characters.
 */
function _backup_migrate_clean_filename($filename) {
  $filename = preg_replace("/[^a-zA-Z0-9\.\-_]/", "", $filename);
  return $filename;
}

/**
 * An output buffer callback which simply throws away the buffer instead of sending it to the browser.
 */
function _backup_migrate_file_dispose_buffer($buffer) {
  return "";
}


/**
 * A backup file which allows for saving to and reading from the server.
 */
class backup_file {
  public $file_info = array();
  public $type = array();
  public $ext = array();
  public $path = "";
  public $name = "";
  public $handle = NULL;
  public $destination = "";

  /**
   * Construct a file object given a file path, or create a temp file for writing.
   */
  public function __construct($params = array()) {
    if (isset($params['filepath']) && file_exists($params['filepath'])) {
      $this->set_filepath($params['filepath']);
    }
    else {
      $this->set_file_info($params);
      $this->temporary_file();
    }
  }

  /**
   * Get the file_id if the file has been saved to a destination.
   */
  public function file_id() {
    // The default file_id is the filename. Destinations can override the file_id if needed.
    return isset($this->file_info['file_id']) ? $this->file_info['file_id'] : $this->filename();
  }

  /**
   * Get the current filepath.
   */
  public function filepath() {
    if ($filepath = backdrop_realpath($this->path)) {
      return $filepath;
    }
    return $this->path;
  }

  /**
   * Get the final filename.
   */
  public function filename($name = NULL) {
    if ($name) {
      $this->name = $name;
    }
    $extension_str = '.' . $this->extension();
    $this->name = _backup_migrate_filename_append_prepare($this->name, $extension_str);
    return $this->name . $extension_str;
  }

  /**
   * Set the current filepath.
   */
  public function set_filepath($path) {
    $this->path = $path;
    $params = array(
      'filename' => basename($path),
      'file_id' => basename($path)
    );
    if (file_exists($path)) {
      $params['filesize'] = filesize($path);
      $params['filetime'] = filectime($path);
    }
    $this->set_file_info($params);
  }

  /**
   * Get one or all pieces of info for the file.
   */
  public function info($key = NULL) {
    if ($key) {
      return @$this->file_info[$key];
    }
    return $this->file_info;
  }


  /**
   * Get one or all pieces of info for the file.
   */
  public function info_set($key, $value) {
    $this->file_info[$key] = $value;
  }
  /**
   * Get the file extension.
   */
  public function extension() {
    return implode(".", $this->ext);
  }

  /**
   * Get the file type.
   */
  public function type() {
    return $this->type;
  }

  /**
   * Get the file mimetype.
   */
  public function mimetype() {
    return @$this->type['filemime'] ? $this->type['filemime'] : 'application/octet-stream';
  }

  /**
   * Get the file mimetype.
   */
  public function type_id() {
    return @$this->type['id'];
  }

  public function filesize() {
    if (empty($this->file_info['filesize'])) {
      $this->calculate_filesize();
    }
    return $this->file_info['filesize'];
  }

  public function calculate_filesize() {
    $this->file_info['filesize'] = '';
    if (!empty($this->path) && file_exists($this->path)) {
      $this->file_info['filesize'] = filesize($this->path);
    }
  }

  /**
   * Can this file be used to backup to.
   */
  public function can_backup() {
    if (!empty($this->type)) {
      return $this->type['backup'];
    }
    return FALSE;
  }

  /**
   * Can this file be used to restore to.
   */
  public function can_restore() {
    if (!empty($this->type)) {
      return $this->type['restore'];
    }
    return FALSE;
  }

  /**
   * Can this file be used to restore to.
   */
  public function is_recognized_type() {
    if (!empty($this->type)) {
      return $this->type['restore'] || $this->type['backup'];
    }
    return FALSE;
  }

  /**
   * Open a file for reading or writing.
   */
  public function open($write = FALSE, $binary = FALSE) {
    if (!$this->handle) {
      $path = $this->filepath();

      // Check if the file can be read/written.
      if ($write && ((file_exists($path) && !is_writable($path)) || !is_writable(dirname($path)))) {
        _backup_migrate_message('The file %path cannot be written to.', array('%path' => $path), 'error');
        return FALSE;
      }
      if (!$write && !is_readable($path)) {
        _backup_migrate_message('The file %path cannot be read.', array('%path' => $path), 'error');
        return FALSE;
      }

      // Open the file.
      $mode = ($write ? "w" : "r") . ($binary ? "b" : "");
      $this->handle = fopen($path, $mode);
      return $this->handle;
    }
    return NULL;
  }

  /**
   * Close a file when we're done reading/writing.
   */
  public function close() {
    fclose($this->handle);
    $this->handle = NULL;
  }

  /**
   * Write a line to the file.
   */
  public function write($data) {
    if (!$this->handle) {
      $this->handle = $this->open(TRUE);
    }
    if ($this->handle) {
      fwrite($this->handle, $data);
    }
  }

  /**
   * Read a line from the file.
   */
  public function read($size = NULL) {
    if (!$this->handle) {
      $this->handle = $this->open();
    }
    if ($this->handle && !feof($this->handle)) {
      return $size ? fread($this->handle, $size) : fgets($this->handle);
    }
    return NULL;
  }

  /**
   * Write data to the file.
   */
  public function put_contents($data) {
    file_put_contents($this->filepath(), $data);
  }

  /**
   * Read data from the file.
   */
  public function get_contents() {
    return file_get_contents($this->filepath());
  }

  /**
   * Transfer file using http to client. Similar to the built in file_transfer,
   *  but it calls module_invoke_all('exit') so that temp files can be deleted.
   */
  public function transfer() {
    $headers = array(
      array('key' => 'Content-Type', 'value' => $this->mimetype()),
      array('key' => 'Content-Disposition', 'value' => 'attachment; filename="'. $this->filename() .'"'),
    );
    // In some circumstances, web-servers will double compress gzipped files.
    // This may help alleviate that issue by disabling mod-deflate.
    if ($this->mimetype() == 'application/x-gzip') {
      $headers[] = array('key' => 'Content-Encoding', 'value' => 'gzip');
    }
    if ($size = $this->info('filesize')) {
      $headers[] = array('key' => 'Content-Length', 'value' => $size);
    }

    // Suppress the warning you get when the buffer is empty.
    @ob_end_clean();

    if ($this->open(FALSE, TRUE)) {
      foreach ($headers as $header) {
        // To prevent HTTP header injection, we delete new lines that are
        // not followed by a space or a tab.
        // See http://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4.2
        $header['value'] = preg_replace('/\r?\n(?!\t| )/', '', $header['value']);
        backdrop_add_http_header($header['key'], $header['value']);
      }
      // Transfer file in 1024 byte chunks to save memory usage.
      while ($data = $this->read(1024)) {
        print $data;
      }
      $this->close();

      // Ask devel.module not to print it's footer.
      $GLOBALS['devel_shutdown'] = FALSE;
    }
    else {
      backdrop_not_found();
    }

    // Start buffering and throw away the results so that errors don't get appended to the file.
    ob_start('_backup_migrate_file_dispose_buffer');
    backup_migrate_cleanup();
    module_invoke_all('exit');
    exit();
  }

  /**
   * Push a file extension onto the file and return the previous file path.
   */
  function push_type($extension) {
    $types = _backup_migrate_filetypes();
    if ($type = @$types[$extension]) {
      $this->push_filetype($type);
    }

    $out = $this->filepath();
    $this->temporary_file();
    return $out;
  }

  /**
   * Push a file extension onto the file and return the previous file path.
   */
  function pop_type() {
    $out = new backup_file(array('filepath' => $this->filepath()));
    $this->pop_filetype();
    $this->temporary_file();
    return $out;
  }

  /**
   * Set the current file type.
   */
  function set_filetype($type) {
    $this->type = $type;
    $this->ext = array($type['extension']);
  }

  /**
   * Set the current file type.
   */
  function push_filetype($type) {
    $this->ext[] = $type['extension'];
    $this->type = $type;
  }

  /**
   * Pop the current file type.
   */
  function pop_filetype() {
    array_pop($this->ext);
    $this->detect_filetype_from_extension();
  }

  /**
   * Set the file info.
   */
  function set_file_info($file_info) {
    $this->file_info = $file_info;

    if (!empty($this->file_info['filename'])) {
      $this->ext = explode('.', $this->file_info['filename']);
    }
    // Remove the underscores added to file extensions by Drupal's upload security.
    foreach ($this->ext as $key => $val) {
      $this->ext[$key] = trim($val, '_');
    }
    $this->filename(array_shift($this->ext));
    $this->detect_filetype_from_extension();
  }

  /**
   * Get the filetype info of the given file, or false if the file is not a valid type.
   */
  function detect_filetype_from_extension() {
    $ext = end($this->ext);
    $this->type = array();
    $types = _backup_migrate_filetypes();
    foreach ($types as $key => $type) {
      if (trim($ext, "_0123456789") === $type['extension']) {
        $this->type = $type;
        $this->type['id'] = $key;
      }
    }
  }

  /**
   * Get a temporary file name with path.
   */
  function temporary_file() {
    $file = backdrop_tempnam('temporary://', 'backup_migrate_');
    // Add the version without the extension. The tempnam function creates this for us.
    backup_migrate_temp_files_add($file);

    if ($this->extension()) {
      $file .= '.'. $this->extension();
      // Add the version with the extension. This is the one we will actually use.
      backup_migrate_temp_files_add($file);
    }
    $this->path = $file;
  }
}
