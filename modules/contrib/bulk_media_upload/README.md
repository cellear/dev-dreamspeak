# Bulk Media Upload

With Bulk Media Upload you can upload a batch of (media) files and a
new node will be created for every file. The files are selected via
a browser file upload dialog and can be stored in a media, file or
image field. Also default values can be chosen for the other fields
assigned to the new node,

This is a port from the Drupal module of the same name, version 7.x-1.x-dev

## Status

This is an initial port of the module ready for testing.

## Features

  - Supported Fields: Image, File
  - Integration of plupload for file-uploads
      (Works in almost every browser and device!)
  - Batch entity creation (Fill out one form, the batch process will do the rest)
  - Overview page for generated nodes (Mostly like admin/content/node)
  - Pathalias integration (Settings for a node type will be used)
  - Choose default values (The other fields assigned to the selected bundle
       (node type, taxonomy vocabulary) will be displayed with the selected widgets
        and you may add some default values)
  - Tokens integration for titles (Choose from user, node, file, date and
    many more tokens)


## Installation

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules.
  
  
## Configuration

- Configuration settings are at admin/config/media/bulk_media_upload,
  where you select
  - Which entity type should be generated?
  - Where to redirect after bulk media upload is finished.

- There is a new option 'Bulk Media' at admin/content.

## Help & Documentation

See readme.txt for more information about the background to
this module.


## License

This project is GPL v2 software. See the LICENSE.txt file in this
directory for complete text.
    
        
## Current porting to Backdrop

- Graham Oliver (github.com/Graham-72/)
- JF (github.com/gifad)

## Credits

### Maintainers for Drupal:

- Benedikt Rötsch (axe312)
- Nedjo Rogers (nedjo)


### Acknowledgement

This port to Backdrop would not, of course, be possible without all
the work done by the developer and maintainer of the Drupal module.
