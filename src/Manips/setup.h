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


   void Manips_setup() {
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
   }
} // for extern "C"

