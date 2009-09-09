This is GEM 0.92
===================
You can get the current distribution from:
http://gem.iem.at

=============================================
NEW:::
------

Gem is now supported by W32, linux and macOS-X
the IRIX version might work (but most probably will not): if you want to use Gem under IRIX,feel free to make it work and report failure and success to me, so i can incorporate any needed changes into the main Gem-trunk.

for installation instructions see below


NEW (gem-0.87):::
-----------------
(note: this NEWs only refer to the packaging, not to features of Gem)

As with GEM-0.87, i have broken the distribution into various packages
gem-<gemver>.tgz :: quite everything (except binaries and auxiliary libraries)
gem-doc-<gemver>.tgz :: example-patches, manual, ...
gem-bin-<gemver>.zip :: W32-binary (containing a single file "Gem.dll")
gem-bin-doc-<gemver>.zip :: W32-binary + documentation

GemLibs-<OS>-<libver>.tgz :: auxiliary libraries (used to be "AuxLibs")

1) the core Gem-packages:
the core Gem-packages all extract into the same directory gem-<gemver>/
there are install-scripts for windoze (and probably IRIX)
these will install the documentation...


2) the GemLib-package:
I don't know, whether it has much sense, to break the core Gem-package (doc/src/bin),
but i do know, that the GemLibs should be in a distinct package (just for the sake of downloading)

the GemLibs have their own version numbering, starting with 1.
You can get the newest GemLib from the place mentioned above


COMPILING DEPENDENCIES:
=======================
current version: GEM-0.92
(GemLibs	>= 1)
PD 	>= 0.34 (0.41 recommended)

-------------------------------------
-------------------------------------

INSTALLATION:
=============

To install GEM on W32:

a) (preferred method)
  use the installer executable to install Gem into ...\pd\extra
  to build the installer you will have to have NSIS installed
  see build/win-nsis for details

b) do it by hand

     I) installing the files

	1) unzip the GEM package

	2) put the Gem.dll-file somewhere pd can find it (e.g.: ...\pd\extra\Gem)

        3) put all the abstractions besides the Gem.dll (e.g.: ...\pd\extra\Gem)

	4) put all the helpfiles (*-help.pd) besides the Gem.dll (e.g.: ...\pd\extra\Gem)

     II) making Pd aware of Gem

        1) start Pd

        2) add the path where you installed Gem to Pd's search paths

           (go to File->Path... and enter the full path; e.g. C:\Programme\pd\extra\Gem\)

        3) tell Pd to load Gem at startup

           (go to File->Startup... and add "Gem" to the list of libraries to load)

        4) don't forget to "Save all settings"



-------------------------------------
To install GEM on linux:
chdir into <Gemdir>/src/
read the README.<os> (like README.linux) etc...

-------------------------------------
-------------------------------------


RUNNING:
========

just installing Gem is not enough !
you will have to tell pd that it should load that library !!
you cannot create any Gem-objects without having loaded the Gem-library into pd !!!

make sure you have the proper binary for you OS
  - windows: Gem.dll, Gem.m_i386
  - macOS-X: Gem.pd_darwin, Gem.d_fat, Gem.d_ppc
  - linux  : Gem.pd_linux, Gem.l_i386, Gem.l_ia64
  - irix   : Gem.pd_irix
  - freeBSD: Gem.pd_freebsd, Gem.b_i386
  - ...

when starting pd, tell it to load Gem with the "-lib" flag
if your Gem-binary lives in pd/extra/, you could just try "pd -lib Gem"
if your Gem-binary lives somewhere else use something like "pd -lib /path/to/my/Gem"
you could also use "pd -path /path/to/my -lib Gem"
after loading Gem you will see a bit of a welcome message on the konsole

NOTE: there *must not* be any file extension with the "-lib"-flag:: "pd -lib Gem.dll" will miserably fail
NOTE: please note the spelling: it is "Gem" and not "gem" nor "GEM"

if you have problems loading any library including Gem, have a look at the pd-documentation
if you have problems loading just Gem (but other libraries work), send me a bug-report (see below)

BUG-REPORT:
===========
if your mail only says "hey, it does not work !", it is an annoyance and no bug-report
please specify at least the following things:

	Operating-System (kernel-version,...)
	video-card, driver
	other hardware that is related to your problem (e.g.: camera)
	does your system work with similar applications (e.g.: capture-software, openGL (games, "glxgears")
	what is the output of pd when you start it with the "-verbose" flag (e.g.:"pd -verbose -lib Gem")

----
have fun

zmoelnig@iem.at

