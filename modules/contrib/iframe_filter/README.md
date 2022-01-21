# Summary

iframe filter embeds content in an `iframe`, which is useful for embedded
external content or widgets and speeding up ad provider load times.

When used, this will place the entirety of the content into a standalone HTML
page that is included within an `iframe`.

## Requirements

None.

## Installation

1. Install this module using the official Backdrop CMS instructions at
   https://backdropcms.org/guide/modules
2. Visit `Administration` > `Configuration` > `Content authoring` >
   `Text editors and formats` (admin/config/content/formats)
3. Create or edit a format.
4. In Enabled filters, check `Embed in iframe`.
5. In `Filter processing order`, move `Embed in iframe` to the last position.
6. Save Configuration.
  
## Usage

1. Create a block or content that you want to be rendered within an iframe,
   using the previously configured text format.
2. Add an HTML comment that contains the height and width of the iframe using
   one of the following formats: `<!-- =wxh -->` or
   `<!-- width="w" height="h" -->`, replacing the w and h are integers with the
   desired dimensions.
3. Save.

Example:

```html
<!-- width="728" height="90" --><script src="http://../something.js"></script>
```

## Troubleshooting

* If there isn't a valid width or height, the content won't be placed inside an iframe.

## Issues

Bugs and Feature requests should be reported in the Issue Queue:
https://github.com/backdrop-contrib/iframe_filter/issues

## Current Maintainers

- Jon Peck (https://github.com/fluxsauce)
- Seeking additional maintainers

## Credits

- Ported to Backdrop CMS by Jon Peck (https://github.com/fluxsauce).
- The Drupal version of this project is maintained by Javier Reartes (https://www.drupal.org/javierreartes)
- Originally created by Nathan Haug (https://www.drupal.org/quicksketch)

## License

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.
