# Description
BLPConverter - Converts blp files to png files and vice versa.
# Version
**8.5**
- Included with updated version:
    - libpng
    - zlib
- Applied change from libpng
- Created readme and license file

**8.4**
- Added support for BLP_RBGAs.
- Expanded return codes.  Instead of always -1 on error, now a variety of 
  negative numbers are possible.

**8.3**
- Added support for reading BLPs of type "Type 1 Encoding 2 AlphaDepth 4 
  AlphaEncoding 1 (DXT3 four bits alpha)". Note that the program treats them 
  exactly the same as AlphaDepth 8 since there is only one type of DXT3 
  compression (that I know of).

**8.2**
- Fixed output filename bug when using wildcards.

**8.1** 
- Added support for BLP_PAL_A4 format.

**8.0**
- Incorporated David Holland's (david.w.holland@gmail.com) changes no allow for Linux-compatible builds.
- Using squish instead of the ATIlib for DXT compression, as it is cross platform.

**7.0** 
- Textures that have 256 or fewer unique colors now use exactly those colors when palettized.
- Made -r option default.  -s turns it off.

**6.0**
- Support for decoding DXT5 BLPs.
- Improved the way mip level are created for images with alpha.
- Fixed bug which could cause palettized BLPs with alpha to not have their alpha converted correctly. (Occured when the first transparent pixel uses palette entry zero.)

**5.0**
- Fixed bug which was causing -g value to be applied twice when using -r and converting a RGBA PNG to a BLP.
- -g will now work when when converting a Palettized PNG that has no alpha.
- Added -h, which forces cHaracter format BLPs. (palettized, no alpha).

**4.0** 
- Added -e and -n options.
- -g option now works when doing a rgb png -> p8 blp.
- -g option will automatically create an alpha channel for RGB source PNGs.
- Removed palettization spam.
- -m option now supports wildcards, ie. *.blp.

**3.0**
- Removed dependency on DirectX.
- Added -m, -i, and -c options.

**2.0**
- Fixed bug preventing files from being processed if there was a '.' in their path.
- Now setting device format to current display format, in hopes of fixing an error
that people are reporting when the program tries to call CreateDevice().

**1.0**
- Initial release

# Installation
This program requires quite a few things in order to compile correctly:

- Windows 2000 and up.  I'm unsure about the various libraries but at the very 
  least this file calls win32 functions to handle input wildcards.
- palbmp: A pallete library (C) Charles Bloom.  http://www.cbloom.co/m
	I'm opting to comply with section 0 of the Bloom Public License: I'm 
	distributing this program using the GPL.  I've also made some very small 
	changes to it: I commented out the stderr spam.
- crblib: Another library (C) Charles Bloom, which is a dependency of palbmp.
- libpng: The offical PNG reference library. http://www.libpng.org/
- squish: A DXT compression library.  http://www.sjbrown.co.uk/?code=squish
# Issues
- Saving pal/alpha to PNG w/ alpha mask doesn't work right.  The moved palette colors don't get updated in the image
array properly. Noticible when making Mip test image.
- Dropping more than one file it won't convert correctly.


# Authors and contributors
Original author: Patrick Cyr (kobuster@yahoo.com) from version 1.0 to 8.4  
contributor: Grabthar - version 8.5

# Licence
[GPLV2](./LICENCE)
