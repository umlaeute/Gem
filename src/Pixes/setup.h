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

  void pix_info_setup();
  void pix_2grey_setup();
   void pix_a_2grey_setup();
   void pix_add_setup();
   void pix_aging_setup();
   void pix_alpha_setup();
   void pix_bitmask_setup();
   void pix_blob_setup();
   void pix_buf_setup();
   void pix_clearblock_setup();
   void pix_color_setup();
   void pix_coloralpha_setup();
   void pix_colormatrix_setup();
   void pix_composite_setup();
   void pix_mix_setup();
   void pix_convolve_setup();
   void pix_coordinate_setup();
   void pix_curve_setup();
   void pix_data_setup();
   void pix_diff_setup();
   //void pix_dot_setup();
   void pix_draw_setup();
   void pix_dump_setup();
   void pix_dvLinux_setup();
   void pix_dv_setup();
   void pix_film_setup();
   void pix_filmLinux_setup();
   void pix_filmNT_setup();
   void pix_filmDarwin_setup();
   void pix_filmDarwinYUV_setup();
   void pix_flip_setup();
   void pix_gain_setup();
   void pix_histo_setup();
   void pix_hsv2rgb_setup();
   void pix_image_setup();
   void pix_imageInPlace_setup();
   void pix_indycam_setup();
   void pix_invert_setup();
   void pix_mask_setup();
   void pix_movie_setup();
   void pix_multiimage_setup();
   void pix_multiply_setup();
   void pix_normalize_setup();
   void pix_offset_setup();
   void pix_pix2sig_setup();
   void pix_puzzle_setup();
   void pix_rectangle_setup();
   void pix_resize_setup();
   void pix_rgb2hsv_setup();
   void pix_rgba_setup();
   void pix_yuv_setup();
   void pix_grey_setup();
   void pix_set_setup();
   void pix_sig2pix_setup();
   void pix_snap_setup();
   void pix_subtract_setup();
   void pix_takealpha_setup();
   void pix_test_setup();
   void pix_texture_setup();
   void pix_texture2_setup();
   void pix_threshold_setup();
   void pix_video_setup();
   void pix_videoLinux_setup();
   void pix_videoNT_setup();
   void pix_videoSGI_setup();
   void pix_videoDarwin_setup();

   void pix_write_setup();
   void pix_zoom_setup();

  void pix_biquad_setup();
  void pix_blur_setup();
  void pix_tIIR_setup();

  void pix_delay_setup();
  void pix_movement_setup();
  void pix_rtx_setup();
  

   void Pixes_setup() {
     pix_info_setup();
      pix_2grey_setup();
      pix_a_2grey_setup();
      pix_add_setup();
      pix_aging_setup();
      pix_alpha_setup();
      pix_bitmask_setup();
      pix_blob_setup();   
      pix_buf_setup();
      pix_clearblock_setup();
      pix_color_setup();
      pix_coloralpha_setup();
      pix_colormatrix_setup();
      pix_composite_setup();
      pix_mix_setup();
      pix_convolve_setup();
      pix_coordinate_setup();
      pix_curve_setup();
      pix_data_setup();
      pix_diff_setup();
      //pix_dot_setup();
      pix_draw_setup();
      pix_dump_setup();
#if defined(HAVE_DV) &&  defined(__linux__)
      pix_dvLinux_setup();
#else
#ifndef MACOSX
      pix_dv_setup();
#endif
#endif
      //      pix_film_setup();
#ifdef __linux
      pix_filmLinux_setup();
#endif
#ifdef _WINDOWS
      pix_filmNT_setup();
#endif
      pix_flip_setup();
      pix_gain_setup();
      pix_histo_setup();
      pix_hsv2rgb_setup();
      pix_image_setup();
      pix_imageInPlace_setup();
#ifdef __sgi
      pix_indycam_setup();
#endif
      pix_invert_setup();
      pix_mask_setup();
      pix_movie_setup();
      pix_multiimage_setup();
      pix_multiply_setup();
      pix_normalize_setup();
      pix_offset_setup();
      pix_pix2sig_setup();
      pix_puzzle_setup();
      pix_rectangle_setup();
      pix_resize_setup();
      pix_rgb2hsv_setup();
      pix_rgba_setup();
      pix_yuv_setup();
      pix_grey_setup();
      pix_set_setup();
      pix_sig2pix_setup();
      pix_snap_setup();
      pix_subtract_setup();
      pix_takealpha_setup();
      pix_test_setup();
      pix_texture_setup();
      pix_texture2_setup();
      pix_threshold_setup();
#ifdef __linux__
      pix_videoLinux_setup();
#endif
#ifdef _WINDOWS
      pix_videoNT_setup();
#endif
#ifdef __sgi
      pix_videoSGI_setup();
#endif
      pix_write_setup();
      pix_zoom_setup();

#ifdef MACOSX
    pix_filmDarwinYUV_setup();
    pix_filmDarwin_setup();
    pix_videoDarwin_setup();
#endif
    pix_biquad_setup();
    pix_blur_setup();
    pix_tIIR_setup();
    pix_delay_setup();
    pix_movement_setup();
    pix_rtx_setup();
   }
} // for extern "C"

