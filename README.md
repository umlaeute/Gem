GEM - Graphics Environment for Multimedia
=========================================


You can get the current distribution from:
https://gem.iem.at
https://git.iem.at/pd/Gem
https://github.com/umlaeute/Gem

If you want to compile the newest (bleeding edge, and possibly unstable) source
code yourself, you can get a copy from the public git repository:

  https://git.iem.at/pd/Gem.git

There is also a mirror on github:

  https://github.com/umlaeute/Gem

Usually, you will only need the read-only repository, which you can get with

$ git clone https://git.iem.at/pd/Gem.git

# NEW

Gem is now supported by Microsoft W, Linux and macOS-X.
The IRIX version might work (but most probably will not): if you want to use Gem
under IRIX, feel free to make it work and report failure and success to me, so i
can incorporate any needed changes into the main Gem-trunk.

for installation instructions see below

# BUILDING

To build & install GEM run:
~~~
$ ./configure
$ make
$ make install
~~~

note: when using a git-clone of the Gem sources, you first need to run
`./autogen.sh` (which is a wrapper around `autoreconf -fiv`).

note: that you might want to help Gem to find the Pd-headers, e.g. by doing

~~~sh
$ ./configure --with-pd=/usr/include/pd
~~~

try the following, to see more available options:

~~~sh
$ ./configure --help=recursive
~~~

## DEPENDENCIES
Gem can utilize a lot of libraries and frameworks in order to improve its capabilities.
Most of these libraries are optional and will be detected automatically when running `./configure`.
Some libraries won't be detected automatically and you must provide additional information
on how to use them (see `./configure --help=recursive` for a list of options).

### hard (required) dependencies

The only hard requirements of Gem are:

- Pure Data
- openGL
- GLU

### soft (optional) dependencies

Support for text output is optional, but will be compiled into the "core" of Gem
(that is: once it is enabled, you will need the relevant runtime libraries in order to be able to use Gem at all)

| Library/Framework | notes                   |
|-------------------|-------------------------|
| ftgl              | Font rendering support  |
| fribidi           | support for RTL-scripts |

Most dependencies are purely optional and are only used in plugins.
If the runtime libraries are not available, Gem won't be able to use those plugins, but it will otherwise work as normal.


| Library/Framework            | Operating System | notes                                                            |
|------------------------------|------------------|------------------------------------------------------------------|
| sdl2                         |                  | Window managment                                                 |
| glfw3                        |                  | Window managment                                                 |
| assimp                       |                  | 3D model loading                                                 |
| ImageIO                      | macOS            | image reading/writing                                            |
| QuickTime                    | macOS, Windows   | only 32bit OS are supported                                      |
| libquicktime                 | Linux            | film reading/writing                                             |
| ImageMagick                  |                  | image reading/writing (many formats)                             |
| libTIFF                      |                  | TIFF image reading/writing                                       |
| libjpeg                      |                  | JPEG image reading/writing                                       |
| gmerlin_avdec                | (mostly) Linux   | film reading                                                     |
| libdc1394                    | Linux            | video capture from industry-grade "IEEE 1394" cameras            |
| libdv/libraw1394/libiec61883 | Linux            | video capture from consumer-grade "firewire" cameras             |
| libv4l2                      | Linux            | V4L2 video capture and output                                    |
| VLC                          |                  | open media as video capture via VLC                              |
| libvncclient                 |                  | grab frames via the VNC protocol                                 |
| DeckLink                     |                  | video input from  Blackmagic's DeckLink hardware                 |
| NDI                          |                  | NDI frame capture and output                                     |
| AVT                          |                  | grab frames from GiGE-devices via Prosilica's AVT SDK            |
| Halcon                       |                  | grab frames from industrial grade cameras via MVTec's HALCON SDK |
| Pylon                        |                  | grab frames from GiGE-devices via Basler's PYLON SDK             |
| OptiTrack                    |                  | capture video from OptiTrack cameras                             |
|                              |                  |                                                                  |

# INSTALLING
`make install` might work (mostly on linux)

If you want to have full control on where to install the files, use the `prefix` and `libdir` variables,
like so:

~~~sh
make install libdir=~/Documents/Pd/extra prefix=~/Documents/Pd/extra/Gem/xtra
~~~

## PREBUILT BINARIES

### Linux

Chances are high, that Gem is already packaged for your distribution.

Just run

~~~sh
apt install gem
~~~

(or the equivalent).



### Microsoft Windows

To install GEM on W32/W64 from pre-built binaries you have two options:

1. installer (preferred method)
  use the installer executable to install Gem into ...\pd\extra
  (to _build_ the installer you will have to have NSIS installed
  see build/win-nsis for details)

2. `deken` (use Pd's *Find externals* feature)

	please note that the deken package comes with all plugins.
	in most cases, you won't need all of them, and having plugins
	installed that you don't need, will considerably slow down load
	time of patches and might leed to undesired side-effects.
	therefore, if loading is too slow (or you experience weird
	things), it might be a good idea to disable plugins you don't
	need.
	disabling plugins is as simple as deleting them (or moving them
	into a subfolder)

	plugins are files of the form: "gem_<type><NAME>.dll"
	e.g. "gem_filmQT.dll" is a plugin for reading films using the
	QuickTime framework.
    a number of plugins (e.g. videoPYLON, videoHALCON, videoAVT,
    videoNDI, videoDECKLINK,...) require proprietary libraries.
	you need to obtain and install these libraries yourself; if you
    haven't done so or don't own a device that can interact with those
    libraries, you can safely remove these plugins.
	if you have no clue what this is about, these plugins are most
	likely not for you (so remove them)

### Apple macOS-X

Use the `deken` package.


# RUNNING

Just *installing* Gem is not enough !
You will have to tell Pd that it should load that library !!
You cannot create any Gem-objects without having loaded the Gem-library into Pd !!!

Typically, you will load Gem by putting a `[declare -lib Gem]` object into any patch that uses Gem.

Alternatively you could also add Gem to the libraries loaded at startup (via Pd's preference system),
or by starting Pd with the `-lib Gem` cmdline arguments.

Once you've successfully loaded Gem, you should see a kind of a splash screen on the Pd-console.

If loading fails, make sure you have the proper binary for you OS:
- windows: Gem.dll, Gem.m_i386, Gem.m_*
- macOS-X: Gem.pd_darwin, Gem.d_fat, Gem.d_ppc, Gem.d_*
- linux  : Gem.pd_linux, Gem.l_i386, Gem.l_ia64, Gem.l_*
- freeBSD: Gem.pd_freebsd, Gem.b_i386, Gem.b_*
- irix   : Gem.pd_irix
 - ...

# BUG-REPORTS:

Please do not hesitate to report any crashes, weirdnesses or other issues, using
our bugtracker at https://bugs.gem.iem.at
or the gem-dev mailinglist (subscription at http://lists.puredata.info)

If your mail only says "hey, it does not work !", it is an annoyance and no bug-report.
Please specify at least the following things:
- Operating-System (kernel-version,...)
- video-card, driver
- other hardware that is related to your problem (e.g.: camera)
- does your system work with similar applications (e.g.: capture-software, openGL (games, "glxgears"))
- what is the output of Pd when you start it with the "-verbose" flag
	(e.g.:"pd -verbose -lib Gem")

Please do not use the puredata bugtracker for reporting Gem-specific bugs.


# FINAL NOTES
have fun

zmoelnig@iem.at

