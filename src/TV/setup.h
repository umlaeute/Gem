////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Copyleft (c) IOhannes m zmölnig . zmoelnig@iem.kug.ac.at . forum::für::umläute . IEM
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
/* -------------------------- setup function ------------------------------ */

extern "C" {

   void tv_biquad_setup();
   void tv_movement_setup();
   void tv_rtx_setup();
   void tv_test_setup();


   void TV_setup() {
      tv_biquad_setup();
      tv_movement_setup();
      tv_rtx_setup();
      tv_test_setup();
   }
} // for extern "C"

