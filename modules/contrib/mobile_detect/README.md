Mobile Detect (2.8.34)
======================

This is a lightweight mobile detection based on the [Mobile Detect library](https://github.com/serbanghita/Mobile-Detect),
which is bundled in this module. 

This module is intended to aid developers utilizing mobile-first and 
responsive design techniques who also have a need for slight changes for 
mobile and tablet users. An example would be showing (or hiding) a block 
or content pane to a particular device. 

This module is not intended (and never will be enhanced) to provide 
theme switching or redirection; other modules already provide this 
functionality. 

Installation
------------

- Install this module using the [official Backdrop CMS instructions](https://backdropcms.org/guide/modules).

If you think everything is installed correctly but it isn't working, you may
need to clear  the Backdrop caches (**admin/config/development/performance**). 

Usage
------------

The base module just provides a factory method for creating a singleton 
of the mobile detection class, for use in themes and other modules: 

    $detect = mobile_detect_get_object();
    $is_tablet = $detect->isTablet();
    $is_mobile = $detect->isMobile();

See the documentation for the Mobile_Detect library for more information.

Backdrop Page Cache Support
-------------------------

Experimental support is provided for working with the Backdrop page cache.

To use this, enable the mobile_detect_caching module.  And then add
to settings.php file:

    $conf['cache_backends'][] = 'modules/mobile_detect/mobile_detect_caching/mobile_detect_caching.inc';
    $conf['cache_class_cache_page'] = 'MobileDetectCache';
    $conf['mobile_detect_library'] = 'modules/mobile_detect/library/Mobile_Detect.php';

You may need to adjust the path if you installed
the module in a different location.

Developers
------------

If your module requires the library provided by this wrapper, you can access the
library by:

- Requiring this module as a dependency in your module
- Loading the file as follows: `include_once (backdrop_get_path('module', 'mobile_detect') . '/library/Mobile_Detect.php');`

Issues
------

Bugs and Feature requests should be reported in the [Issue Queue](https://github.com/backdrop-contrib/mobile_detect/issues)

Current Maintainers
-------------------

- [Laryn Kragt Bakker](https://github.com/laryn), [CEDC.org](https://CEDC.org) 

Credits
-------

- Ported to Backdrop by [Laryn Kragt Bakker](https://github.com/laryn), [CEDC.org](https://CEDC.org)
- Maintained for Drupal by [nonom](https://www.drupal.org/u/nonom), [darol100](https://www.drupal.org/u/darol100), and [mpdonadio](https://www.drupal.org/u/mpdonadio).


License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.
