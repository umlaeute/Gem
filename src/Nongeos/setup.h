////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Copyleft (c) IOhannes m zm�lnig . zmoelnig@iem.kug.ac.at . forum::f�r::uml�ute . IEM
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
/* -------------------------- setup function ------------------------------ */

extern "C" {

   void light_setup();
   void world_light_setup();


   void Nongeos_setup() {
      light_setup();
      world_light_setup();
   }
} // for extern "C"

