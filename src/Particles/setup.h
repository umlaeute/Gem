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

   void part_color_setup();
   void part_damp_setup();
   void part_draw_setup();
   void part_follow_setup();
   void part_gravity_setup();
   void part_head_setup();
   void part_killold_setup();
   void part_killslow_setup();
   void part_orbitpoint_setup();
   void part_size_setup();
   void part_source_setup();
   void part_targetcolor_setup();
   void part_targetsize_setup();
   void part_velcone_setup();
   void part_velsphere_setup();


   void Particles_setup() {
      part_color_setup();
      part_damp_setup();
      part_draw_setup();
      part_follow_setup();
      part_gravity_setup();
      part_head_setup();
      part_killold_setup();
      part_killslow_setup();
      part_orbitpoint_setup();
      part_size_setup();
      part_source_setup();
      part_targetcolor_setup();
      part_targetsize_setup();
      part_velcone_setup();
      part_velsphere_setup();
   }
} // for extern "C"

