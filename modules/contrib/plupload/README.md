#  Plupload

This module integrates the Plupload library version 1.5.8 with Backdrop forms.

This is a port from the Drupal module of the same name, version 7.x-1.7.

The Plupload library provides a JavaScript API for dealing with file uploads.
It supports features like multiple file selection, file type filtering, request chunking, 
client side image scaling and it uses different runtimes to achieve this 
such as HTML 5, Silverlight, Flash, Gears and BrowserPlus.
Visit http://plupload.com for details.

Note that **Versions 2.x and 3.x of the library are currently unsupported**. 
The latest 1.x.x version is 1.5.8 which has been obtained 
from https://github.com/moxiecode/plupload/archive/v1.5.8.zip.


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

Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules.
  
The Plupload library (v1.5.8) is included in this release. 
The site status report will confirm that it is installed.

## License

This project is GPL v2 software. See the LICENSE.txt file in this
directory for complete text.
    
        
## Current porting to Backdrop

- Graham Oliver (github.com/Graham-72/)


## Maintainers for Backdrop

- Seeking a maintainer

## Credits

### Maintainers for Drupal:

- budalokko
- Janez Urevc (slashrsm)
- Murat Günana (muri)
- Justin Ellison (justintime)
- David Rothstein (David_Rothstein)

Development of the Drupal module is supported by PhpStorm.


### Acknowledgement

This port to Backdrop would not, of course, be possible without all
the work done by the developer and maintainer of the Drupal module.

Members of the Backdrop team have contributed to this port.

