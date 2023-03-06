# pgf2webp

This is a simple C library with an associated Node.js module to convert
[PGF](https://en.wikipedia.org/wiki/Progressive_Graphics_File) images to
[WebP](https://en.wikipedia.org/wiki/WebP). PGF files are used by programs like
[Digikam](https://www.digikam.org/) to store their thumbnails, but are not
easily consumable by web browsers, this is where this module can help.

Given that the Node module is in C++, the C interface wasn't strictly needed,
but it didn't cost much and can come in handy when binding from other
languages.

This code depends on the upstream `libpgf` and `libwebp` for the
decoding/encoding of images.
