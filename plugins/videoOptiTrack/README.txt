videoOptiTrack
==============

backend for pix_video using Natural Point's "CameraSDK" library for their OptiTrack tracking system.

CameraSDK is a proprietary library that can do image-acquisition from NaturalPoint's OptiTrack (tracking) cameras.

At time of writing (2012-01) you can download the CameraSDK free of charge from NaturalPoint's homepage
   http://www.naturalpoint.com/optitrack/products/camera-sdk/

At time of writing (2012-01), only a W32 version of the library exists (no linux, no mac).
Also, the library only offers C++ bindings, which means that (due to name mangling), it will only work 
with videoOptiTrack compiled with VisualStudio (no MinGW).

Please note, that the win-vs2003 project is still a stub (and has not been used yet, so it might nor work).
Development has been done in VS2008.


Working with OptiTrack cameras
------------------------------

Currently you can only acquire images in Greyscale/raw and MJPEG format (depending on the capabilities of your
camera). The default is MJPEG. You can switch to Greyscale by using a "quality" that is <0.
While the cameras (or at least some of them) can do on-board image preprocessing, getting this meta-data is not 
(yet) supported by the videoOptiTrack module.



mfgasdr
IOhannes
