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
   void yuv_add_setup();
   void yuv_adjust_setup();
   void yuv_average_setup();
   void yuv_bandw_setup();
   void yuv_bitshift_setup();
   void yuv_blank_setup();
   void yuv_chroma_key_setup();
   void yuv_clamp_setup();
   void yuv_compare_setup();
   void yuv_difference_setup();
   void yuv_dual_setup();
   void yuv_emboss_setup();
   void yuv_film_setup();
   void yuv_filmDarwin_setup();
   void yuv_gain_setup();
   void yuv_invert_setup();
   void yuv_luma_key_setup();
   void yuv_movie_setup();
   void yuv_mult_setup();
   void yuv_pixel_data_setup();
   void yuv_pixel_isolate_setup();
   void yuv_pixel_replace_setup();
   void yuv_posterize_setup();
   void yuv_set_setup();
   void yuv_subtract_setup();


   void yuv_setup() {
#ifdef YUV
      yuv_add_setup();
      yuv_adjust_setup();
      yuv_average_setup();
      yuv_bandw_setup();
      yuv_bitshift_setup();
      yuv_blank_setup();
      yuv_chroma_key_setup();
      yuv_clamp_setup();
      yuv_compare_setup();
      yuv_difference_setup();
      yuv_dual_setup();
      yuv_emboss_setup();
      yuv_film_setup();
#ifdef MACOSX
      yuv_filmDarwin_setup();
#endif
      yuv_gain_setup();
      yuv_invert_setup();
      yuv_luma_key_setup();
      yuv_movie_setup();
      yuv_mult_setup();
      yuv_pixel_data_setup();
      yuv_pixel_isolate_setup();
      yuv_pixel_replace_setup();
      yuv_posterize_setup();
      yuv_set_setup();
      yuv_subtract_setup();
#endif
   }
} // for extern "C"

