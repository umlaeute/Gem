////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

/* -------------------------- setup function ------------------------------ */
#ifndef MACOSX
#include "../Base/config.h"
#endif

#include "GemMan.h"

// notice that this is not inside of the extern "C"
void MarkEx_setup();

extern "C" {

  void gemwin_setup();
  void gemhead_setup();
  void gemkeyname_setup();
  void gemkeyboard_setup();
  void gemmouse_setup();
#ifndef MACOSX
  void gemtablet_setup();
  void gemorb_setup();
#endif
  void render_trigger_setup();
  
  void accumrotate_setup();
  void alpha_setup();
  void ambient_setup();
  void ambientRGB_setup();
  void color_setup();
  void colorRGB_setup();
  void depth_setup();
  void diffuse_setup();
  void diffuseRGB_setup();
  void emission_setup();
  void emissionRGB_setup();
  void linear_path_setup();
  void ortho_setup();
  void rotate_setup();
  void rotateXYZ_setup();
  void scale_setup();
  void scaleXYZ_setup();
  void separator_setup();
  void shininess_setup();
  void specular_setup();
  void specularRGB_setup();
  void spline_path_setup();
  void translate_setup();
  void translateXYZ_setup();
  
  void circle_setup();
  void colorSquare_setup();
  void cone_setup();
  void cube_setup();
  void cuboid_setup();
  void curve_setup();
  void cylinder_setup();
  void disk_setup();
#ifndef NOMODEL
  void model_setup();
  void multimodel_setup();
#endif
  void teapot_setup();
  void polygon_setup();
  void primTri_setup();
  void rectangle_setup();
  void sphere_setup();
  void square_setup();
#ifndef NOTEXT
  void text2d_setup();
  void text3d_setup();
  void textoutline_setup();
#endif
  void triangle_setup();
  
  void newWave_setup();		//tigital
  
  void world_light_setup();
  void light_setup();
  
  void part_head_setup();
  void part_color_setup();
  void part_draw_setup();
  void part_damp_setup();
  void part_follow_setup();
  void part_gravity_setup();
  void part_killold_setup();
  void part_killslow_setup();
  void part_orbitpoint_setup();
  void part_size_setup();
  void part_source_setup();
  void part_targetcolor_setup();
  void part_targetsize_setup();
  void part_velcone_setup();
  void part_velsphere_setup();
  
  void pix_image_setup();
  void pix_multiimage_setup();
  void pix_draw_setup();
  void pix_imageInPlace_setup();
  void pix_texture_setup();
  void pix_resize_setup();
  void pix_snap_setup();
  void pix_capture_setup();
  void pix_buf_setup();
  void pix_zoom_setup();
  void pix_write_setup();
  void pix_texture2_setup();

  void pix_pix2sig_setup();
  void pix_sig2pix_setup();
		
  void pix_normalize_setup();
  void pix_rgba_setup();
  void pix_2grey_setup();
  void pix_a_2grey_setup();
  void pix_add_setup();
  void pix_alpha_setup();
  void pix_bitmask_setup();
  void pix_clearblock_setup();
  void pix_coloralpha_setup();
  void pix_color_setup();
  void pix_colormatrix_setup();
  void pix_composite_setup();
  void pix_convolve_setup();
  void pix_coordinate_setup();
  void pix_data_setup();
  void pix_flip_setup();
  void pix_gain_setup();
  void pix_invert_setup();
  void pix_mask_setup();
  void pix_multiply_setup();
  void pix_offset_setup();
  void pix_rectangle_setup();
  void pix_subtract_setup();
  void pix_threshold_setup();

  void pix_aging_setup();
  

  void pix_blob_setup();
  void pix_diff_setup();
  void pix_dump_setup();
  void pix_set_setup();
  void pix_hsv2rgb_setup();
  void pix_rgb2hsv_setup();
  void pix_takealpha_setup();
  void pix_curve_setup();
  void pix_histo_setup();

  void pix_test_setup();

  void imageVert_setup();
#ifdef MACOSX
  void pix_filmDarwin_setup();
#else
  void pix_filmNT_setup();
  void pix_filmLinux_setup();
#endif
  void pix_movie_setup();
#ifdef YUV
  void pix_movieYUV_setup();
  
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
  void yuv_emboss_setup();
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
#ifdef MACOSX
  void pix_filmDarwinYUV_setup();
  void yuv_filmDarwin_setup();
#endif //MACOSX
#endif //YUV

//#ifdef MACOSX && VIDEO
  //void pix_video_setup();
  void pix_videoDarwin_setup();
  //void pix_dv_setup();
  //void pix_dvDarwin();
#ifndef MACOSX
  void pix_indycam_setup();
  void pix_videoSGI_setup();
  void pix_videoLinux_setup();
  void pix_videoNT_setup();

  void pix_dv_setup();
  void pix_dvLinux_setup();
  void pix_dvNT_setup();
#endif
  void pix_puzzle_setup();

  void tv_biquad_setup();
  void tv_movement_setup();
  void tv_rtx_setup();

  GEM_EXTERN void Gem_setup()
  {
    // startup GEM
    GemMan::initGem();

    MarkEx_setup();

    // Controls
    gemwin_setup();
    gemhead_setup();
    gemkeyboard_setup();
    gemkeyname_setup();
    gemmouse_setup();
#ifndef MACOSX
    gemorb_setup();
    gemtablet_setup();
#endif
    render_trigger_setup();
    
    // Manipulators
    accumrotate_setup();
    alpha_setup();
    ambient_setup();
    ambientRGB_setup();
    color_setup();
    colorRGB_setup();
    depth_setup();
    diffuse_setup();
    diffuseRGB_setup();
    emission_setup();
    emissionRGB_setup();
    linear_path_setup();
    ortho_setup();
    rotate_setup();
    rotateXYZ_setup();
    scale_setup();
    scaleXYZ_setup();
    separator_setup();
    shininess_setup();
    specular_setup();
    specularRGB_setup();
    spline_path_setup();
    translate_setup();
    translateXYZ_setup();

    // Geos
    circle_setup();
    colorSquare_setup();
    cone_setup();
    cube_setup();
    cuboid_setup();
    curve_setup();
    cylinder_setup();
    disk_setup();
#ifndef NOMODEL
    model_setup();
    multimodel_setup();
#endif
#if HAVE_LIBGLUT || MACOSX
    teapot_setup();
#endif
    polygon_setup();
    primTri_setup();
    rectangle_setup();
    sphere_setup();
    square_setup();
#ifndef NOTEXT
    text2d_setup();
    text3d_setup();
    textoutline_setup();
#endif
    triangle_setup();
    newWave_setup();		//tigital

    // Particles
    part_head_setup();
    part_color_setup();
    part_damp_setup();
    part_draw_setup();
    part_follow_setup();
    part_gravity_setup();
    part_killold_setup();
    part_killslow_setup();
    part_orbitpoint_setup();
    part_source_setup();
    part_size_setup();
    part_targetcolor_setup();
    part_targetsize_setup();
    part_velcone_setup();
    part_velsphere_setup();

    // Non-geos
    world_light_setup();
    light_setup();

    // Pixes
    ////////
    //  Pix-Geos
    pix_image_setup();
    pix_multiimage_setup();
    pix_draw_setup();
    pix_imageInPlace_setup();
    pix_texture_setup();
    pix_texture2_setup();
    pix_snap_setup();

    // Pix-controls
    pix_buf_setup();
    pix_resize_setup();
    pix_zoom_setup();
    pix_write_setup();

    // Pix-manips
    pix_normalize_setup();
    pix_rgba_setup();
    pix_2grey_setup();
    pix_a_2grey_setup();
    pix_add_setup();
    pix_alpha_setup();
    pix_bitmask_setup();
    pix_clearblock_setup();
    pix_coloralpha_setup();
    pix_color_setup();
    pix_colormatrix_setup();
    pix_convolve_setup();
    pix_composite_setup();
    pix_coordinate_setup();
    pix_data_setup();
    pix_flip_setup();
    pix_gain_setup();
    pix_invert_setup();
    pix_mask_setup();
    pix_multiply_setup();
    pix_offset_setup();
    pix_rectangle_setup();
    pix_subtract_setup();
    pix_threshold_setup();

    pix_aging_setup();

    pix_blob_setup();
    pix_diff_setup();
    pix_dump_setup();
    pix_set_setup();
    pix_hsv2rgb_setup();
    pix_rgb2hsv_setup();
    pix_takealpha_setup();
    pix_curve_setup();
    pix_histo_setup();
    pix_test_setup();

    imageVert_setup();

    pix_pix2sig_setup();
    pix_sig2pix_setup();
    pix_puzzle_setup();

    pix_movie_setup();
#ifdef HAVE_DV
#ifdef _WINDOWS

#endif
#ifdef __linux__
    pix_dvLinux_setup();
#endif /* OS */
#else
#ifndef MACOSX
    pix_dv_setup();
#endif
#endif /* HAVE_DV */

#ifdef __sgi
    // Sgi-specific
    pix_videoSGI_setup();
    pix_indycam_setup();
#endif
#ifdef __linux__
    pix_videoLinux_setup();
    pix_filmLinux_setup();
#endif
#ifdef MACOSX
    pix_filmDarwin_setup();
    pix_videoDarwin_setup();
#endif

#ifdef YUV
    pix_movieYUV_setup();
    
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
    yuv_emboss_setup();
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
#ifdef MACOSX
    pix_filmDarwinYUV_setup();
    yuv_filmDarwin_setup();
#endif //MACOSX
#endif //YUV

    // WinNT-specific
#ifdef _WINDOWS
    pix_videoNT_setup();
    pix_filmNT_setup();
#endif

    tv_biquad_setup();
    tv_movement_setup();
    tv_rtx_setup();

  }

  GEM_EXTERN void gem_setup()
  {
    Gem_setup();
  }

  GEM_EXTERN void GEM_setup()
  {
    Gem_setup();
  }

}   // for extern "C"
