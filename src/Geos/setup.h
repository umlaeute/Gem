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

   void circle_setup();
   void colorSquare_setup();
   void cone_setup();
   void cube_setup();
   void cuboid_setup();
   void curve_setup();
   void cylinder_setup();
   void disk_setup();
   void imageVert_setup();
   void model_setup();
   void multimodel_setup();
   void polygon_setup();
   void primTri_setup();
   void rectangle_setup();
   void sphere_setup();
   void square_setup();
   void teapot_setup();
   void text2d_setup();
   void text3d_setup();
   void textoutline_setup();
   void textextruded_setup();
   void triangle_setup();
   void newWave_setup();		//tigital
   void slideSquares_setup();		//tigital
   void rubber_setup();			//tigital
   void ripple_setup();			//tigital
//  void terrain_setup();

   void Geos_setup() {
      circle_setup();
      colorSquare_setup();
      cone_setup();
      cube_setup();
      cuboid_setup();
      curve_setup();
      cylinder_setup();
      disk_setup();
      imageVert_setup();
#ifndef NOMODEL
      model_setup();
      multimodel_setup();
#endif
      polygon_setup();
      primTri_setup();
      rectangle_setup();
      sphere_setup();
      square_setup();
      teapot_setup();
#ifndef NOTEXT
      text2d_setup();
      text3d_setup();
      textoutline_setup();
      textextruded_setup();
#endif
      triangle_setup();
      newWave_setup();		//tigital
      slideSquares_setup();	//tigital
      rubber_setup();		//tigital
      ripple_setup();		//tigital
//      terrain_setup();


   }
} // for extern "C"

