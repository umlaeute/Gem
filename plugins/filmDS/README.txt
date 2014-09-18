filmDS - DirectShow plugin to read film footage from disk
===

this is a re-implementation of the filmDS.

the original plugin used Microsoft's ATL, and is thus unportable to
non-microsoft compilers, including mingw64.
it also depended on the following two libraries:
 - Microsoft Windows SDK
 - Windows Driver Kit
which require a largish portion of your harddisk (about 2GB iirc).

the new implementation does not require anything (iinm) that does not already
come with your compiler.


BUGS
====

#### EOM
when the end-of-film is reached, [pix_film] just hangs on the last frame,
rather than outputting a bang.

#### frame accurate seeking
both the original and the new implementation do a bad job when it comes to frame accurate seeking

#### first frame
it seems that the new implementation will get the first frame wrong
(it rather shows the last one).
this is probably related to the "frame accurate seeking" problem,
but the original implementation did not have this problem

#### speed
it is still absymally slow.
e.g. the following are usage stats (in percent) after loading 
BigBuckBunny:

implementation  || nop | manual | auto
================++=====+========+=====
original filmDS ++  2  +  55    +  20
----------------++-----+--------+-----
new      filmDS ++ 66  +  95    +  97



TODO
====

i think, a lot of code from the original implementation can be taken.

esp. we should set the timebase to FRAMES rather than SECONDS.
