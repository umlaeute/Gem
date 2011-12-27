videoVLC
========
open up a media via VLC and display it as a live video source


USAGE
-----
simply use any URI that VLC is capable of opening as the device name.
e.g.
[device screen://(
  will capture the screen and output that through [pix_video]
[device v4l2:///dev/video1(
  will open /dev/video1 using v4l2 
[device file:///tmp/kottan.flv(
[device /tmp/kottan.flv(
  will open the movie-file 'kottan.flv'


output size
-----------
currently, there is no attempt to guess the original output size of the media you want to play back.
instead, the media is always re-scaled to thhe size you request (or the default).
change it with [dimen <w> <h>(


properties
----------
VLC offers many, many options for configuring the media playback.
you can use them via the properties system.
currently there is no way to get a list of supported properties.
you can get the available options from VLC, e.g.:
$ vlc -H

example:
vlc has an option "--screen-fps <float>"; therefore:
[clearProps, setProps screen-fps 40, device screen://(
 will grab the screen at 40fps (rather than the default 1fps)

the default options are "--noaudio" and "--no-video-title-show"


pre-requisites
--------------
videoVLC depends on VLC-1.1.11
if you want to compile videoVLC yourself, you will also need to have the 
SDK (header-files + libvlc library) installed

on W32, you might want to make sure that you add the full path to VLC to 
your PATH environment variable, so videoVLC can find VLC and all the plugins!
something along the lines of
$ set PATH=%PATH%:%ProgramFiles%\VideoLAN\VLC


AUTHORS
-------
brought to you by IOhannes m zmölnig
