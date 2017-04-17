INTRODUCTION
============
This is a fork of the IFF-ILBM [GIMP] plugin by Johannes Teveßen.

[IFF]-[ILBM] is an image file format that was popular on the Amiga series of epic home computers.

It was forked by Emil Brink (<emil@obsession.se>) since Mr Teveßen seems to
have stopped maintaining it, and the most recent version I could find of
the original didn't build against current versions of GIMP (this was created in early 2012, using GIMP 2.6.11).

Before releasing this in early 2012 I did multiple attempts to contact Mr Teveßen,
and later learned that he passed away in 2003. I'm very sorry for the loss of all
those who knew him, and humbly dedicate my work on this code to his memory. I hope
he would appreciate that his code at least gets to live on.

The first forked version was numbered 0.9.9. The current version is 0.10.1.

CHANGES 0.10.2
==============
This is kind of an "anti-release", since it doesn't add anything new.

* Disable the "Save as HAM" mode, since it's become clear it doesn't work at all (see GitHub Issue #4).

CHANGES 0.10.1
==============
* Fix bug that actually caused all saved images to be "slightly" corrupt, by having malformed `BODY`-type chunks.

CHANGES 0.9.12
==============
* Fix bug that prevented images with mask to load properly. Instead you got the left half, with the right one blank.

CHANGES 0.9.11
==============
* Fix bug that broke writing the `CAMG` chunk, it was given the wrong size which resulted in the file being corrupt.

CHANGES 0.9.10
==============
* Change date written in the `ANNO` chunk, it was left at 2002.
* Updated date shown in the GIMP's plug-in browser. Not very maintainable but required.
* Corrected Mr Teveßen's name in the plug-in browser, had incorrectly-encoded `ß`.
* Added the skeleton for an IFF/ILBM parser tool written in Python.

CHANGES AFTER FORKING
=====================
* Modified to compile using current versions of GIMP.
* Changed a lot of coding style things to make it suit me better. Might be considered rude, but I had serious trouble working in the original style and really wanted to have this working.
* Rely on gimptool for building.
* Removed use of autoconf/automake, I consider those to be gross overkill for a project of this size and complexity.
* Added code to detect when the colors look 12-bit, and automatically "smear" them into proper 24-bit colors. This helps retain the intended saturation, but will mis-represent images where *all* colors where really intended to be 12-bit. The smearing is only done if all colors are "0xR0G0B0".

BUILDING
========
1. Make sure you have GIMP's development packages installed, most importantly make sure that `gimptool-2.0` exists on your system.
2. Also make sure you have basic C compilation tools, like a C compiler and a `make` program.
3. Type <kbd>make install</kbd>.

This will compile the plugin, and then invoke `gimptool-2.0`'s installation logic, which will typically *copy* the plugin to your personal plugin folder (somethine like `~/.gimp-2.6/plug-ins/`).

Note that since the built binary is *copied*, you will need to re-do the copying if you change the plugin in any way. You can also manually create a symbolic link from the above location pointing at the binary you just built but that requires you to retain the source code directory which might be undesirable if all you want to do is *use* the plugin (not hack on it).

---
Below is the original README from version 0.9.4:
<pre>
    -DG_DISABLE_ASSERT
/*
 *  This is the IFF-ILBM Gimp plugin by Johannes Teveßen,
 *  eMail: &lt;j.tevessen@gmx.net>
 *
 *  Short:
 *    IFF (InterchangeFileFormat) is a universal data
 *    exchange format developed by Electronic Arts.
 *    It has shipped with the legendary Amiga computers
 *    for years. IFF-ILBM (InterLeavedBitMap) is a
 *    subtype for storing bitmapped images in a non-
 *    chunky, line oriented format.  Great for pictures
 *    upto 256 colors, quite tricky for 24bit ones.
 *      IFF is very extensible; programs shall skip
 *    unknown chunks (blocks of information).  Different
 *    types of data can be stored in the same file
 *    using a recursive or stringent structure.  This
 *    feature is rarely used.  Compression (using a
 *    runlength algorithm) is bad.
 *      Being born into the Amiga custom chipset, IFF-
 *    ILBM supports a CAMG (CommodoreAMiGa) chunk
 *    containing Amiga native flags for the screenmode
 *    (resolution and decoding algorithm) to use.
 *    This includes HAM (Hold-And-Modify, a choice-
 *    reduced 4096..16M mode), EHB (Extra-HalfBrite,
 *    supports twice as many colors, but the upper
 *    half is a mirror of the lower half in 50%
 *    intensity), and asymmetric LoRes, Interlaced,
 *    HiRes, and SuperHiRes modes.  This allowes
 *    geometries of 1:1, 1:2, 1:4, and 2:1.  Two
 *    aspect fields contain more exact informations,
 *    but they're often not fulfilled correctly.
 *      If CAMG is missing, for example, one can
 *    only guess for HAM.
 *
 *  Already there:
 *
 *  o Support for uncompressed and ByteRun1 compressed
 *    bitmaps
 *  o Loading/Saving indexed pictures 1..256 colors
 *  o Loading/Saving transparency (alpha channel) in
 *    1bit-Mode (mask). Loading transparentColor mask.
 *  o Loading/Saving 24bit RGB pictures with 1bit
 *    alpha channel (25 planes in file)
 *  o Promoting indexed pictures to grayscale if possible
 *  o Auto-converting grayscale pictures to 256 color
 *    indexed mode on saving (convert image to indexed
 *    before saving to reduce amount of "colors")
 *  o Loading of HAM6 (as RGB) and EHB6 (as indexed64)
 *  o HAM6 transparency
 *
 *
 *  Missing:
 *
 *  o bmhd.masking == mskLasso (evil without a real blitter,
 *    need to do XOR throughout lines)
 *  o On indexed images, transparent color is ignored
 *    while loading
 *  o mskHasTransparentColor and HAM: Check whether it is okay
 *    to affect only the pixels directly drawn with that pen,
 *    or also add HAMified ones with the same color
 *  o Support for compression mode 2 (which has never really
 *    been important---it compresses column-by-column and
 *    compresses slightly better than ByteRun1)
 *  o Save options requester
 *  o Save HAM/EHB?
 *  o Check for 15/16 bit HighColor ILBMs (?)
 *  o Improve IFF parsing
 *  o Add support for multiple images via catalogs
 *  o Somehow integrate ILBM-TINY thumbnails
 *  o Try to guess HAM if no CAMG chunk is given
 *  o Load option to allow scaling of LoRes/Interlaced-Images
 *  o Add EHB2-5 support (yes, they're out there!)
 *  o Add aspectX/Y support (currently 1/1)
 *  o What about *real* alpha channels? 32planes, mask set?
 *  o Maybe integrate DRNG cycling chains as gradients
 *  o Could also be used to read ILBM palettes
 *  o On 24bit pictures with alpha channel the latter
 *    is only saved as 1bit (in ILBM mode)
 *  o Huge code cleanup. Maybe in spring.
 *
 *
 *  Used code fragments:
 *
 *  o ByteRun1 encode/decoder:
 *    Parts lent from packer.c/unpacker.c by Jerry Morrison
 *    and Steve Shaw, Electronic Arts. This code is placed
 *    in the Public Domain, dated 1985-11-15
 *  o GIMP plugin skeleton:
 *    Code of xpm.c and jpeg.c (GPL)1995 Spencer Kimball and Peter Mattis
 *    and png.c (GPL)1998 Michael Sweet and Daniel Skarda
 *    used as a template
 *
 *  Please report bugs and suggestions.
 *
 *  Changes: see ./ChangeLog).
 */
</pre>

 [GIMP]: http://www.gimp.org/
 [IFF]: http://en.wikipedia.org/wiki/Interchange_File_Format
 [ILBM]: http://en.wikipedia.org/wiki/ILBM
