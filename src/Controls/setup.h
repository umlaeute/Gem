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

   void gemhead_setup();
   void gemkeyboard_setup();
   void gemkeyname_setup();
   void gemmouse_setup();
   void gemorb_setup();
   void gemtablet_setup();
   void gemwin_setup();
   void render_trigger_setup();


   void Controls_setup() {
      gemhead_setup();
      gemkeyboard_setup();
      gemkeyname_setup();
      gemmouse_setup();
#ifndef MACOSX
      gemorb_setup();
      gemtablet_setup();
#endif
      gemwin_setup();
      render_trigger_setup();
   }
} // for extern "C"

