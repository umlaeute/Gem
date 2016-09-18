videoDECKLINK
=============

DeckLink-backend for pix_video

Blackmagic's DeckLink devices allow image-acquisition of so-called
"professional" video formats, like SDI or HDMI.
You have to acquire their Blackmagic DeckLink SDK in order to use it with Gem.

	http://www.



installation instructions for linux
===================================
0. obvious prerequisites
you need a C++-compiler, Pd (inclusing headers) and the Gem-sources (including
the videoDECKLINK plugin)
you should have Gem running (preferrably self-compiled)

1. getting the DeckLink SDK


TODO: describe this step in detail


2. compiling videoDECKLINK

TODO: describe this step in detail

3. using videoDECKLINK

start Pd/Gem
create an object [pix_video]
on the Pd-console, you should see (among other things) something like:
"[pix_video]: backend #0='DeckLink'        : decklink SDI HDMI"
the backend-# will be different) depending on the number of video-backends found
on your system; it's only important that you find one backend named "DeckLink"

once the plugin loaded correctly, you can start using it.


possible issues
===============


TODO: describe this step in detail



fmgasdr
IOhannes m zmölnig
Graz, 29.06.2015
