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

   void pix_2grey_setup();
   void pix_a_2grey_setup();
   void pix_add_setup();
   void pix_aging_setup();
   void pix_alpha_setup();
   void pix_background_setup();
   void pix_backlight_setup();
   void pix_biquad_setup();
   void pix_bitmask_setup();
   void pix_blob_setup();
   void pix_blur_setup();
   void pix_buf_setup();
   void pix_buffer_setup();
   void pix_buffer_write_setup();
   void pix_buffer_read_setup();
   void pix_chroma_key_setup();
   void pix_clearblock_setup();
   void pix_color_setup();
   void pix_coloralpha_setup();
   void pix_colormatrix_setup();
   void pix_colorreduce_setup();
   void pix_compare_setup();
   void pix_composite_setup();
   void pix_convert_setup();
   void pix_convolve_setup();
   void pix_coordinate_setup();
   void pix_crop_setup();
   void pix_curve_setup();
   void pix_data_setup();
   void pix_delay_setup();
   void pix_depot_setup();
   void pix_diff_setup();
   void pix_dot_setup();
   void pix_draw_setup();
   void pix_dump_setup();
   void pix_duotone_setup();
   void pix_emboss_setup();
   void pix_filmNEW_setup();
   void pix_movieNEW_setup();
   void pix_videoNEW_setup();
#ifdef __APPLE__ 
   void pix_filmDarwin_setup();
   void pix_filmDarwinYUV_setup();
#else
   void pix_filmNT_setup();
   void pix_filmQT_setup();
   void pix_filmFFMPEG_setup();
#endif

   void pix_flip_setup();
   void pix_gain_setup();
   void pix_grey_setup();
   void pix_halftone_setup();
   void pix_histo_setup();
   void pix_hsv2rgb_setup();
   void pix_image_setup();
   void pix_imageInPlace_setup();
   void pix_indycam_setup();
   void pix_info_setup();
   void pix_invert_setup();
   void pix_kaleidoscope_setup();
   void pix_levels_setup();
   void pix_lumaoffset_setup();
   void pix_mask_setup();
   void pix_metaimage_setup();
   void pix_mix_setup();
   void pix_motionblur_setup();
   void pix_movement_setup();
   void pix_movie_setup();
   void pix_movieYUV_setup();
   void pix_multiimage_setup();
   void pix_multiply_setup();
   void pix_normalize_setup();
   void pix_offset_setup();
   void pix_pix2sig_setup();
   void pix_posterize_setup();
   void pix_puzzle_setup();
   void pix_rds_setup();
   void pix_rectangle_setup();
   void pix_refraction_setup();
   void pix_resize_setup();
   void pix_rgb2hsv_setup();
   void pix_rgba_setup();
   void pix_roll_setup();
   void pix_rtx_setup();
   void pix_scanline_setup();
   void pix_set_setup();
   void pix_sig2pix_setup();
   void pix_snap_setup();
   void pix_snap2tex_setup();
   void pix_subtract_setup();
   void pix_takealpha_setup();
   void pix_test_setup();
   void pix_texture_setup();
   void pix_texture2_setup();
   void pix_threshold_setup();
   void pix_tIIR_setup();
   void pix_video_setup();
#ifndef __APPLE__
   void pix_videoLinux_setup();
   void pix_videoNT_setup();
   void pix_videoDS_setup();
   void pix_videoSGI_setup();
#else
   void pix_videoDarwin_setup();
#endif
   void pix_write_setup();
   void pix_yuv_setup();
   void pix_zoom_setup();

  void Pixes_setup() {
   // setting up [pix_film]
#ifdef __NEW__
    pix_movieNEW_setup();
    pix_filmNEW_setup();
#endif // __NEW__

#ifdef __linux__
    pix_movieNEW_setup();
    pix_filmNEW_setup();
#  if defined(HAVE_LIBAVFORMAT) & defined(HAVE_LIBAVCODEC)
    pix_filmFFMPEG_setup();
#  endif
#elif defined _WINDOWS
    pix_filmNT_setup();
#	ifdef HAVE_QUICKTIME
    pix_filmQT_setup();
#	endif
#elif defined __APPLE__
    pix_filmDarwinYUV_setup();
    pix_filmDarwin_setup();
#endif

    // setting up [pix_video]
#ifdef __sgi
    pix_videoSGI_setup();
    pix_indycam_setup();
#elif defined __linux__
    pix_videoNEW_setup();
    pix_videoLinux_setup();
#elif defined _WINDOWS
    pix_videoNT_setup();
#  ifdef HAVE_DIRECTSHOW
    pix_videoDS_setup();
#  endif
#elif defined __APPLE__
    pix_videoDarwin_setup();
#endif

    pix_2grey_setup();
    pix_a_2grey_setup();
    pix_add_setup();
    pix_aging_setup();
    pix_alpha_setup();
    pix_background_setup();
    pix_backlight_setup();
    pix_biquad_setup();
    pix_bitmask_setup();
    pix_blob_setup();
    pix_blur_setup();
    pix_buf_setup();
    pix_buffer_setup();
    pix_buffer_write_setup();
    pix_buffer_read_setup();
    pix_chroma_key_setup();
    pix_clearblock_setup();
    pix_color_setup();
    pix_coloralpha_setup();
    pix_colormatrix_setup();
    pix_colorreduce_setup();
    pix_compare_setup();
    pix_composite_setup();
    pix_convert_setup();
    pix_convolve_setup();
    pix_coordinate_setup();
    pix_crop_setup();
    pix_curve_setup();
    pix_data_setup();
    pix_delay_setup();
    pix_depot_setup();
    pix_diff_setup();
    pix_dot_setup();
    pix_draw_setup();
    pix_dump_setup();
    pix_duotone_setup();
    pix_emboss_setup();
    pix_flip_setup();
    pix_gain_setup();
    pix_grey_setup();
    pix_halftone_setup();
    pix_histo_setup();
    pix_hsv2rgb_setup();
    pix_image_setup();
    pix_imageInPlace_setup();
    pix_info_setup();
    pix_invert_setup();
    pix_kaleidoscope_setup();
    pix_levels_setup();
    pix_lumaoffset_setup();
    pix_mask_setup();
    pix_metaimage_setup();
    pix_mix_setup();
    pix_motionblur_setup();
    pix_movement_setup();
#ifndef __linux__
    pix_movie_setup();
#endif
    pix_movieYUV_setup();
    pix_multiimage_setup();
    pix_multiply_setup();
    pix_normalize_setup();
    pix_offset_setup();
    pix_pix2sig_setup();
    pix_posterize_setup();
    pix_puzzle_setup();
    pix_rds_setup();
    pix_rectangle_setup();
    pix_refraction_setup();
    pix_resize_setup();
    pix_rgb2hsv_setup();
    pix_rgba_setup();
    pix_roll_setup();
    pix_rtx_setup();
    pix_scanline_setup();
    pix_set_setup();
    pix_sig2pix_setup();
    pix_snap_setup();
    pix_snap2tex_setup();
    pix_subtract_setup();
    pix_takealpha_setup();
    pix_test_setup();
    pix_texture_setup();
    pix_texture2_setup();
    pix_threshold_setup();
    pix_tIIR_setup();
    pix_write_setup();
    pix_yuv_setup();
    pix_zoom_setup();
  }
} // for extern "C"
