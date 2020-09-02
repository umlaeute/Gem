NDI-backends for pix_video/pix_record
=====================================

Network Device Interface (NDI) is a royalty-free software standard
developed by NewTek to enable video-compatible products to communicate,
deliver, and receive high-definition video over a computer network in a
high-quality, low-latency manner that is frame-accurate and suitable for
switching in a live production environment. [Wikipedia]

You have to download their SDK from their website, in order to use it with Gem:

	http://ndi.tv/

The NDI plugins for Gem's pix_video/pix_record objects, allow you to send
and receive NDI video streams.


# installation instructions for linux

## TL;DR
When building Gem, the NDI plugins should be automatically built.
However, in order to *use* them, you must obtain the NDK runtime
(libraries) from http://ndi.tv/

## Longer build instructions

### obvious prerequisites
You need a C++-compiler, Pd (inclusing headers) and the Gem-sources
(including the NDI plugin).
You should have Gem running (preferrably self-compiled)

## getting NDI

For compiling only, you can use the included NDI headers.  However, if
you want to *use* the plugin, you will need the NDI runtime and/or the
NDI SDK (which includes both the NDI headers and the NDI runtime.

Download NDI from http://ndi.tv/ and install it onto your computer (and
make sure to read their installation instructions).
The directory you installed into shall henceforward be referred to as
NDIROOT.  Note that the NDIROOT must not contain any whitespace
characters.


## compiling the NDI plugins

Open a terminal and enter *this* directory (where this README is found)

~~~
$ cd [Gem]/src/plugins/NDI/
~~~

If you haven't done so yet, run autoreconf (if you built Gem from source,
you might have already done so):

~~~
$ autoreconf -fiv
~~~

Now run `configure`
You will have to tell configure where to find your NDI.

If you want to use the included NDI headers use:

~~~
$ ./configure --with-ndi=local
~~~

If you want to use the headers from the downloaded SDK, use this instead:

~~~
$ ./configure --with-ndi=${NDIROOT}
~~~

You might also have to tell configure where to find the Pd-sources by
specifying the /path/to/pd with the "--with-pd=" flag.
For more information try:

~~~
$ ./configure --help
~~~


If all went well, you should see a line "checking for NDI... yes" near
the end of configure's output.

Now run the build:

~~~
$ make
~~~

You will get an error message if something failed.
You should carefully read the error message to see what went wrong.

If you want, you can install the plugin into /usr/local/lib/pd/extra/Gem
(or wherever you specified with the "--libdir=" flag) by running:

~~~
$ sudo make install
~~~

You can also manually install the plugin, by copying the files
".libs/gem_*NDI.so" next to your Gem-binary.


# using the NDI plugins

## using videoNDI

The *videoNDI* plugin enables `[pix_video]` to receive NDI-streams.

1. start Pd/Gem
2. create an object [pix_video]
3. on the Pd-console, you should see (among other things) something like:

~~~
"[pix_video]: backend #0='NDI'        : ndi"
~~~

   The backend-# will be different) depending on the number of
   video-backends found on your system; it's only important that you find
   one backend named "NDI".

   If this didn't work, it's probably because Gem couldn't find the NDI
   library. In this case it should complain that it couldn't find
   "libndi.so.4" or similar. You might even get a hint where to get the
   binaries from.
   In any case, find the missing file in the NDI SDK and put it besides
   the gem_videoNDI.so file.

4. Once the plugin loaded correctly, you can start using it.
   Tell [pix_video] to open a device named like "<hostname> (<streamname>)".
   There's a space between the <hostname> and the parenthesized
   <streamname> (and possibly there are spaces in the <streamname>) which
   you must escape (e.g. using a backslash).

   example:

   - `[device MYMACHINE\ (TestStream)(`
 
      this will use the NDI stream "TestStream" sent from "MyMachine".

   - `[device 127.0.0.1:5961(`

      this will use an NDI stream sent from the local machine on port 5961.

## using recordNDI

The *recordNDI* plugin enables `[pix_record]` to send NDI-streams.

1. start Pd/Gem
2. create a `[pix_record]` object (and connect it to some pix stream)
3. send a `[codec ndi(` message to `[pix_record]`
4. send a `[filename TestStream(` message to `[pix_record]`
5. start streaming by sending a `[record 1(` message the object
6. this will emit an NDI stream with the name "MYMACHINE (TestStream)",
where 'MYMACHINE' is the hostname of your machine (in capitals), and
'TestStream' is the name you just gave to your stream.

# Colophon

fmgasdr
IOhannes m zmölnig
Graz, 01.09.2020






