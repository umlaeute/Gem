////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemGLUtil.h"
#include <ctype.h>
#include <string.h>

#define _GL_UNDEFINED -10

GEM_EXTERN int getGLbitfield(int argc, t_atom *argv){
  int accum=0;  
  int mode=0;

  if (!argc%2)argc--;
  for (int n=0; n<argc; n++){
    if (n%2){ // && or ||
      char c=*argv->a_w.w_symbol->s_name;
      switch (c) {
      case '|':
	mode = 0;
	break;
      case '&':
	mode=1;
	break;
      default:
	return _GL_UNDEFINED;
      }
      argv++;
    } else {
      int value=getGLdefine(argv++);
      if (value==_GL_UNDEFINED)return _GL_UNDEFINED;
      if (mode==0)accum|=value;
      else accum&=value;
    }
  }
  return accum;

}

GEM_EXTERN int getGLdefine(t_atom *ap)
{
  if (!ap)return _GL_UNDEFINED;
  if (ap->a_type == A_SYMBOL)return getGLdefine(ap->a_w.w_symbol);
  if (ap->a_type == A_FLOAT)return atom_getint(ap);
  return _GL_UNDEFINED;
}

GEM_EXTERN int getGLdefine(t_symbol *s)
{
  if (s && s->s_name)return getGLdefine(s->s_name);
}

GEM_EXTERN int getGLdefine(char *name)
{
    char *c=name;
    int count=0;
    while (*c){*c++=toupper(*c);count++;}
    if (count<4)return _GL_UNDEFINED;
    if (!(name[0]=='G' && name[1]=='L' && name[2]=='_'))return _GL_UNDEFINED;
    name+=3;
    switch (count){
	case 4:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_Q
		else if (!strcmp(name, "Q")) return GL_Q;
#endif
#ifdef GL_R
		else if (!strcmp(name, "R")) return GL_R;
#endif
#ifdef GL_S
		else if (!strcmp(name, "S")) return GL_S;
#endif
#ifdef GL_T
		else if (!strcmp(name, "T")) return GL_T;
#endif
		else return _GL_UNDEFINED;
	case 5:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_2D
		else if (!strcmp(name, "2D")) return GL_2D;
#endif
#ifdef GL_3D
		else if (!strcmp(name, "3D")) return GL_3D;
#endif
#ifdef GL_CW
		else if (!strcmp(name, "CW")) return GL_CW;
#endif
#ifdef GL_OR
		else if (!strcmp(name, "OR")) return GL_OR;
#endif
		else return _GL_UNDEFINED;
	case 6:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_ADD
		else if (!strcmp(name, "ADD")) return GL_ADD;
#endif
#ifdef GL_AND
		else if (!strcmp(name, "AND")) return GL_AND;
#endif
#ifdef GL_BGR
		else if (!strcmp(name, "BGR")) return GL_BGR;
#endif
#ifdef GL_CCW
		else if (!strcmp(name, "CCW")) return GL_CCW;
#endif
#ifdef GL_EXP
		else if (!strcmp(name, "EXP")) return GL_EXP;
#endif
#ifdef GL_FOG
		else if (!strcmp(name, "FOG")) return GL_FOG;
#endif
#ifdef GL_INT
		else if (!strcmp(name, "INT")) return GL_INT;
#endif
#ifdef GL_MAX
		else if (!strcmp(name, "MAX")) return GL_MAX;
#endif
#ifdef GL_MIN
		else if (!strcmp(name, "MIN")) return GL_MIN;
#endif
#ifdef GL_NOR
		else if (!strcmp(name, "NOR")) return GL_NOR;
#endif
#ifdef GL_ONE
		else if (!strcmp(name, "ONE")) return GL_ONE;
#endif
#ifdef GL_RED
		else if (!strcmp(name, "RED")) return GL_RED;
#endif
#ifdef GL_RGB
		else if (!strcmp(name, "RGB")) return GL_RGB;
#endif
#ifdef GL_SET
		else if (!strcmp(name, "SET")) return GL_SET;
#endif
#ifdef GL_V2F
		else if (!strcmp(name, "V2F")) return GL_V2F;
#endif
#ifdef GL_V3F
		else if (!strcmp(name, "V3F")) return GL_V3F;
#endif
#ifdef GL_XOR
		else if (!strcmp(name, "XOR")) return GL_XOR;
#endif
		else return _GL_UNDEFINED;
	case 7:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_AUX0
		else if (!strcmp(name, "AUX0")) return GL_AUX0;
#endif
#ifdef GL_AUX1
		else if (!strcmp(name, "AUX1")) return GL_AUX1;
#endif
#ifdef GL_AUX2
		else if (!strcmp(name, "AUX2")) return GL_AUX2;
#endif
#ifdef GL_AUX3
		else if (!strcmp(name, "AUX3")) return GL_AUX3;
#endif
#ifdef GL_BACK
		else if (!strcmp(name, "BACK")) return GL_BACK;
#endif
#ifdef GL_BGRA
		else if (!strcmp(name, "BGRA")) return GL_BGRA;
#endif
#ifdef GL_BLUE
		else if (!strcmp(name, "BLUE")) return GL_BLUE;
#endif
#ifdef GL_BYTE
		else if (!strcmp(name, "BYTE")) return GL_BYTE;
#endif
#ifdef GL_COPY
		else if (!strcmp(name, "COPY")) return GL_COPY;
#endif
#ifdef GL_DECR
		else if (!strcmp(name, "DECR")) return GL_DECR;
#endif
#ifdef GL_EXP2
		else if (!strcmp(name, "EXP2")) return GL_EXP2;
#endif
#ifdef GL_FILL
		else if (!strcmp(name, "FILL")) return GL_FILL;
#endif
#ifdef GL_FLAT
		else if (!strcmp(name, "FLAT")) return GL_FLAT;
#endif
#ifdef GL_INCR
		else if (!strcmp(name, "INCR")) return GL_INCR;
#endif
#ifdef GL_KEEP
		else if (!strcmp(name, "KEEP")) return GL_KEEP;
#endif
#ifdef GL_LEFT
		else if (!strcmp(name, "LEFT")) return GL_LEFT;
#endif
#ifdef GL_LESS
		else if (!strcmp(name, "LESS")) return GL_LESS;
#endif
#ifdef GL_LINE
		else if (!strcmp(name, "LINE")) return GL_LINE;
#endif
#ifdef GL_LOAD
		else if (!strcmp(name, "LOAD")) return GL_LOAD;
#endif
#ifdef GL_MULT
		else if (!strcmp(name, "MULT")) return GL_MULT;
#endif
#ifdef GL_NAND
		else if (!strcmp(name, "NAND")) return GL_NAND;
#endif
#ifdef GL_NONE
		else if (!strcmp(name, "NONE")) return GL_NONE;
#endif
#ifdef GL_NOOP
		else if (!strcmp(name, "NOOP")) return GL_NOOP;
#endif
#ifdef GL_RGB4
		else if (!strcmp(name, "RGB4")) return GL_RGB4;
#endif
#ifdef GL_RGB5
		else if (!strcmp(name, "RGB5")) return GL_RGB5;
#endif
#ifdef GL_RGB8
		else if (!strcmp(name, "RGB8")) return GL_RGB8;
#endif
#ifdef GL_RGBA
		else if (!strcmp(name, "RGBA")) return GL_RGBA;
#endif
#ifdef GL_TRUE
		else if (!strcmp(name, "TRUE")) return GL_TRUE;
#endif
#ifdef GL_ZERO
		else if (!strcmp(name, "ZERO")) return GL_ZERO;
#endif
		else return _GL_UNDEFINED;
	case 8:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_ACCUM
		else if (!strcmp(name, "ACCUM")) return GL_ACCUM;
#endif
#ifdef GL_ALPHA
		else if (!strcmp(name, "ALPHA")) return GL_ALPHA;
#endif
#ifdef GL_BLEND
		else if (!strcmp(name, "BLEND")) return GL_BLEND;
#endif
#ifdef GL_CLAMP
		else if (!strcmp(name, "CLAMP")) return GL_CLAMP;
#endif
#ifdef GL_CLEAR
		else if (!strcmp(name, "CLEAR")) return GL_CLEAR;
#endif
#ifdef GL_COEFF
		else if (!strcmp(name, "COEFF")) return GL_COEFF;
#endif
#ifdef GL_COLOR
		else if (!strcmp(name, "COLOR")) return GL_COLOR;
#endif
#ifdef GL_DECAL
		else if (!strcmp(name, "DECAL")) return GL_DECAL;
#endif
#ifdef GL_DEPTH
		else if (!strcmp(name, "DEPTH")) return GL_DEPTH;
#endif
#ifdef GL_EQUAL
		else if (!strcmp(name, "EQUAL")) return GL_EQUAL;
#endif
#ifdef GL_EQUIV
		else if (!strcmp(name, "EQUIV")) return GL_EQUIV;
#endif
#ifdef GL_FALSE
		else if (!strcmp(name, "FALSE")) return GL_FALSE;
#endif
#ifdef GL_FLOAT
		else if (!strcmp(name, "FLOAT")) return GL_FLOAT;
#endif
#ifdef GL_FRONT
		else if (!strcmp(name, "FRONT")) return GL_FRONT;
#endif
#ifdef GL_GREEN
		else if (!strcmp(name, "GREEN")) return GL_GREEN;
#endif
#ifdef GL_LINES
		else if (!strcmp(name, "LINES")) return GL_LINES;
#endif
#ifdef GL_NEVER
		else if (!strcmp(name, "NEVER")) return GL_NEVER;
#endif
#ifdef GL_ORDER
		else if (!strcmp(name, "ORDER")) return GL_ORDER;
#endif
#ifdef GL_POINT
		else if (!strcmp(name, "POINT")) return GL_POINT;
#endif
#ifdef GL_QUADS
		else if (!strcmp(name, "QUADS")) return GL_QUADS;
#endif
#ifdef GL_RGB10
		else if (!strcmp(name, "RGB10")) return GL_RGB10;
#endif
#ifdef GL_RGB12
		else if (!strcmp(name, "RGB12")) return GL_RGB12;
#endif
#ifdef GL_RGB16
		else if (!strcmp(name, "RGB16")) return GL_RGB16;
#endif
#ifdef GL_RGBA2
		else if (!strcmp(name, "RGBA2")) return GL_RGBA2;
#endif
#ifdef GL_RGBA4
		else if (!strcmp(name, "RGBA4")) return GL_RGBA4;
#endif
#ifdef GL_RGBA8
		else if (!strcmp(name, "RGBA8")) return GL_RGBA8;
#endif
#ifdef GL_RIGHT
		else if (!strcmp(name, "RIGHT")) return GL_RIGHT;
#endif
#ifdef GL_SHORT
		else if (!strcmp(name, "SHORT")) return GL_SHORT;
#endif
		else return _GL_UNDEFINED;
	case 9:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_ALPHA4
		else if (!strcmp(name, "ALPHA4")) return GL_ALPHA4;
#endif
#ifdef GL_ALPHA8
		else if (!strcmp(name, "ALPHA8")) return GL_ALPHA8;
#endif
#ifdef GL_ALWAYS
		else if (!strcmp(name, "ALWAYS")) return GL_ALWAYS;
#endif
#ifdef GL_BITMAP
		else if (!strcmp(name, "BITMAP")) return GL_BITMAP;
#endif
#ifdef GL_DITHER
		else if (!strcmp(name, "DITHER")) return GL_DITHER;
#endif
#ifdef GL_DOMAIN
		else if (!strcmp(name, "DOMAIN")) return GL_DOMAIN;
#endif
#ifdef GL_DOUBLE
		else if (!strcmp(name, "DOUBLE")) return GL_DOUBLE;
#endif
#ifdef GL_GEQUAL
		else if (!strcmp(name, "GEQUAL")) return GL_GEQUAL;
#endif
#ifdef GL_INVERT
		else if (!strcmp(name, "INVERT")) return GL_INVERT;
#endif
#ifdef GL_LEQUAL
		else if (!strcmp(name, "LEQUAL")) return GL_LEQUAL;
#endif
#ifdef GL_LIGHT0
		else if (!strcmp(name, "LIGHT0")) return GL_LIGHT0;
#endif
#ifdef GL_LIGHT1
		else if (!strcmp(name, "LIGHT1")) return GL_LIGHT1;
#endif
#ifdef GL_LIGHT2
		else if (!strcmp(name, "LIGHT2")) return GL_LIGHT2;
#endif
#ifdef GL_LIGHT3
		else if (!strcmp(name, "LIGHT3")) return GL_LIGHT3;
#endif
#ifdef GL_LIGHT4
		else if (!strcmp(name, "LIGHT4")) return GL_LIGHT4;
#endif
#ifdef GL_LIGHT5
		else if (!strcmp(name, "LIGHT5")) return GL_LIGHT5;
#endif
#ifdef GL_LIGHT6
		else if (!strcmp(name, "LIGHT6")) return GL_LIGHT6;
#endif
#ifdef GL_LIGHT7
		else if (!strcmp(name, "LIGHT7")) return GL_LIGHT7;
#endif
#ifdef GL_LINEAR
		else if (!strcmp(name, "LINEAR")) return GL_LINEAR;
#endif
#ifdef GL_MINMAX
		else if (!strcmp(name, "MINMAX")) return GL_MINMAX;
#endif
#ifdef GL_NICEST
		else if (!strcmp(name, "NICEST")) return GL_NICEST;
#endif
#ifdef GL_POINTS
		else if (!strcmp(name, "POINTS")) return GL_POINTS;
#endif
#ifdef GL_REDUCE
		else if (!strcmp(name, "REDUCE")) return GL_REDUCE;
#endif
#ifdef GL_RENDER
		else if (!strcmp(name, "RENDER")) return GL_RENDER;
#endif
#ifdef GL_REPEAT
		else if (!strcmp(name, "REPEAT")) return GL_REPEAT;
#endif
#ifdef GL_RETURN
		else if (!strcmp(name, "RETURN")) return GL_RETURN;
#endif
#ifdef GL_RGBA12
		else if (!strcmp(name, "RGBA12")) return GL_RGBA12;
#endif
#ifdef GL_RGBA16
		else if (!strcmp(name, "RGBA16")) return GL_RGBA16;
#endif
#ifdef GL_SELECT
		else if (!strcmp(name, "SELECT")) return GL_SELECT;
#endif
#ifdef GL_SMOOTH
		else if (!strcmp(name, "SMOOTH")) return GL_SMOOTH;
#endif
#ifdef GL_STEREO
		else if (!strcmp(name, "STEREO")) return GL_STEREO;
#endif
#ifdef GL_VENDOR
		else if (!strcmp(name, "VENDOR")) return GL_VENDOR;
#endif
#ifdef GL_ZOOM_X
		else if (!strcmp(name, "ZOOM_X")) return GL_ZOOM_X;
#endif
#ifdef GL_ZOOM_Y
		else if (!strcmp(name, "ZOOM_Y")) return GL_ZOOM_Y;
#endif
		else return _GL_UNDEFINED;
	case 10:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_2_BYTES
		else if (!strcmp(name, "2_BYTES")) return GL_2_BYTES;
#endif
#ifdef GL_3_BYTES
		else if (!strcmp(name, "3_BYTES")) return GL_3_BYTES;
#endif
#ifdef GL_422_EXT
		else if (!strcmp(name, "422_EXT")) return GL_422_EXT;
#endif
#ifdef GL_4_BYTES
		else if (!strcmp(name, "4_BYTES")) return GL_4_BYTES;
#endif
#ifdef GL_ALPHA12
		else if (!strcmp(name, "ALPHA12")) return GL_ALPHA12;
#endif
#ifdef GL_ALPHA16
		else if (!strcmp(name, "ALPHA16")) return GL_ALPHA16;
#endif
#ifdef GL_AMBIENT
		else if (!strcmp(name, "AMBIENT")) return GL_AMBIENT;
#endif
#ifdef GL_BGR_EXT
		else if (!strcmp(name, "BGR_EXT")) return GL_BGR_EXT;
#endif
#ifdef GL_C3F_V3F
		else if (!strcmp(name, "C3F_V3F")) return GL_C3F_V3F;
#endif
#ifdef GL_COMPILE
		else if (!strcmp(name, "COMPILE")) return GL_COMPILE;
#endif
#ifdef GL_DIFFUSE
		else if (!strcmp(name, "DIFFUSE")) return GL_DIFFUSE;
#endif
#ifdef GL_FASTEST
		else if (!strcmp(name, "FASTEST")) return GL_FASTEST;
#endif
#ifdef GL_FOG_BIT
		else if (!strcmp(name, "FOG_BIT")) return GL_FOG_BIT;
#endif
#ifdef GL_FOG_END
		else if (!strcmp(name, "FOG_END")) return GL_FOG_END;
#endif
#ifdef GL_GREATER
		else if (!strcmp(name, "GREATER")) return GL_GREATER;
#endif
#ifdef GL_MAX_EXT
		else if (!strcmp(name, "MAX_EXT")) return GL_MAX_EXT;
#endif
#ifdef GL_MIN_EXT
		else if (!strcmp(name, "MIN_EXT")) return GL_MIN_EXT;
#endif
#ifdef GL_N3F_V3F
		else if (!strcmp(name, "N3F_V3F")) return GL_N3F_V3F;
#endif
#ifdef GL_NEAREST
		else if (!strcmp(name, "NEAREST")) return GL_NEAREST;
#endif
#ifdef GL_POLYGON
		else if (!strcmp(name, "POLYGON")) return GL_POLYGON;
#endif
#ifdef GL_REPLACE
		else if (!strcmp(name, "REPLACE")) return GL_REPLACE;
#endif
#ifdef GL_RGB5_A1
		else if (!strcmp(name, "RGB5_A1")) return GL_RGB5_A1;
#endif
#ifdef GL_STENCIL
		else if (!strcmp(name, "STENCIL")) return GL_STENCIL;
#endif
#ifdef GL_T2F_V3F
		else if (!strcmp(name, "T2F_V3F")) return GL_T2F_V3F;
#endif
#ifdef GL_T4F_V4F
		else if (!strcmp(name, "T4F_V4F")) return GL_T4F_V4F;
#endif
#ifdef GL_TEXTURE
		else if (!strcmp(name, "TEXTURE")) return GL_TEXTURE;
#endif
#ifdef GL_VERSION
		else if (!strcmp(name, "VERSION")) return GL_VERSION;
#endif
		else return _GL_UNDEFINED;
	case 11:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_3D_COLOR
		else if (!strcmp(name, "3D_COLOR")) return GL_3D_COLOR;
#endif
#ifdef GL_ABGR_EXT
		else if (!strcmp(name, "ABGR_EXT")) return GL_ABGR_EXT;
#endif
#ifdef GL_BGRA_EXT
		else if (!strcmp(name, "BGRA_EXT")) return GL_BGRA_EXT;
#endif
#ifdef GL_C4UB_V2F
		else if (!strcmp(name, "C4UB_V2F")) return GL_C4UB_V2F;
#endif
#ifdef GL_C4UB_V3F
		else if (!strcmp(name, "C4UB_V3F")) return GL_C4UB_V3F;
#endif
#ifdef GL_CMYK_EXT
		else if (!strcmp(name, "CMYK_EXT")) return GL_CMYK_EXT;
#endif
#ifdef GL_CUBIC_HP
		else if (!strcmp(name, "CUBIC_HP")) return GL_CUBIC_HP;
#endif
#ifdef GL_EMISSION
		else if (!strcmp(name, "EMISSION")) return GL_EMISSION;
#endif
#ifdef GL_EVAL_BIT
		else if (!strcmp(name, "EVAL_BIT")) return GL_EVAL_BIT;
#endif
#ifdef GL_EXT_abgr
		else if (!strcmp(name, "EXT_ABGR")) return GL_EXT_abgr;
#endif
#ifdef GL_EXT_bgra
		else if (!strcmp(name, "EXT_BGRA")) return GL_EXT_bgra;
#endif
#ifdef GL_FEEDBACK
		else if (!strcmp(name, "FEEDBACK")) return GL_FEEDBACK;
#endif
#ifdef GL_FOG_HINT
		else if (!strcmp(name, "FOG_HINT")) return GL_FOG_HINT;
#endif
#ifdef GL_FOG_MODE
		else if (!strcmp(name, "FOG_MODE")) return GL_FOG_MODE;
#endif
#ifdef GL_FUNC_ADD
		else if (!strcmp(name, "FUNC_ADD")) return GL_FUNC_ADD;
#endif
#ifdef GL_HINT_BIT
		else if (!strcmp(name, "HINT_BIT")) return GL_HINT_BIT;
#endif
#ifdef GL_LIGHTING
		else if (!strcmp(name, "LIGHTING")) return GL_LIGHTING;
#endif
#ifdef GL_LINE_BIT
		else if (!strcmp(name, "LINE_BIT")) return GL_LINE_BIT;
#endif
#ifdef GL_LIST_BIT
		else if (!strcmp(name, "LIST_BIT")) return GL_LIST_BIT;
#endif
#ifdef GL_LOGIC_OP
		else if (!strcmp(name, "LOGIC_OP")) return GL_LOGIC_OP;
#endif
#ifdef GL_MODULATE
		else if (!strcmp(name, "MODULATE")) return GL_MODULATE;
#endif
#ifdef GL_NOTEQUAL
		else if (!strcmp(name, "NOTEQUAL")) return GL_NOTEQUAL;
#endif
#ifdef GL_NO_ERROR
		else if (!strcmp(name, "NO_ERROR")) return GL_NO_ERROR;
#endif
#ifdef GL_POSITION
		else if (!strcmp(name, "POSITION")) return GL_POSITION;
#endif
#ifdef GL_R3_G3_B2
		else if (!strcmp(name, "R3_G3_B2")) return GL_R3_G3_B2;
#endif
#ifdef GL_RED_BIAS
		else if (!strcmp(name, "RED_BIAS")) return GL_RED_BIAS;
#endif
#ifdef GL_RED_BITS
		else if (!strcmp(name, "RED_BITS")) return GL_RED_BITS;
#endif
#ifdef GL_RENDERER
		else if (!strcmp(name, "RENDERER")) return GL_RENDERER;
#endif
#ifdef GL_RGB10_A2
		else if (!strcmp(name, "RGB10_A2")) return GL_RGB10_A2;
#endif
#ifdef GL_RGB2_EXT
		else if (!strcmp(name, "RGB2_EXT")) return GL_RGB2_EXT;
#endif
#ifdef GL_RGB4_EXT
		else if (!strcmp(name, "RGB4_EXT")) return GL_RGB4_EXT;
#endif
#ifdef GL_RGB5_EXT
		else if (!strcmp(name, "RGB5_EXT")) return GL_RGB5_EXT;
#endif
#ifdef GL_RGB8_EXT
		else if (!strcmp(name, "RGB8_EXT")) return GL_RGB8_EXT;
#endif
#ifdef GL_SPECULAR
		else if (!strcmp(name, "SPECULAR")) return GL_SPECULAR;
#endif
#ifdef GL_VIEWPORT
		else if (!strcmp(name, "VIEWPORT")) return GL_VIEWPORT;
#endif
		else return _GL_UNDEFINED;
	case 12:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_1PASS_EXT
		else if (!strcmp(name, "1PASS_EXT")) return GL_1PASS_EXT;
#endif
#ifdef GL_BACK_LEFT
		else if (!strcmp(name, "BACK_LEFT")) return GL_BACK_LEFT;
#endif
#ifdef GL_BLEND_DST
		else if (!strcmp(name, "BLEND_DST")) return GL_BLEND_DST;
#endif
#ifdef GL_BLEND_SRC
		else if (!strcmp(name, "BLEND_SRC")) return GL_BLEND_SRC;
#endif
#ifdef GL_BLUE_BIAS
		else if (!strcmp(name, "BLUE_BIAS")) return GL_BLUE_BIAS;
#endif
#ifdef GL_BLUE_BITS
		else if (!strcmp(name, "BLUE_BITS")) return GL_BLUE_BITS;
#endif
#ifdef GL_CMYKA_EXT
		else if (!strcmp(name, "CMYKA_EXT")) return GL_CMYKA_EXT;
#endif
#ifdef GL_CUBIC_EXT
		else if (!strcmp(name, "CUBIC_EXT")) return GL_CUBIC_EXT;
#endif
#ifdef GL_CULL_FACE
		else if (!strcmp(name, "CULL_FACE")) return GL_CULL_FACE;
#endif
#ifdef GL_DONT_CARE
		else if (!strcmp(name, "DONT_CARE")) return GL_DONT_CARE;
#endif
#ifdef GL_DST_ALPHA
		else if (!strcmp(name, "DST_ALPHA")) return GL_DST_ALPHA;
#endif
#ifdef GL_DST_COLOR
		else if (!strcmp(name, "DST_COLOR")) return GL_DST_COLOR;
#endif
#ifdef GL_EDGE_FLAG
		else if (!strcmp(name, "EDGE_FLAG")) return GL_EDGE_FLAG;
#endif
#ifdef GL_EXT_cmyka
		else if (!strcmp(name, "EXT_CMYKA")) return GL_EXT_cmyka;
#endif
#ifdef GL_EYE_PLANE
		else if (!strcmp(name, "EYE_PLANE")) return GL_EYE_PLANE;
#endif
#ifdef GL_FOG_COLOR
		else if (!strcmp(name, "FOG_COLOR")) return GL_FOG_COLOR;
#endif
#ifdef GL_FOG_INDEX
		else if (!strcmp(name, "FOG_INDEX")) return GL_FOG_INDEX;
#endif
#ifdef GL_FOG_START
		else if (!strcmp(name, "FOG_START")) return GL_FOG_START;
#endif
#ifdef GL_HISTOGRAM
		else if (!strcmp(name, "HISTOGRAM")) return GL_HISTOGRAM;
#endif
#ifdef GL_INTENSITY
		else if (!strcmp(name, "INTENSITY")) return GL_INTENSITY;
#endif
#ifdef GL_LINE_LOOP
		else if (!strcmp(name, "LINE_LOOP")) return GL_LINE_LOOP;
#endif
#ifdef GL_LIST_BASE
		else if (!strcmp(name, "LIST_BASE")) return GL_LIST_BASE;
#endif
#ifdef GL_LIST_MODE
		else if (!strcmp(name, "LIST_MODE")) return GL_LIST_MODE;
#endif
#ifdef GL_LUMINANCE
		else if (!strcmp(name, "LUMINANCE")) return GL_LUMINANCE;
#endif
#ifdef GL_MAP_COLOR
		else if (!strcmp(name, "MAP_COLOR")) return GL_MAP_COLOR;
#endif
#ifdef GL_MODELVIEW
		else if (!strcmp(name, "MODELVIEW")) return GL_MODELVIEW;
#endif
#ifdef GL_NORMALIZE
		else if (!strcmp(name, "NORMALIZE")) return GL_NORMALIZE;
#endif
#ifdef GL_PHONG_WIN
		else if (!strcmp(name, "PHONG_WIN")) return GL_PHONG_WIN;
#endif
#ifdef GL_POINT_BIT
		else if (!strcmp(name, "POINT_BIT")) return GL_POINT_BIT;
#endif
#ifdef GL_RED_SCALE
		else if (!strcmp(name, "RED_SCALE")) return GL_RED_SCALE;
#endif
#ifdef GL_RGB10_EXT
		else if (!strcmp(name, "RGB10_EXT")) return GL_RGB10_EXT;
#endif
#ifdef GL_RGB12_EXT
		else if (!strcmp(name, "RGB12_EXT")) return GL_RGB12_EXT;
#endif
#ifdef GL_RGB16_EXT
		else if (!strcmp(name, "RGB16_EXT")) return GL_RGB16_EXT;
#endif
#ifdef GL_RGBA2_EXT
		else if (!strcmp(name, "RGBA2_EXT")) return GL_RGBA2_EXT;
#endif
#ifdef GL_RGBA4_EXT
		else if (!strcmp(name, "RGBA4_EXT")) return GL_RGBA4_EXT;
#endif
#ifdef GL_RGBA8_EXT
		else if (!strcmp(name, "RGBA8_EXT")) return GL_RGBA8_EXT;
#endif
#ifdef GL_RGBA_MODE
		else if (!strcmp(name, "RGBA_MODE")) return GL_RGBA_MODE;
#endif
#ifdef GL_SHININESS
		else if (!strcmp(name, "SHININESS")) return GL_SHININESS;
#endif
#ifdef GL_SPARE0_NV
		else if (!strcmp(name, "SPARE0_NV")) return GL_SPARE0_NV;
#endif
#ifdef GL_SPARE1_NV
		else if (!strcmp(name, "SPARE1_NV")) return GL_SPARE1_NV;
#endif
#ifdef GL_SRC_ALPHA
		else if (!strcmp(name, "SRC_ALPHA")) return GL_SRC_ALPHA;
#endif
#ifdef GL_SRC_COLOR
		else if (!strcmp(name, "SRC_COLOR")) return GL_SRC_COLOR;
#endif
#ifdef GL_TRIANGLES
		else if (!strcmp(name, "TRIANGLES")) return GL_TRIANGLES;
#endif
		else return _GL_UNDEFINED;
	case 13:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_1PASS_SGIS
		else if (!strcmp(name, "1PASS_SGIS")) return GL_1PASS_SGIS;
#endif
#ifdef GL_ALPHA4_EXT
		else if (!strcmp(name, "ALPHA4_EXT")) return GL_ALPHA4_EXT;
#endif
#ifdef GL_ALPHA8_EXT
		else if (!strcmp(name, "ALPHA8_EXT")) return GL_ALPHA8_EXT;
#endif
#ifdef GL_ALPHA_BIAS
		else if (!strcmp(name, "ALPHA_BIAS")) return GL_ALPHA_BIAS;
#endif
#ifdef GL_ALPHA_BITS
		else if (!strcmp(name, "ALPHA_BITS")) return GL_ALPHA_BITS;
#endif
#ifdef GL_ALPHA_TEST
		else if (!strcmp(name, "ALPHA_TEST")) return GL_ALPHA_TEST;
#endif
#ifdef GL_AVERAGE_HP
		else if (!strcmp(name, "AVERAGE_HP")) return GL_AVERAGE_HP;
#endif
#ifdef GL_BACK_RIGHT
		else if (!strcmp(name, "BACK_RIGHT")) return GL_BACK_RIGHT;
#endif
#ifdef GL_BLUE_SCALE
		else if (!strcmp(name, "BLUE_SCALE")) return GL_BLUE_SCALE;
#endif
#ifdef GL_DEPTH_BIAS
		else if (!strcmp(name, "DEPTH_BIAS")) return GL_DEPTH_BIAS;
#endif
#ifdef GL_DEPTH_BITS
		else if (!strcmp(name, "DEPTH_BITS")) return GL_DEPTH_BITS;
#endif
#ifdef GL_DEPTH_FUNC
		else if (!strcmp(name, "DEPTH_FUNC")) return GL_DEPTH_FUNC;
#endif
#ifdef GL_DEPTH_TEST
		else if (!strcmp(name, "DEPTH_TEST")) return GL_DEPTH_TEST;
#endif
#ifdef GL_DISCARD_NV
		else if (!strcmp(name, "DISCARD_NV")) return GL_DISCARD_NV;
#endif
#ifdef GL_DOUBLE_EXT
		else if (!strcmp(name, "DOUBLE_EXT")) return GL_DOUBLE_EXT;
#endif
#ifdef GL_ENABLE_BIT
		else if (!strcmp(name, "ENABLE_BIT")) return GL_ENABLE_BIT;
#endif
#ifdef GL_EXTENSIONS
		else if (!strcmp(name, "EXTENSIONS")) return GL_EXTENSIONS;
#endif
#ifdef GL_EYE_LINEAR
		else if (!strcmp(name, "EYE_LINEAR")) return GL_EYE_LINEAR;
#endif
#ifdef GL_FRONT_FACE
		else if (!strcmp(name, "FRONT_FACE")) return GL_FRONT_FACE;
#endif
#ifdef GL_FRONT_LEFT
		else if (!strcmp(name, "FRONT_LEFT")) return GL_FRONT_LEFT;
#endif
#ifdef GL_GREEN_BIAS
		else if (!strcmp(name, "GREEN_BIAS")) return GL_GREEN_BIAS;
#endif
#ifdef GL_GREEN_BITS
		else if (!strcmp(name, "GREEN_BITS")) return GL_GREEN_BITS;
#endif
#ifdef GL_INDEX_BITS
		else if (!strcmp(name, "INDEX_BITS")) return GL_INDEX_BITS;
#endif
#ifdef GL_INDEX_MODE
		else if (!strcmp(name, "INDEX_MODE")) return GL_INDEX_MODE;
#endif
#ifdef GL_INTENSITY4
		else if (!strcmp(name, "INTENSITY4")) return GL_INTENSITY4;
#endif
#ifdef GL_INTENSITY8
		else if (!strcmp(name, "INTENSITY8")) return GL_INTENSITY8;
#endif
#ifdef GL_LINE_STRIP
		else if (!strcmp(name, "LINE_STRIP")) return GL_LINE_STRIP;
#endif
#ifdef GL_LINE_TOKEN
		else if (!strcmp(name, "LINE_TOKEN")) return GL_LINE_TOKEN;
#endif
#ifdef GL_LINE_WIDTH
		else if (!strcmp(name, "LINE_WIDTH")) return GL_LINE_WIDTH;
#endif
#ifdef GL_LIST_INDEX
		else if (!strcmp(name, "LIST_INDEX")) return GL_LIST_INDEX;
#endif
#ifdef GL_LUMINANCE4
		else if (!strcmp(name, "LUMINANCE4")) return GL_LUMINANCE4;
#endif
#ifdef GL_LUMINANCE8
		else if (!strcmp(name, "LUMINANCE8")) return GL_LUMINANCE8;
#endif
#ifdef GL_MAP1_INDEX
		else if (!strcmp(name, "MAP1_INDEX")) return GL_MAP1_INDEX;
#endif
#ifdef GL_MAP2_INDEX
		else if (!strcmp(name, "MAP2_INDEX")) return GL_MAP2_INDEX;
#endif
#ifdef GL_MAX_LIGHTS
		else if (!strcmp(name, "MAX_LIGHTS")) return GL_MAX_LIGHTS;
#endif
#ifdef GL_MINMAX_EXT
		else if (!strcmp(name, "MINMAX_EXT")) return GL_MINMAX_EXT;
#endif
#ifdef GL_OR_REVERSE
		else if (!strcmp(name, "OR_REVERSE")) return GL_OR_REVERSE;
#endif
#ifdef GL_POINT_SIZE
		else if (!strcmp(name, "POINT_SIZE")) return GL_POINT_SIZE;
#endif
#ifdef GL_PROJECTION
		else if (!strcmp(name, "PROJECTION")) return GL_PROJECTION;
#endif
#ifdef GL_QUAD_STRIP
		else if (!strcmp(name, "QUAD_STRIP")) return GL_QUAD_STRIP;
#endif
#ifdef GL_REDUCE_EXT
		else if (!strcmp(name, "REDUCE_EXT")) return GL_REDUCE_EXT;
#endif
#ifdef GL_RGBA12_EXT
		else if (!strcmp(name, "RGBA12_EXT")) return GL_RGBA12_EXT;
#endif
#ifdef GL_RGBA16_EXT
		else if (!strcmp(name, "RGBA16_EXT")) return GL_RGBA16_EXT;
#endif
#ifdef GL_SGIX_async
		else if (!strcmp(name, "SGIX_ASYNC")) return GL_SGIX_async;
#endif
#ifdef GL_SGIX_ycrcb
		else if (!strcmp(name, "SGIX_YCRCB")) return GL_SGIX_ycrcb;
#endif
#ifdef GL_SPHERE_MAP
		else if (!strcmp(name, "SPHERE_MAP")) return GL_SPHERE_MAP;
#endif
#ifdef GL_SUN_vertex
		else if (!strcmp(name, "SUN_VERTEX")) return GL_SUN_vertex;
#endif
#ifdef GL_TEXTURE_1D
		else if (!strcmp(name, "TEXTURE_1D")) return GL_TEXTURE_1D;
#endif
#ifdef GL_TEXTURE_2D
		else if (!strcmp(name, "TEXTURE_2D")) return GL_TEXTURE_2D;
#endif
#ifdef GL_TEXTURE_3D
		else if (!strcmp(name, "TEXTURE_3D")) return GL_TEXTURE_3D;
#endif
#ifdef GL_YCRCB_SGIX
		else if (!strcmp(name, "YCRCB_SGIX")) return GL_YCRCB_SGIX;
#endif
		else return _GL_UNDEFINED;
	case 14:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_2PASS_0_EXT
		else if (!strcmp(name, "2PASS_0_EXT")) return GL_2PASS_0_EXT;
#endif
#ifdef GL_2PASS_1_EXT
		else if (!strcmp(name, "2PASS_1_EXT")) return GL_2PASS_1_EXT;
#endif
#ifdef GL_422_REV_EXT
		else if (!strcmp(name, "422_REV_EXT")) return GL_422_REV_EXT;
#endif
#ifdef GL_4PASS_0_EXT
		else if (!strcmp(name, "4PASS_0_EXT")) return GL_4PASS_0_EXT;
#endif
#ifdef GL_4PASS_1_EXT
		else if (!strcmp(name, "4PASS_1_EXT")) return GL_4PASS_1_EXT;
#endif
#ifdef GL_4PASS_2_EXT
		else if (!strcmp(name, "4PASS_2_EXT")) return GL_4PASS_2_EXT;
#endif
#ifdef GL_4PASS_3_EXT
		else if (!strcmp(name, "4PASS_3_EXT")) return GL_4PASS_3_EXT;
#endif
#ifdef GL_ALPHA12_EXT
		else if (!strcmp(name, "ALPHA12_EXT")) return GL_ALPHA12_EXT;
#endif
#ifdef GL_ALPHA16_EXT
		else if (!strcmp(name, "ALPHA16_EXT")) return GL_ALPHA16_EXT;
#endif
#ifdef GL_ALPHA_SCALE
		else if (!strcmp(name, "ALPHA_SCALE")) return GL_ALPHA_SCALE;
#endif
#ifdef GL_AND_REVERSE
		else if (!strcmp(name, "AND_REVERSE")) return GL_AND_REVERSE;
#endif
#ifdef GL_AUTO_NORMAL
		else if (!strcmp(name, "AUTO_NORMAL")) return GL_AUTO_NORMAL;
#endif
#ifdef GL_AUX_BUFFERS
		else if (!strcmp(name, "AUX_BUFFERS")) return GL_AUX_BUFFERS;
#endif
#ifdef GL_AVERAGE_EXT
		else if (!strcmp(name, "AVERAGE_EXT")) return GL_AVERAGE_EXT;
#endif
#ifdef GL_BLEND_COLOR
		else if (!strcmp(name, "BLEND_COLOR")) return GL_BLEND_COLOR;
#endif
#ifdef GL_C4F_N3F_V3F
		else if (!strcmp(name, "C4F_N3F_V3F")) return GL_C4F_N3F_V3F;
#endif
#ifdef GL_CLIP_PLANE0
		else if (!strcmp(name, "CLIP_PLANE0")) return GL_CLIP_PLANE0;
#endif
#ifdef GL_CLIP_PLANE1
		else if (!strcmp(name, "CLIP_PLANE1")) return GL_CLIP_PLANE1;
#endif
#ifdef GL_CLIP_PLANE2
		else if (!strcmp(name, "CLIP_PLANE2")) return GL_CLIP_PLANE2;
#endif
#ifdef GL_CLIP_PLANE3
		else if (!strcmp(name, "CLIP_PLANE3")) return GL_CLIP_PLANE3;
#endif
#ifdef GL_CLIP_PLANE4
		else if (!strcmp(name, "CLIP_PLANE4")) return GL_CLIP_PLANE4;
#endif
#ifdef GL_CLIP_PLANE5
		else if (!strcmp(name, "CLIP_PLANE5")) return GL_CLIP_PLANE5;
#endif
#ifdef GL_COLOR_ARRAY
		else if (!strcmp(name, "COLOR_ARRAY")) return GL_COLOR_ARRAY;
#endif
#ifdef GL_COLOR_INDEX
		else if (!strcmp(name, "COLOR_INDEX")) return GL_COLOR_INDEX;
#endif
#ifdef GL_COLOR_TABLE
		else if (!strcmp(name, "COLOR_TABLE")) return GL_COLOR_TABLE;
#endif
#ifdef GL_COMBINE4_NV
		else if (!strcmp(name, "COMBINE4_NV")) return GL_COMBINE4_NV;
#endif
#ifdef GL_COMBINE_EXT
		else if (!strcmp(name, "COMBINE_EXT")) return GL_COMBINE_EXT;
#endif
#ifdef GL_CURRENT_BIT
		else if (!strcmp(name, "CURRENT_BIT")) return GL_CURRENT_BIT;
#endif
#ifdef GL_DEPTH_RANGE
		else if (!strcmp(name, "DEPTH_RANGE")) return GL_DEPTH_RANGE;
#endif
#ifdef GL_DEPTH_SCALE
		else if (!strcmp(name, "DEPTH_SCALE")) return GL_DEPTH_SCALE;
#endif
#ifdef GL_DRAW_BUFFER
		else if (!strcmp(name, "DRAW_BUFFER")) return GL_DRAW_BUFFER;
#endif
#ifdef GL_EXT_texture
		else if (!strcmp(name, "EXT_TEXTURE")) return GL_EXT_texture;
#endif
#ifdef GL_FOG_DENSITY
		else if (!strcmp(name, "FOG_DENSITY")) return GL_FOG_DENSITY;
#endif
#ifdef GL_FRONT_RIGHT
		else if (!strcmp(name, "FRONT_RIGHT")) return GL_FRONT_RIGHT;
#endif
#ifdef GL_GREEN_SCALE
		else if (!strcmp(name, "GREEN_SCALE")) return GL_GREEN_SCALE;
#endif
#ifdef GL_INDEX_ARRAY
		else if (!strcmp(name, "INDEX_ARRAY")) return GL_INDEX_ARRAY;
#endif
#ifdef GL_INDEX_SHIFT
		else if (!strcmp(name, "INDEX_SHIFT")) return GL_INDEX_SHIFT;
#endif
#ifdef GL_INTENSITY12
		else if (!strcmp(name, "INTENSITY12")) return GL_INTENSITY12;
#endif
#ifdef GL_INTENSITY16
		else if (!strcmp(name, "INTENSITY16")) return GL_INTENSITY16;
#endif
#ifdef GL_IUI_V2F_EXT
		else if (!strcmp(name, "IUI_V2F_EXT")) return GL_IUI_V2F_EXT;
#endif
#ifdef GL_IUI_V3F_EXT
		else if (!strcmp(name, "IUI_V3F_EXT")) return GL_IUI_V3F_EXT;
#endif
#ifdef GL_LINE_SMOOTH
		else if (!strcmp(name, "LINE_SMOOTH")) return GL_LINE_SMOOTH;
#endif
#ifdef GL_LUMINANCE12
		else if (!strcmp(name, "LUMINANCE12")) return GL_LUMINANCE12;
#endif
#ifdef GL_LUMINANCE16
		else if (!strcmp(name, "LUMINANCE16")) return GL_LUMINANCE16;
#endif
#ifdef GL_MAP1_NORMAL
		else if (!strcmp(name, "MAP1_NORMAL")) return GL_MAP1_NORMAL;
#endif
#ifdef GL_MAP2_NORMAL
		else if (!strcmp(name, "MAP2_NORMAL")) return GL_MAP2_NORMAL;
#endif
#ifdef GL_MAP_STENCIL
		else if (!strcmp(name, "MAP_STENCIL")) return GL_MAP_STENCIL;
#endif
#ifdef GL_MATRIX_MODE
		else if (!strcmp(name, "MATRIX_MODE")) return GL_MATRIX_MODE;
#endif
#ifdef GL_MINMAX_SINK
		else if (!strcmp(name, "MINMAX_SINK")) return GL_MINMAX_SINK;
#endif
#ifdef GL_OR_INVERTED
		else if (!strcmp(name, "OR_INVERTED")) return GL_OR_INVERTED;
#endif
#ifdef GL_PERTURB_EXT
		else if (!strcmp(name, "PERTURB_EXT")) return GL_PERTURB_EXT;
#endif
#ifdef GL_POINT_TOKEN
		else if (!strcmp(name, "POINT_TOKEN")) return GL_POINT_TOKEN;
#endif
#ifdef GL_POLYGON_BIT
		else if (!strcmp(name, "POLYGON_BIT")) return GL_POLYGON_BIT;
#endif
#ifdef GL_READ_BUFFER
		else if (!strcmp(name, "READ_BUFFER")) return GL_READ_BUFFER;
#endif
#ifdef GL_RENDER_MODE
		else if (!strcmp(name, "RENDER_MODE")) return GL_RENDER_MODE;
#endif
#ifdef GL_REPLACE_EXT
		else if (!strcmp(name, "REPLACE_EXT")) return GL_REPLACE_EXT;
#endif
#ifdef GL_RESTART_SUN
		else if (!strcmp(name, "RESTART_SUN")) return GL_RESTART_SUN;
#endif
#ifdef GL_RGB5_A1_EXT
		else if (!strcmp(name, "RGB5_A1_EXT")) return GL_RGB5_A1_EXT;
#endif
#ifdef GL_SAMPLES_ARB
		else if (!strcmp(name, "SAMPLES_ARB")) return GL_SAMPLES_ARB;
#endif
#ifdef GL_SAMPLES_EXT
		else if (!strcmp(name, "SAMPLES_EXT")) return GL_SAMPLES_EXT;
#endif
#ifdef GL_SCISSOR_BIT
		else if (!strcmp(name, "SCISSOR_BIT")) return GL_SCISSOR_BIT;
#endif
#ifdef GL_SCISSOR_BOX
		else if (!strcmp(name, "SCISSOR_BOX")) return GL_SCISSOR_BOX;
#endif
#ifdef GL_SGIX_shadow
		else if (!strcmp(name, "SGIX_SHADOW")) return GL_SGIX_shadow;
#endif
#ifdef GL_SGIX_sprite
		else if (!strcmp(name, "SGIX_SPRITE")) return GL_SGIX_sprite;
#endif
#ifdef GL_SGIX_ycrcba
		else if (!strcmp(name, "SGIX_YCRCBA")) return GL_SGIX_ycrcba;
#endif
#ifdef GL_SHADE_MODEL
		else if (!strcmp(name, "SHADE_MODEL")) return GL_SHADE_MODEL;
#endif
#ifdef GL_SPOT_CUTOFF
		else if (!strcmp(name, "SPOT_CUTOFF")) return GL_SPOT_CUTOFF;
#endif
#ifdef GL_SPRITE_SGIX
		else if (!strcmp(name, "SPRITE_SGIX")) return GL_SPRITE_SGIX;
#endif
#ifdef GL_STENCIL_REF
		else if (!strcmp(name, "STENCIL_REF")) return GL_STENCIL_REF;
#endif
#ifdef GL_T2F_C3F_V3F
		else if (!strcmp(name, "T2F_C3F_V3F")) return GL_T2F_C3F_V3F;
#endif
#ifdef GL_T2F_N3F_V3F
		else if (!strcmp(name, "T2F_N3F_V3F")) return GL_T2F_N3F_V3F;
#endif
#ifdef GL_TEXTURE_BIT
		else if (!strcmp(name, "TEXTURE_BIT")) return GL_TEXTURE_BIT;
#endif
#ifdef GL_TEXTURE_ENV
		else if (!strcmp(name, "TEXTURE_ENV")) return GL_TEXTURE_ENV;
#endif
#ifdef GL_VERSION_1_1
		else if (!strcmp(name, "VERSION_1_1")) return GL_VERSION_1_1;
#endif
#ifdef GL_VERSION_1_2
		else if (!strcmp(name, "VERSION_1_2")) return GL_VERSION_1_2;
#endif
#ifdef GL_YCRCBA_SGIX
		else if (!strcmp(name, "YCRCBA_SGIX")) return GL_YCRCBA_SGIX;
#endif
		else return _GL_UNDEFINED;
	case 15:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_2PASS_0_SGIS
		else if (!strcmp(name, "2PASS_0_SGIS")) return GL_2PASS_0_SGIS;
#endif
#ifdef GL_2PASS_1_SGIS
		else if (!strcmp(name, "2PASS_1_SGIS")) return GL_2PASS_1_SGIS;
#endif
#ifdef GL_3DFX_tbuffer
		else if (!strcmp(name, "3DFX_TBUFFER")) return GL_3DFX_tbuffer;
#endif
#ifdef GL_4PASS_0_SGIS
		else if (!strcmp(name, "4PASS_0_SGIS")) return GL_4PASS_0_SGIS;
#endif
#ifdef GL_4PASS_1_SGIS
		else if (!strcmp(name, "4PASS_1_SGIS")) return GL_4PASS_1_SGIS;
#endif
#ifdef GL_4PASS_2_SGIS
		else if (!strcmp(name, "4PASS_2_SGIS")) return GL_4PASS_2_SGIS;
#endif
#ifdef GL_4PASS_3_SGIS
		else if (!strcmp(name, "4PASS_3_SGIS")) return GL_4PASS_3_SGIS;
#endif
#ifdef GL_AND_INVERTED
		else if (!strcmp(name, "AND_INVERTED")) return GL_AND_INVERTED;
#endif
#ifdef GL_BITMAP_TOKEN
		else if (!strcmp(name, "BITMAP_TOKEN")) return GL_BITMAP_TOKEN;
#endif
#ifdef GL_COLOR_MATRIX
		else if (!strcmp(name, "COLOR_MATRIX")) return GL_COLOR_MATRIX;
#endif
#ifdef GL_COMBINER0_NV
		else if (!strcmp(name, "COMBINER0_NV")) return GL_COMBINER0_NV;
#endif
#ifdef GL_COMBINER1_NV
		else if (!strcmp(name, "COMBINER1_NV")) return GL_COMBINER1_NV;
#endif
#ifdef GL_COMBINER2_NV
		else if (!strcmp(name, "COMBINER2_NV")) return GL_COMBINER2_NV;
#endif
#ifdef GL_COMBINER3_NV
		else if (!strcmp(name, "COMBINER3_NV")) return GL_COMBINER3_NV;
#endif
#ifdef GL_COMBINER4_NV
		else if (!strcmp(name, "COMBINER4_NV")) return GL_COMBINER4_NV;
#endif
#ifdef GL_COMBINER5_NV
		else if (!strcmp(name, "COMBINER5_NV")) return GL_COMBINER5_NV;
#endif
#ifdef GL_COMBINER6_NV
		else if (!strcmp(name, "COMBINER6_NV")) return GL_COMBINER6_NV;
#endif
#ifdef GL_COMBINER7_NV
		else if (!strcmp(name, "COMBINER7_NV")) return GL_COMBINER7_NV;
#endif
#ifdef GL_CONSTANT_EXT
		else if (!strcmp(name, "CONSTANT_EXT")) return GL_CONSTANT_EXT;
#endif
#ifdef GL_DOT3_RGB_EXT
		else if (!strcmp(name, "DOT3_RGB_EXT")) return GL_DOT3_RGB_EXT;
#endif
#ifdef GL_DOUBLEBUFFER
		else if (!strcmp(name, "DOUBLEBUFFER")) return GL_DOUBLEBUFFER;
#endif
#ifdef GL_E_TIMES_F_NV
		else if (!strcmp(name, "E_TIMES_F_NV")) return GL_E_TIMES_F_NV;
#endif
#ifdef GL_FILTER4_SGIS
		else if (!strcmp(name, "FILTER4_SGIS")) return GL_FILTER4_SGIS;
#endif
#ifdef GL_FUNC_ADD_EXT
		else if (!strcmp(name, "FUNC_ADD_EXT")) return GL_FUNC_ADD_EXT;
#endif
#ifdef GL_INDEX_OFFSET
		else if (!strcmp(name, "INDEX_OFFSET")) return GL_INDEX_OFFSET;
#endif
#ifdef GL_INVALID_ENUM
		else if (!strcmp(name, "INVALID_ENUM")) return GL_INVALID_ENUM;
#endif
#ifdef GL_LIGHTING_BIT
		else if (!strcmp(name, "LIGHTING_BIT")) return GL_LIGHTING_BIT;
#endif
#ifdef GL_LINE_STIPPLE
		else if (!strcmp(name, "LINE_STIPPLE")) return GL_LINE_STIPPLE;
#endif
#ifdef GL_MAP1_COLOR_4
		else if (!strcmp(name, "MAP1_COLOR_4")) return GL_MAP1_COLOR_4;
#endif
#ifdef GL_MAP2_COLOR_4
		else if (!strcmp(name, "MAP2_COLOR_4")) return GL_MAP2_COLOR_4;
#endif
#ifdef GL_NORMAL_ARRAY
		else if (!strcmp(name, "NORMAL_ARRAY")) return GL_NORMAL_ARRAY;
#endif
#ifdef GL_OBJECT_PLANE
		else if (!strcmp(name, "OBJECT_PLANE")) return GL_OBJECT_PLANE;
#endif
#ifdef GL_POINT_SMOOTH
		else if (!strcmp(name, "POINT_SMOOTH")) return GL_POINT_SMOOTH;
#endif
#ifdef GL_POLYGON_MODE
		else if (!strcmp(name, "POLYGON_MODE")) return GL_POLYGON_MODE;
#endif
#ifdef GL_PREVIOUS_EXT
		else if (!strcmp(name, "PREVIOUS_EXT")) return GL_PREVIOUS_EXT;
#endif
#ifdef GL_R1UI_V3F_SUN
		else if (!strcmp(name, "R1UI_V3F_SUN")) return GL_R1UI_V3F_SUN;
#endif
#ifdef GL_RGB10_A2_EXT
		else if (!strcmp(name, "RGB10_A2_EXT")) return GL_RGB10_A2_EXT;
#endif
#ifdef GL_SAMPLES_3DFX
		else if (!strcmp(name, "SAMPLES_3DFX")) return GL_SAMPLES_3DFX;
#endif
#ifdef GL_SAMPLES_SGIS
		else if (!strcmp(name, "SAMPLES_SGIS")) return GL_SAMPLES_SGIS;
#endif
#ifdef GL_SCISSOR_TEST
		else if (!strcmp(name, "SCISSOR_TEST")) return GL_SCISSOR_TEST;
#endif
#ifdef GL_SEPARABLE_2D
		else if (!strcmp(name, "SEPARABLE_2D")) return GL_SEPARABLE_2D;
#endif
#ifdef GL_SGIX_clipmap
		else if (!strcmp(name, "SGIX_CLIPMAP")) return GL_SGIX_clipmap;
#endif
#ifdef GL_SINGLE_COLOR
		else if (!strcmp(name, "SINGLE_COLOR")) return GL_SINGLE_COLOR;
#endif
#ifdef GL_STENCIL_BITS
		else if (!strcmp(name, "STENCIL_BITS")) return GL_STENCIL_BITS;
#endif
#ifdef GL_STENCIL_FAIL
		else if (!strcmp(name, "STENCIL_FAIL")) return GL_STENCIL_FAIL;
#endif
#ifdef GL_STENCIL_FUNC
		else if (!strcmp(name, "STENCIL_FUNC")) return GL_STENCIL_FUNC;
#endif
#ifdef GL_STENCIL_TEST
		else if (!strcmp(name, "STENCIL_TEST")) return GL_STENCIL_TEST;
#endif
#ifdef GL_T2F_C4UB_V3F
		else if (!strcmp(name, "T2F_C4UB_V3F")) return GL_T2F_C4UB_V3F;
#endif
#ifdef GL_TEXTURE0_ARB
		else if (!strcmp(name, "TEXTURE0_ARB")) return GL_TEXTURE0_ARB;
#endif
#ifdef GL_TEXTURE1_ARB
		else if (!strcmp(name, "TEXTURE1_ARB")) return GL_TEXTURE1_ARB;
#endif
#ifdef GL_TEXTURE2_ARB
		else if (!strcmp(name, "TEXTURE2_ARB")) return GL_TEXTURE2_ARB;
#endif
#ifdef GL_TEXTURE3_ARB
		else if (!strcmp(name, "TEXTURE3_ARB")) return GL_TEXTURE3_ARB;
#endif
#ifdef GL_TEXTURE4_ARB
		else if (!strcmp(name, "TEXTURE4_ARB")) return GL_TEXTURE4_ARB;
#endif
#ifdef GL_TEXTURE5_ARB
		else if (!strcmp(name, "TEXTURE5_ARB")) return GL_TEXTURE5_ARB;
#endif
#ifdef GL_TEXTURE6_ARB
		else if (!strcmp(name, "TEXTURE6_ARB")) return GL_TEXTURE6_ARB;
#endif
#ifdef GL_TEXTURE7_ARB
		else if (!strcmp(name, "TEXTURE7_ARB")) return GL_TEXTURE7_ARB;
#endif
#ifdef GL_TEXTURE8_ARB
		else if (!strcmp(name, "TEXTURE8_ARB")) return GL_TEXTURE8_ARB;
#endif
#ifdef GL_TEXTURE9_ARB
		else if (!strcmp(name, "TEXTURE9_ARB")) return GL_TEXTURE9_ARB;
#endif
#ifdef GL_TRIANGLE_FAN
		else if (!strcmp(name, "TRIANGLE_FAN")) return GL_TRIANGLE_FAN;
#endif
#ifdef GL_UNSIGNED_INT
		else if (!strcmp(name, "UNSIGNED_INT")) return GL_UNSIGNED_INT;
#endif
#ifdef GL_VERTEX_ARRAY
		else if (!strcmp(name, "VERTEX_ARRAY")) return GL_VERTEX_ARRAY;
#endif
#ifdef GL_VIEWPORT_BIT
		else if (!strcmp(name, "VIEWPORT_BIT")) return GL_VIEWPORT_BIT;
#endif
		else return _GL_UNDEFINED;
	case 16:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_CLAMP_TO_EDGE
		else if (!strcmp(name, "CLAMP_TO_EDGE")) return GL_CLAMP_TO_EDGE;
#endif
#ifdef GL_COLOR_INDEXES
		else if (!strcmp(name, "COLOR_INDEXES")) return GL_COLOR_INDEXES;
#endif
#ifdef GL_COLOR_SUM_EXT
		else if (!strcmp(name, "COLOR_SUM_EXT")) return GL_COLOR_SUM_EXT;
#endif
#ifdef GL_COPY_INVERTED
		else if (!strcmp(name, "COPY_INVERTED")) return GL_COPY_INVERTED;
#endif
#ifdef GL_CURRENT_COLOR
		else if (!strcmp(name, "CURRENT_COLOR")) return GL_CURRENT_COLOR;
#endif
#ifdef GL_CURRENT_INDEX
		else if (!strcmp(name, "CURRENT_INDEX")) return GL_CURRENT_INDEX;
#endif
#ifdef GL_DECR_WRAP_EXT
		else if (!strcmp(name, "DECR_WRAP_EXT")) return GL_DECR_WRAP_EXT;
#endif
#ifdef GL_DOT3_RGBA_EXT
		else if (!strcmp(name, "DOT3_RGBA_EXT")) return GL_DOT3_RGBA_EXT;
#endif
#ifdef GL_EMBOSS_MAP_NV
		else if (!strcmp(name, "EMBOSS_MAP_NV")) return GL_EMBOSS_MAP_NV;
#endif
#ifdef GL_EXT_fog_coord
		else if (!strcmp(name, "EXT_FOG_COORD")) return GL_EXT_fog_coord;
#endif
#ifdef GL_EXT_histogram
		else if (!strcmp(name, "EXT_HISTOGRAM")) return GL_EXT_histogram;
#endif
#ifdef GL_EXT_texture3D
		else if (!strcmp(name, "EXT_TEXTURE3D")) return GL_EXT_texture3D;
#endif
#ifdef GL_EYE_LINE_SGIS
		else if (!strcmp(name, "EYE_LINE_SGIS")) return GL_EYE_LINE_SGIS;
#endif
#ifdef GL_EYE_RADIAL_NV
		else if (!strcmp(name, "EYE_RADIAL_NV")) return GL_EYE_RADIAL_NV;
#endif
#ifdef GL_FOG_FUNC_SGIS
		else if (!strcmp(name, "FOG_FUNC_SGIS")) return GL_FOG_FUNC_SGIS;
#endif
#ifdef GL_FUNC_SUBTRACT
		else if (!strcmp(name, "FUNC_SUBTRACT")) return GL_FUNC_SUBTRACT;
#endif
#ifdef GL_GLEXT_VERSION
		else if (!strcmp(name, "GLEXT_VERSION")) return GL_GLEXT_VERSION;
#endif
#ifdef GL_HISTOGRAM_EXT
		else if (!strcmp(name, "HISTOGRAM_EXT")) return GL_HISTOGRAM_EXT;
#endif
#ifdef GL_INCR_WRAP_EXT
		else if (!strcmp(name, "INCR_WRAP_EXT")) return GL_INCR_WRAP_EXT;
#endif
#ifdef GL_INDEX_BIT_PGI
		else if (!strcmp(name, "INDEX_BIT_PGI")) return GL_INDEX_BIT_PGI;
#endif
#ifdef GL_INTENSITY_EXT
		else if (!strcmp(name, "INTENSITY_EXT")) return GL_INTENSITY_EXT;
#endif
#ifdef GL_INVALID_VALUE
		else if (!strcmp(name, "INVALID_VALUE")) return GL_INVALID_VALUE;
#endif
#ifdef GL_LOGIC_OP_MODE
		else if (!strcmp(name, "LOGIC_OP_MODE")) return GL_LOGIC_OP_MODE;
#endif
#ifdef GL_MAP1_VERTEX_3
		else if (!strcmp(name, "MAP1_VERTEX_3")) return GL_MAP1_VERTEX_3;
#endif
#ifdef GL_MAP1_VERTEX_4
		else if (!strcmp(name, "MAP1_VERTEX_4")) return GL_MAP1_VERTEX_4;
#endif
#ifdef GL_MAP2_VERTEX_3
		else if (!strcmp(name, "MAP2_VERTEX_3")) return GL_MAP2_VERTEX_3;
#endif
#ifdef GL_MAP2_VERTEX_4
		else if (!strcmp(name, "MAP2_VERTEX_4")) return GL_MAP2_VERTEX_4;
#endif
#ifdef GL_MINMAX_FORMAT
		else if (!strcmp(name, "MINMAX_FORMAT")) return GL_MINMAX_FORMAT;
#endif
#ifdef GL_NORMAL_MAP_NV
		else if (!strcmp(name, "NORMAL_MAP_NV")) return GL_NORMAL_MAP_NV;
#endif
#ifdef GL_OBJECT_LINEAR
		else if (!strcmp(name, "OBJECT_LINEAR")) return GL_OBJECT_LINEAR;
#endif
#ifdef GL_OUT_OF_MEMORY
		else if (!strcmp(name, "OUT_OF_MEMORY")) return GL_OUT_OF_MEMORY;
#endif
#ifdef GL_POLYGON_TOKEN
		else if (!strcmp(name, "POLYGON_TOKEN")) return GL_POLYGON_TOKEN;
#endif
#ifdef GL_RGB_SCALE_EXT
		else if (!strcmp(name, "RGB_SCALE_EXT")) return GL_RGB_SCALE_EXT;
#endif
#ifdef GL_SGIX_resample
		else if (!strcmp(name, "SGIX_RESAMPLE")) return GL_SGIX_resample;
#endif
#ifdef GL_SPOT_EXPONENT
		else if (!strcmp(name, "SPOT_EXPONENT")) return GL_SPOT_EXPONENT;
#endif
#ifdef GL_STENCIL_INDEX
		else if (!strcmp(name, "STENCIL_INDEX")) return GL_STENCIL_INDEX;
#endif
#ifdef GL_SUBPIXEL_BITS
		else if (!strcmp(name, "SUBPIXEL_BITS")) return GL_SUBPIXEL_BITS;
#endif
#ifdef GL_TEXTURE10_ARB
		else if (!strcmp(name, "TEXTURE10_ARB")) return GL_TEXTURE10_ARB;
#endif
#ifdef GL_TEXTURE11_ARB
		else if (!strcmp(name, "TEXTURE11_ARB")) return GL_TEXTURE11_ARB;
#endif
#ifdef GL_TEXTURE12_ARB
		else if (!strcmp(name, "TEXTURE12_ARB")) return GL_TEXTURE12_ARB;
#endif
#ifdef GL_TEXTURE13_ARB
		else if (!strcmp(name, "TEXTURE13_ARB")) return GL_TEXTURE13_ARB;
#endif
#ifdef GL_TEXTURE14_ARB
		else if (!strcmp(name, "TEXTURE14_ARB")) return GL_TEXTURE14_ARB;
#endif
#ifdef GL_TEXTURE15_ARB
		else if (!strcmp(name, "TEXTURE15_ARB")) return GL_TEXTURE15_ARB;
#endif
#ifdef GL_TEXTURE16_ARB
		else if (!strcmp(name, "TEXTURE16_ARB")) return GL_TEXTURE16_ARB;
#endif
#ifdef GL_TEXTURE17_ARB
		else if (!strcmp(name, "TEXTURE17_ARB")) return GL_TEXTURE17_ARB;
#endif
#ifdef GL_TEXTURE18_ARB
		else if (!strcmp(name, "TEXTURE18_ARB")) return GL_TEXTURE18_ARB;
#endif
#ifdef GL_TEXTURE19_ARB
		else if (!strcmp(name, "TEXTURE19_ARB")) return GL_TEXTURE19_ARB;
#endif
#ifdef GL_TEXTURE20_ARB
		else if (!strcmp(name, "TEXTURE20_ARB")) return GL_TEXTURE20_ARB;
#endif
#ifdef GL_TEXTURE21_ARB
		else if (!strcmp(name, "TEXTURE21_ARB")) return GL_TEXTURE21_ARB;
#endif
#ifdef GL_TEXTURE22_ARB
		else if (!strcmp(name, "TEXTURE22_ARB")) return GL_TEXTURE22_ARB;
#endif
#ifdef GL_TEXTURE23_ARB
		else if (!strcmp(name, "TEXTURE23_ARB")) return GL_TEXTURE23_ARB;
#endif
#ifdef GL_TEXTURE24_ARB
		else if (!strcmp(name, "TEXTURE24_ARB")) return GL_TEXTURE24_ARB;
#endif
#ifdef GL_TEXTURE25_ARB
		else if (!strcmp(name, "TEXTURE25_ARB")) return GL_TEXTURE25_ARB;
#endif
#ifdef GL_TEXTURE26_ARB
		else if (!strcmp(name, "TEXTURE26_ARB")) return GL_TEXTURE26_ARB;
#endif
#ifdef GL_TEXTURE27_ARB
		else if (!strcmp(name, "TEXTURE27_ARB")) return GL_TEXTURE27_ARB;
#endif
#ifdef GL_TEXTURE28_ARB
		else if (!strcmp(name, "TEXTURE28_ARB")) return GL_TEXTURE28_ARB;
#endif
#ifdef GL_TEXTURE29_ARB
		else if (!strcmp(name, "TEXTURE29_ARB")) return GL_TEXTURE29_ARB;
#endif
#ifdef GL_TEXTURE30_ARB
		else if (!strcmp(name, "TEXTURE30_ARB")) return GL_TEXTURE30_ARB;
#endif
#ifdef GL_TEXTURE31_ARB
		else if (!strcmp(name, "TEXTURE31_ARB")) return GL_TEXTURE31_ARB;
#endif
#ifdef GL_TEXTURE_DEPTH
		else if (!strcmp(name, "TEXTURE_DEPTH")) return GL_TEXTURE_DEPTH;
#endif
#ifdef GL_TEXTURE_GEN_Q
		else if (!strcmp(name, "TEXTURE_GEN_Q")) return GL_TEXTURE_GEN_Q;
#endif
#ifdef GL_TEXTURE_GEN_R
		else if (!strcmp(name, "TEXTURE_GEN_R")) return GL_TEXTURE_GEN_R;
#endif
#ifdef GL_TEXTURE_GEN_S
		else if (!strcmp(name, "TEXTURE_GEN_S")) return GL_TEXTURE_GEN_S;
#endif
#ifdef GL_TEXTURE_GEN_T
		else if (!strcmp(name, "TEXTURE_GEN_T")) return GL_TEXTURE_GEN_T;
#endif
#ifdef GL_TEXTURE_WIDTH
		else if (!strcmp(name, "TEXTURE_WIDTH")) return GL_TEXTURE_WIDTH;
#endif
#ifdef GL_TRANSFORM_BIT
		else if (!strcmp(name, "TRANSFORM_BIT")) return GL_TRANSFORM_BIT;
#endif
#ifdef GL_UNSIGNED_BYTE
		else if (!strcmp(name, "UNSIGNED_BYTE")) return GL_UNSIGNED_BYTE;
#endif
#ifdef GL_VARIABLE_A_NV
		else if (!strcmp(name, "VARIABLE_A_NV")) return GL_VARIABLE_A_NV;
#endif
#ifdef GL_VARIABLE_B_NV
		else if (!strcmp(name, "VARIABLE_B_NV")) return GL_VARIABLE_B_NV;
#endif
#ifdef GL_VARIABLE_C_NV
		else if (!strcmp(name, "VARIABLE_C_NV")) return GL_VARIABLE_C_NV;
#endif
#ifdef GL_VARIABLE_D_NV
		else if (!strcmp(name, "VARIABLE_D_NV")) return GL_VARIABLE_D_NV;
#endif
#ifdef GL_VARIABLE_E_NV
		else if (!strcmp(name, "VARIABLE_E_NV")) return GL_VARIABLE_E_NV;
#endif
#ifdef GL_VARIABLE_F_NV
		else if (!strcmp(name, "VARIABLE_F_NV")) return GL_VARIABLE_F_NV;
#endif
#ifdef GL_VARIABLE_G_NV
		else if (!strcmp(name, "VARIABLE_G_NV")) return GL_VARIABLE_G_NV;
#endif
#ifdef GL_WIN_swap_hint
		else if (!strcmp(name, "WIN_SWAP_HINT")) return GL_WIN_swap_hint;
#endif
		else return _GL_UNDEFINED;
	case 17:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_ACCUM_RED_BITS
		else if (!strcmp(name, "ACCUM_RED_BITS")) return GL_ACCUM_RED_BITS;
#endif
#ifdef GL_ADD_SIGNED_EXT
		else if (!strcmp(name, "ADD_SIGNED_EXT")) return GL_ADD_SIGNED_EXT;
#endif
#ifdef GL_ALPHA_MAX_SGIX
		else if (!strcmp(name, "ALPHA_MAX_SGIX")) return GL_ALPHA_MAX_SGIX;
#endif
#ifdef GL_ALPHA_MIN_SGIX
		else if (!strcmp(name, "ALPHA_MIN_SGIX")) return GL_ALPHA_MIN_SGIX;
#endif
#ifdef GL_ALPHA_TEST_REF
		else if (!strcmp(name, "ALPHA_TEST_REF")) return GL_ALPHA_TEST_REF;
#endif
#ifdef GL_BLEND_EQUATION
		else if (!strcmp(name, "BLEND_EQUATION")) return GL_BLEND_EQUATION;
#endif
#ifdef GL_COLOR3_BIT_PGI
		else if (!strcmp(name, "COLOR3_BIT_PGI")) return GL_COLOR3_BIT_PGI;
#endif
#ifdef GL_COLOR4_BIT_PGI
		else if (!strcmp(name, "COLOR4_BIT_PGI")) return GL_COLOR4_BIT_PGI;
#endif
#ifdef GL_COLOR_LOGIC_OP
		else if (!strcmp(name, "COLOR_LOGIC_OP")) return GL_COLOR_LOGIC_OP;
#endif
#ifdef GL_COLOR_MATERIAL
		else if (!strcmp(name, "COLOR_MATERIAL")) return GL_COLOR_MATERIAL;
#endif
#ifdef GL_CONSTANT_ALPHA
		else if (!strcmp(name, "CONSTANT_ALPHA")) return GL_CONSTANT_ALPHA;
#endif
#ifdef GL_CONSTANT_COLOR
		else if (!strcmp(name, "CONSTANT_COLOR")) return GL_CONSTANT_COLOR;
#endif
#ifdef GL_CONVOLUTION_1D
		else if (!strcmp(name, "CONVOLUTION_1D")) return GL_CONVOLUTION_1D;
#endif
#ifdef GL_CONVOLUTION_2D
		else if (!strcmp(name, "CONVOLUTION_2D")) return GL_CONVOLUTION_2D;
#endif
#ifdef GL_CULL_FACE_MODE
		else if (!strcmp(name, "CULL_FACE_MODE")) return GL_CULL_FACE_MODE;
#endif
#ifdef GL_CURRENT_NORMAL
		else if (!strcmp(name, "CURRENT_NORMAL")) return GL_CURRENT_NORMAL;
#endif
#ifdef GL_EXT_422_pixels
		else if (!strcmp(name, "EXT_422_PIXELS")) return GL_EXT_422_pixels;
#endif
#ifdef GL_EXT_index_func
		else if (!strcmp(name, "EXT_INDEX_FUNC")) return GL_EXT_index_func;
#endif
#ifdef GL_EXT_subtexture
		else if (!strcmp(name, "EXT_SUBTEXTURE")) return GL_EXT_subtexture;
#endif
#ifdef GL_EYE_POINT_SGIS
		else if (!strcmp(name, "EYE_POINT_SGIS")) return GL_EYE_POINT_SGIS;
#endif
#ifdef GL_FOG_SCALE_SGIX
		else if (!strcmp(name, "FOG_SCALE_SGIX")) return GL_FOG_SCALE_SGIX;
#endif
#ifdef GL_FRAMEZOOM_SGIX
		else if (!strcmp(name, "FRAMEZOOM_SGIX")) return GL_FRAMEZOOM_SGIX;
#endif
#ifdef GL_FRONT_AND_BACK
		else if (!strcmp(name, "FRONT_AND_BACK")) return GL_FRONT_AND_BACK;
#endif
#ifdef GL_HISTOGRAM_SINK
		else if (!strcmp(name, "HISTOGRAM_SINK")) return GL_HISTOGRAM_SINK;
#endif
#ifdef GL_INDEX_LOGIC_OP
		else if (!strcmp(name, "INDEX_LOGIC_OP")) return GL_INDEX_LOGIC_OP;
#endif
#ifdef GL_INDEX_TEST_EXT
		else if (!strcmp(name, "INDEX_TEST_EXT")) return GL_INDEX_TEST_EXT;
#endif
#ifdef GL_INTENSITY4_EXT
		else if (!strcmp(name, "INTENSITY4_EXT")) return GL_INTENSITY4_EXT;
#endif
#ifdef GL_INTENSITY8_EXT
		else if (!strcmp(name, "INTENSITY8_EXT")) return GL_INTENSITY8_EXT;
#endif
#ifdef GL_INTERLACE_SGIX
		else if (!strcmp(name, "INTERLACE_SGIX")) return GL_INTERLACE_SGIX;
#endif
#ifdef GL_LUMINANCE4_EXT
		else if (!strcmp(name, "LUMINANCE4_EXT")) return GL_LUMINANCE4_EXT;
#endif
#ifdef GL_LUMINANCE8_EXT
		else if (!strcmp(name, "LUMINANCE8_EXT")) return GL_LUMINANCE8_EXT;
#endif
#ifdef GL_MAX_EVAL_ORDER
		else if (!strcmp(name, "MAX_EVAL_ORDER")) return GL_MAX_EVAL_ORDER;
#endif
#ifdef GL_MODELVIEW0_ARB
		else if (!strcmp(name, "MODELVIEW0_ARB")) return GL_MODELVIEW0_ARB;
#endif
#ifdef GL_MODELVIEW0_EXT
		else if (!strcmp(name, "MODELVIEW0_EXT")) return GL_MODELVIEW0_EXT;
#endif
#ifdef GL_MODELVIEW1_ARB
		else if (!strcmp(name, "MODELVIEW1_ARB")) return GL_MODELVIEW1_ARB;
#endif
#ifdef GL_MODELVIEW1_EXT
		else if (!strcmp(name, "MODELVIEW1_EXT")) return GL_MODELVIEW1_EXT;
#endif
#ifdef GL_MODELVIEW2_ARB
		else if (!strcmp(name, "MODELVIEW2_ARB")) return GL_MODELVIEW2_ARB;
#endif
#ifdef GL_MODELVIEW3_ARB
		else if (!strcmp(name, "MODELVIEW3_ARB")) return GL_MODELVIEW3_ARB;
#endif
#ifdef GL_MODELVIEW4_ARB
		else if (!strcmp(name, "MODELVIEW4_ARB")) return GL_MODELVIEW4_ARB;
#endif
#ifdef GL_MODELVIEW5_ARB
		else if (!strcmp(name, "MODELVIEW5_ARB")) return GL_MODELVIEW5_ARB;
#endif
#ifdef GL_MODELVIEW6_ARB
		else if (!strcmp(name, "MODELVIEW6_ARB")) return GL_MODELVIEW6_ARB;
#endif
#ifdef GL_MODELVIEW7_ARB
		else if (!strcmp(name, "MODELVIEW7_ARB")) return GL_MODELVIEW7_ARB;
#endif
#ifdef GL_MODELVIEW8_ARB
		else if (!strcmp(name, "MODELVIEW8_ARB")) return GL_MODELVIEW8_ARB;
#endif
#ifdef GL_MODELVIEW9_ARB
		else if (!strcmp(name, "MODELVIEW9_ARB")) return GL_MODELVIEW9_ARB;
#endif
#ifdef GL_NORMAL_BIT_PGI
		else if (!strcmp(name, "NORMAL_BIT_PGI")) return GL_NORMAL_BIT_PGI;
#endif
#ifdef GL_NORMAL_MAP_ARB
		else if (!strcmp(name, "NORMAL_MAP_ARB")) return GL_NORMAL_MAP_ARB;
#endif
#ifdef GL_NORMAL_MAP_EXT
		else if (!strcmp(name, "NORMAL_MAP_EXT")) return GL_NORMAL_MAP_EXT;
#endif
#ifdef GL_PACK_ALIGNMENT
		else if (!strcmp(name, "PACK_ALIGNMENT")) return GL_PACK_ALIGNMENT;
#endif
#ifdef GL_PACK_LSB_FIRST
		else if (!strcmp(name, "PACK_LSB_FIRST")) return GL_PACK_LSB_FIRST;
#endif
#ifdef GL_PACK_SKIP_ROWS
		else if (!strcmp(name, "PACK_SKIP_ROWS")) return GL_PACK_SKIP_ROWS;
#endif
#ifdef GL_PGI_misc_hints
		else if (!strcmp(name, "PGI_MISC_HINTS")) return GL_PGI_misc_hints;
#endif
#ifdef GL_PHONG_HINT_WIN
		else if (!strcmp(name, "PHONG_HINT_WIN")) return GL_PHONG_HINT_WIN;
#endif
#ifdef GL_PIXEL_MODE_BIT
		else if (!strcmp(name, "PIXEL_MODE_BIT")) return GL_PIXEL_MODE_BIT;
#endif
#ifdef GL_POLYGON_SMOOTH
		else if (!strcmp(name, "POLYGON_SMOOTH")) return GL_POLYGON_SMOOTH;
#endif
#ifdef GL_RESCALE_NORMAL
		else if (!strcmp(name, "RESCALE_NORMAL")) return GL_RESCALE_NORMAL;
#endif
#ifdef GL_SGIS_texture4D
		else if (!strcmp(name, "SGIS_TEXTURE4D")) return GL_SGIS_texture4D;
#endif
#ifdef GL_SGIX_fog_scale
		else if (!strcmp(name, "SGIX_FOG_SCALE")) return GL_SGIX_fog_scale;
#endif
#ifdef GL_SGIX_framezoom
		else if (!strcmp(name, "SGIX_FRAMEZOOM")) return GL_SGIX_framezoom;
#endif
#ifdef GL_SGIX_interlace
		else if (!strcmp(name, "SGIX_INTERLACE")) return GL_SGIX_interlace;
#endif
#ifdef GL_SGIX_subsample
		else if (!strcmp(name, "SGIX_SUBSAMPLE")) return GL_SGIX_subsample;
#endif
#ifdef GL_SOURCE3_RGB_NV
		else if (!strcmp(name, "SOURCE3_RGB_NV")) return GL_SOURCE3_RGB_NV;
#endif
#ifdef GL_SPOT_DIRECTION
		else if (!strcmp(name, "SPOT_DIRECTION")) return GL_SPOT_DIRECTION;
#endif
#ifdef GL_STACK_OVERFLOW
		else if (!strcmp(name, "STACK_OVERFLOW")) return GL_STACK_OVERFLOW;
#endif
#ifdef GL_TEXTURE_3D_EXT
		else if (!strcmp(name, "TEXTURE_3D_EXT")) return GL_TEXTURE_3D_EXT;
#endif
#ifdef GL_TEXTURE_BORDER
		else if (!strcmp(name, "TEXTURE_BORDER")) return GL_TEXTURE_BORDER;
#endif
#ifdef GL_TEXTURE_HEIGHT
		else if (!strcmp(name, "TEXTURE_HEIGHT")) return GL_TEXTURE_HEIGHT;
#endif
#ifdef GL_TEXTURE_MATRIX
		else if (!strcmp(name, "TEXTURE_MATRIX")) return GL_TEXTURE_MATRIX;
#endif
#ifdef GL_TEXTURE_WRAP_R
		else if (!strcmp(name, "TEXTURE_WRAP_R")) return GL_TEXTURE_WRAP_R;
#endif
#ifdef GL_TEXTURE_WRAP_S
		else if (!strcmp(name, "TEXTURE_WRAP_S")) return GL_TEXTURE_WRAP_S;
#endif
#ifdef GL_TEXTURE_WRAP_T
		else if (!strcmp(name, "TEXTURE_WRAP_T")) return GL_TEXTURE_WRAP_T;
#endif
#ifdef GL_TRIANGLE_STRIP
		else if (!strcmp(name, "TRIANGLE_STRIP")) return GL_TRIANGLE_STRIP;
#endif
#ifdef GL_UNSIGNED_SHORT
		else if (!strcmp(name, "UNSIGNED_SHORT")) return GL_UNSIGNED_SHORT;
#endif
#ifdef GL_YCRCB_422_SGIX
		else if (!strcmp(name, "YCRCB_422_SGIX")) return GL_YCRCB_422_SGIX;
#endif
#ifdef GL_YCRCB_444_SGIX
		else if (!strcmp(name, "YCRCB_444_SGIX")) return GL_YCRCB_444_SGIX;
#endif
		else return _GL_UNDEFINED;
	case 18:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_422_AVERAGE_EXT
		else if (!strcmp(name, "422_AVERAGE_EXT")) return GL_422_AVERAGE_EXT;
#endif
#ifdef GL_ACCUM_BLUE_BITS
		else if (!strcmp(name, "ACCUM_BLUE_BITS")) return GL_ACCUM_BLUE_BITS;
#endif
#ifdef GL_ALL_ATTRIB_BITS
		else if (!strcmp(name, "ALL_ATTRIB_BITS")) return GL_ALL_ATTRIB_BITS;
#endif
#ifdef GL_ALPHA_TEST_FUNC
		else if (!strcmp(name, "ALPHA_TEST_FUNC")) return GL_ALPHA_TEST_FUNC;
#endif
#ifdef GL_ARB_multisample
		else if (!strcmp(name, "ARB_MULTISAMPLE")) return GL_ARB_multisample;
#endif
#ifdef GL_ATTENUATION_EXT
		else if (!strcmp(name, "ATTENUATION_EXT")) return GL_ATTENUATION_EXT;
#endif
#ifdef GL_BLEND_COLOR_EXT
		else if (!strcmp(name, "BLEND_COLOR_EXT")) return GL_BLEND_COLOR_EXT;
#endif
#ifdef GL_COLOR_ARRAY_EXT
		else if (!strcmp(name, "COLOR_ARRAY_EXT")) return GL_COLOR_ARRAY_EXT;
#endif
#ifdef GL_COLOR_TABLE_SGI
		else if (!strcmp(name, "COLOR_TABLE_SGI")) return GL_COLOR_TABLE_SGI;
#endif
#ifdef GL_COLOR_WRITEMASK
		else if (!strcmp(name, "COLOR_WRITEMASK")) return GL_COLOR_WRITEMASK;
#endif
#ifdef GL_COMBINE_RGB_EXT
		else if (!strcmp(name, "COMBINE_RGB_EXT")) return GL_COMBINE_RGB_EXT;
#endif
#ifdef GL_CONSTANT_BORDER
		else if (!strcmp(name, "CONSTANT_BORDER")) return GL_CONSTANT_BORDER;
#endif
#ifdef GL_CULL_VERTEX_EXT
		else if (!strcmp(name, "CULL_VERTEX_EXT")) return GL_CULL_VERTEX_EXT;
#endif
#ifdef GL_CULL_VERTEX_IBM
		else if (!strcmp(name, "CULL_VERTEX_IBM")) return GL_CULL_VERTEX_IBM;
#endif
#ifdef GL_DEPTH_COMPONENT
		else if (!strcmp(name, "DEPTH_COMPONENT")) return GL_DEPTH_COMPONENT;
#endif
#ifdef GL_DEPTH_WRITEMASK
		else if (!strcmp(name, "DEPTH_WRITEMASK")) return GL_DEPTH_WRITEMASK;
#endif
#ifdef GL_EDGE_FLAG_ARRAY
		else if (!strcmp(name, "EDGE_FLAG_ARRAY")) return GL_EDGE_FLAG_ARRAY;
#endif
#ifdef GL_EMBOSS_LIGHT_NV
		else if (!strcmp(name, "EMBOSS_LIGHT_NV")) return GL_EMBOSS_LIGHT_NV;
#endif
#ifdef GL_EXT_blend_color
		else if (!strcmp(name, "EXT_BLEND_COLOR")) return GL_EXT_blend_color;
#endif
#ifdef GL_EXT_convolution
		else if (!strcmp(name, "EXT_CONVOLUTION")) return GL_EXT_convolution;
#endif
#ifdef GL_EXT_cull_vertex
		else if (!strcmp(name, "EXT_CULL_VERTEX")) return GL_EXT_cull_vertex;
#endif
#ifdef GL_EXT_multisample
		else if (!strcmp(name, "EXT_MULTISAMPLE")) return GL_EXT_multisample;
#endif
#ifdef GL_FOG_OFFSET_SGIX
		else if (!strcmp(name, "FOG_OFFSET_SGIX")) return GL_FOG_OFFSET_SGIX;
#endif
#ifdef GL_HISTOGRAM_WIDTH
		else if (!strcmp(name, "HISTOGRAM_WIDTH")) return GL_HISTOGRAM_WIDTH;
#endif
#ifdef GL_IBM_cull_vertex
		else if (!strcmp(name, "IBM_CULL_VERTEX")) return GL_IBM_cull_vertex;
#endif
#ifdef GL_INDEX_ARRAY_EXT
		else if (!strcmp(name, "INDEX_ARRAY_EXT")) return GL_INDEX_ARRAY_EXT;
#endif
#ifdef GL_INDEX_WRITEMASK
		else if (!strcmp(name, "INDEX_WRITEMASK")) return GL_INDEX_WRITEMASK;
#endif
#ifdef GL_INTENSITY12_EXT
		else if (!strcmp(name, "INTENSITY12_EXT")) return GL_INTENSITY12_EXT;
#endif
#ifdef GL_INTENSITY16_EXT
		else if (!strcmp(name, "INTENSITY16_EXT")) return GL_INTENSITY16_EXT;
#endif
#ifdef GL_INTERPOLATE_EXT
		else if (!strcmp(name, "INTERPOLATE_EXT")) return GL_INTERPOLATE_EXT;
#endif
#ifdef GL_IUI_N3F_V2F_EXT
		else if (!strcmp(name, "IUI_N3F_V2F_EXT")) return GL_IUI_N3F_V2F_EXT;
#endif
#ifdef GL_IUI_N3F_V3F_EXT
		else if (!strcmp(name, "IUI_N3F_V3F_EXT")) return GL_IUI_N3F_V3F_EXT;
#endif
#ifdef GL_LUMINANCE12_EXT
		else if (!strcmp(name, "LUMINANCE12_EXT")) return GL_LUMINANCE12_EXT;
#endif
#ifdef GL_LUMINANCE16_EXT
		else if (!strcmp(name, "LUMINANCE16_EXT")) return GL_LUMINANCE16_EXT;
#endif
#ifdef GL_LUMINANCE_ALPHA
		else if (!strcmp(name, "LUMINANCE_ALPHA")) return GL_LUMINANCE_ALPHA;
#endif
#ifdef GL_MAX_CLIP_PLANES
		else if (!strcmp(name, "MAX_CLIP_PLANES")) return GL_MAX_CLIP_PLANES;
#endif
#ifdef GL_MESA_window_pos
		else if (!strcmp(name, "MESA_WINDOW_POS")) return GL_MESA_window_pos;
#endif
#ifdef GL_MINMAX_SINK_EXT
		else if (!strcmp(name, "MINMAX_SINK_EXT")) return GL_MINMAX_SINK_EXT;
#endif
#ifdef GL_MODELVIEW10_ARB
		else if (!strcmp(name, "MODELVIEW10_ARB")) return GL_MODELVIEW10_ARB;
#endif
#ifdef GL_MODELVIEW11_ARB
		else if (!strcmp(name, "MODELVIEW11_ARB")) return GL_MODELVIEW11_ARB;
#endif
#ifdef GL_MODELVIEW12_ARB
		else if (!strcmp(name, "MODELVIEW12_ARB")) return GL_MODELVIEW12_ARB;
#endif
#ifdef GL_MODELVIEW13_ARB
		else if (!strcmp(name, "MODELVIEW13_ARB")) return GL_MODELVIEW13_ARB;
#endif
#ifdef GL_MODELVIEW14_ARB
		else if (!strcmp(name, "MODELVIEW14_ARB")) return GL_MODELVIEW14_ARB;
#endif
#ifdef GL_MODELVIEW15_ARB
		else if (!strcmp(name, "MODELVIEW15_ARB")) return GL_MODELVIEW15_ARB;
#endif
#ifdef GL_MODELVIEW16_ARB
		else if (!strcmp(name, "MODELVIEW16_ARB")) return GL_MODELVIEW16_ARB;
#endif
#ifdef GL_MODELVIEW17_ARB
		else if (!strcmp(name, "MODELVIEW17_ARB")) return GL_MODELVIEW17_ARB;
#endif
#ifdef GL_MODELVIEW18_ARB
		else if (!strcmp(name, "MODELVIEW18_ARB")) return GL_MODELVIEW18_ARB;
#endif
#ifdef GL_MODELVIEW19_ARB
		else if (!strcmp(name, "MODELVIEW19_ARB")) return GL_MODELVIEW19_ARB;
#endif
#ifdef GL_MODELVIEW20_ARB
		else if (!strcmp(name, "MODELVIEW20_ARB")) return GL_MODELVIEW20_ARB;
#endif
#ifdef GL_MODELVIEW21_ARB
		else if (!strcmp(name, "MODELVIEW21_ARB")) return GL_MODELVIEW21_ARB;
#endif
#ifdef GL_MODELVIEW22_ARB
		else if (!strcmp(name, "MODELVIEW22_ARB")) return GL_MODELVIEW22_ARB;
#endif
#ifdef GL_MODELVIEW23_ARB
		else if (!strcmp(name, "MODELVIEW23_ARB")) return GL_MODELVIEW23_ARB;
#endif
#ifdef GL_MODELVIEW24_ARB
		else if (!strcmp(name, "MODELVIEW24_ARB")) return GL_MODELVIEW24_ARB;
#endif
#ifdef GL_MODELVIEW25_ARB
		else if (!strcmp(name, "MODELVIEW25_ARB")) return GL_MODELVIEW25_ARB;
#endif
#ifdef GL_MODELVIEW26_ARB
		else if (!strcmp(name, "MODELVIEW26_ARB")) return GL_MODELVIEW26_ARB;
#endif
#ifdef GL_MODELVIEW27_ARB
		else if (!strcmp(name, "MODELVIEW27_ARB")) return GL_MODELVIEW27_ARB;
#endif
#ifdef GL_MODELVIEW28_ARB
		else if (!strcmp(name, "MODELVIEW28_ARB")) return GL_MODELVIEW28_ARB;
#endif
#ifdef GL_MODELVIEW29_ARB
		else if (!strcmp(name, "MODELVIEW29_ARB")) return GL_MODELVIEW29_ARB;
#endif
#ifdef GL_MODELVIEW30_ARB
		else if (!strcmp(name, "MODELVIEW30_ARB")) return GL_MODELVIEW30_ARB;
#endif
#ifdef GL_MODELVIEW31_ARB
		else if (!strcmp(name, "MODELVIEW31_ARB")) return GL_MODELVIEW31_ARB;
#endif
#ifdef GL_MULTISAMPLE_ARB
		else if (!strcmp(name, "MULTISAMPLE_ARB")) return GL_MULTISAMPLE_ARB;
#endif
#ifdef GL_MULTISAMPLE_EXT
		else if (!strcmp(name, "MULTISAMPLE_EXT")) return GL_MULTISAMPLE_EXT;
#endif
#ifdef GL_NV_blend_square
		else if (!strcmp(name, "NV_BLEND_SQUARE")) return GL_NV_blend_square;
#endif
#ifdef GL_NV_fog_distance
		else if (!strcmp(name, "NV_FOG_DISTANCE")) return GL_NV_fog_distance;
#endif
#ifdef GL_OPERAND3_RGB_NV
		else if (!strcmp(name, "OPERAND3_RGB_NV")) return GL_OPERAND3_RGB_NV;
#endif
#ifdef GL_PACK_ROW_LENGTH
		else if (!strcmp(name, "PACK_ROW_LENGTH")) return GL_PACK_ROW_LENGTH;
#endif
#ifdef GL_PACK_SWAP_BYTES
		else if (!strcmp(name, "PACK_SWAP_BYTES")) return GL_PACK_SWAP_BYTES;
#endif
#ifdef GL_POLYGON_STIPPLE
		else if (!strcmp(name, "POLYGON_STIPPLE")) return GL_POLYGON_STIPPLE;
#endif
#ifdef GL_PROXY_HISTOGRAM
		else if (!strcmp(name, "PROXY_HISTOGRAM")) return GL_PROXY_HISTOGRAM;
#endif
#ifdef GL_SAMPLE_MASK_EXT
		else if (!strcmp(name, "SAMPLE_MASK_EXT")) return GL_SAMPLE_MASK_EXT;
#endif
#ifdef GL_SCALE_BY_TWO_NV
		else if (!strcmp(name, "SCALE_BY_TWO_NV")) return GL_SCALE_BY_TWO_NV;
#endif
#ifdef GL_SGIX_fog_offset
		else if (!strcmp(name, "SGIX_FOG_OFFSET")) return GL_SGIX_fog_offset;
#endif
#ifdef GL_SGI_color_table
		else if (!strcmp(name, "SGI_COLOR_TABLE")) return GL_SGI_color_table;
#endif
#ifdef GL_SOURCE0_RGB_EXT
		else if (!strcmp(name, "SOURCE0_RGB_EXT")) return GL_SOURCE0_RGB_EXT;
#endif
#ifdef GL_SOURCE1_RGB_EXT
		else if (!strcmp(name, "SOURCE1_RGB_EXT")) return GL_SOURCE1_RGB_EXT;
#endif
#ifdef GL_SOURCE2_RGB_EXT
		else if (!strcmp(name, "SOURCE2_RGB_EXT")) return GL_SOURCE2_RGB_EXT;
#endif
#ifdef GL_SOURCE3_RGB_EXT
		else if (!strcmp(name, "SOURCE3_RGB_EXT")) return GL_SOURCE3_RGB_EXT;
#endif
#ifdef GL_SOURCE4_RGB_EXT
		else if (!strcmp(name, "SOURCE4_RGB_EXT")) return GL_SOURCE4_RGB_EXT;
#endif
#ifdef GL_SOURCE5_RGB_EXT
		else if (!strcmp(name, "SOURCE5_RGB_EXT")) return GL_SOURCE5_RGB_EXT;
#endif
#ifdef GL_SOURCE6_RGB_EXT
		else if (!strcmp(name, "SOURCE6_RGB_EXT")) return GL_SOURCE6_RGB_EXT;
#endif
#ifdef GL_SOURCE7_RGB_EXT
		else if (!strcmp(name, "SOURCE7_RGB_EXT")) return GL_SOURCE7_RGB_EXT;
#endif
#ifdef GL_STACK_UNDERFLOW
		else if (!strcmp(name, "STACK_UNDERFLOW")) return GL_STACK_UNDERFLOW;
#endif
#ifdef GL_T2F_C4F_N3F_V3F
		else if (!strcmp(name, "T2F_C4F_N3F_V3F")) return GL_T2F_C4F_N3F_V3F;
#endif
#ifdef GL_T2F_IUI_V2F_EXT
		else if (!strcmp(name, "T2F_IUI_V2F_EXT")) return GL_T2F_IUI_V2F_EXT;
#endif
#ifdef GL_T2F_IUI_V3F_EXT
		else if (!strcmp(name, "T2F_IUI_V3F_EXT")) return GL_T2F_IUI_V3F_EXT;
#endif
#ifdef GL_T4F_C4F_N3F_V4F
		else if (!strcmp(name, "T4F_C4F_N3F_V4F")) return GL_T4F_C4F_N3F_V4F;
#endif
#ifdef GL_TABLE_TOO_LARGE
		else if (!strcmp(name, "TABLE_TOO_LARGE")) return GL_TABLE_TOO_LARGE;
#endif
#ifdef GL_TEXTURE_4D_SGIS
		else if (!strcmp(name, "TEXTURE_4D_SGIS")) return GL_TEXTURE_4D_SGIS;
#endif
#ifdef GL_TEXTURE_MAX_LOD
		else if (!strcmp(name, "TEXTURE_MAX_LOD")) return GL_TEXTURE_MAX_LOD;
#endif
#ifdef GL_TEXTURE_MIN_LOD
		else if (!strcmp(name, "TEXTURE_MIN_LOD")) return GL_TEXTURE_MIN_LOD;
#endif
#ifdef GL_VERTEX4_BIT_PGI
		else if (!strcmp(name, "VERTEX4_BIT_PGI")) return GL_VERTEX4_BIT_PGI;
#endif
#ifdef GL_WRAP_BORDER_SUN
		else if (!strcmp(name, "WRAP_BORDER_SUN")) return GL_WRAP_BORDER_SUN;
#endif
		else return _GL_UNDEFINED;
	case 19:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_3DFX_multisample
		else if (!strcmp(name, "3DFX_MULTISAMPLE")) return GL_3DFX_multisample;
#endif
#ifdef GL_3D_COLOR_TEXTURE
		else if (!strcmp(name, "3D_COLOR_TEXTURE")) return GL_3D_COLOR_TEXTURE;
#endif
#ifdef GL_4D_COLOR_TEXTURE
		else if (!strcmp(name, "4D_COLOR_TEXTURE")) return GL_4D_COLOR_TEXTURE;
#endif
#ifdef GL_ACCUM_ALPHA_BITS
		else if (!strcmp(name, "ACCUM_ALPHA_BITS")) return GL_ACCUM_ALPHA_BITS;
#endif
#ifdef GL_ACCUM_BUFFER_BIT
		else if (!strcmp(name, "ACCUM_BUFFER_BIT")) return GL_ACCUM_BUFFER_BIT;
#endif
#ifdef GL_ACCUM_GREEN_BITS
		else if (!strcmp(name, "ACCUM_GREEN_BITS")) return GL_ACCUM_GREEN_BITS;
#endif
#ifdef GL_ARB_multitexture
		else if (!strcmp(name, "ARB_MULTITEXTURE")) return GL_ARB_multitexture;
#endif
#ifdef GL_COLOR_ARRAY_SIZE
		else if (!strcmp(name, "COLOR_ARRAY_SIZE")) return GL_COLOR_ARRAY_SIZE;
#endif
#ifdef GL_COLOR_ARRAY_TYPE
		else if (!strcmp(name, "COLOR_ARRAY_TYPE")) return GL_COLOR_ARRAY_TYPE;
#endif
#ifdef GL_COLOR_BUFFER_BIT
		else if (!strcmp(name, "COLOR_BUFFER_BIT")) return GL_COLOR_BUFFER_BIT;
#endif
#ifdef GL_COLOR_INDEX1_EXT
		else if (!strcmp(name, "COLOR_INDEX1_EXT")) return GL_COLOR_INDEX1_EXT;
#endif
#ifdef GL_COLOR_INDEX2_EXT
		else if (!strcmp(name, "COLOR_INDEX2_EXT")) return GL_COLOR_INDEX2_EXT;
#endif
#ifdef GL_COLOR_INDEX4_EXT
		else if (!strcmp(name, "COLOR_INDEX4_EXT")) return GL_COLOR_INDEX4_EXT;
#endif
#ifdef GL_COLOR_INDEX8_EXT
		else if (!strcmp(name, "COLOR_INDEX8_EXT")) return GL_COLOR_INDEX8_EXT;
#endif
#ifdef GL_COLOR_MATRIX_SGI
		else if (!strcmp(name, "COLOR_MATRIX_SGI")) return GL_COLOR_MATRIX_SGI;
#endif
#ifdef GL_COLOR_TABLE_BIAS
		else if (!strcmp(name, "COLOR_TABLE_BIAS")) return GL_COLOR_TABLE_BIAS;
#endif
#ifdef GL_COMBINER_BIAS_NV
		else if (!strcmp(name, "COMBINER_BIAS_NV")) return GL_COMBINER_BIAS_NV;
#endif
#ifdef GL_COPY_PIXEL_TOKEN
		else if (!strcmp(name, "COPY_PIXEL_TOKEN")) return GL_COPY_PIXEL_TOKEN;
#endif
#ifdef GL_DEPTH_BUFFER_BIT
		else if (!strcmp(name, "DEPTH_BUFFER_BIT")) return GL_DEPTH_BUFFER_BIT;
#endif
#ifdef GL_DRAW_PIXEL_TOKEN
		else if (!strcmp(name, "DRAW_PIXEL_TOKEN")) return GL_DRAW_PIXEL_TOKEN;
#endif
#ifdef GL_DUAL_ALPHA4_SGIS
		else if (!strcmp(name, "DUAL_ALPHA4_SGIS")) return GL_DUAL_ALPHA4_SGIS;
#endif
#ifdef GL_DUAL_ALPHA8_SGIS
		else if (!strcmp(name, "DUAL_ALPHA8_SGIS")) return GL_DUAL_ALPHA8_SGIS;
#endif
#ifdef GL_EDGEFLAG_BIT_PGI
		else if (!strcmp(name, "EDGEFLAG_BIT_PGI")) return GL_EDGEFLAG_BIT_PGI;
#endif
#ifdef GL_EXPAND_NEGATE_NV
		else if (!strcmp(name, "EXPAND_NEGATE_NV")) return GL_EXPAND_NEGATE_NV;
#endif
#ifdef GL_EXPAND_NORMAL_NV
		else if (!strcmp(name, "EXPAND_NORMAL_NV")) return GL_EXPAND_NORMAL_NV;
#endif
#ifdef GL_EXT_blend_minmax
		else if (!strcmp(name, "EXT_BLEND_MINMAX")) return GL_EXT_blend_minmax;
#endif
#ifdef GL_EXT_color_matrix
		else if (!strcmp(name, "EXT_COLOR_MATRIX")) return GL_EXT_color_matrix;
#endif
#ifdef GL_EXT_copy_texture
		else if (!strcmp(name, "EXT_COPY_TEXTURE")) return GL_EXT_copy_texture;
#endif
#ifdef GL_EXT_stencil_wrap
		else if (!strcmp(name, "EXT_STENCIL_WRAP")) return GL_EXT_stencil_wrap;
#endif
#ifdef GL_EXT_vertex_array
		else if (!strcmp(name, "EXT_VERTEX_ARRAY")) return GL_EXT_vertex_array;
#endif
#ifdef GL_GLOBAL_ALPHA_SUN
		else if (!strcmp(name, "GLOBAL_ALPHA_SUN")) return GL_GLOBAL_ALPHA_SUN;
#endif
#ifdef GL_HISTOGRAM_FORMAT
		else if (!strcmp(name, "HISTOGRAM_FORMAT")) return GL_HISTOGRAM_FORMAT;
#endif
#ifdef GL_IGNORE_BORDER_HP
		else if (!strcmp(name, "IGNORE_BORDER_HP")) return GL_IGNORE_BORDER_HP;
#endif
#ifdef GL_IMAGE_SCALE_X_HP
		else if (!strcmp(name, "IMAGE_SCALE_X_HP")) return GL_IMAGE_SCALE_X_HP;
#endif
#ifdef GL_IMAGE_SCALE_Y_HP
		else if (!strcmp(name, "IMAGE_SCALE_Y_HP")) return GL_IMAGE_SCALE_Y_HP;
#endif
#ifdef GL_INDEX_ARRAY_TYPE
		else if (!strcmp(name, "INDEX_ARRAY_TYPE")) return GL_INDEX_ARRAY_TYPE;
#endif
#ifdef GL_INGR_color_clamp
		else if (!strcmp(name, "INGR_COLOR_CLAMP")) return GL_INGR_color_clamp;
#endif
#ifdef GL_LINE_RESET_TOKEN
		else if (!strcmp(name, "LINE_RESET_TOKEN")) return GL_LINE_RESET_TOKEN;
#endif
#ifdef GL_LINE_SMOOTH_HINT
		else if (!strcmp(name, "LINE_SMOOTH_HINT")) return GL_LINE_SMOOTH_HINT;
#endif
#ifdef GL_LINE_WIDTH_RANGE
		else if (!strcmp(name, "LINE_WIDTH_RANGE")) return GL_LINE_WIDTH_RANGE;
#endif
#ifdef GL_MAP1_GRID_DOMAIN
		else if (!strcmp(name, "MAP1_GRID_DOMAIN")) return GL_MAP1_GRID_DOMAIN;
#endif
#ifdef GL_MAP1_TANGENT_EXT
		else if (!strcmp(name, "MAP1_TANGENT_EXT")) return GL_MAP1_TANGENT_EXT;
#endif
#ifdef GL_MAP2_GRID_DOMAIN
		else if (!strcmp(name, "MAP2_GRID_DOMAIN")) return GL_MAP2_GRID_DOMAIN;
#endif
#ifdef GL_MAP2_TANGENT_EXT
		else if (!strcmp(name, "MAP2_TANGENT_EXT")) return GL_MAP2_TANGENT_EXT;
#endif
#ifdef GL_MAX_LIST_NESTING
		else if (!strcmp(name, "MAX_LIST_NESTING")) return GL_MAX_LIST_NESTING;
#endif
#ifdef GL_MAX_SHININESS_NV
		else if (!strcmp(name, "MAX_SHININESS_NV")) return GL_MAX_SHININESS_NV;
#endif
#ifdef GL_MAX_TEXTURE_SIZE
		else if (!strcmp(name, "MAX_TEXTURE_SIZE")) return GL_MAX_TEXTURE_SIZE;
#endif
#ifdef GL_MODELVIEW_MATRIX
		else if (!strcmp(name, "MODELVIEW_MATRIX")) return GL_MODELVIEW_MATRIX;
#endif
#ifdef GL_MULTISAMPLE_3DFX
		else if (!strcmp(name, "MULTISAMPLE_3DFX")) return GL_MULTISAMPLE_3DFX;
#endif
#ifdef GL_MULTISAMPLE_SGIS
		else if (!strcmp(name, "MULTISAMPLE_SGIS")) return GL_MULTISAMPLE_SGIS;
#endif
#ifdef GL_NAME_STACK_DEPTH
		else if (!strcmp(name, "NAME_STACK_DEPTH")) return GL_NAME_STACK_DEPTH;
#endif
#ifdef GL_NORMAL_ARRAY_EXT
		else if (!strcmp(name, "NORMAL_ARRAY_EXT")) return GL_NORMAL_ARRAY_EXT;
#endif
#ifdef GL_NV_texgen_emboss
		else if (!strcmp(name, "NV_TEXGEN_EMBOSS")) return GL_NV_texgen_emboss;
#endif
#ifdef GL_OBJECT_LINE_SGIS
		else if (!strcmp(name, "OBJECT_LINE_SGIS")) return GL_OBJECT_LINE_SGIS;
#endif
#ifdef GL_OPERAND0_RGB_EXT
		else if (!strcmp(name, "OPERAND0_RGB_EXT")) return GL_OPERAND0_RGB_EXT;
#endif
#ifdef GL_OPERAND1_RGB_EXT
		else if (!strcmp(name, "OPERAND1_RGB_EXT")) return GL_OPERAND1_RGB_EXT;
#endif
#ifdef GL_OPERAND2_RGB_EXT
		else if (!strcmp(name, "OPERAND2_RGB_EXT")) return GL_OPERAND2_RGB_EXT;
#endif
#ifdef GL_OPERAND3_RGB_EXT
		else if (!strcmp(name, "OPERAND3_RGB_EXT")) return GL_OPERAND3_RGB_EXT;
#endif
#ifdef GL_OPERAND4_RGB_EXT
		else if (!strcmp(name, "OPERAND4_RGB_EXT")) return GL_OPERAND4_RGB_EXT;
#endif
#ifdef GL_OPERAND5_RGB_EXT
		else if (!strcmp(name, "OPERAND5_RGB_EXT")) return GL_OPERAND5_RGB_EXT;
#endif
#ifdef GL_OPERAND6_RGB_EXT
		else if (!strcmp(name, "OPERAND6_RGB_EXT")) return GL_OPERAND6_RGB_EXT;
#endif
#ifdef GL_OPERAND7_RGB_EXT
		else if (!strcmp(name, "OPERAND7_RGB_EXT")) return GL_OPERAND7_RGB_EXT;
#endif
#ifdef GL_PACK_SKIP_IMAGES
		else if (!strcmp(name, "PACK_SKIP_IMAGES")) return GL_PACK_SKIP_IMAGES;
#endif
#ifdef GL_PACK_SKIP_PIXELS
		else if (!strcmp(name, "PACK_SKIP_PIXELS")) return GL_PACK_SKIP_PIXELS;
#endif
#ifdef GL_PGI_vertex_hints
		else if (!strcmp(name, "PGI_VERTEX_HINTS")) return GL_PGI_vertex_hints;
#endif
#ifdef GL_PIXEL_MAP_A_TO_A
		else if (!strcmp(name, "PIXEL_MAP_A_TO_A")) return GL_PIXEL_MAP_A_TO_A;
#endif
#ifdef GL_PIXEL_MAP_B_TO_B
		else if (!strcmp(name, "PIXEL_MAP_B_TO_B")) return GL_PIXEL_MAP_B_TO_B;
#endif
#ifdef GL_PIXEL_MAP_G_TO_G
		else if (!strcmp(name, "PIXEL_MAP_G_TO_G")) return GL_PIXEL_MAP_G_TO_G;
#endif
#ifdef GL_PIXEL_MAP_I_TO_A
		else if (!strcmp(name, "PIXEL_MAP_I_TO_A")) return GL_PIXEL_MAP_I_TO_A;
#endif
#ifdef GL_PIXEL_MAP_I_TO_B
		else if (!strcmp(name, "PIXEL_MAP_I_TO_B")) return GL_PIXEL_MAP_I_TO_B;
#endif
#ifdef GL_PIXEL_MAP_I_TO_G
		else if (!strcmp(name, "PIXEL_MAP_I_TO_G")) return GL_PIXEL_MAP_I_TO_G;
#endif
#ifdef GL_PIXEL_MAP_I_TO_I
		else if (!strcmp(name, "PIXEL_MAP_I_TO_I")) return GL_PIXEL_MAP_I_TO_I;
#endif
#ifdef GL_PIXEL_MAP_I_TO_R
		else if (!strcmp(name, "PIXEL_MAP_I_TO_R")) return GL_PIXEL_MAP_I_TO_R;
#endif
#ifdef GL_PIXEL_MAP_R_TO_R
		else if (!strcmp(name, "PIXEL_MAP_R_TO_R")) return GL_PIXEL_MAP_R_TO_R;
#endif
#ifdef GL_PIXEL_MAP_S_TO_S
		else if (!strcmp(name, "PIXEL_MAP_S_TO_S")) return GL_PIXEL_MAP_S_TO_S;
#endif
#ifdef GL_POINT_SIZE_RANGE
		else if (!strcmp(name, "POINT_SIZE_RANGE")) return GL_POINT_SIZE_RANGE;
#endif
#ifdef GL_PRIMARY_COLOR_NV
		else if (!strcmp(name, "PRIMARY_COLOR_NV")) return GL_PRIMARY_COLOR_NV;
#endif
#ifdef GL_PROXY_TEXTURE_1D
		else if (!strcmp(name, "PROXY_TEXTURE_1D")) return GL_PROXY_TEXTURE_1D;
#endif
#ifdef GL_PROXY_TEXTURE_2D
		else if (!strcmp(name, "PROXY_TEXTURE_2D")) return GL_PROXY_TEXTURE_2D;
#endif
#ifdef GL_PROXY_TEXTURE_3D
		else if (!strcmp(name, "PROXY_TEXTURE_3D")) return GL_PROXY_TEXTURE_3D;
#endif
#ifdef GL_QUAD_ALPHA4_SGIS
		else if (!strcmp(name, "QUAD_ALPHA4_SGIS")) return GL_QUAD_ALPHA4_SGIS;
#endif
#ifdef GL_QUAD_ALPHA8_SGIS
		else if (!strcmp(name, "QUAD_ALPHA8_SGIS")) return GL_QUAD_ALPHA8_SGIS;
#endif
#ifdef GL_R1UI_C3F_V3F_SUN
		else if (!strcmp(name, "R1UI_C3F_V3F_SUN")) return GL_R1UI_C3F_V3F_SUN;
#endif
#ifdef GL_R1UI_N3F_V3F_SUN
		else if (!strcmp(name, "R1UI_N3F_V3F_SUN")) return GL_R1UI_N3F_V3F_SUN;
#endif
#ifdef GL_R1UI_T2F_V3F_SUN
		else if (!strcmp(name, "R1UI_T2F_V3F_SUN")) return GL_R1UI_T2F_V3F_SUN;
#endif
#ifdef GL_REPLICATE_BORDER
		else if (!strcmp(name, "REPLICATE_BORDER")) return GL_REPLICATE_BORDER;
#endif
#ifdef GL_SAMPLE_MASK_SGIS
		else if (!strcmp(name, "SAMPLE_MASK_SGIS")) return GL_SAMPLE_MASK_SGIS;
#endif
#ifdef GL_SCALE_BY_FOUR_NV
		else if (!strcmp(name, "SCALE_BY_FOUR_NV")) return GL_SCALE_BY_FOUR_NV;
#endif
#ifdef GL_SEPARABLE_2D_EXT
		else if (!strcmp(name, "SEPARABLE_2D_EXT")) return GL_SEPARABLE_2D_EXT;
#endif
#ifdef GL_SGIS_multisample
		else if (!strcmp(name, "SGIS_MULTISAMPLE")) return GL_SGIS_multisample;
#endif
#ifdef GL_SGIS_texture_lod
		else if (!strcmp(name, "SGIS_TEXTURE_LOD")) return GL_SGIS_texture_lod;
#endif
#ifdef GL_SGIX_async_pixel
		else if (!strcmp(name, "SGIX_ASYNC_PIXEL")) return GL_SGIX_async_pixel;
#endif
#ifdef GL_SGIX_instruments
		else if (!strcmp(name, "SGIX_INSTRUMENTS")) return GL_SGIX_instruments;
#endif
#ifdef GL_SGIX_pixel_tiles
		else if (!strcmp(name, "SGIX_PIXEL_TILES")) return GL_SGIX_pixel_tiles;
#endif
#ifdef GL_SIGNED_NEGATE_NV
		else if (!strcmp(name, "SIGNED_NEGATE_NV")) return GL_SIGNED_NEGATE_NV;
#endif
#ifdef GL_SINGLE_COLOR_EXT
		else if (!strcmp(name, "SINGLE_COLOR_EXT")) return GL_SINGLE_COLOR_EXT;
#endif
#ifdef GL_SOURCE3_ALPHA_NV
		else if (!strcmp(name, "SOURCE3_ALPHA_NV")) return GL_SOURCE3_ALPHA_NV;
#endif
#ifdef GL_SPRITE_AXIS_SGIX
		else if (!strcmp(name, "SPRITE_AXIS_SGIX")) return GL_SPRITE_AXIS_SGIX;
#endif
#ifdef GL_SPRITE_MODE_SGIX
		else if (!strcmp(name, "SPRITE_MODE_SGIX")) return GL_SPRITE_MODE_SGIX;
#endif
#ifdef GL_SUN_global_alpha
		else if (!strcmp(name, "SUN_GLOBAL_ALPHA")) return GL_SUN_global_alpha;
#endif
#ifdef GL_TEXTURE_ENV_MODE
		else if (!strcmp(name, "TEXTURE_ENV_MODE")) return GL_TEXTURE_ENV_MODE;
#endif
#ifdef GL_TEXTURE_GEN_MODE
		else if (!strcmp(name, "TEXTURE_GEN_MODE")) return GL_TEXTURE_GEN_MODE;
#endif
#ifdef GL_TEXTURE_PRIORITY
		else if (!strcmp(name, "TEXTURE_PRIORITY")) return GL_TEXTURE_PRIORITY;
#endif
#ifdef GL_TEXTURE_RED_SIZE
		else if (!strcmp(name, "TEXTURE_RED_SIZE")) return GL_TEXTURE_RED_SIZE;
#endif
#ifdef GL_TEXTURE_RESIDENT
		else if (!strcmp(name, "TEXTURE_RESIDENT")) return GL_TEXTURE_RESIDENT;
#endif
#ifdef GL_UNPACK_ALIGNMENT
		else if (!strcmp(name, "UNPACK_ALIGNMENT")) return GL_UNPACK_ALIGNMENT;
#endif
#ifdef GL_UNPACK_LSB_FIRST
		else if (!strcmp(name, "UNPACK_LSB_FIRST")) return GL_UNPACK_LSB_FIRST;
#endif
#ifdef GL_UNPACK_SKIP_ROWS
		else if (!strcmp(name, "UNPACK_SKIP_ROWS")) return GL_UNPACK_SKIP_ROWS;
#endif
#ifdef GL_VERTEX23_BIT_PGI
		else if (!strcmp(name, "VERTEX23_BIT_PGI")) return GL_VERTEX23_BIT_PGI;
#endif
#ifdef GL_VERTEX_ARRAY_EXT
		else if (!strcmp(name, "VERTEX_ARRAY_EXT")) return GL_VERTEX_ARRAY_EXT;
#endif
#ifdef GL_VERTEX_BLEND_ARB
		else if (!strcmp(name, "VERTEX_BLEND_ARB")) return GL_VERTEX_BLEND_ARB;
#endif
#ifdef GL_WEIGHT_ARRAY_ARB
		else if (!strcmp(name, "WEIGHT_ARRAY_ARB")) return GL_WEIGHT_ARRAY_ARB;
#endif
#ifdef GL_WIN_specular_fog
		else if (!strcmp(name, "WIN_SPECULAR_FOG")) return GL_WIN_specular_fog;
#endif
#ifdef WIN32_LEAN_AND_MEAN
		else if (!strcmp(name, "WIN32_LEAN_AND_MEAN")) return WIN32_LEAN_AND_MEAN;
#endif
		else return _GL_UNDEFINED;
	case 20:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_ACCUM_CLEAR_VALUE
		else if (!strcmp(name, "ACCUM_CLEAR_VALUE")) return GL_ACCUM_CLEAR_VALUE;
#endif
#ifdef GL_ASYNC_MARKER_SGIX
		else if (!strcmp(name, "ASYNC_MARKER_SGIX")) return GL_ASYNC_MARKER_SGIX;
#endif
#ifdef GL_BLEND_DST_RGB_EXT
		else if (!strcmp(name, "BLEND_DST_RGB_EXT")) return GL_BLEND_DST_RGB_EXT;
#endif
#ifdef GL_BLEND_SRC_RGB_EXT
		else if (!strcmp(name, "BLEND_SRC_RGB_EXT")) return GL_BLEND_SRC_RGB_EXT;
#endif
#ifdef GL_CLIP_FAR_HINT_PGI
		else if (!strcmp(name, "CLIP_FAR_HINT_PGI")) return GL_CLIP_FAR_HINT_PGI;
#endif
#ifdef GL_COLOR_CLEAR_VALUE
		else if (!strcmp(name, "COLOR_CLEAR_VALUE")) return GL_COLOR_CLEAR_VALUE;
#endif
#ifdef GL_COLOR_INDEX12_EXT
		else if (!strcmp(name, "COLOR_INDEX12_EXT")) return GL_COLOR_INDEX12_EXT;
#endif
#ifdef GL_COLOR_INDEX16_EXT
		else if (!strcmp(name, "COLOR_INDEX16_EXT")) return GL_COLOR_INDEX16_EXT;
#endif
#ifdef GL_COLOR_TABLE_SCALE
		else if (!strcmp(name, "COLOR_TABLE_SCALE")) return GL_COLOR_TABLE_SCALE;
#endif
#ifdef GL_COLOR_TABLE_WIDTH
		else if (!strcmp(name, "COLOR_TABLE_WIDTH")) return GL_COLOR_TABLE_WIDTH;
#endif
#ifdef GL_COMBINER_INPUT_NV
		else if (!strcmp(name, "COMBINER_INPUT_NV")) return GL_COMBINER_INPUT_NV;
#endif
#ifdef GL_COMBINER_SCALE_NV
		else if (!strcmp(name, "COMBINER_SCALE_NV")) return GL_COMBINER_SCALE_NV;
#endif
#ifdef GL_COMBINE_ALPHA_EXT
		else if (!strcmp(name, "COMBINE_ALPHA_EXT")) return GL_COMBINE_ALPHA_EXT;
#endif
#ifdef GL_CONVOLUTION_WIDTH
		else if (!strcmp(name, "CONVOLUTION_WIDTH")) return GL_CONVOLUTION_WIDTH;
#endif
#ifdef GL_DEPTH_CLEAR_VALUE
		else if (!strcmp(name, "DEPTH_CLEAR_VALUE")) return GL_DEPTH_CLEAR_VALUE;
#endif
#ifdef GL_DUAL_ALPHA12_SGIS
		else if (!strcmp(name, "DUAL_ALPHA12_SGIS")) return GL_DUAL_ALPHA12_SGIS;
#endif
#ifdef GL_DUAL_ALPHA16_SGIS
		else if (!strcmp(name, "DUAL_ALPHA16_SGIS")) return GL_DUAL_ALPHA16_SGIS;
#endif
#ifdef GL_EXT_index_texture
		else if (!strcmp(name, "EXT_INDEX_TEXTURE")) return GL_EXT_index_texture;
#endif
#ifdef GL_EXT_light_texture
		else if (!strcmp(name, "EXT_LIGHT_TEXTURE")) return GL_EXT_light_texture;
#endif
#ifdef GL_EXT_packed_pixels
		else if (!strcmp(name, "EXT_PACKED_PIXELS")) return GL_EXT_packed_pixels;
#endif
#ifdef GL_FUNC_SUBTRACT_EXT
		else if (!strcmp(name, "FUNC_SUBTRACT_EXT")) return GL_FUNC_SUBTRACT_EXT;
#endif
#ifdef GL_HP_occlusion_test
		else if (!strcmp(name, "HP_OCCLUSION_TEST")) return GL_HP_occlusion_test;
#endif
#ifdef GL_INDEX_CLEAR_VALUE
		else if (!strcmp(name, "INDEX_CLEAR_VALUE")) return GL_INDEX_CLEAR_VALUE;
#endif
#ifdef GL_INVALID_OPERATION
		else if (!strcmp(name, "INVALID_OPERATION")) return GL_INVALID_OPERATION;
#endif
#ifdef GL_LUMINANCE4_ALPHA4
		else if (!strcmp(name, "LUMINANCE4_ALPHA4")) return GL_LUMINANCE4_ALPHA4;
#endif
#ifdef GL_LUMINANCE6_ALPHA2
		else if (!strcmp(name, "LUMINANCE6_ALPHA2")) return GL_LUMINANCE6_ALPHA2;
#endif
#ifdef GL_LUMINANCE8_ALPHA8
		else if (!strcmp(name, "LUMINANCE8_ALPHA8")) return GL_LUMINANCE8_ALPHA8;
#endif
#ifdef GL_MAP1_BINORMAL_EXT
		else if (!strcmp(name, "MAP1_BINORMAL_EXT")) return GL_MAP1_BINORMAL_EXT;
#endif
#ifdef GL_MAP2_BINORMAL_EXT
		else if (!strcmp(name, "MAP2_BINORMAL_EXT")) return GL_MAP2_BINORMAL_EXT;
#endif
#ifdef GL_MAX_VIEWPORT_DIMS
		else if (!strcmp(name, "MAX_VIEWPORT_DIMS")) return GL_MAX_VIEWPORT_DIMS;
#endif
#ifdef GL_MINMAX_FORMAT_EXT
		else if (!strcmp(name, "MINMAX_FORMAT_EXT")) return GL_MINMAX_FORMAT_EXT;
#endif
#ifdef GL_NORMAL_ARRAY_TYPE
		else if (!strcmp(name, "NORMAL_ARRAY_TYPE")) return GL_NORMAL_ARRAY_TYPE;
#endif
#ifdef GL_OBJECT_POINT_SGIS
		else if (!strcmp(name, "OBJECT_POINT_SGIS")) return GL_OBJECT_POINT_SGIS;
#endif
#ifdef GL_OCCLUSION_TEST_HP
		else if (!strcmp(name, "OCCLUSION_TEST_HP")) return GL_OCCLUSION_TEST_HP;
#endif
#ifdef GL_OPERAND3_ALPHA_NV
		else if (!strcmp(name, "OPERAND3_ALPHA_NV")) return GL_OPERAND3_ALPHA_NV;
#endif
#ifdef GL_PACK_IMAGE_HEIGHT
		else if (!strcmp(name, "PACK_IMAGE_HEIGHT")) return GL_PACK_IMAGE_HEIGHT;
#endif
#ifdef GL_POINT_SMOOTH_HINT
		else if (!strcmp(name, "POINT_SMOOTH_HINT")) return GL_POINT_SMOOTH_HINT;
#endif
#ifdef GL_PRIMARY_COLOR_EXT
		else if (!strcmp(name, "PRIMARY_COLOR_EXT")) return GL_PRIMARY_COLOR_EXT;
#endif
#ifdef GL_PROJECTION_MATRIX
		else if (!strcmp(name, "PROJECTION_MATRIX")) return GL_PROJECTION_MATRIX;
#endif
#ifdef GL_PROXY_COLOR_TABLE
		else if (!strcmp(name, "PROXY_COLOR_TABLE")) return GL_PROXY_COLOR_TABLE;
#endif
#ifdef GL_R1UI_C4UB_V3F_SUN
		else if (!strcmp(name, "R1UI_C4UB_V3F_SUN")) return GL_R1UI_C4UB_V3F_SUN;
#endif
#ifdef GL_REFLECTION_MAP_NV
		else if (!strcmp(name, "REFLECTION_MAP_NV")) return GL_REFLECTION_MAP_NV;
#endif
#ifdef GL_SGIS_fog_function
		else if (!strcmp(name, "SGIS_FOG_FUNCTION")) return GL_SGIS_fog_function;
#endif
#ifdef GL_SGIX_flush_raster
		else if (!strcmp(name, "SGIX_FLUSH_RASTER")) return GL_SGIX_flush_raster;
#endif
#ifdef GL_SOURCE0_ALPHA_EXT
		else if (!strcmp(name, "SOURCE0_ALPHA_EXT")) return GL_SOURCE0_ALPHA_EXT;
#endif
#ifdef GL_SOURCE1_ALPHA_EXT
		else if (!strcmp(name, "SOURCE1_ALPHA_EXT")) return GL_SOURCE1_ALPHA_EXT;
#endif
#ifdef GL_SOURCE2_ALPHA_EXT
		else if (!strcmp(name, "SOURCE2_ALPHA_EXT")) return GL_SOURCE2_ALPHA_EXT;
#endif
#ifdef GL_SOURCE3_ALPHA_EXT
		else if (!strcmp(name, "SOURCE3_ALPHA_EXT")) return GL_SOURCE3_ALPHA_EXT;
#endif
#ifdef GL_SOURCE4_ALPHA_EXT
		else if (!strcmp(name, "SOURCE4_ALPHA_EXT")) return GL_SOURCE4_ALPHA_EXT;
#endif
#ifdef GL_SOURCE5_ALPHA_EXT
		else if (!strcmp(name, "SOURCE5_ALPHA_EXT")) return GL_SOURCE5_ALPHA_EXT;
#endif
#ifdef GL_SOURCE6_ALPHA_EXT
		else if (!strcmp(name, "SOURCE6_ALPHA_EXT")) return GL_SOURCE6_ALPHA_EXT;
#endif
#ifdef GL_SOURCE7_ALPHA_EXT
		else if (!strcmp(name, "SOURCE7_ALPHA_EXT")) return GL_SOURCE7_ALPHA_EXT;
#endif
#ifdef GL_SPRITE_AXIAL_SGIX
		else if (!strcmp(name, "SPRITE_AXIAL_SGIX")) return GL_SPRITE_AXIAL_SGIX;
#endif
#ifdef GL_STENCIL_WRITEMASK
		else if (!strcmp(name, "STENCIL_WRITEMASK")) return GL_STENCIL_WRITEMASK;
#endif
#ifdef GL_SUN_triangle_list
		else if (!strcmp(name, "SUN_TRIANGLE_LIST")) return GL_SUN_triangle_list;
#endif
#ifdef GL_TANGENT_ARRAY_EXT
		else if (!strcmp(name, "TANGENT_ARRAY_EXT")) return GL_TANGENT_ARRAY_EXT;
#endif
#ifdef GL_TEXCOORD1_BIT_PGI
		else if (!strcmp(name, "TEXCOORD1_BIT_PGI")) return GL_TEXCOORD1_BIT_PGI;
#endif
#ifdef GL_TEXCOORD2_BIT_PGI
		else if (!strcmp(name, "TEXCOORD2_BIT_PGI")) return GL_TEXCOORD2_BIT_PGI;
#endif
#ifdef GL_TEXCOORD3_BIT_PGI
		else if (!strcmp(name, "TEXCOORD3_BIT_PGI")) return GL_TEXCOORD3_BIT_PGI;
#endif
#ifdef GL_TEXCOORD4_BIT_PGI
		else if (!strcmp(name, "TEXCOORD4_BIT_PGI")) return GL_TEXCOORD4_BIT_PGI;
#endif
#ifdef GL_TEXTURE_BLUE_SIZE
		else if (!strcmp(name, "TEXTURE_BLUE_SIZE")) return GL_TEXTURE_BLUE_SIZE;
#endif
#ifdef GL_TEXTURE_DEPTH_EXT
		else if (!strcmp(name, "TEXTURE_DEPTH_EXT")) return GL_TEXTURE_DEPTH_EXT;
#endif
#ifdef GL_TEXTURE_ENV_COLOR
		else if (!strcmp(name, "TEXTURE_ENV_COLOR")) return GL_TEXTURE_ENV_COLOR;
#endif
#ifdef GL_TEXTURE_LIGHT_EXT
		else if (!strcmp(name, "TEXTURE_LIGHT_EXT")) return GL_TEXTURE_LIGHT_EXT;
#endif
#ifdef GL_TEXTURE_MAX_LEVEL
		else if (!strcmp(name, "TEXTURE_MAX_LEVEL")) return GL_TEXTURE_MAX_LEVEL;
#endif
#ifdef GL_TRIANGLE_LIST_SUN
		else if (!strcmp(name, "TRIANGLE_LIST_SUN")) return GL_TRIANGLE_LIST_SUN;
#endif
#ifdef GL_UNPACK_ROW_LENGTH
		else if (!strcmp(name, "UNPACK_ROW_LENGTH")) return GL_UNPACK_ROW_LENGTH;
#endif
#ifdef GL_UNPACK_SWAP_BYTES
		else if (!strcmp(name, "UNPACK_SWAP_BYTES")) return GL_UNPACK_SWAP_BYTES;
#endif
#ifdef GL_VERTEX_ARRAY_SIZE
		else if (!strcmp(name, "VERTEX_ARRAY_SIZE")) return GL_VERTEX_ARRAY_SIZE;
#endif
#ifdef GL_VERTEX_ARRAY_TYPE
		else if (!strcmp(name, "VERTEX_ARRAY_TYPE")) return GL_VERTEX_ARRAY_TYPE;
#endif
#ifdef GL_WIN_phong_shading
		else if (!strcmp(name, "WIN_PHONG_SHADING")) return GL_WIN_phong_shading;
#endif
		else return _GL_UNDEFINED;
	case 21:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_ACTIVE_TEXTURE_ARB
		else if (!strcmp(name, "ACTIVE_TEXTURE_ARB")) return GL_ACTIVE_TEXTURE_ARB;
#endif
#ifdef GL_ATTRIB_STACK_DEPTH
		else if (!strcmp(name, "ATTRIB_STACK_DEPTH")) return GL_ATTRIB_STACK_DEPTH;
#endif
#ifdef GL_BINORMAL_ARRAY_EXT
		else if (!strcmp(name, "BINORMAL_ARRAY_EXT")) return GL_BINORMAL_ARRAY_EXT;
#endif
#ifdef GL_BLEND_EQUATION_EXT
		else if (!strcmp(name, "BLEND_EQUATION_EXT")) return GL_BLEND_EQUATION_EXT;
#endif
#ifdef GL_CLAMP_TO_EDGE_SGIS
		else if (!strcmp(name, "CLAMP_TO_EDGE_SGIS")) return GL_CLAMP_TO_EDGE_SGIS;
#endif
#ifdef GL_CLIP_NEAR_HINT_PGI
		else if (!strcmp(name, "CLIP_NEAR_HINT_PGI")) return GL_CLIP_NEAR_HINT_PGI;
#endif
#ifdef GL_COLOR_ARRAY_STRIDE
		else if (!strcmp(name, "COLOR_ARRAY_STRIDE")) return GL_COLOR_ARRAY_STRIDE;
#endif
#ifdef GL_COLOR_SUM_CLAMP_NV
		else if (!strcmp(name, "COLOR_SUM_CLAMP_NV")) return GL_COLOR_SUM_CLAMP_NV;
#endif
#ifdef GL_COLOR_TABLE_FORMAT
		else if (!strcmp(name, "COLOR_TABLE_FORMAT")) return GL_COLOR_TABLE_FORMAT;
#endif
#ifdef GL_COMPRESSED_RGB_ARB
		else if (!strcmp(name, "COMPRESSED_RGB_ARB")) return GL_COMPRESSED_RGB_ARB;
#endif
#ifdef GL_CONSTANT_ALPHA_EXT
		else if (!strcmp(name, "CONSTANT_ALPHA_EXT")) return GL_CONSTANT_ALPHA_EXT;
#endif
#ifdef GL_CONSTANT_BORDER_HP
		else if (!strcmp(name, "CONSTANT_BORDER_HP")) return GL_CONSTANT_BORDER_HP;
#endif
#ifdef GL_CONSTANT_COLOR0_NV
		else if (!strcmp(name, "CONSTANT_COLOR0_NV")) return GL_CONSTANT_COLOR0_NV;
#endif
#ifdef GL_CONSTANT_COLOR1_NV
		else if (!strcmp(name, "CONSTANT_COLOR1_NV")) return GL_CONSTANT_COLOR1_NV;
#endif
#ifdef GL_CONSTANT_COLOR_EXT
		else if (!strcmp(name, "CONSTANT_COLOR_EXT")) return GL_CONSTANT_COLOR_EXT;
#endif
#ifdef GL_CONVOLUTION_1D_EXT
		else if (!strcmp(name, "CONVOLUTION_1D_EXT")) return GL_CONVOLUTION_1D_EXT;
#endif
#ifdef GL_CONVOLUTION_2D_EXT
		else if (!strcmp(name, "CONVOLUTION_2D_EXT")) return GL_CONVOLUTION_2D_EXT;
#endif
#ifdef GL_CONVOLUTION_FORMAT
		else if (!strcmp(name, "CONVOLUTION_FORMAT")) return GL_CONVOLUTION_FORMAT;
#endif
#ifdef GL_CONVOLUTION_HEIGHT
		else if (!strcmp(name, "CONVOLUTION_HEIGHT")) return GL_CONVOLUTION_HEIGHT;
#endif
#ifdef GL_CURRENT_WEIGHT_ARB
		else if (!strcmp(name, "CURRENT_WEIGHT_ARB")) return GL_CURRENT_WEIGHT_ARB;
#endif
#ifdef GL_EMBOSS_CONSTANT_NV
		else if (!strcmp(name, "EMBOSS_CONSTANT_NV")) return GL_EMBOSS_CONSTANT_NV;
#endif
#ifdef GL_EXT_blend_logic_op
		else if (!strcmp(name, "EXT_BLEND_LOGIC_OP")) return GL_EXT_blend_logic_op;
#endif
#ifdef GL_EXT_blend_subtract
		else if (!strcmp(name, "EXT_BLEND_SUBTRACT")) return GL_EXT_blend_subtract;
#endif
#ifdef GL_EXT_color_subtable
		else if (!strcmp(name, "EXT_COLOR_SUBTABLE")) return GL_EXT_color_subtable;
#endif
#ifdef GL_EXT_index_material
		else if (!strcmp(name, "EXT_INDEX_MATERIAL")) return GL_EXT_index_material;
#endif
#ifdef GL_EXT_misc_attribute
		else if (!strcmp(name, "EXT_MISC_ATTRIBUTE")) return GL_EXT_misc_attribute;
#endif
#ifdef GL_EXT_polygon_offset
		else if (!strcmp(name, "EXT_POLYGON_OFFSET")) return GL_EXT_polygon_offset;
#endif
#ifdef GL_EXT_rescale_normal
		else if (!strcmp(name, "EXT_RESCALE_NORMAL")) return GL_EXT_rescale_normal;
#endif
#ifdef GL_EXT_texture_object
		else if (!strcmp(name, "EXT_TEXTURE_OBJECT")) return GL_EXT_texture_object;
#endif
#ifdef GL_FOG_COORDINATE_EXT
		else if (!strcmp(name, "FOG_COORDINATE_EXT")) return GL_FOG_COORDINATE_EXT;
#endif
#ifdef GL_FRAGMENT_COLOR_EXT
		else if (!strcmp(name, "FRAGMENT_COLOR_EXT")) return GL_FRAGMENT_COLOR_EXT;
#endif
#ifdef GL_FRAGMENT_DEPTH_EXT
		else if (!strcmp(name, "FRAGMENT_DEPTH_EXT")) return GL_FRAGMENT_DEPTH_EXT;
#endif
#ifdef GL_HISTOGRAM_RED_SIZE
		else if (!strcmp(name, "HISTOGRAM_RED_SIZE")) return GL_HISTOGRAM_RED_SIZE;
#endif
#ifdef GL_HISTOGRAM_SINK_EXT
		else if (!strcmp(name, "HISTOGRAM_SINK_EXT")) return GL_HISTOGRAM_SINK_EXT;
#endif
#ifdef GL_HP_image_transform
		else if (!strcmp(name, "HP_IMAGE_TRANSFORM")) return GL_HP_image_transform;
#endif
#ifdef GL_IBM_rasterpos_clip
		else if (!strcmp(name, "IBM_RASTERPOS_CLIP")) return GL_IBM_rasterpos_clip;
#endif
#ifdef GL_INDEX_ARRAY_STRIDE
		else if (!strcmp(name, "INDEX_ARRAY_STRIDE")) return GL_INDEX_ARRAY_STRIDE;
#endif
#ifdef GL_INDEX_MATERIAL_EXT
		else if (!strcmp(name, "INDEX_MATERIAL_EXT")) return GL_INDEX_MATERIAL_EXT;
#endif
#ifdef GL_INDEX_TEST_REF_EXT
		else if (!strcmp(name, "INDEX_TEST_REF_EXT")) return GL_INDEX_TEST_REF_EXT;
#endif
#ifdef GL_LINEAR_ATTENUATION
		else if (!strcmp(name, "LINEAR_ATTENUATION")) return GL_LINEAR_ATTENUATION;
#endif
#ifdef GL_LINEAR_DETAIL_SGIS
		else if (!strcmp(name, "LINEAR_DETAIL_SGIS")) return GL_LINEAR_DETAIL_SGIS;
#endif
#ifdef GL_LIST_PRIORITY_SGIX
		else if (!strcmp(name, "LIST_PRIORITY_SGIX")) return GL_LIST_PRIORITY_SGIX;
#endif
#ifdef GL_LUMINANCE12_ALPHA4
		else if (!strcmp(name, "LUMINANCE12_ALPHA4")) return GL_LUMINANCE12_ALPHA4;
#endif
#ifdef GL_MAP1_GRID_SEGMENTS
		else if (!strcmp(name, "MAP1_GRID_SEGMENTS")) return GL_MAP1_GRID_SEGMENTS;
#endif
#ifdef GL_MAP2_GRID_SEGMENTS
		else if (!strcmp(name, "MAP2_GRID_SEGMENTS")) return GL_MAP2_GRID_SEGMENTS;
#endif
#ifdef GL_OPERAND0_ALPHA_EXT
		else if (!strcmp(name, "OPERAND0_ALPHA_EXT")) return GL_OPERAND0_ALPHA_EXT;
#endif
#ifdef GL_OPERAND1_ALPHA_EXT
		else if (!strcmp(name, "OPERAND1_ALPHA_EXT")) return GL_OPERAND1_ALPHA_EXT;
#endif
#ifdef GL_OPERAND2_ALPHA_EXT
		else if (!strcmp(name, "OPERAND2_ALPHA_EXT")) return GL_OPERAND2_ALPHA_EXT;
#endif
#ifdef GL_OPERAND3_ALPHA_EXT
		else if (!strcmp(name, "OPERAND3_ALPHA_EXT")) return GL_OPERAND3_ALPHA_EXT;
#endif
#ifdef GL_OPERAND4_ALPHA_EXT
		else if (!strcmp(name, "OPERAND4_ALPHA_EXT")) return GL_OPERAND4_ALPHA_EXT;
#endif
#ifdef GL_OPERAND5_ALPHA_EXT
		else if (!strcmp(name, "OPERAND5_ALPHA_EXT")) return GL_OPERAND5_ALPHA_EXT;
#endif
#ifdef GL_OPERAND6_ALPHA_EXT
		else if (!strcmp(name, "OPERAND6_ALPHA_EXT")) return GL_OPERAND6_ALPHA_EXT;
#endif
#ifdef GL_OPERAND7_ALPHA_EXT
		else if (!strcmp(name, "OPERAND7_ALPHA_EXT")) return GL_OPERAND7_ALPHA_EXT;
#endif
#ifdef GL_PACK_CMYK_HINT_EXT
		else if (!strcmp(name, "PACK_CMYK_HINT_EXT")) return GL_PACK_CMYK_HINT_EXT;
#endif
#ifdef GL_PACK_RESAMPLE_SGIX
		else if (!strcmp(name, "PACK_RESAMPLE_SGIX")) return GL_PACK_RESAMPLE_SGIX;
#endif
#ifdef GL_PASS_THROUGH_TOKEN
		else if (!strcmp(name, "PASS_THROUGH_TOKEN")) return GL_PASS_THROUGH_TOKEN;
#endif
#ifdef GL_PIXEL_TEXTURE_SGIS
		else if (!strcmp(name, "PIXEL_TEXTURE_SGIS")) return GL_PIXEL_TEXTURE_SGIS;
#endif
#ifdef GL_PIXEL_TEX_GEN_SGIX
		else if (!strcmp(name, "PIXEL_TEX_GEN_SGIX")) return GL_PIXEL_TEX_GEN_SGIX;
#endif
#ifdef GL_POINT_SIZE_MAX_EXT
		else if (!strcmp(name, "POINT_SIZE_MAX_EXT")) return GL_POINT_SIZE_MAX_EXT;
#endif
#ifdef GL_POINT_SIZE_MIN_EXT
		else if (!strcmp(name, "POINT_SIZE_MIN_EXT")) return GL_POINT_SIZE_MIN_EXT;
#endif
#ifdef GL_POLYGON_OFFSET_EXT
		else if (!strcmp(name, "POLYGON_OFFSET_EXT")) return GL_POLYGON_OFFSET_EXT;
#endif
#ifdef GL_RED_MAX_CLAMP_INGR
		else if (!strcmp(name, "RED_MAX_CLAMP_INGR")) return GL_RED_MAX_CLAMP_INGR;
#endif
#ifdef GL_RED_MIN_CLAMP_INGR
		else if (!strcmp(name, "RED_MIN_CLAMP_INGR")) return GL_RED_MIN_CLAMP_INGR;
#endif
#ifdef GL_REFLECTION_MAP_ARB
		else if (!strcmp(name, "REFLECTION_MAP_ARB")) return GL_REFLECTION_MAP_ARB;
#endif
#ifdef GL_REFLECTION_MAP_EXT
		else if (!strcmp(name, "REFLECTION_MAP_EXT")) return GL_REFLECTION_MAP_EXT;
#endif
#ifdef GL_REPLACE_MIDDLE_SUN
		else if (!strcmp(name, "REPLACE_MIDDLE_SUN")) return GL_REPLACE_MIDDLE_SUN;
#endif
#ifdef GL_REPLACE_OLDEST_SUN
		else if (!strcmp(name, "REPLACE_OLDEST_SUN")) return GL_REPLACE_OLDEST_SUN;
#endif
#ifdef GL_RESCALE_NORMAL_EXT
		else if (!strcmp(name, "RESCALE_NORMAL_EXT")) return GL_RESCALE_NORMAL_EXT;
#endif
#ifdef GL_SAMPLE_BUFFERS_ARB
		else if (!strcmp(name, "SAMPLE_BUFFERS_ARB")) return GL_SAMPLE_BUFFERS_ARB;
#endif
#ifdef GL_SAMPLE_BUFFERS_EXT
		else if (!strcmp(name, "SAMPLE_BUFFERS_EXT")) return GL_SAMPLE_BUFFERS_EXT;
#endif
#ifdef GL_SAMPLE_PATTERN_EXT
		else if (!strcmp(name, "SAMPLE_PATTERN_EXT")) return GL_SAMPLE_PATTERN_EXT;
#endif
#ifdef GL_SECONDARY_COLOR_NV
		else if (!strcmp(name, "SECONDARY_COLOR_NV")) return GL_SECONDARY_COLOR_NV;
#endif
#ifdef GL_SGIS_pixel_texture
		else if (!strcmp(name, "SGIS_PIXEL_TEXTURE")) return GL_SGIS_pixel_texture;
#endif
#ifdef GL_SGIX_depth_texture
		else if (!strcmp(name, "SGIX_DEPTH_TEXTURE")) return GL_SGIX_depth_texture;
#endif
#ifdef GL_SGIX_list_priority
		else if (!strcmp(name, "SGIX_LIST_PRIORITY")) return GL_SGIX_list_priority;
#endif
#ifdef GL_SGIX_pixel_texture
		else if (!strcmp(name, "SGIX_PIXEL_TEXTURE")) return GL_SGIX_pixel_texture;
#endif
#ifdef GL_SGI_vertex_preclip
		else if (!strcmp(name, "SGI_VERTEX_PRECLIP")) return GL_SGI_vertex_preclip;
#endif
#ifdef GL_SIGNED_IDENTITY_NV
		else if (!strcmp(name, "SIGNED_IDENTITY_NV")) return GL_SIGNED_IDENTITY_NV;
#endif
#ifdef GL_SRC_ALPHA_SATURATE
		else if (!strcmp(name, "SRC_ALPHA_SATURATE")) return GL_SRC_ALPHA_SATURATE;
#endif
#ifdef GL_STENCIL_BUFFER_BIT
		else if (!strcmp(name, "STENCIL_BUFFER_BIT")) return GL_STENCIL_BUFFER_BIT;
#endif
#ifdef GL_STENCIL_VALUE_MASK
		else if (!strcmp(name, "STENCIL_VALUE_MASK")) return GL_STENCIL_VALUE_MASK;
#endif
#ifdef GL_SUNX_constant_data
		else if (!strcmp(name, "SUNX_CONSTANT_DATA")) return GL_SUNX_constant_data;
#endif
#ifdef GL_TEXTURE_ALPHA_SIZE
		else if (!strcmp(name, "TEXTURE_ALPHA_SIZE")) return GL_TEXTURE_ALPHA_SIZE;
#endif
#ifdef GL_TEXTURE_BASE_LEVEL
		else if (!strcmp(name, "TEXTURE_BASE_LEVEL")) return GL_TEXTURE_BASE_LEVEL;
#endif
#ifdef GL_TEXTURE_BINDING_1D
		else if (!strcmp(name, "TEXTURE_BINDING_1D")) return GL_TEXTURE_BINDING_1D;
#endif
#ifdef GL_TEXTURE_BINDING_2D
		else if (!strcmp(name, "TEXTURE_BINDING_2D")) return GL_TEXTURE_BINDING_2D;
#endif
#ifdef GL_TEXTURE_BINDING_3D
		else if (!strcmp(name, "TEXTURE_BINDING_3D")) return GL_TEXTURE_BINDING_3D;
#endif
#ifdef GL_TEXTURE_COMPONENTS
		else if (!strcmp(name, "TEXTURE_COMPONENTS")) return GL_TEXTURE_COMPONENTS;
#endif
#ifdef GL_TEXTURE_GREEN_SIZE
		else if (!strcmp(name, "TEXTURE_GREEN_SIZE")) return GL_TEXTURE_GREEN_SIZE;
#endif
#ifdef GL_TEXTURE_MAG_FILTER
		else if (!strcmp(name, "TEXTURE_MAG_FILTER")) return GL_TEXTURE_MAG_FILTER;
#endif
#ifdef GL_TEXTURE_MIN_FILTER
		else if (!strcmp(name, "TEXTURE_MIN_FILTER")) return GL_TEXTURE_MIN_FILTER;
#endif
#ifdef GL_TEXTURE_NORMAL_EXT
		else if (!strcmp(name, "TEXTURE_NORMAL_EXT")) return GL_TEXTURE_NORMAL_EXT;
#endif
#ifdef GL_TEXTURE_WRAP_R_EXT
		else if (!strcmp(name, "TEXTURE_WRAP_R_EXT")) return GL_TEXTURE_WRAP_R_EXT;
#endif
#ifdef GL_UNPACK_SKIP_IMAGES
		else if (!strcmp(name, "UNPACK_SKIP_IMAGES")) return GL_UNPACK_SKIP_IMAGES;
#endif
#ifdef GL_UNPACK_SKIP_PIXELS
		else if (!strcmp(name, "UNPACK_SKIP_PIXELS")) return GL_UNPACK_SKIP_PIXELS;
#endif
#ifdef GL_UNSIGNED_INVERT_NV
		else if (!strcmp(name, "UNSIGNED_INVERT_NV")) return GL_UNSIGNED_INVERT_NV;
#endif
#ifdef GL_WIDE_LINE_HINT_PGI
		else if (!strcmp(name, "WIDE_LINE_HINT_PGI")) return GL_WIDE_LINE_HINT_PGI;
#endif
		else return _GL_UNDEFINED;
	case 22:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_422_REV_AVERAGE_EXT
		else if (!strcmp(name, "422_REV_AVERAGE_EXT")) return GL_422_REV_AVERAGE_EXT;
#endif
#ifdef GL_AMBIENT_AND_DIFFUSE
		else if (!strcmp(name, "AMBIENT_AND_DIFFUSE")) return GL_AMBIENT_AND_DIFFUSE;
#endif
#ifdef GL_ARB_texture_env_add
		else if (!strcmp(name, "ARB_TEXTURE_ENV_ADD")) return GL_ARB_texture_env_add;
#endif
#ifdef GL_BLEND_DST_ALPHA_EXT
		else if (!strcmp(name, "BLEND_DST_ALPHA_EXT")) return GL_BLEND_DST_ALPHA_EXT;
#endif
#ifdef GL_BLEND_SRC_ALPHA_EXT
		else if (!strcmp(name, "BLEND_SRC_ALPHA_EXT")) return GL_BLEND_SRC_ALPHA_EXT;
#endif
#ifdef GL_BLUE_MAX_CLAMP_INGR
		else if (!strcmp(name, "BLUE_MAX_CLAMP_INGR")) return GL_BLUE_MAX_CLAMP_INGR;
#endif
#ifdef GL_BLUE_MIN_CLAMP_INGR
		else if (!strcmp(name, "BLUE_MIN_CLAMP_INGR")) return GL_BLUE_MIN_CLAMP_INGR;
#endif
#ifdef GL_COLOR_ARRAY_POINTER
		else if (!strcmp(name, "COLOR_ARRAY_POINTER")) return GL_COLOR_ARRAY_POINTER;
#endif
#ifdef GL_COLOR_MATERIAL_FACE
		else if (!strcmp(name, "COLOR_MATERIAL_FACE")) return GL_COLOR_MATERIAL_FACE;
#endif
#ifdef GL_COMBINER_MAPPING_NV
		else if (!strcmp(name, "COMBINER_MAPPING_NV")) return GL_COMBINER_MAPPING_NV;
#endif
#ifdef GL_COMBINER_MUX_SUM_NV
		else if (!strcmp(name, "COMBINER_MUX_SUM_NV")) return GL_COMBINER_MUX_SUM_NV;
#endif
#ifdef GL_COMPILE_AND_EXECUTE
		else if (!strcmp(name, "COMPILE_AND_EXECUTE")) return GL_COMPILE_AND_EXECUTE;
#endif
#ifdef GL_COMPRESSED_RGBA_ARB
		else if (!strcmp(name, "COMPRESSED_RGBA_ARB")) return GL_COMPRESSED_RGBA_ARB;
#endif
#ifdef GL_CURRENT_TANGENT_EXT
		else if (!strcmp(name, "CURRENT_TANGENT_EXT")) return GL_CURRENT_TANGENT_EXT;
#endif
#ifdef GL_EDGE_FLAG_ARRAY_EXT
		else if (!strcmp(name, "EDGE_FLAG_ARRAY_EXT")) return GL_EDGE_FLAG_ARRAY_EXT;
#endif
#ifdef GL_EXT_pixel_transform
		else if (!strcmp(name, "EXT_PIXEL_TRANSFORM")) return GL_EXT_pixel_transform;
#endif
#ifdef GL_EXT_secondary_color
		else if (!strcmp(name, "EXT_SECONDARY_COLOR")) return GL_EXT_secondary_color;
#endif
#ifdef GL_EXT_texture_env_add
		else if (!strcmp(name, "EXT_TEXTURE_ENV_ADD")) return GL_EXT_texture_env_add;
#endif
#ifdef GL_FRAGMENT_NORMAL_EXT
		else if (!strcmp(name, "FRAGMENT_NORMAL_EXT")) return GL_FRAGMENT_NORMAL_EXT;
#endif
#ifdef GL_HALF_BIAS_NEGATE_NV
		else if (!strcmp(name, "HALF_BIAS_NEGATE_NV")) return GL_HALF_BIAS_NEGATE_NV;
#endif
#ifdef GL_HALF_BIAS_NORMAL_NV
		else if (!strcmp(name, "HALF_BIAS_NORMAL_NV")) return GL_HALF_BIAS_NORMAL_NV;
#endif
#ifdef GL_HISTOGRAM_BLUE_SIZE
		else if (!strcmp(name, "HISTOGRAM_BLUE_SIZE")) return GL_HISTOGRAM_BLUE_SIZE;
#endif
#ifdef GL_HISTOGRAM_WIDTH_EXT
		else if (!strcmp(name, "HISTOGRAM_WIDTH_EXT")) return GL_HISTOGRAM_WIDTH_EXT;
#endif
#ifdef GL_HP_texture_lighting
		else if (!strcmp(name, "HP_TEXTURE_LIGHTING")) return GL_HP_texture_lighting;
#endif
#ifdef GL_IMAGE_MAG_FILTER_HP
		else if (!strcmp(name, "IMAGE_MAG_FILTER_HP")) return GL_IMAGE_MAG_FILTER_HP;
#endif
#ifdef GL_IMAGE_MIN_FILTER_HP
		else if (!strcmp(name, "IMAGE_MIN_FILTER_HP")) return GL_IMAGE_MIN_FILTER_HP;
#endif
#ifdef GL_INDEX_ARRAY_POINTER
		else if (!strcmp(name, "INDEX_ARRAY_POINTER")) return GL_INDEX_ARRAY_POINTER;
#endif
#ifdef GL_INDEX_TEST_FUNC_EXT
		else if (!strcmp(name, "INDEX_TEST_FUNC_EXT")) return GL_INDEX_TEST_FUNC_EXT;
#endif
#ifdef GL_INGR_interlace_read
		else if (!strcmp(name, "INGR_INTERLACE_READ")) return GL_INGR_interlace_read;
#endif
#ifdef GL_INTERLACE_READ_INGR
		else if (!strcmp(name, "INTERLACE_READ_INGR")) return GL_INTERLACE_READ_INGR;
#endif
#ifdef GL_IR_INSTRUMENT1_SGIX
		else if (!strcmp(name, "IR_INSTRUMENT1_SGIX")) return GL_IR_INSTRUMENT1_SGIX;
#endif
#ifdef GL_LIGHT_ENV_MODE_SGIX
		else if (!strcmp(name, "LIGHT_ENV_MODE_SGIX")) return GL_LIGHT_ENV_MODE_SGIX;
#endif
#ifdef GL_LIGHT_MODEL_AMBIENT
		else if (!strcmp(name, "LIGHT_MODEL_AMBIENT")) return GL_LIGHT_MODEL_AMBIENT;
#endif
#ifdef GL_LINEAR_SHARPEN_SGIS
		else if (!strcmp(name, "LINEAR_SHARPEN_SGIS")) return GL_LINEAR_SHARPEN_SGIS;
#endif
#ifdef GL_LINE_STIPPLE_REPEAT
		else if (!strcmp(name, "LINE_STIPPLE_REPEAT")) return GL_LINE_STIPPLE_REPEAT;
#endif
#ifdef GL_LUMINANCE12_ALPHA12
		else if (!strcmp(name, "LUMINANCE12_ALPHA12")) return GL_LUMINANCE12_ALPHA12;
#endif
#ifdef GL_LUMINANCE16_ALPHA16
		else if (!strcmp(name, "LUMINANCE16_ALPHA16")) return GL_LUMINANCE16_ALPHA16;
#endif
#ifdef GL_MAT_AMBIENT_BIT_PGI
		else if (!strcmp(name, "MAT_AMBIENT_BIT_PGI")) return GL_MAT_AMBIENT_BIT_PGI;
#endif
#ifdef GL_MAT_DIFFUSE_BIT_PGI
		else if (!strcmp(name, "MAT_DIFFUSE_BIT_PGI")) return GL_MAT_DIFFUSE_BIT_PGI;
#endif
#ifdef GL_MAX_3D_TEXTURE_SIZE
		else if (!strcmp(name, "MAX_3D_TEXTURE_SIZE")) return GL_MAX_3D_TEXTURE_SIZE;
#endif
#ifdef GL_MAX_PIXEL_MAP_TABLE
		else if (!strcmp(name, "MAX_PIXEL_MAP_TABLE")) return GL_MAX_PIXEL_MAP_TABLE;
#endif
#ifdef GL_MAX_VERTEX_HINT_PGI
		else if (!strcmp(name, "MAX_VERTEX_HINT_PGI")) return GL_MAX_VERTEX_HINT_PGI;
#endif
#ifdef GL_MESA_resize_buffers
		else if (!strcmp(name, "MESA_RESIZE_BUFFERS")) return GL_MESA_resize_buffers;
#endif
#ifdef GL_MULTISAMPLE_BIT_ARB
		else if (!strcmp(name, "MULTISAMPLE_BIT_ARB")) return GL_MULTISAMPLE_BIT_ARB;
#endif
#ifdef GL_NORMAL_ARRAY_STRIDE
		else if (!strcmp(name, "NORMAL_ARRAY_STRIDE")) return GL_NORMAL_ARRAY_STRIDE;
#endif
#ifdef GL_ONE_MINUS_DST_ALPHA
		else if (!strcmp(name, "ONE_MINUS_DST_ALPHA")) return GL_ONE_MINUS_DST_ALPHA;
#endif
#ifdef GL_ONE_MINUS_DST_COLOR
		else if (!strcmp(name, "ONE_MINUS_DST_COLOR")) return GL_ONE_MINUS_DST_COLOR;
#endif
#ifdef GL_ONE_MINUS_SRC_ALPHA
		else if (!strcmp(name, "ONE_MINUS_SRC_ALPHA")) return GL_ONE_MINUS_SRC_ALPHA;
#endif
#ifdef GL_ONE_MINUS_SRC_COLOR
		else if (!strcmp(name, "ONE_MINUS_SRC_COLOR")) return GL_ONE_MINUS_SRC_COLOR;
#endif
#ifdef GL_POINT_SIZE_MAX_SGIS
		else if (!strcmp(name, "POINT_SIZE_MAX_SGIS")) return GL_POINT_SIZE_MAX_SGIS;
#endif
#ifdef GL_POINT_SIZE_MIN_SGIS
		else if (!strcmp(name, "POINT_SIZE_MIN_SGIS")) return GL_POINT_SIZE_MIN_SGIS;
#endif
#ifdef GL_POLYGON_OFFSET_FILL
		else if (!strcmp(name, "POLYGON_OFFSET_FILL")) return GL_POLYGON_OFFSET_FILL;
#endif
#ifdef GL_POLYGON_OFFSET_LINE
		else if (!strcmp(name, "POLYGON_OFFSET_LINE")) return GL_POLYGON_OFFSET_LINE;
#endif
#ifdef GL_POLYGON_SMOOTH_HINT
		else if (!strcmp(name, "POLYGON_SMOOTH_HINT")) return GL_POLYGON_SMOOTH_HINT;
#endif
#ifdef GL_POLYGON_STIPPLE_BIT
		else if (!strcmp(name, "POLYGON_STIPPLE_BIT")) return GL_POLYGON_STIPPLE_BIT;
#endif
#ifdef GL_PROXY_HISTOGRAM_EXT
		else if (!strcmp(name, "PROXY_HISTOGRAM_EXT")) return GL_PROXY_HISTOGRAM_EXT;
#endif
#ifdef GL_REPLICATE_BORDER_HP
		else if (!strcmp(name, "REPLICATE_BORDER_HP")) return GL_REPLICATE_BORDER_HP;
#endif
#ifdef GL_SAMPLE_BUFFERS_3DFX
		else if (!strcmp(name, "SAMPLE_BUFFERS_3DFX")) return GL_SAMPLE_BUFFERS_3DFX;
#endif
#ifdef GL_SAMPLE_BUFFERS_SGIS
		else if (!strcmp(name, "SAMPLE_BUFFERS_SGIS")) return GL_SAMPLE_BUFFERS_SGIS;
#endif
#ifdef GL_SAMPLE_COVERAGE_ARB
		else if (!strcmp(name, "SAMPLE_COVERAGE_ARB")) return GL_SAMPLE_COVERAGE_ARB;
#endif
#ifdef GL_SAMPLE_PATTERN_SGIS
		else if (!strcmp(name, "SAMPLE_PATTERN_SGIS")) return GL_SAMPLE_PATTERN_SGIS;
#endif
#ifdef GL_SGIS_detail_texture
		else if (!strcmp(name, "SGIS_DETAIL_TEXTURE")) return GL_SGIS_detail_texture;
#endif
#ifdef GL_SGIX_ir_instrument1
		else if (!strcmp(name, "SGIX_IR_INSTRUMENT1")) return GL_SGIX_ir_instrument1;
#endif
#ifdef GL_SGIX_polynomial_ffd
		else if (!strcmp(name, "SGIX_POLYNOMIAL_FFD")) return GL_SGIX_polynomial_ffd;
#endif
#ifdef GL_SGIX_shadow_ambient
		else if (!strcmp(name, "SGIX_SHADOW_AMBIENT")) return GL_SGIX_shadow_ambient;
#endif
#ifdef GL_SGIX_texture_select
		else if (!strcmp(name, "SGIX_TEXTURE_SELECT")) return GL_SGIX_texture_select;
#endif
#ifdef GL_SHADOW_AMBIENT_SGIX
		else if (!strcmp(name, "SHADOW_AMBIENT_SGIX")) return GL_SHADOW_AMBIENT_SGIX;
#endif
#ifdef GL_STENCIL_CLEAR_VALUE
		else if (!strcmp(name, "STENCIL_CLEAR_VALUE")) return GL_STENCIL_CLEAR_VALUE;
#endif
#ifdef GL_T2F_IUI_N3F_V2F_EXT
		else if (!strcmp(name, "T2F_IUI_N3F_V2F_EXT")) return GL_T2F_IUI_N3F_V2F_EXT;
#endif
#ifdef GL_T2F_IUI_N3F_V3F_EXT
		else if (!strcmp(name, "T2F_IUI_N3F_V3F_EXT")) return GL_T2F_IUI_N3F_V3F_EXT;
#endif
#ifdef GL_TABLE_TOO_LARGE_EXT
		else if (!strcmp(name, "TABLE_TOO_LARGE_EXT")) return GL_TABLE_TOO_LARGE_EXT;
#endif
#ifdef GL_TEXTURE_4DSIZE_SGIS
		else if (!strcmp(name, "TEXTURE_4DSIZE_SGIS")) return GL_TEXTURE_4DSIZE_SGIS;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY
		else if (!strcmp(name, "TEXTURE_COORD_ARRAY")) return GL_TEXTURE_COORD_ARRAY;
#endif
#ifdef GL_TEXTURE_STACK_DEPTH
		else if (!strcmp(name, "TEXTURE_STACK_DEPTH")) return GL_TEXTURE_STACK_DEPTH;
#endif
#ifdef GL_TEXTURE_WRAP_Q_SGIS
		else if (!strcmp(name, "TEXTURE_WRAP_Q_SGIS")) return GL_TEXTURE_WRAP_Q_SGIS;
#endif
#ifdef GL_UNPACK_IMAGE_HEIGHT
		else if (!strcmp(name, "UNPACK_IMAGE_HEIGHT")) return GL_UNPACK_IMAGE_HEIGHT;
#endif
#ifdef GL_UNSIGNED_BYTE_3_3_2
		else if (!strcmp(name, "UNSIGNED_BYTE_3_3_2")) return GL_UNSIGNED_BYTE_3_3_2;
#endif
#ifdef GL_VERTEX_ARRAY_STRIDE
		else if (!strcmp(name, "VERTEX_ARRAY_STRIDE")) return GL_VERTEX_ARRAY_STRIDE;
#endif
#ifdef GL_VERTEX_PRECLIP_SGIX
		else if (!strcmp(name, "VERTEX_PRECLIP_SGIX")) return GL_VERTEX_PRECLIP_SGIX;
#endif
		else return _GL_UNDEFINED;
	case 23:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_ALPHA_MAX_CLAMP_INGR
		else if (!strcmp(name, "ALPHA_MAX_CLAMP_INGR")) return GL_ALPHA_MAX_CLAMP_INGR;
#endif
#ifdef GL_ALPHA_MIN_CLAMP_INGR
		else if (!strcmp(name, "ALPHA_MIN_CLAMP_INGR")) return GL_ALPHA_MIN_CLAMP_INGR;
#endif
#ifdef GL_ALWAYS_FAST_HINT_PGI
		else if (!strcmp(name, "ALWAYS_FAST_HINT_PGI")) return GL_ALWAYS_FAST_HINT_PGI;
#endif
#ifdef GL_ALWAYS_SOFT_HINT_PGI
		else if (!strcmp(name, "ALWAYS_SOFT_HINT_PGI")) return GL_ALWAYS_SOFT_HINT_PGI;
#endif
#ifdef GL_APPLE_transform_hint
		else if (!strcmp(name, "APPLE_TRANSFORM_HINT")) return GL_APPLE_transform_hint;
#endif
#ifdef GL_ARB_texture_cube_map
		else if (!strcmp(name, "ARB_TEXTURE_CUBE_MAP")) return GL_ARB_texture_cube_map;
#endif
#ifdef GL_ARB_transpose_matrix
		else if (!strcmp(name, "ARB_TRANSPOSE_MATRIX")) return GL_ARB_transpose_matrix;
#endif
#ifdef GL_ASYNC_HISTOGRAM_SGIX
		else if (!strcmp(name, "ASYNC_HISTOGRAM_SGIX")) return GL_ASYNC_HISTOGRAM_SGIX;
#endif
#ifdef GL_ASYNC_TEX_IMAGE_SGIX
		else if (!strcmp(name, "ASYNC_TEX_IMAGE_SGIX")) return GL_ASYNC_TEX_IMAGE_SGIX;
#endif
#ifdef GL_CLAMP_TO_BORDER_SGIS
		else if (!strcmp(name, "CLAMP_TO_BORDER_SGIS")) return GL_CLAMP_TO_BORDER_SGIS;
#endif
#ifdef GL_COLOR_ARRAY_LIST_IBM
		else if (!strcmp(name, "COLOR_ARRAY_LIST_IBM")) return GL_COLOR_ARRAY_LIST_IBM;
#endif
#ifdef GL_COLOR_ARRAY_SIZE_EXT
		else if (!strcmp(name, "COLOR_ARRAY_SIZE_EXT")) return GL_COLOR_ARRAY_SIZE_EXT;
#endif
#ifdef GL_COLOR_ARRAY_TYPE_EXT
		else if (!strcmp(name, "COLOR_ARRAY_TYPE_EXT")) return GL_COLOR_ARRAY_TYPE_EXT;
#endif
#ifdef GL_COLOR_TABLE_BIAS_SGI
		else if (!strcmp(name, "COLOR_TABLE_BIAS_SGI")) return GL_COLOR_TABLE_BIAS_SGI;
#endif
#ifdef GL_COLOR_TABLE_RED_SIZE
		else if (!strcmp(name, "COLOR_TABLE_RED_SIZE")) return GL_COLOR_TABLE_RED_SIZE;
#endif
#ifdef GL_COMPRESSED_ALPHA_ARB
		else if (!strcmp(name, "COMPRESSED_ALPHA_ARB")) return GL_COMPRESSED_ALPHA_ARB;
#endif
#ifdef GL_CONSTANT_ATTENUATION
		else if (!strcmp(name, "CONSTANT_ATTENUATION")) return GL_CONSTANT_ATTENUATION;
#endif
#ifdef GL_CURRENT_BINORMAL_EXT
		else if (!strcmp(name, "CURRENT_BINORMAL_EXT")) return GL_CURRENT_BINORMAL_EXT;
#endif
#ifdef GL_CURRENT_RASTER_COLOR
		else if (!strcmp(name, "CURRENT_RASTER_COLOR")) return GL_CURRENT_RASTER_COLOR;
#endif
#ifdef GL_CURRENT_RASTER_INDEX
		else if (!strcmp(name, "CURRENT_RASTER_INDEX")) return GL_CURRENT_RASTER_INDEX;
#endif
#ifdef GL_DUAL_INTENSITY4_SGIS
		else if (!strcmp(name, "DUAL_INTENSITY4_SGIS")) return GL_DUAL_INTENSITY4_SGIS;
#endif
#ifdef GL_DUAL_INTENSITY8_SGIS
		else if (!strcmp(name, "DUAL_INTENSITY8_SGIS")) return GL_DUAL_INTENSITY8_SGIS;
#endif
#ifdef GL_DUAL_LUMINANCE4_SGIS
		else if (!strcmp(name, "DUAL_LUMINANCE4_SGIS")) return GL_DUAL_LUMINANCE4_SGIS;
#endif
#ifdef GL_DUAL_LUMINANCE8_SGIS
		else if (!strcmp(name, "DUAL_LUMINANCE8_SGIS")) return GL_DUAL_LUMINANCE8_SGIS;
#endif
#ifdef GL_EXT_clip_volume_hint
		else if (!strcmp(name, "EXT_CLIP_VOLUME_HINT")) return GL_EXT_clip_volume_hint;
#endif
#ifdef GL_EXT_coordinate_frame
		else if (!strcmp(name, "EXT_COORDINATE_FRAME")) return GL_EXT_coordinate_frame;
#endif
#ifdef GL_EXT_paletted_texture
		else if (!strcmp(name, "EXT_PALETTED_TEXTURE")) return GL_EXT_paletted_texture;
#endif
#ifdef GL_EXT_point_parameters
		else if (!strcmp(name, "EXT_POINT_PARAMETERS")) return GL_EXT_point_parameters;
#endif
#ifdef GL_EXT_texture_env_dot3
		else if (!strcmp(name, "EXT_TEXTURE_ENV_DOT3")) return GL_EXT_texture_env_dot3;
#endif
#ifdef GL_EXT_texture_lod_bias
		else if (!strcmp(name, "EXT_TEXTURE_LOD_BIAS")) return GL_EXT_texture_lod_bias;
#endif
#ifdef GL_EXT_vertex_weighting
		else if (!strcmp(name, "EXT_VERTEX_WEIGHTING")) return GL_EXT_vertex_weighting;
#endif
#ifdef GL_FEEDBACK_BUFFER_SIZE
		else if (!strcmp(name, "FEEDBACK_BUFFER_SIZE")) return GL_FEEDBACK_BUFFER_SIZE;
#endif
#ifdef GL_FEEDBACK_BUFFER_TYPE
		else if (!strcmp(name, "FEEDBACK_BUFFER_TYPE")) return GL_FEEDBACK_BUFFER_TYPE;
#endif
#ifdef GL_FOG_DISTANCE_MODE_NV
		else if (!strcmp(name, "FOG_DISTANCE_MODE_NV")) return GL_FOG_DISTANCE_MODE_NV;
#endif
#ifdef GL_FOG_FUNC_POINTS_SGIS
		else if (!strcmp(name, "FOG_FUNC_POINTS_SGIS")) return GL_FOG_FUNC_POINTS_SGIS;
#endif
#ifdef GL_FOG_SCALE_VALUE_SGIX
		else if (!strcmp(name, "FOG_SCALE_VALUE_SGIX")) return GL_FOG_SCALE_VALUE_SGIX;
#endif
#ifdef GL_FRAGMENT_LIGHT0_SGIX
		else if (!strcmp(name, "FRAGMENT_LIGHT0_SGIX")) return GL_FRAGMENT_LIGHT0_SGIX;
#endif
#ifdef GL_FRAGMENT_LIGHT1_SGIX
		else if (!strcmp(name, "FRAGMENT_LIGHT1_SGIX")) return GL_FRAGMENT_LIGHT1_SGIX;
#endif
#ifdef GL_FRAGMENT_LIGHT2_SGIX
		else if (!strcmp(name, "FRAGMENT_LIGHT2_SGIX")) return GL_FRAGMENT_LIGHT2_SGIX;
#endif
#ifdef GL_FRAGMENT_LIGHT3_SGIX
		else if (!strcmp(name, "FRAGMENT_LIGHT3_SGIX")) return GL_FRAGMENT_LIGHT3_SGIX;
#endif
#ifdef GL_FRAGMENT_LIGHT4_SGIX
		else if (!strcmp(name, "FRAGMENT_LIGHT4_SGIX")) return GL_FRAGMENT_LIGHT4_SGIX;
#endif
#ifdef GL_FRAGMENT_LIGHT5_SGIX
		else if (!strcmp(name, "FRAGMENT_LIGHT5_SGIX")) return GL_FRAGMENT_LIGHT5_SGIX;
#endif
#ifdef GL_FRAGMENT_LIGHT6_SGIX
		else if (!strcmp(name, "FRAGMENT_LIGHT6_SGIX")) return GL_FRAGMENT_LIGHT6_SGIX;
#endif
#ifdef GL_FRAGMENT_LIGHT7_SGIX
		else if (!strcmp(name, "FRAGMENT_LIGHT7_SGIX")) return GL_FRAGMENT_LIGHT7_SGIX;
#endif
#ifdef GL_GENERATE_MIPMAP_SGIS
		else if (!strcmp(name, "GENERATE_MIPMAP_SGIS")) return GL_GENERATE_MIPMAP_SGIS;
#endif
#ifdef GL_GREEN_MAX_CLAMP_INGR
		else if (!strcmp(name, "GREEN_MAX_CLAMP_INGR")) return GL_GREEN_MAX_CLAMP_INGR;
#endif
#ifdef GL_GREEN_MIN_CLAMP_INGR
		else if (!strcmp(name, "GREEN_MIN_CLAMP_INGR")) return GL_GREEN_MIN_CLAMP_INGR;
#endif
#ifdef GL_HISTOGRAM_ALPHA_SIZE
		else if (!strcmp(name, "HISTOGRAM_ALPHA_SIZE")) return GL_HISTOGRAM_ALPHA_SIZE;
#endif
#ifdef GL_HISTOGRAM_FORMAT_EXT
		else if (!strcmp(name, "HISTOGRAM_FORMAT_EXT")) return GL_HISTOGRAM_FORMAT_EXT;
#endif
#ifdef GL_HISTOGRAM_GREEN_SIZE
		else if (!strcmp(name, "HISTOGRAM_GREEN_SIZE")) return GL_HISTOGRAM_GREEN_SIZE;
#endif
#ifdef GL_IMAGE_TRANSLATE_X_HP
		else if (!strcmp(name, "IMAGE_TRANSLATE_X_HP")) return GL_IMAGE_TRANSLATE_X_HP;
#endif
#ifdef GL_IMAGE_TRANSLATE_Y_HP
		else if (!strcmp(name, "IMAGE_TRANSLATE_Y_HP")) return GL_IMAGE_TRANSLATE_Y_HP;
#endif
#ifdef GL_INDEX_ARRAY_LIST_IBM
		else if (!strcmp(name, "INDEX_ARRAY_LIST_IBM")) return GL_INDEX_ARRAY_LIST_IBM;
#endif
#ifdef GL_INDEX_ARRAY_TYPE_EXT
		else if (!strcmp(name, "INDEX_ARRAY_TYPE_EXT")) return GL_INDEX_ARRAY_TYPE_EXT;
#endif
#ifdef GL_LIGHT_MODEL_TWO_SIDE
		else if (!strcmp(name, "LIGHT_MODEL_TWO_SIDE")) return GL_LIGHT_MODEL_TWO_SIDE;
#endif
#ifdef GL_LINEAR_MIPMAP_LINEAR
		else if (!strcmp(name, "LINEAR_MIPMAP_LINEAR")) return GL_LINEAR_MIPMAP_LINEAR;
#endif
#ifdef GL_LINE_STIPPLE_PATTERN
		else if (!strcmp(name, "LINE_STIPPLE_PATTERN")) return GL_LINE_STIPPLE_PATTERN;
#endif
#ifdef GL_MAP1_TEXTURE_COORD_1
		else if (!strcmp(name, "MAP1_TEXTURE_COORD_1")) return GL_MAP1_TEXTURE_COORD_1;
#endif
#ifdef GL_MAP1_TEXTURE_COORD_2
		else if (!strcmp(name, "MAP1_TEXTURE_COORD_2")) return GL_MAP1_TEXTURE_COORD_2;
#endif
#ifdef GL_MAP1_TEXTURE_COORD_3
		else if (!strcmp(name, "MAP1_TEXTURE_COORD_3")) return GL_MAP1_TEXTURE_COORD_3;
#endif
#ifdef GL_MAP1_TEXTURE_COORD_4
		else if (!strcmp(name, "MAP1_TEXTURE_COORD_4")) return GL_MAP1_TEXTURE_COORD_4;
#endif
#ifdef GL_MAP2_TEXTURE_COORD_1
		else if (!strcmp(name, "MAP2_TEXTURE_COORD_1")) return GL_MAP2_TEXTURE_COORD_1;
#endif
#ifdef GL_MAP2_TEXTURE_COORD_2
		else if (!strcmp(name, "MAP2_TEXTURE_COORD_2")) return GL_MAP2_TEXTURE_COORD_2;
#endif
#ifdef GL_MAP2_TEXTURE_COORD_3
		else if (!strcmp(name, "MAP2_TEXTURE_COORD_3")) return GL_MAP2_TEXTURE_COORD_3;
#endif
#ifdef GL_MAP2_TEXTURE_COORD_4
		else if (!strcmp(name, "MAP2_TEXTURE_COORD_4")) return GL_MAP2_TEXTURE_COORD_4;
#endif
#ifdef GL_MAT_EMISSION_BIT_PGI
		else if (!strcmp(name, "MAT_EMISSION_BIT_PGI")) return GL_MAT_EMISSION_BIT_PGI;
#endif
#ifdef GL_MAT_SPECULAR_BIT_PGI
		else if (!strcmp(name, "MAT_SPECULAR_BIT_PGI")) return GL_MAT_SPECULAR_BIT_PGI;
#endif
#ifdef GL_MAX_ELEMENTS_INDICES
		else if (!strcmp(name, "MAX_ELEMENTS_INDICES")) return GL_MAX_ELEMENTS_INDICES;
#endif
#ifdef GL_MAX_NAME_STACK_DEPTH
		else if (!strcmp(name, "MAX_NAME_STACK_DEPTH")) return GL_MAX_NAME_STACK_DEPTH;
#endif
#ifdef GL_MAX_SPOT_EXPONENT_NV
		else if (!strcmp(name, "MAX_SPOT_EXPONENT_NV")) return GL_MAX_SPOT_EXPONENT_NV;
#endif
#ifdef GL_MAX_VERTEX_UNITS_ARB
		else if (!strcmp(name, "MAX_VERTEX_UNITS_ARB")) return GL_MAX_VERTEX_UNITS_ARB;
#endif
#ifdef GL_MULTISAMPLE_BIT_3DFX
		else if (!strcmp(name, "MULTISAMPLE_BIT_3DFX")) return GL_MULTISAMPLE_BIT_3DFX;
#endif
#ifdef GL_NORMAL_ARRAY_POINTER
		else if (!strcmp(name, "NORMAL_ARRAY_POINTER")) return GL_NORMAL_ARRAY_POINTER;
#endif
#ifdef GL_NV_texgen_reflection
		else if (!strcmp(name, "NV_TEXGEN_REFLECTION")) return GL_NV_texgen_reflection;
#endif
#ifdef GL_PACK_SKIP_IMAGES_EXT
		else if (!strcmp(name, "PACK_SKIP_IMAGES_EXT")) return GL_PACK_SKIP_IMAGES_EXT;
#endif
#ifdef GL_PIXEL_MAG_FILTER_EXT
		else if (!strcmp(name, "PIXEL_MAG_FILTER_EXT")) return GL_PIXEL_MAG_FILTER_EXT;
#endif
#ifdef GL_PIXEL_MIN_FILTER_EXT
		else if (!strcmp(name, "PIXEL_MIN_FILTER_EXT")) return GL_PIXEL_MIN_FILTER_EXT;
#endif
#ifdef GL_POLYGON_OFFSET_POINT
		else if (!strcmp(name, "POLYGON_OFFSET_POINT")) return GL_POLYGON_OFFSET_POINT;
#endif
#ifdef GL_POLYGON_OFFSET_UNITS
		else if (!strcmp(name, "POLYGON_OFFSET_UNITS")) return GL_POLYGON_OFFSET_UNITS;
#endif
#ifdef GL_PROXY_TEXTURE_1D_EXT
		else if (!strcmp(name, "PROXY_TEXTURE_1D_EXT")) return GL_PROXY_TEXTURE_1D_EXT;
#endif
#ifdef GL_PROXY_TEXTURE_2D_EXT
		else if (!strcmp(name, "PROXY_TEXTURE_2D_EXT")) return GL_PROXY_TEXTURE_2D_EXT;
#endif
#ifdef GL_PROXY_TEXTURE_3D_EXT
		else if (!strcmp(name, "PROXY_TEXTURE_3D_EXT")) return GL_PROXY_TEXTURE_3D_EXT;
#endif
#ifdef GL_QUAD_INTENSITY4_SGIS
		else if (!strcmp(name, "QUAD_INTENSITY4_SGIS")) return GL_QUAD_INTENSITY4_SGIS;
#endif
#ifdef GL_QUAD_INTENSITY8_SGIS
		else if (!strcmp(name, "QUAD_INTENSITY8_SGIS")) return GL_QUAD_INTENSITY8_SGIS;
#endif
#ifdef GL_QUAD_LUMINANCE4_SGIS
		else if (!strcmp(name, "QUAD_LUMINANCE4_SGIS")) return GL_QUAD_LUMINANCE4_SGIS;
#endif
#ifdef GL_QUAD_LUMINANCE8_SGIS
		else if (!strcmp(name, "QUAD_LUMINANCE8_SGIS")) return GL_QUAD_LUMINANCE8_SGIS;
#endif
#ifdef GL_R1UI_C4F_N3F_V3F_SUN
		else if (!strcmp(name, "R1UI_C4F_N3F_V3F_SUN")) return GL_R1UI_C4F_N3F_V3F_SUN;
#endif
#ifdef GL_R1UI_T2F_N3F_V3F_SUN
		else if (!strcmp(name, "R1UI_T2F_N3F_V3F_SUN")) return GL_R1UI_T2F_N3F_V3F_SUN;
#endif
#ifdef GL_REFERENCE_PLANE_SGIX
		else if (!strcmp(name, "REFERENCE_PLANE_SGIX")) return GL_REFERENCE_PLANE_SGIX;
#endif
#ifdef GL_REPLACEMENT_CODE_SUN
		else if (!strcmp(name, "REPLACEMENT_CODE_SUN")) return GL_REPLACEMENT_CODE_SUN;
#endif
#ifdef GL_SCALE_BY_ONE_HALF_NV
		else if (!strcmp(name, "SCALE_BY_ONE_HALF_NV")) return GL_SCALE_BY_ONE_HALF_NV;
#endif
#ifdef GL_SGIS_generate_mipmap
		else if (!strcmp(name, "SGIS_GENERATE_MIPMAP")) return GL_SGIS_generate_mipmap;
#endif
#ifdef GL_SGIS_sharpen_texture
		else if (!strcmp(name, "SGIS_SHARPEN_TEXTURE")) return GL_SGIS_sharpen_texture;
#endif
#ifdef GL_SGIS_texture_filter4
		else if (!strcmp(name, "SGIS_TEXTURE_FILTER4")) return GL_SGIS_texture_filter4;
#endif
#ifdef GL_SGIX_async_histogram
		else if (!strcmp(name, "SGIX_ASYNC_HISTOGRAM")) return GL_SGIX_async_histogram;
#endif
#ifdef GL_SGIX_igloo_interface
		else if (!strcmp(name, "SGIX_IGLOO_INTERFACE")) return GL_SGIX_igloo_interface;
#endif
#ifdef GL_SGIX_reference_plane
		else if (!strcmp(name, "SGIX_REFERENCE_PLANE")) return GL_SGIX_reference_plane;
#endif
#ifdef GL_SGIX_texture_add_env
		else if (!strcmp(name, "SGIX_TEXTURE_ADD_ENV")) return GL_SGIX_texture_add_env;
#endif
#ifdef GL_SGIX_ycrcb_subsample
		else if (!strcmp(name, "SGIX_YCRCB_SUBSAMPLE")) return GL_SGIX_ycrcb_subsample;
#endif
#ifdef GL_TEXTURE_BORDER_COLOR
		else if (!strcmp(name, "TEXTURE_BORDER_COLOR")) return GL_TEXTURE_BORDER_COLOR;
#endif
#ifdef GL_TEXTURE_COMPARE_SGIX
		else if (!strcmp(name, "TEXTURE_COMPARE_SGIX")) return GL_TEXTURE_COMPARE_SGIX;
#endif
#ifdef GL_TEXTURE_CUBE_MAP_ARB
		else if (!strcmp(name, "TEXTURE_CUBE_MAP_ARB")) return GL_TEXTURE_CUBE_MAP_ARB;
#endif
#ifdef GL_TEXTURE_CUBE_MAP_EXT
		else if (!strcmp(name, "TEXTURE_CUBE_MAP_EXT")) return GL_TEXTURE_CUBE_MAP_EXT;
#endif
#ifdef GL_TEXTURE_LOD_BIAS_EXT
		else if (!strcmp(name, "TEXTURE_LOD_BIAS_EXT")) return GL_TEXTURE_LOD_BIAS_EXT;
#endif
#ifdef GL_TEXTURE_MAX_LOD_SGIS
		else if (!strcmp(name, "TEXTURE_MAX_LOD_SGIS")) return GL_TEXTURE_MAX_LOD_SGIS;
#endif
#ifdef GL_TEXTURE_MIN_LOD_SGIS
		else if (!strcmp(name, "TEXTURE_MIN_LOD_SGIS")) return GL_TEXTURE_MIN_LOD_SGIS;
#endif
#ifdef GL_TEXTURE_PRIORITY_EXT
		else if (!strcmp(name, "TEXTURE_PRIORITY_EXT")) return GL_TEXTURE_PRIORITY_EXT;
#endif
#ifdef GL_TEXTURE_RED_SIZE_EXT
		else if (!strcmp(name, "TEXTURE_RED_SIZE_EXT")) return GL_TEXTURE_RED_SIZE_EXT;
#endif
#ifdef GL_TEXTURE_RESIDENT_EXT
		else if (!strcmp(name, "TEXTURE_RESIDENT_EXT")) return GL_TEXTURE_RESIDENT_EXT;
#endif
#ifdef GL_TRANSFORM_HINT_APPLE
		else if (!strcmp(name, "TRANSFORM_HINT_APPLE")) return GL_TRANSFORM_HINT_APPLE;
#endif
#ifdef GL_UNPACK_CMYK_HINT_EXT
		else if (!strcmp(name, "UNPACK_CMYK_HINT_EXT")) return GL_UNPACK_CMYK_HINT_EXT;
#endif
#ifdef GL_UNPACK_RESAMPLE_SGIX
		else if (!strcmp(name, "UNPACK_RESAMPLE_SGIX")) return GL_UNPACK_RESAMPLE_SGIX;
#endif
#ifdef GL_UNSIGNED_IDENTITY_NV
		else if (!strcmp(name, "UNSIGNED_IDENTITY_NV")) return GL_UNSIGNED_IDENTITY_NV;
#endif
#ifdef GL_UNSIGNED_INT_8_8_8_8
		else if (!strcmp(name, "UNSIGNED_INT_8_8_8_8")) return GL_UNSIGNED_INT_8_8_8_8;
#endif
#ifdef GL_UNSIGNED_SHORT_5_6_5
		else if (!strcmp(name, "UNSIGNED_SHORT_5_6_5")) return GL_UNSIGNED_SHORT_5_6_5;
#endif
#ifdef GL_VERTEX_ARRAY_POINTER
		else if (!strcmp(name, "VERTEX_ARRAY_POINTER")) return GL_VERTEX_ARRAY_POINTER;
#endif
#ifdef GL_VERTEX_DATA_HINT_PGI
		else if (!strcmp(name, "VERTEX_DATA_HINT_PGI")) return GL_VERTEX_DATA_HINT_PGI;
#endif
#ifdef GL_VERTEX_WEIGHTING_EXT
		else if (!strcmp(name, "VERTEX_WEIGHTING_EXT")) return GL_VERTEX_WEIGHTING_EXT;
#endif
#ifdef GL_WEIGHT_SUM_UNITY_ARB
		else if (!strcmp(name, "WEIGHT_SUM_UNITY_ARB")) return GL_WEIGHT_SUM_UNITY_ARB;
#endif
		else return _GL_UNDEFINED;
	case 24:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_APPLE_specular_vector
		else if (!strcmp(name, "APPLE_SPECULAR_VECTOR")) return GL_APPLE_specular_vector;
#endif
#ifdef GL_BACK_NORMALS_HINT_PGI
		else if (!strcmp(name, "BACK_NORMALS_HINT_PGI")) return GL_BACK_NORMALS_HINT_PGI;
#endif
#ifdef GL_COLOR_ARRAY_COUNT_EXT
		else if (!strcmp(name, "COLOR_ARRAY_COUNT_EXT")) return GL_COLOR_ARRAY_COUNT_EXT;
#endif
#ifdef GL_COLOR_TABLE_BLUE_SIZE
		else if (!strcmp(name, "COLOR_TABLE_BLUE_SIZE")) return GL_COLOR_TABLE_BLUE_SIZE;
#endif
#ifdef GL_COLOR_TABLE_SCALE_SGI
		else if (!strcmp(name, "COLOR_TABLE_SCALE_SGI")) return GL_COLOR_TABLE_SCALE_SGI;
#endif
#ifdef GL_COLOR_TABLE_WIDTH_EXT
		else if (!strcmp(name, "COLOR_TABLE_WIDTH_EXT")) return GL_COLOR_TABLE_WIDTH_EXT;
#endif
#ifdef GL_COLOR_TABLE_WIDTH_SGI
		else if (!strcmp(name, "COLOR_TABLE_WIDTH_SGI")) return GL_COLOR_TABLE_WIDTH_SGI;
#endif
#ifdef GL_COMBINER_AB_OUTPUT_NV
		else if (!strcmp(name, "COMBINER_AB_OUTPUT_NV")) return GL_COMBINER_AB_OUTPUT_NV;
#endif
#ifdef GL_COMBINER_CD_OUTPUT_NV
		else if (!strcmp(name, "COMBINER_CD_OUTPUT_NV")) return GL_COMBINER_CD_OUTPUT_NV;
#endif
#ifdef GL_CONVOLUTION_HINT_SGIX
		else if (!strcmp(name, "CONVOLUTION_HINT_SGIX")) return GL_CONVOLUTION_HINT_SGIX;
#endif
#ifdef GL_CONVOLUTION_WIDTH_EXT
		else if (!strcmp(name, "CONVOLUTION_WIDTH_EXT")) return GL_CONVOLUTION_WIDTH_EXT;
#endif
#ifdef GL_DUAL_INTENSITY12_SGIS
		else if (!strcmp(name, "DUAL_INTENSITY12_SGIS")) return GL_DUAL_INTENSITY12_SGIS;
#endif
#ifdef GL_DUAL_INTENSITY16_SGIS
		else if (!strcmp(name, "DUAL_INTENSITY16_SGIS")) return GL_DUAL_INTENSITY16_SGIS;
#endif
#ifdef GL_DUAL_LUMINANCE12_SGIS
		else if (!strcmp(name, "DUAL_LUMINANCE12_SGIS")) return GL_DUAL_LUMINANCE12_SGIS;
#endif
#ifdef GL_DUAL_LUMINANCE16_SGIS
		else if (!strcmp(name, "DUAL_LUMINANCE16_SGIS")) return GL_DUAL_LUMINANCE16_SGIS;
#endif
#ifdef GL_EXT_multi_draw_arrays
		else if (!strcmp(name, "EXT_MULTI_DRAW_ARRAYS")) return GL_EXT_multi_draw_arrays;
#endif
#ifdef GL_EYE_PLANE_ABSOLUTE_NV
		else if (!strcmp(name, "EYE_PLANE_ABSOLUTE_NV")) return GL_EYE_PLANE_ABSOLUTE_NV;
#endif
#ifdef GL_FOG_OFFSET_VALUE_SGIX
		else if (!strcmp(name, "FOG_OFFSET_VALUE_SGIX")) return GL_FOG_OFFSET_VALUE_SGIX;
#endif
#ifdef GL_FRAGMENT_MATERIAL_EXT
		else if (!strcmp(name, "FRAGMENT_MATERIAL_EXT")) return GL_FRAGMENT_MATERIAL_EXT;
#endif
#ifdef GL_FRAMEZOOM_FACTOR_SGIX
		else if (!strcmp(name, "FRAMEZOOM_FACTOR_SGIX")) return GL_FRAMEZOOM_FACTOR_SGIX;
#endif
#ifdef GL_FULL_STIPPLE_HINT_PGI
		else if (!strcmp(name, "FULL_STIPPLE_HINT_PGI")) return GL_FULL_STIPPLE_HINT_PGI;
#endif
#ifdef GL_FUNC_REVERSE_SUBTRACT
		else if (!strcmp(name, "FUNC_REVERSE_SUBTRACT")) return GL_FUNC_REVERSE_SUBTRACT;
#endif
#ifdef GL_IMAGE_CUBIC_WEIGHT_HP
		else if (!strcmp(name, "IMAGE_CUBIC_WEIGHT_HP")) return GL_IMAGE_CUBIC_WEIGHT_HP;
#endif
#ifdef GL_IMAGE_ROTATE_ANGLE_HP
		else if (!strcmp(name, "IMAGE_ROTATE_ANGLE_HP")) return GL_IMAGE_ROTATE_ANGLE_HP;
#endif
#ifdef GL_IMAGE_TRANSFORM_2D_HP
		else if (!strcmp(name, "IMAGE_TRANSFORM_2D_HP")) return GL_IMAGE_TRANSFORM_2D_HP;
#endif
#ifdef GL_INDEX_ARRAY_COUNT_EXT
		else if (!strcmp(name, "INDEX_ARRAY_COUNT_EXT")) return GL_INDEX_ARRAY_COUNT_EXT;
#endif
#ifdef GL_INTEL_parallel_arrays
		else if (!strcmp(name, "INTEL_PARALLEL_ARRAYS")) return GL_INTEL_parallel_arrays;
#endif
#ifdef GL_LINEAR_MIPMAP_NEAREST
		else if (!strcmp(name, "LINEAR_MIPMAP_NEAREST")) return GL_LINEAR_MIPMAP_NEAREST;
#endif
#ifdef GL_LUMINANCE4_ALPHA4_EXT
		else if (!strcmp(name, "LUMINANCE4_ALPHA4_EXT")) return GL_LUMINANCE4_ALPHA4_EXT;
#endif
#ifdef GL_LUMINANCE6_ALPHA2_EXT
		else if (!strcmp(name, "LUMINANCE6_ALPHA2_EXT")) return GL_LUMINANCE6_ALPHA2_EXT;
#endif
#ifdef GL_LUMINANCE8_ALPHA8_EXT
		else if (!strcmp(name, "LUMINANCE8_ALPHA8_EXT")) return GL_LUMINANCE8_ALPHA8_EXT;
#endif
#ifdef GL_MAT_SHININESS_BIT_PGI
		else if (!strcmp(name, "MAT_SHININESS_BIT_PGI")) return GL_MAT_SHININESS_BIT_PGI;
#endif
#ifdef GL_MAX_CONVOLUTION_WIDTH
		else if (!strcmp(name, "MAX_CONVOLUTION_WIDTH")) return GL_MAX_CONVOLUTION_WIDTH;
#endif
#ifdef GL_MAX_ELEMENTS_VERTICES
		else if (!strcmp(name, "MAX_ELEMENTS_VERTICES")) return GL_MAX_ELEMENTS_VERTICES;
#endif
#ifdef GL_MAX_TEXTURE_UNITS_ARB
		else if (!strcmp(name, "MAX_TEXTURE_UNITS_ARB")) return GL_MAX_TEXTURE_UNITS_ARB;
#endif
#ifdef GL_MODELVIEW0_MATRIX_EXT
		else if (!strcmp(name, "MODELVIEW0_MATRIX_EXT")) return GL_MODELVIEW0_MATRIX_EXT;
#endif
#ifdef GL_MODELVIEW_MATRIX1_EXT
		else if (!strcmp(name, "MODELVIEW_MATRIX1_EXT")) return GL_MODELVIEW_MATRIX1_EXT;
#endif
#ifdef GL_MODELVIEW_STACK_DEPTH
		else if (!strcmp(name, "MODELVIEW_STACK_DEPTH")) return GL_MODELVIEW_STACK_DEPTH;
#endif
#ifdef GL_NEAREST_MIPMAP_LINEAR
		else if (!strcmp(name, "NEAREST_MIPMAP_LINEAR")) return GL_NEAREST_MIPMAP_LINEAR;
#endif
#ifdef GL_NORMAL_ARRAY_LIST_IBM
		else if (!strcmp(name, "NORMAL_ARRAY_LIST_IBM")) return GL_NORMAL_ARRAY_LIST_IBM;
#endif
#ifdef GL_NORMAL_ARRAY_TYPE_EXT
		else if (!strcmp(name, "NORMAL_ARRAY_TYPE_EXT")) return GL_NORMAL_ARRAY_TYPE_EXT;
#endif
#ifdef GL_NV_light_max_exponent
		else if (!strcmp(name, "NV_LIGHT_MAX_EXPONENT")) return GL_NV_light_max_exponent;
#endif
#ifdef GL_NV_register_combiners
		else if (!strcmp(name, "NV_REGISTER_COMBINERS")) return GL_NV_register_combiners;
#endif
#ifdef GL_NV_vertex_array_range
		else if (!strcmp(name, "NV_VERTEX_ARRAY_RANGE")) return GL_NV_vertex_array_range;
#endif
#ifdef GL_PACK_IMAGE_DEPTH_SGIS
		else if (!strcmp(name, "PACK_IMAGE_DEPTH_SGIS")) return GL_PACK_IMAGE_DEPTH_SGIS;
#endif
#ifdef GL_PACK_IMAGE_HEIGHT_EXT
		else if (!strcmp(name, "PACK_IMAGE_HEIGHT_EXT")) return GL_PACK_IMAGE_HEIGHT_EXT;
#endif
#ifdef GL_PARALLEL_ARRAYS_INTEL
		else if (!strcmp(name, "PARALLEL_ARRAYS_INTEL")) return GL_PARALLEL_ARRAYS_INTEL;
#endif
#ifdef GL_PIXEL_MAP_A_TO_A_SIZE
		else if (!strcmp(name, "PIXEL_MAP_A_TO_A_SIZE")) return GL_PIXEL_MAP_A_TO_A_SIZE;
#endif
#ifdef GL_PIXEL_MAP_B_TO_B_SIZE
		else if (!strcmp(name, "PIXEL_MAP_B_TO_B_SIZE")) return GL_PIXEL_MAP_B_TO_B_SIZE;
#endif
#ifdef GL_PIXEL_MAP_G_TO_G_SIZE
		else if (!strcmp(name, "PIXEL_MAP_G_TO_G_SIZE")) return GL_PIXEL_MAP_G_TO_G_SIZE;
#endif
#ifdef GL_PIXEL_MAP_I_TO_A_SIZE
		else if (!strcmp(name, "PIXEL_MAP_I_TO_A_SIZE")) return GL_PIXEL_MAP_I_TO_A_SIZE;
#endif
#ifdef GL_PIXEL_MAP_I_TO_B_SIZE
		else if (!strcmp(name, "PIXEL_MAP_I_TO_B_SIZE")) return GL_PIXEL_MAP_I_TO_B_SIZE;
#endif
#ifdef GL_PIXEL_MAP_I_TO_G_SIZE
		else if (!strcmp(name, "PIXEL_MAP_I_TO_G_SIZE")) return GL_PIXEL_MAP_I_TO_G_SIZE;
#endif
#ifdef GL_PIXEL_MAP_I_TO_I_SIZE
		else if (!strcmp(name, "PIXEL_MAP_I_TO_I_SIZE")) return GL_PIXEL_MAP_I_TO_I_SIZE;
#endif
#ifdef GL_PIXEL_MAP_I_TO_R_SIZE
		else if (!strcmp(name, "PIXEL_MAP_I_TO_R_SIZE")) return GL_PIXEL_MAP_I_TO_R_SIZE;
#endif
#ifdef GL_PIXEL_MAP_R_TO_R_SIZE
		else if (!strcmp(name, "PIXEL_MAP_R_TO_R_SIZE")) return GL_PIXEL_MAP_R_TO_R_SIZE;
#endif
#ifdef GL_PIXEL_MAP_S_TO_S_SIZE
		else if (!strcmp(name, "PIXEL_MAP_S_TO_S_SIZE")) return GL_PIXEL_MAP_S_TO_S_SIZE;
#endif
#ifdef GL_PIXEL_TILE_WIDTH_SGIX
		else if (!strcmp(name, "PIXEL_TILE_WIDTH_SGIX")) return GL_PIXEL_TILE_WIDTH_SGIX;
#endif
#ifdef GL_POLYGON_OFFSET_FACTOR
		else if (!strcmp(name, "POLYGON_OFFSET_FACTOR")) return GL_POLYGON_OFFSET_FACTOR;
#endif
#ifdef GL_PROXY_COLOR_TABLE_SGI
		else if (!strcmp(name, "PROXY_COLOR_TABLE_SGI")) return GL_PROXY_COLOR_TABLE_SGI;
#endif
#ifdef GL_PROXY_TEXTURE_4D_SGIS
		else if (!strcmp(name, "PROXY_TEXTURE_4D_SGIS")) return GL_PROXY_TEXTURE_4D_SGIS;
#endif
#ifdef GL_QUADRATIC_ATTENUATION
		else if (!strcmp(name, "QUADRATIC_ATTENUATION")) return GL_QUADRATIC_ATTENUATION;
#endif
#ifdef GL_REGISTER_COMBINERS_NV
		else if (!strcmp(name, "REGISTER_COMBINERS_NV")) return GL_REGISTER_COMBINERS_NV;
#endif
#ifdef GL_SAMPLE_MASK_VALUE_EXT
		else if (!strcmp(name, "SAMPLE_MASK_VALUE_EXT")) return GL_SAMPLE_MASK_VALUE_EXT;
#endif
#ifdef GL_SELECTION_BUFFER_SIZE
		else if (!strcmp(name, "SELECTION_BUFFER_SIZE")) return GL_SELECTION_BUFFER_SIZE;
#endif
#ifdef GL_SGIX_texture_lod_bias
		else if (!strcmp(name, "SGIX_TEXTURE_LOD_BIAS")) return GL_SGIX_texture_lod_bias;
#endif
#ifdef GL_TEXTURE_BLUE_SIZE_EXT
		else if (!strcmp(name, "TEXTURE_BLUE_SIZE_EXT")) return GL_TEXTURE_BLUE_SIZE_EXT;
#endif
#ifdef GL_TEXTURE_ENV_BIAS_SGIX
		else if (!strcmp(name, "TEXTURE_ENV_BIAS_SGIX")) return GL_TEXTURE_ENV_BIAS_SGIX;
#endif
#ifdef GL_TEXTURE_GEQUAL_R_SGIX
		else if (!strcmp(name, "TEXTURE_GEQUAL_R_SGIX")) return GL_TEXTURE_GEQUAL_R_SGIX;
#endif
#ifdef GL_TEXTURE_LEQUAL_R_SGIX
		else if (!strcmp(name, "TEXTURE_LEQUAL_R_SGIX")) return GL_TEXTURE_LEQUAL_R_SGIX;
#endif
#ifdef GL_TEXTURE_TOO_LARGE_EXT
		else if (!strcmp(name, "TEXTURE_TOO_LARGE_EXT")) return GL_TEXTURE_TOO_LARGE_EXT;
#endif
#ifdef GL_VERTEX_ARRAY_LIST_IBM
		else if (!strcmp(name, "VERTEX_ARRAY_LIST_IBM")) return GL_VERTEX_ARRAY_LIST_IBM;
#endif
#ifdef GL_VERTEX_ARRAY_RANGE_NV
		else if (!strcmp(name, "VERTEX_ARRAY_RANGE_NV")) return GL_VERTEX_ARRAY_RANGE_NV;
#endif
#ifdef GL_VERTEX_ARRAY_SIZE_EXT
		else if (!strcmp(name, "VERTEX_ARRAY_SIZE_EXT")) return GL_VERTEX_ARRAY_SIZE_EXT;
#endif
#ifdef GL_VERTEX_ARRAY_TYPE_EXT
		else if (!strcmp(name, "VERTEX_ARRAY_TYPE_EXT")) return GL_VERTEX_ARRAY_TYPE_EXT;
#endif
#ifdef GL_WEIGHT_ARRAY_SIZE_ARB
		else if (!strcmp(name, "WEIGHT_ARRAY_SIZE_ARB")) return GL_WEIGHT_ARRAY_SIZE_ARB;
#endif
#ifdef GL_WEIGHT_ARRAY_TYPE_ARB
		else if (!strcmp(name, "WEIGHT_ARRAY_TYPE_ARB")) return GL_WEIGHT_ARRAY_TYPE_ARB;
#endif
		else return _GL_UNDEFINED;
	case 25:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_ALL_CLIENT_ATTRIB_BITS
		else if (!strcmp(name, "ALL_CLIENT_ATTRIB_BITS")) return GL_ALL_CLIENT_ATTRIB_BITS;
#endif
#ifdef GL_ASYNC_DRAW_PIXELS_SGIX
		else if (!strcmp(name, "ASYNC_DRAW_PIXELS_SGIX")) return GL_ASYNC_DRAW_PIXELS_SGIX;
#endif
#ifdef GL_ASYNC_READ_PIXELS_SGIX
		else if (!strcmp(name, "ASYNC_READ_PIXELS_SGIX")) return GL_ASYNC_READ_PIXELS_SGIX;
#endif
#ifdef GL_CLIENT_ALL_ATTRIB_BITS
		else if (!strcmp(name, "CLIENT_ALL_ATTRIB_BITS")) return GL_CLIENT_ALL_ATTRIB_BITS;
#endif
#ifdef GL_CLIENT_PIXEL_STORE_BIT
		else if (!strcmp(name, "CLIENT_PIXEL_STORE_BIT")) return GL_CLIENT_PIXEL_STORE_BIT;
#endif
#ifdef GL_COLOR_ARRAY_STRIDE_EXT
		else if (!strcmp(name, "COLOR_ARRAY_STRIDE_EXT")) return GL_COLOR_ARRAY_STRIDE_EXT;
#endif
#ifdef GL_COLOR_TABLE_ALPHA_SIZE
		else if (!strcmp(name, "COLOR_TABLE_ALPHA_SIZE")) return GL_COLOR_TABLE_ALPHA_SIZE;
#endif
#ifdef GL_COLOR_TABLE_FORMAT_EXT
		else if (!strcmp(name, "COLOR_TABLE_FORMAT_EXT")) return GL_COLOR_TABLE_FORMAT_EXT;
#endif
#ifdef GL_COLOR_TABLE_FORMAT_SGI
		else if (!strcmp(name, "COLOR_TABLE_FORMAT_SGI")) return GL_COLOR_TABLE_FORMAT_SGI;
#endif
#ifdef GL_COLOR_TABLE_GREEN_SIZE
		else if (!strcmp(name, "COLOR_TABLE_GREEN_SIZE")) return GL_COLOR_TABLE_GREEN_SIZE;
#endif
#ifdef GL_COMBINER_SUM_OUTPUT_NV
		else if (!strcmp(name, "COMBINER_SUM_OUTPUT_NV")) return GL_COMBINER_SUM_OUTPUT_NV;
#endif
#ifdef GL_CONVOLUTION_FORMAT_EXT
		else if (!strcmp(name, "CONVOLUTION_FORMAT_EXT")) return GL_CONVOLUTION_FORMAT_EXT;
#endif
#ifdef GL_CONVOLUTION_HEIGHT_EXT
		else if (!strcmp(name, "CONVOLUTION_HEIGHT_EXT")) return GL_CONVOLUTION_HEIGHT_EXT;
#endif
#ifdef GL_CURRENT_TEXTURE_COORDS
		else if (!strcmp(name, "CURRENT_TEXTURE_COORDS")) return GL_CURRENT_TEXTURE_COORDS;
#endif
#ifdef GL_DEFORMATIONS_MASK_SGIX
		else if (!strcmp(name, "DEFORMATIONS_MASK_SGIX")) return GL_DEFORMATIONS_MASK_SGIX;
#endif
#ifdef GL_DEPTH_COMPONENT16_SGIX
		else if (!strcmp(name, "DEPTH_COMPONENT16_SGIX")) return GL_DEPTH_COMPONENT16_SGIX;
#endif
#ifdef GL_DEPTH_COMPONENT24_SGIX
		else if (!strcmp(name, "DEPTH_COMPONENT24_SGIX")) return GL_DEPTH_COMPONENT24_SGIX;
#endif
#ifdef GL_DEPTH_COMPONENT32_SGIX
		else if (!strcmp(name, "DEPTH_COMPONENT32_SGIX")) return GL_DEPTH_COMPONENT32_SGIX;
#endif
#ifdef GL_DETAIL_TEXTURE_2D_SGIS
		else if (!strcmp(name, "DETAIL_TEXTURE_2D_SGIS")) return GL_DETAIL_TEXTURE_2D_SGIS;
#endif
#ifdef GL_EDGE_FLAG_ARRAY_STRIDE
		else if (!strcmp(name, "EDGE_FLAG_ARRAY_STRIDE")) return GL_EDGE_FLAG_ARRAY_STRIDE;
#endif
#ifdef GL_FRAGMENT_LIGHTING_SGIX
		else if (!strcmp(name, "FRAGMENT_LIGHTING_SGIX")) return GL_FRAGMENT_LIGHTING_SGIX;
#endif
#ifdef GL_HISTOGRAM_RED_SIZE_EXT
		else if (!strcmp(name, "HISTOGRAM_RED_SIZE_EXT")) return GL_HISTOGRAM_RED_SIZE_EXT;
#endif
#ifdef GL_IBM_vertex_array_lists
		else if (!strcmp(name, "IBM_VERTEX_ARRAY_LISTS")) return GL_IBM_vertex_array_lists;
#endif
#ifdef GL_INDEX_ARRAY_STRIDE_EXT
		else if (!strcmp(name, "INDEX_ARRAY_STRIDE_EXT")) return GL_INDEX_ARRAY_STRIDE_EXT;
#endif
#ifdef GL_INVERTED_SCREEN_W_REND
		else if (!strcmp(name, "INVERTED_SCREEN_W_REND")) return GL_INVERTED_SCREEN_W_REND;
#endif
#ifdef GL_LINE_WIDTH_GRANULARITY
		else if (!strcmp(name, "LINE_WIDTH_GRANULARITY")) return GL_LINE_WIDTH_GRANULARITY;
#endif
#ifdef GL_LUMINANCE12_ALPHA4_EXT
		else if (!strcmp(name, "LUMINANCE12_ALPHA4_EXT")) return GL_LUMINANCE12_ALPHA4_EXT;
#endif
#ifdef GL_MATERIAL_SIDE_HINT_PGI
		else if (!strcmp(name, "MATERIAL_SIDE_HINT_PGI")) return GL_MATERIAL_SIDE_HINT_PGI;
#endif
#ifdef GL_MAX_ACTIVE_LIGHTS_SGIX
		else if (!strcmp(name, "MAX_ACTIVE_LIGHTS_SGIX")) return GL_MAX_ACTIVE_LIGHTS_SGIX;
#endif
#ifdef GL_MAX_ATTRIB_STACK_DEPTH
		else if (!strcmp(name, "MAX_ATTRIB_STACK_DEPTH")) return GL_MAX_ATTRIB_STACK_DEPTH;
#endif
#ifdef GL_MAX_CLIPMAP_DEPTH_SGIX
		else if (!strcmp(name, "MAX_CLIPMAP_DEPTH_SGIX")) return GL_MAX_CLIPMAP_DEPTH_SGIX;
#endif
#ifdef GL_MAX_CONVOLUTION_HEIGHT
		else if (!strcmp(name, "MAX_CONVOLUTION_HEIGHT")) return GL_MAX_CONVOLUTION_HEIGHT;
#endif
#ifdef GL_NEAREST_MIPMAP_NEAREST
		else if (!strcmp(name, "NEAREST_MIPMAP_NEAREST")) return GL_NEAREST_MIPMAP_NEAREST;
#endif
#ifdef GL_NORMAL_ARRAY_COUNT_EXT
		else if (!strcmp(name, "NORMAL_ARRAY_COUNT_EXT")) return GL_NORMAL_ARRAY_COUNT_EXT;
#endif
#ifdef GL_PACK_SKIP_VOLUMES_SGIS
		else if (!strcmp(name, "PACK_SKIP_VOLUMES_SGIS")) return GL_PACK_SKIP_VOLUMES_SGIS;
#endif
#ifdef GL_PIXEL_CUBIC_WEIGHT_EXT
		else if (!strcmp(name, "PIXEL_CUBIC_WEIGHT_EXT")) return GL_PIXEL_CUBIC_WEIGHT_EXT;
#endif
#ifdef GL_PIXEL_GROUP_COLOR_SGIS
		else if (!strcmp(name, "PIXEL_GROUP_COLOR_SGIS")) return GL_PIXEL_GROUP_COLOR_SGIS;
#endif
#ifdef GL_PIXEL_TILE_HEIGHT_SGIX
		else if (!strcmp(name, "PIXEL_TILE_HEIGHT_SGIX")) return GL_PIXEL_TILE_HEIGHT_SGIX;
#endif
#ifdef GL_PIXEL_TRANSFORM_2D_EXT
		else if (!strcmp(name, "PIXEL_TRANSFORM_2D_EXT")) return GL_PIXEL_TRANSFORM_2D_EXT;
#endif
#ifdef GL_POINT_SIZE_GRANULARITY
		else if (!strcmp(name, "POINT_SIZE_GRANULARITY")) return GL_POINT_SIZE_GRANULARITY;
#endif
#ifdef GL_PROJECTION_STACK_DEPTH
		else if (!strcmp(name, "PROJECTION_STACK_DEPTH")) return GL_PROJECTION_STACK_DEPTH;
#endif
#ifdef GL_RESAMPLE_DECIMATE_SGIX
		else if (!strcmp(name, "RESAMPLE_DECIMATE_SGIX")) return GL_RESAMPLE_DECIMATE_SGIX;
#endif
#ifdef GL_SAMPLE_MASK_INVERT_EXT
		else if (!strcmp(name, "SAMPLE_MASK_INVERT_EXT")) return GL_SAMPLE_MASK_INVERT_EXT;
#endif
#ifdef GL_SAMPLE_MASK_VALUE_SGIS
		else if (!strcmp(name, "SAMPLE_MASK_VALUE_SGIS")) return GL_SAMPLE_MASK_VALUE_SGIS;
#endif
#ifdef GL_SGIS_point_line_texgen
		else if (!strcmp(name, "SGIS_POINT_LINE_TEXGEN")) return GL_SGIS_point_line_texgen;
#endif
#ifdef GL_SGIX_fragment_lighting
		else if (!strcmp(name, "SGIX_FRAGMENT_LIGHTING")) return GL_SGIX_fragment_lighting;
#endif
#ifdef GL_SGIX_tag_sample_buffer
		else if (!strcmp(name, "SGIX_TAG_SAMPLE_BUFFER")) return GL_SGIX_tag_sample_buffer;
#endif
#ifdef GL_SHADOW_ATTENUATION_EXT
		else if (!strcmp(name, "SHADOW_ATTENUATION_EXT")) return GL_SHADOW_ATTENUATION_EXT;
#endif
#ifdef GL_TANGENT_ARRAY_TYPE_EXT
		else if (!strcmp(name, "TANGENT_ARRAY_TYPE_EXT")) return GL_TANGENT_ARRAY_TYPE_EXT;
#endif
#ifdef GL_TEXTURE_1D_BINDING_EXT
		else if (!strcmp(name, "TEXTURE_1D_BINDING_EXT")) return GL_TEXTURE_1D_BINDING_EXT;
#endif
#ifdef GL_TEXTURE_2D_BINDING_EXT
		else if (!strcmp(name, "TEXTURE_2D_BINDING_EXT")) return GL_TEXTURE_2D_BINDING_EXT;
#endif
#ifdef GL_TEXTURE_3D_BINDING_EXT
		else if (!strcmp(name, "TEXTURE_3D_BINDING_EXT")) return GL_TEXTURE_3D_BINDING_EXT;
#endif
#ifdef GL_TEXTURE_ALPHA_SIZE_EXT
		else if (!strcmp(name, "TEXTURE_ALPHA_SIZE_EXT")) return GL_TEXTURE_ALPHA_SIZE_EXT;
#endif
#ifdef GL_TEXTURE_COMPRESSED_ARB
		else if (!strcmp(name, "TEXTURE_COMPRESSED_ARB")) return GL_TEXTURE_COMPRESSED_ARB;
#endif
#ifdef GL_TEXTURE_GREEN_SIZE_EXT
		else if (!strcmp(name, "TEXTURE_GREEN_SIZE_EXT")) return GL_TEXTURE_GREEN_SIZE_EXT;
#endif
#ifdef GL_TEXTURE_INDEX_SIZE_EXT
		else if (!strcmp(name, "TEXTURE_INDEX_SIZE_EXT")) return GL_TEXTURE_INDEX_SIZE_EXT;
#endif
#ifdef GL_TEXTURE_INTENSITY_SIZE
		else if (!strcmp(name, "TEXTURE_INTENSITY_SIZE")) return GL_TEXTURE_INTENSITY_SIZE;
#endif
#ifdef GL_TEXTURE_LUMINANCE_SIZE
		else if (!strcmp(name, "TEXTURE_LUMINANCE_SIZE")) return GL_TEXTURE_LUMINANCE_SIZE;
#endif
#ifdef GL_TEXTURE_MAX_LEVEL_SGIS
		else if (!strcmp(name, "TEXTURE_MAX_LEVEL_SGIS")) return GL_TEXTURE_MAX_LEVEL_SGIS;
#endif
#ifdef GL_UNPACK_SKIP_IMAGES_EXT
		else if (!strcmp(name, "UNPACK_SKIP_IMAGES_EXT")) return GL_UNPACK_SKIP_IMAGES_EXT;
#endif
#ifdef GL_UNSIGNED_SHORT_4_4_4_4
		else if (!strcmp(name, "UNSIGNED_SHORT_4_4_4_4")) return GL_UNSIGNED_SHORT_4_4_4_4;
#endif
#ifdef GL_UNSIGNED_SHORT_5_5_5_1
		else if (!strcmp(name, "UNSIGNED_SHORT_5_5_5_1")) return GL_UNSIGNED_SHORT_5_5_5_1;
#endif
#ifdef GL_VERTEX_ARRAY_COUNT_EXT
		else if (!strcmp(name, "VERTEX_ARRAY_COUNT_EXT")) return GL_VERTEX_ARRAY_COUNT_EXT;
#endif
		else return _GL_UNDEFINED;
	case 26:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_ACTIVE_VERTEX_UNITS_ARB
		else if (!strcmp(name, "ACTIVE_VERTEX_UNITS_ARB")) return GL_ACTIVE_VERTEX_UNITS_ARB;
#endif
#ifdef GL_ALLOW_DRAW_FRG_HINT_PGI
		else if (!strcmp(name, "ALLOW_DRAW_FRG_HINT_PGI")) return GL_ALLOW_DRAW_FRG_HINT_PGI;
#endif
#ifdef GL_ALLOW_DRAW_MEM_HINT_PGI
		else if (!strcmp(name, "ALLOW_DRAW_MEM_HINT_PGI")) return GL_ALLOW_DRAW_MEM_HINT_PGI;
#endif
#ifdef GL_ALLOW_DRAW_OBJ_HINT_PGI
		else if (!strcmp(name, "ALLOW_DRAW_OBJ_HINT_PGI")) return GL_ALLOW_DRAW_OBJ_HINT_PGI;
#endif
#ifdef GL_ALLOW_DRAW_WIN_HINT_PGI
		else if (!strcmp(name, "ALLOW_DRAW_WIN_HINT_PGI")) return GL_ALLOW_DRAW_WIN_HINT_PGI;
#endif
#ifdef GL_ARB_texture_compression
		else if (!strcmp(name, "ARB_TEXTURE_COMPRESSION")) return GL_ARB_texture_compression;
#endif
#ifdef GL_BINORMAL_ARRAY_TYPE_EXT
		else if (!strcmp(name, "BINORMAL_ARRAY_TYPE_EXT")) return GL_BINORMAL_ARRAY_TYPE_EXT;
#endif
#ifdef GL_CLIENT_VERTEX_ARRAY_BIT
		else if (!strcmp(name, "CLIENT_VERTEX_ARRAY_BIT")) return GL_CLIENT_VERTEX_ARRAY_BIT;
#endif
#ifdef GL_COLOR_ARRAY_POINTER_EXT
		else if (!strcmp(name, "COLOR_ARRAY_POINTER_EXT")) return GL_COLOR_ARRAY_POINTER_EXT;
#endif
#ifdef GL_CONVOLUTION_BORDER_MODE
		else if (!strcmp(name, "CONVOLUTION_BORDER_MODE")) return GL_CONVOLUTION_BORDER_MODE;
#endif
#ifdef GL_CONVOLUTION_FILTER_BIAS
		else if (!strcmp(name, "CONVOLUTION_FILTER_BIAS")) return GL_CONVOLUTION_FILTER_BIAS;
#endif
#ifdef GL_CURRENT_RASTER_DISTANCE
		else if (!strcmp(name, "CURRENT_RASTER_DISTANCE")) return GL_CURRENT_RASTER_DISTANCE;
#endif
#ifdef GL_CURRENT_RASTER_POSITION
		else if (!strcmp(name, "CURRENT_RASTER_POSITION")) return GL_CURRENT_RASTER_POSITION;
#endif
#ifdef GL_EDGE_FLAG_ARRAY_POINTER
		else if (!strcmp(name, "EDGE_FLAG_ARRAY_POINTER")) return GL_EDGE_FLAG_ARRAY_POINTER;
#endif
#ifdef GL_EXT_blend_func_separate
		else if (!strcmp(name, "EXT_BLEND_FUNC_SEPARATE")) return GL_EXT_blend_func_separate;
#endif
#ifdef GL_EXT_draw_range_elements
		else if (!strcmp(name, "EXT_DRAW_RANGE_ELEMENTS")) return GL_EXT_draw_range_elements;
#endif
#ifdef GL_EXT_index_array_formats
		else if (!strcmp(name, "EXT_INDEX_ARRAY_FORMATS")) return GL_EXT_index_array_formats;
#endif
#ifdef GL_EXT_texture_env_combine
		else if (!strcmp(name, "EXT_TEXTURE_ENV_COMBINE")) return GL_EXT_texture_env_combine;
#endif
#ifdef GL_FEEDBACK_BUFFER_POINTER
		else if (!strcmp(name, "FEEDBACK_BUFFER_POINTER")) return GL_FEEDBACK_BUFFER_POINTER;
#endif
#ifdef GL_GLOBAL_ALPHA_FACTOR_SUN
		else if (!strcmp(name, "GLOBAL_ALPHA_FACTOR_SUN")) return GL_GLOBAL_ALPHA_FACTOR_SUN;
#endif
#ifdef GL_HISTOGRAM_BLUE_SIZE_EXT
		else if (!strcmp(name, "HISTOGRAM_BLUE_SIZE_EXT")) return GL_HISTOGRAM_BLUE_SIZE_EXT;
#endif
#ifdef GL_INDEX_ARRAY_POINTER_EXT
		else if (!strcmp(name, "INDEX_ARRAY_POINTER_EXT")) return GL_INDEX_ARRAY_POINTER_EXT;
#endif
#ifdef GL_INDEX_MATERIAL_FACE_EXT
		else if (!strcmp(name, "INDEX_MATERIAL_FACE_EXT")) return GL_INDEX_MATERIAL_FACE_EXT;
#endif
#ifdef GL_LUMINANCE12_ALPHA12_EXT
		else if (!strcmp(name, "LUMINANCE12_ALPHA12_EXT")) return GL_LUMINANCE12_ALPHA12_EXT;
#endif
#ifdef GL_LUMINANCE16_ALPHA16_EXT
		else if (!strcmp(name, "LUMINANCE16_ALPHA16_EXT")) return GL_LUMINANCE16_ALPHA16_EXT;
#endif
#ifdef GL_MAX_3D_TEXTURE_SIZE_EXT
		else if (!strcmp(name, "MAX_3D_TEXTURE_SIZE_EXT")) return GL_MAX_3D_TEXTURE_SIZE_EXT;
#endif
#ifdef GL_MAX_TEXTURE_STACK_DEPTH
		else if (!strcmp(name, "MAX_TEXTURE_STACK_DEPTH")) return GL_MAX_TEXTURE_STACK_DEPTH;
#endif
#ifdef GL_NORMAL_ARRAY_STRIDE_EXT
		else if (!strcmp(name, "NORMAL_ARRAY_STRIDE_EXT")) return GL_NORMAL_ARRAY_STRIDE_EXT;
#endif
#ifdef GL_NV_texture_env_combine4
		else if (!strcmp(name, "NV_TEXTURE_ENV_COMBINE4")) return GL_NV_texture_env_combine4;
#endif
#ifdef GL_PIXEL_TEX_GEN_MODE_SGIX
		else if (!strcmp(name, "PIXEL_TEX_GEN_MODE_SGIX")) return GL_PIXEL_TEX_GEN_MODE_SGIX;
#endif
#ifdef GL_POLYGON_OFFSET_BIAS_EXT
		else if (!strcmp(name, "POLYGON_OFFSET_BIAS_EXT")) return GL_POLYGON_OFFSET_BIAS_EXT;
#endif
#ifdef GL_RECLAIM_MEMORY_HINT_PGI
		else if (!strcmp(name, "RECLAIM_MEMORY_HINT_PGI")) return GL_RECLAIM_MEMORY_HINT_PGI;
#endif
#ifdef GL_REND_screen_coordinates
		else if (!strcmp(name, "REND_SCREEN_COORDINATES")) return GL_REND_screen_coordinates;
#endif
#ifdef GL_RESAMPLE_REPLICATE_SGIX
		else if (!strcmp(name, "RESAMPLE_REPLICATE_SGIX")) return GL_RESAMPLE_REPLICATE_SGIX;
#endif
#ifdef GL_RESAMPLE_ZERO_FILL_SGIX
		else if (!strcmp(name, "RESAMPLE_ZERO_FILL_SGIX")) return GL_RESAMPLE_ZERO_FILL_SGIX;
#endif
#ifdef GL_SAMPLE_ALPHA_TO_ONE_ARB
		else if (!strcmp(name, "SAMPLE_ALPHA_TO_ONE_ARB")) return GL_SAMPLE_ALPHA_TO_ONE_ARB;
#endif
#ifdef GL_SAMPLE_ALPHA_TO_ONE_EXT
		else if (!strcmp(name, "SAMPLE_ALPHA_TO_ONE_EXT")) return GL_SAMPLE_ALPHA_TO_ONE_EXT;
#endif
#ifdef GL_SAMPLE_MASK_INVERT_SGIS
		else if (!strcmp(name, "SAMPLE_MASK_INVERT_SGIS")) return GL_SAMPLE_MASK_INVERT_SGIS;
#endif
#ifdef GL_SCREEN_COORDINATES_REND
		else if (!strcmp(name, "SCREEN_COORDINATES_REND")) return GL_SCREEN_COORDINATES_REND;
#endif
#ifdef GL_SEPARATE_SPECULAR_COLOR
		else if (!strcmp(name, "SEPARATE_SPECULAR_COLOR")) return GL_SEPARATE_SPECULAR_COLOR;
#endif
#ifdef GL_SGIS_texture_color_mask
		else if (!strcmp(name, "SGIS_TEXTURE_COLOR_MASK")) return GL_SGIS_texture_color_mask;
#endif
#ifdef GL_SGIS_texture_edge_clamp
		else if (!strcmp(name, "SGIS_TEXTURE_EDGE_CLAMP")) return GL_SGIS_texture_edge_clamp;
#endif
#ifdef GL_SGIX_blend_alpha_minmax
		else if (!strcmp(name, "SGIX_BLEND_ALPHA_MINMAX")) return GL_SGIX_blend_alpha_minmax;
#endif
#ifdef GL_SGIX_texture_scale_bias
		else if (!strcmp(name, "SGIX_TEXTURE_SCALE_BIAS")) return GL_SGIX_texture_scale_bias;
#endif
#ifdef GL_SGI_texture_color_table
		else if (!strcmp(name, "SGI_TEXTURE_COLOR_TABLE")) return GL_SGI_texture_color_table;
#endif
#ifdef GL_SMOOTH_LINE_WIDTH_RANGE
		else if (!strcmp(name, "SMOOTH_LINE_WIDTH_RANGE")) return GL_SMOOTH_LINE_WIDTH_RANGE;
#endif
#ifdef GL_SMOOTH_POINT_SIZE_RANGE
		else if (!strcmp(name, "SMOOTH_POINT_SIZE_RANGE")) return GL_SMOOTH_POINT_SIZE_RANGE;
#endif
#ifdef GL_SPRITE_EYE_ALIGNED_SGIX
		else if (!strcmp(name, "SPRITE_EYE_ALIGNED_SGIX")) return GL_SPRITE_EYE_ALIGNED_SGIX;
#endif
#ifdef GL_SPRITE_TRANSLATION_SGIX
		else if (!strcmp(name, "SPRITE_TRANSLATION_SGIX")) return GL_SPRITE_TRANSLATION_SGIX;
#endif
#ifdef GL_STENCIL_PASS_DEPTH_FAIL
		else if (!strcmp(name, "STENCIL_PASS_DEPTH_FAIL")) return GL_STENCIL_PASS_DEPTH_FAIL;
#endif
#ifdef GL_STENCIL_PASS_DEPTH_PASS
		else if (!strcmp(name, "STENCIL_PASS_DEPTH_PASS")) return GL_STENCIL_PASS_DEPTH_PASS;
#endif
#ifdef GL_STRICT_SCISSOR_HINT_PGI
		else if (!strcmp(name, "STRICT_SCISSOR_HINT_PGI")) return GL_STRICT_SCISSOR_HINT_PGI;
#endif
#ifdef GL_TEXTURE_4D_BINDING_SGIS
		else if (!strcmp(name, "TEXTURE_4D_BINDING_SGIS")) return GL_TEXTURE_4D_BINDING_SGIS;
#endif
#ifdef GL_TEXTURE_BASE_LEVEL_SGIS
		else if (!strcmp(name, "TEXTURE_BASE_LEVEL_SGIS")) return GL_TEXTURE_BASE_LEVEL_SGIS;
#endif
#ifdef GL_TEXTURE_COLOR_TABLE_SGI
		else if (!strcmp(name, "TEXTURE_COLOR_TABLE_SGI")) return GL_TEXTURE_COLOR_TABLE_SGI;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_EXT
		else if (!strcmp(name, "TEXTURE_COORD_ARRAY_EXT")) return GL_TEXTURE_COORD_ARRAY_EXT;
#endif
#ifdef GL_TEXTURE_INTERNAL_FORMAT
		else if (!strcmp(name, "TEXTURE_INTERNAL_FORMAT")) return GL_TEXTURE_INTERNAL_FORMAT;
#endif
#ifdef GL_TEXTURE_LOD_BIAS_R_SGIX
		else if (!strcmp(name, "TEXTURE_LOD_BIAS_R_SGIX")) return GL_TEXTURE_LOD_BIAS_R_SGIX;
#endif
#ifdef GL_TEXTURE_LOD_BIAS_S_SGIX
		else if (!strcmp(name, "TEXTURE_LOD_BIAS_S_SGIX")) return GL_TEXTURE_LOD_BIAS_S_SGIX;
#endif
#ifdef GL_TEXTURE_LOD_BIAS_T_SGIX
		else if (!strcmp(name, "TEXTURE_LOD_BIAS_T_SGIX")) return GL_TEXTURE_LOD_BIAS_T_SGIX;
#endif
#ifdef GL_TEXTURE_PRE_SPECULAR_HP
		else if (!strcmp(name, "TEXTURE_PRE_SPECULAR_HP")) return GL_TEXTURE_PRE_SPECULAR_HP;
#endif
#ifdef GL_UNPACK_IMAGE_DEPTH_SGIS
		else if (!strcmp(name, "UNPACK_IMAGE_DEPTH_SGIS")) return GL_UNPACK_IMAGE_DEPTH_SGIS;
#endif
#ifdef GL_UNPACK_IMAGE_HEIGHT_EXT
		else if (!strcmp(name, "UNPACK_IMAGE_HEIGHT_EXT")) return GL_UNPACK_IMAGE_HEIGHT_EXT;
#endif
#ifdef GL_UNSIGNED_BYTE_2_3_3_REV
		else if (!strcmp(name, "UNSIGNED_BYTE_2_3_3_REV")) return GL_UNSIGNED_BYTE_2_3_3_REV;
#endif
#ifdef GL_UNSIGNED_BYTE_3_3_2_EXT
		else if (!strcmp(name, "UNSIGNED_BYTE_3_3_2_EXT")) return GL_UNSIGNED_BYTE_3_3_2_EXT;
#endif
#ifdef GL_UNSIGNED_INT_10_10_10_2
		else if (!strcmp(name, "UNSIGNED_INT_10_10_10_2")) return GL_UNSIGNED_INT_10_10_10_2;
#endif
#ifdef GL_VERTEX_ARRAY_STRIDE_EXT
		else if (!strcmp(name, "VERTEX_ARRAY_STRIDE_EXT")) return GL_VERTEX_ARRAY_STRIDE_EXT;
#endif
#ifdef GL_VERTEX_WEIGHT_ARRAY_EXT
		else if (!strcmp(name, "VERTEX_WEIGHT_ARRAY_EXT")) return GL_VERTEX_WEIGHT_ARRAY_EXT;
#endif
#ifdef GL_WEIGHT_ARRAY_STRIDE_ARB
		else if (!strcmp(name, "WEIGHT_ARRAY_STRIDE_ARB")) return GL_WEIGHT_ARRAY_STRIDE_ARB;
#endif
#ifdef GL_WIN_draw_range_elements
		else if (!strcmp(name, "WIN_DRAW_RANGE_ELEMENTS")) return GL_WIN_draw_range_elements;
#endif
		else return _GL_UNDEFINED;
	case 27:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_ALIASED_LINE_WIDTH_RANGE
		else if (!strcmp(name, "ALIASED_LINE_WIDTH_RANGE")) return GL_ALIASED_LINE_WIDTH_RANGE;
#endif
#ifdef GL_ALIASED_POINT_SIZE_RANGE
		else if (!strcmp(name, "ALIASED_POINT_SIZE_RANGE")) return GL_ALIASED_POINT_SIZE_RANGE;
#endif
#ifdef GL_COLOR_MATERIAL_PARAMETER
		else if (!strcmp(name, "COLOR_MATERIAL_PARAMETER")) return GL_COLOR_MATERIAL_PARAMETER;
#endif
#ifdef GL_COLOR_MATRIX_STACK_DEPTH
		else if (!strcmp(name, "COLOR_MATRIX_STACK_DEPTH")) return GL_COLOR_MATRIX_STACK_DEPTH;
#endif
#ifdef GL_COLOR_TABLE_RED_SIZE_EXT
		else if (!strcmp(name, "COLOR_TABLE_RED_SIZE_EXT")) return GL_COLOR_TABLE_RED_SIZE_EXT;
#endif
#ifdef GL_COLOR_TABLE_RED_SIZE_SGI
		else if (!strcmp(name, "COLOR_TABLE_RED_SIZE_SGI")) return GL_COLOR_TABLE_RED_SIZE_SGI;
#endif
#ifdef GL_COMPRESSED_INTENSITY_ARB
		else if (!strcmp(name, "COMPRESSED_INTENSITY_ARB")) return GL_COMPRESSED_INTENSITY_ARB;
#endif
#ifdef GL_COMPRESSED_LUMINANCE_ARB
		else if (!strcmp(name, "COMPRESSED_LUMINANCE_ARB")) return GL_COMPRESSED_LUMINANCE_ARB;
#endif
#ifdef GL_COMPRESSED_RGB_FXT1_3DFX
		else if (!strcmp(name, "COMPRESSED_RGB_FXT1_3DFX")) return GL_COMPRESSED_RGB_FXT1_3DFX;
#endif
#ifdef GL_CONSERVE_MEMORY_HINT_PGI
		else if (!strcmp(name, "CONSERVE_MEMORY_HINT_PGI")) return GL_CONSERVE_MEMORY_HINT_PGI;
#endif
#ifdef GL_CONVOLUTION_BORDER_COLOR
		else if (!strcmp(name, "CONVOLUTION_BORDER_COLOR")) return GL_CONVOLUTION_BORDER_COLOR;
#endif
#ifdef GL_CONVOLUTION_FILTER_SCALE
		else if (!strcmp(name, "CONVOLUTION_FILTER_SCALE")) return GL_CONVOLUTION_FILTER_SCALE;
#endif
#ifdef GL_DETAIL_TEXTURE_MODE_SGIS
		else if (!strcmp(name, "DETAIL_TEXTURE_MODE_SGIS")) return GL_DETAIL_TEXTURE_MODE_SGIS;
#endif
#ifdef GL_DISTANCE_ATTENUATION_EXT
		else if (!strcmp(name, "DISTANCE_ATTENUATION_EXT")) return GL_DISTANCE_ATTENUATION_EXT;
#endif
#ifdef GL_DUAL_TEXTURE_SELECT_SGIS
		else if (!strcmp(name, "DUAL_TEXTURE_SELECT_SGIS")) return GL_DUAL_TEXTURE_SELECT_SGIS;
#endif
#ifdef GL_EDGE_FLAG_ARRAY_LIST_IBM
		else if (!strcmp(name, "EDGE_FLAG_ARRAY_LIST_IBM")) return GL_EDGE_FLAG_ARRAY_LIST_IBM;
#endif
#ifdef GL_FOG_COORDINATE_ARRAY_EXT
		else if (!strcmp(name, "FOG_COORDINATE_ARRAY_EXT")) return GL_FOG_COORDINATE_ARRAY_EXT;
#endif
#ifdef GL_FOG_SPECULAR_TEXTURE_WIN
		else if (!strcmp(name, "FOG_SPECULAR_TEXTURE_WIN")) return GL_FOG_SPECULAR_TEXTURE_WIN;
#endif
#ifdef GL_HISTOGRAM_ALPHA_SIZE_EXT
		else if (!strcmp(name, "HISTOGRAM_ALPHA_SIZE_EXT")) return GL_HISTOGRAM_ALPHA_SIZE_EXT;
#endif
#ifdef GL_HISTOGRAM_GREEN_SIZE_EXT
		else if (!strcmp(name, "HISTOGRAM_GREEN_SIZE_EXT")) return GL_HISTOGRAM_GREEN_SIZE_EXT;
#endif
#ifdef GL_HISTOGRAM_LUMINANCE_SIZE
		else if (!strcmp(name, "HISTOGRAM_LUMINANCE_SIZE")) return GL_HISTOGRAM_LUMINANCE_SIZE;
#endif
#ifdef GL_IMAGE_ROTATE_ORIGIN_X_HP
		else if (!strcmp(name, "IMAGE_ROTATE_ORIGIN_X_HP")) return GL_IMAGE_ROTATE_ORIGIN_X_HP;
#endif
#ifdef GL_IMAGE_ROTATE_ORIGIN_Y_HP
		else if (!strcmp(name, "IMAGE_ROTATE_ORIGIN_Y_HP")) return GL_IMAGE_ROTATE_ORIGIN_Y_HP;
#endif
#ifdef GL_LIGHT_MODEL_LOCAL_VIEWER
		else if (!strcmp(name, "LIGHT_MODEL_LOCAL_VIEWER")) return GL_LIGHT_MODEL_LOCAL_VIEWER;
#endif
#ifdef GL_LINEAR_DETAIL_ALPHA_SGIS
		else if (!strcmp(name, "LINEAR_DETAIL_ALPHA_SGIS")) return GL_LINEAR_DETAIL_ALPHA_SGIS;
#endif
#ifdef GL_LINEAR_DETAIL_COLOR_SGIS
		else if (!strcmp(name, "LINEAR_DETAIL_COLOR_SGIS")) return GL_LINEAR_DETAIL_COLOR_SGIS;
#endif
#ifdef GL_MAX_4D_TEXTURE_SIZE_SGIS
		else if (!strcmp(name, "MAX_4D_TEXTURE_SIZE_SGIS")) return GL_MAX_4D_TEXTURE_SIZE_SGIS;
#endif
#ifdef GL_MAX_ASYNC_HISTOGRAM_SGIX
		else if (!strcmp(name, "MAX_ASYNC_HISTOGRAM_SGIX")) return GL_MAX_ASYNC_HISTOGRAM_SGIX;
#endif
#ifdef GL_MAX_ASYNC_TEX_IMAGE_SGIX
		else if (!strcmp(name, "MAX_ASYNC_TEX_IMAGE_SGIX")) return GL_MAX_ASYNC_TEX_IMAGE_SGIX;
#endif
#ifdef GL_MAX_ELEMENTS_INDICES_EXT
		else if (!strcmp(name, "MAX_ELEMENTS_INDICES_EXT")) return GL_MAX_ELEMENTS_INDICES_EXT;
#endif
#ifdef GL_MAX_ELEMENTS_INDICES_WIN
		else if (!strcmp(name, "MAX_ELEMENTS_INDICES_WIN")) return GL_MAX_ELEMENTS_INDICES_WIN;
#endif
#ifdef GL_MAX_FOG_FUNC_POINTS_SGIS
		else if (!strcmp(name, "MAX_FOG_FUNC_POINTS_SGIS")) return GL_MAX_FOG_FUNC_POINTS_SGIS;
#endif
#ifdef GL_MAX_FRAGMENT_LIGHTS_SGIX
		else if (!strcmp(name, "MAX_FRAGMENT_LIGHTS_SGIX")) return GL_MAX_FRAGMENT_LIGHTS_SGIX;
#endif
#ifdef GL_MAX_GENERAL_COMBINERS_NV
		else if (!strcmp(name, "MAX_GENERAL_COMBINERS_NV")) return GL_MAX_GENERAL_COMBINERS_NV;
#endif
#ifdef GL_MAX_TEXTURE_LOD_BIAS_EXT
		else if (!strcmp(name, "MAX_TEXTURE_LOD_BIAS_EXT")) return GL_MAX_TEXTURE_LOD_BIAS_EXT;
#endif
#ifdef GL_NORMAL_ARRAY_POINTER_EXT
		else if (!strcmp(name, "NORMAL_ARRAY_POINTER_EXT")) return GL_NORMAL_ARRAY_POINTER_EXT;
#endif
#ifdef GL_NUM_GENERAL_COMBINERS_NV
		else if (!strcmp(name, "NUM_GENERAL_COMBINERS_NV")) return GL_NUM_GENERAL_COMBINERS_NV;
#endif
#ifdef GL_OCCLUSION_TEST_RESULT_HP
		else if (!strcmp(name, "OCCLUSION_TEST_RESULT_HP")) return GL_OCCLUSION_TEST_RESULT_HP;
#endif
#ifdef GL_ONE_MINUS_CONSTANT_ALPHA
		else if (!strcmp(name, "ONE_MINUS_CONSTANT_ALPHA")) return GL_ONE_MINUS_CONSTANT_ALPHA;
#endif
#ifdef GL_ONE_MINUS_CONSTANT_COLOR
		else if (!strcmp(name, "ONE_MINUS_CONSTANT_COLOR")) return GL_ONE_MINUS_CONSTANT_COLOR;
#endif
#ifdef GL_PACK_SUBSAMPLE_RATE_SGIX
		else if (!strcmp(name, "PACK_SUBSAMPLE_RATE_SGIX")) return GL_PACK_SUBSAMPLE_RATE_SGIX;
#endif
#ifdef GL_QUAD_TEXTURE_SELECT_SGIS
		else if (!strcmp(name, "QUAD_TEXTURE_SELECT_SGIS")) return GL_QUAD_TEXTURE_SELECT_SGIS;
#endif
#ifdef GL_R1UI_T2F_C4F_N3F_V3F_SUN
		else if (!strcmp(name, "R1UI_T2F_C4F_N3F_V3F_SUN")) return GL_R1UI_T2F_C4F_N3F_V3F_SUN;
#endif
#ifdef GL_SAMPLE_ALPHA_TO_MASK_EXT
		else if (!strcmp(name, "SAMPLE_ALPHA_TO_MASK_EXT")) return GL_SAMPLE_ALPHA_TO_MASK_EXT;
#endif
#ifdef GL_SAMPLE_ALPHA_TO_ONE_SGIS
		else if (!strcmp(name, "SAMPLE_ALPHA_TO_ONE_SGIS")) return GL_SAMPLE_ALPHA_TO_ONE_SGIS;
#endif
#ifdef GL_SELECTION_BUFFER_POINTER
		else if (!strcmp(name, "SELECTION_BUFFER_POINTER")) return GL_SELECTION_BUFFER_POINTER;
#endif
#ifdef GL_STRICT_LIGHTING_HINT_PGI
		else if (!strcmp(name, "STRICT_LIGHTING_HINT_PGI")) return GL_STRICT_LIGHTING_HINT_PGI;
#endif
#ifdef GL_TANGENT_ARRAY_STRIDE_EXT
		else if (!strcmp(name, "TANGENT_ARRAY_STRIDE_EXT")) return GL_TANGENT_ARRAY_STRIDE_EXT;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_SIZE
		else if (!strcmp(name, "TEXTURE_COORD_ARRAY_SIZE")) return GL_TEXTURE_COORD_ARRAY_SIZE;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_TYPE
		else if (!strcmp(name, "TEXTURE_COORD_ARRAY_TYPE")) return GL_TEXTURE_COORD_ARRAY_TYPE;
#endif
#ifdef GL_TEXTURE_DEFORMATION_SGIX
		else if (!strcmp(name, "TEXTURE_DEFORMATION_SGIX")) return GL_TEXTURE_DEFORMATION_SGIX;
#endif
#ifdef GL_TEXTURE_LIGHTING_MODE_HP
		else if (!strcmp(name, "TEXTURE_LIGHTING_MODE_HP")) return GL_TEXTURE_LIGHTING_MODE_HP;
#endif
#ifdef GL_TEXTURE_POST_SPECULAR_HP
		else if (!strcmp(name, "TEXTURE_POST_SPECULAR_HP")) return GL_TEXTURE_POST_SPECULAR_HP;
#endif
#ifdef GL_UNPACK_SKIP_VOLUMES_SGIS
		else if (!strcmp(name, "UNPACK_SKIP_VOLUMES_SGIS")) return GL_UNPACK_SKIP_VOLUMES_SGIS;
#endif
#ifdef GL_UNSIGNED_INT_8_8_8_8_EXT
		else if (!strcmp(name, "UNSIGNED_INT_8_8_8_8_EXT")) return GL_UNSIGNED_INT_8_8_8_8_EXT;
#endif
#ifdef GL_UNSIGNED_INT_8_8_8_8_REV
		else if (!strcmp(name, "UNSIGNED_INT_8_8_8_8_REV")) return GL_UNSIGNED_INT_8_8_8_8_REV;
#endif
#ifdef GL_UNSIGNED_SHORT_5_6_5_REV
		else if (!strcmp(name, "UNSIGNED_SHORT_5_6_5_REV")) return GL_UNSIGNED_SHORT_5_6_5_REV;
#endif
#ifdef GL_VERTEX_ARRAY_POINTER_EXT
		else if (!strcmp(name, "VERTEX_ARRAY_POINTER_EXT")) return GL_VERTEX_ARRAY_POINTER_EXT;
#endif
#ifdef GL_VERTEX_PRECLIP_HINT_SGIX
		else if (!strcmp(name, "VERTEX_PRECLIP_HINT_SGIX")) return GL_VERTEX_PRECLIP_HINT_SGIX;
#endif
#ifdef GL_WEIGHT_ARRAY_POINTER_ARB
		else if (!strcmp(name, "WEIGHT_ARRAY_POINTER_ARB")) return GL_WEIGHT_ARRAY_POINTER_ARB;
#endif
		else return _GL_UNDEFINED;
	case 28:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_BINORMAL_ARRAY_STRIDE_EXT
		else if (!strcmp(name, "BINORMAL_ARRAY_STRIDE_EXT")) return GL_BINORMAL_ARRAY_STRIDE_EXT;
#endif
#ifdef GL_CLIENT_ACTIVE_TEXTURE_ARB
		else if (!strcmp(name, "CLIENT_ACTIVE_TEXTURE_ARB")) return GL_CLIENT_ACTIVE_TEXTURE_ARB;
#endif
#ifdef GL_CLIENT_ATTRIB_STACK_DEPTH
		else if (!strcmp(name, "CLIENT_ATTRIB_STACK_DEPTH")) return GL_CLIENT_ATTRIB_STACK_DEPTH;
#endif
#ifdef GL_COLOR_TABLE_BLUE_SIZE_EXT
		else if (!strcmp(name, "COLOR_TABLE_BLUE_SIZE_EXT")) return GL_COLOR_TABLE_BLUE_SIZE_EXT;
#endif
#ifdef GL_COLOR_TABLE_BLUE_SIZE_SGI
		else if (!strcmp(name, "COLOR_TABLE_BLUE_SIZE_SGI")) return GL_COLOR_TABLE_BLUE_SIZE_SGI;
#endif
#ifdef GL_COMPRESSED_RGBA_FXT1_3DFX
		else if (!strcmp(name, "COMPRESSED_RGBA_FXT1_3DFX")) return GL_COMPRESSED_RGBA_FXT1_3DFX;
#endif
#ifdef GL_CURRENT_VERTEX_WEIGHT_EXT
		else if (!strcmp(name, "CURRENT_VERTEX_WEIGHT_EXT")) return GL_CURRENT_VERTEX_WEIGHT_EXT;
#endif
#ifdef GL_DETAIL_TEXTURE_LEVEL_SGIS
		else if (!strcmp(name, "DETAIL_TEXTURE_LEVEL_SGIS")) return GL_DETAIL_TEXTURE_LEVEL_SGIS;
#endif
#ifdef GL_DISTANCE_ATTENUATION_SGIS
		else if (!strcmp(name, "DISTANCE_ATTENUATION_SGIS")) return GL_DISTANCE_ATTENUATION_SGIS;
#endif
#ifdef GL_EDGE_FLAG_ARRAY_COUNT_EXT
		else if (!strcmp(name, "EDGE_FLAG_ARRAY_COUNT_EXT")) return GL_EDGE_FLAG_ARRAY_COUNT_EXT;
#endif
#ifdef GL_EXT_compiled_vertex_array
		else if (!strcmp(name, "EXT_COMPILED_VERTEX_ARRAY")) return GL_EXT_compiled_vertex_array;
#endif
#ifdef GL_EYE_DISTANCE_TO_LINE_SGIS
		else if (!strcmp(name, "EYE_DISTANCE_TO_LINE_SGIS")) return GL_EYE_DISTANCE_TO_LINE_SGIS;
#endif
#ifdef GL_FOG_COORDINATE_SOURCE_EXT
		else if (!strcmp(name, "FOG_COORDINATE_SOURCE_EXT")) return GL_FOG_COORDINATE_SOURCE_EXT;
#endif
#ifdef GL_FUNC_REVERSE_SUBTRACT_EXT
		else if (!strcmp(name, "FUNC_REVERSE_SUBTRACT_EXT")) return GL_FUNC_REVERSE_SUBTRACT_EXT;
#endif
#ifdef GL_GENERATE_MIPMAP_HINT_SGIS
		else if (!strcmp(name, "GENERATE_MIPMAP_HINT_SGIS")) return GL_GENERATE_MIPMAP_HINT_SGIS;
#endif
#ifdef GL_GEOMETRY_DEFORMATION_SGIX
		else if (!strcmp(name, "GEOMETRY_DEFORMATION_SGIX")) return GL_GEOMETRY_DEFORMATION_SGIX;
#endif
#ifdef GL_IBM_multimode_draw_arrays
		else if (!strcmp(name, "IBM_MULTIMODE_DRAW_ARRAYS")) return GL_IBM_multimode_draw_arrays;
#endif
#ifdef GL_LIGHT_MODEL_COLOR_CONTROL
		else if (!strcmp(name, "LIGHT_MODEL_COLOR_CONTROL")) return GL_LIGHT_MODEL_COLOR_CONTROL;
#endif
#ifdef GL_LINEAR_SHARPEN_ALPHA_SGIS
		else if (!strcmp(name, "LINEAR_SHARPEN_ALPHA_SGIS")) return GL_LINEAR_SHARPEN_ALPHA_SGIS;
#endif
#ifdef GL_LINEAR_SHARPEN_COLOR_SGIS
		else if (!strcmp(name, "LINEAR_SHARPEN_COLOR_SGIS")) return GL_LINEAR_SHARPEN_COLOR_SGIS;
#endif
#ifdef GL_MAT_COLOR_INDEXES_BIT_PGI
		else if (!strcmp(name, "MAT_COLOR_INDEXES_BIT_PGI")) return GL_MAT_COLOR_INDEXES_BIT_PGI;
#endif
#ifdef GL_MAX_CONVOLUTION_WIDTH_EXT
		else if (!strcmp(name, "MAX_CONVOLUTION_WIDTH_EXT")) return GL_MAX_CONVOLUTION_WIDTH_EXT;
#endif
#ifdef GL_MAX_ELEMENTS_VERTICES_EXT
		else if (!strcmp(name, "MAX_ELEMENTS_VERTICES_EXT")) return GL_MAX_ELEMENTS_VERTICES_EXT;
#endif
#ifdef GL_MAX_ELEMENTS_VERTICES_WIN
		else if (!strcmp(name, "MAX_ELEMENTS_VERTICES_WIN")) return GL_MAX_ELEMENTS_VERTICES_WIN;
#endif
#ifdef GL_MAX_FRAMEZOOM_FACTOR_SGIX
		else if (!strcmp(name, "MAX_FRAMEZOOM_FACTOR_SGIX")) return GL_MAX_FRAMEZOOM_FACTOR_SGIX;
#endif
#ifdef GL_MAX_MODELVIEW_STACK_DEPTH
		else if (!strcmp(name, "MAX_MODELVIEW_STACK_DEPTH")) return GL_MAX_MODELVIEW_STACK_DEPTH;
#endif
#ifdef GL_PIXEL_SUBSAMPLE_2424_SGIX
		else if (!strcmp(name, "PIXEL_SUBSAMPLE_2424_SGIX")) return GL_PIXEL_SUBSAMPLE_2424_SGIX;
#endif
#ifdef GL_PIXEL_SUBSAMPLE_4242_SGIX
		else if (!strcmp(name, "PIXEL_SUBSAMPLE_4242_SGIX")) return GL_PIXEL_SUBSAMPLE_4242_SGIX;
#endif
#ifdef GL_PIXEL_SUBSAMPLE_4444_SGIX
		else if (!strcmp(name, "PIXEL_SUBSAMPLE_4444_SGIX")) return GL_PIXEL_SUBSAMPLE_4444_SGIX;
#endif
#ifdef GL_POLYGON_OFFSET_FACTOR_EXT
		else if (!strcmp(name, "POLYGON_OFFSET_FACTOR_EXT")) return GL_POLYGON_OFFSET_FACTOR_EXT;
#endif
#ifdef GL_POST_CONVOLUTION_RED_BIAS
		else if (!strcmp(name, "POST_CONVOLUTION_RED_BIAS")) return GL_POST_CONVOLUTION_RED_BIAS;
#endif
#ifdef GL_SAMPLE_ALPHA_TO_MASK_SGIS
		else if (!strcmp(name, "SAMPLE_ALPHA_TO_MASK_SGIS")) return GL_SAMPLE_ALPHA_TO_MASK_SGIS;
#endif
#ifdef GL_SAMPLE_COVERAGE_VALUE_ARB
		else if (!strcmp(name, "SAMPLE_COVERAGE_VALUE_ARB")) return GL_SAMPLE_COVERAGE_VALUE_ARB;
#endif
#ifdef GL_SECONDARY_COLOR_ARRAY_EXT
		else if (!strcmp(name, "SECONDARY_COLOR_ARRAY_EXT")) return GL_SECONDARY_COLOR_ARRAY_EXT;
#endif
#ifdef GL_SGIS_texture_border_clamp
		else if (!strcmp(name, "SGIS_TEXTURE_BORDER_CLAMP")) return GL_SGIS_texture_border_clamp;
#endif
#ifdef GL_SGIX_convolution_accuracy
		else if (!strcmp(name, "SGIX_CONVOLUTION_ACCURACY")) return GL_SGIX_convolution_accuracy;
#endif
#ifdef GL_SGIX_texture_multi_buffer
		else if (!strcmp(name, "SGIX_TEXTURE_MULTI_BUFFER")) return GL_SGIX_texture_multi_buffer;
#endif
#ifdef GL_STRICT_DEPTHFUNC_HINT_PGI
		else if (!strcmp(name, "STRICT_DEPTHFUNC_HINT_PGI")) return GL_STRICT_DEPTHFUNC_HINT_PGI;
#endif
#ifdef GL_TANGENT_ARRAY_POINTER_EXT
		else if (!strcmp(name, "TANGENT_ARRAY_POINTER_EXT")) return GL_TANGENT_ARRAY_POINTER_EXT;
#endif
#ifdef GL_TEXTURE_FILTER4_SIZE_SGIS
		else if (!strcmp(name, "TEXTURE_FILTER4_SIZE_SGIS")) return GL_TEXTURE_FILTER4_SIZE_SGIS;
#endif
#ifdef GL_TEXTURE_MATERIAL_FACE_EXT
		else if (!strcmp(name, "TEXTURE_MATERIAL_FACE_EXT")) return GL_TEXTURE_MATERIAL_FACE_EXT;
#endif
#ifdef GL_UNPACK_CONSTANT_DATA_SUNX
		else if (!strcmp(name, "UNPACK_CONSTANT_DATA_SUNX")) return GL_UNPACK_CONSTANT_DATA_SUNX;
#endif
		else return _GL_UNDEFINED;
	case 29:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_BINORMAL_ARRAY_POINTER_EXT
		else if (!strcmp(name, "BINORMAL_ARRAY_POINTER_EXT")) return GL_BINORMAL_ARRAY_POINTER_EXT;
#endif
#ifdef GL_CALLIGRAPHIC_FRAGMENT_SGIX
		else if (!strcmp(name, "CALLIGRAPHIC_FRAGMENT_SGIX")) return GL_CALLIGRAPHIC_FRAGMENT_SGIX;
#endif
#ifdef GL_COLOR_TABLE_ALPHA_SIZE_EXT
		else if (!strcmp(name, "COLOR_TABLE_ALPHA_SIZE_EXT")) return GL_COLOR_TABLE_ALPHA_SIZE_EXT;
#endif
#ifdef GL_COLOR_TABLE_ALPHA_SIZE_SGI
		else if (!strcmp(name, "COLOR_TABLE_ALPHA_SIZE_SGI")) return GL_COLOR_TABLE_ALPHA_SIZE_SGI;
#endif
#ifdef GL_COLOR_TABLE_GREEN_SIZE_EXT
		else if (!strcmp(name, "COLOR_TABLE_GREEN_SIZE_EXT")) return GL_COLOR_TABLE_GREEN_SIZE_EXT;
#endif
#ifdef GL_COLOR_TABLE_GREEN_SIZE_SGI
		else if (!strcmp(name, "COLOR_TABLE_GREEN_SIZE_SGI")) return GL_COLOR_TABLE_GREEN_SIZE_SGI;
#endif
#ifdef GL_COLOR_TABLE_INTENSITY_SIZE
		else if (!strcmp(name, "COLOR_TABLE_INTENSITY_SIZE")) return GL_COLOR_TABLE_INTENSITY_SIZE;
#endif
#ifdef GL_COLOR_TABLE_LUMINANCE_SIZE
		else if (!strcmp(name, "COLOR_TABLE_LUMINANCE_SIZE")) return GL_COLOR_TABLE_LUMINANCE_SIZE;
#endif
#ifdef GL_COMBINER_AB_DOT_PRODUCT_NV
		else if (!strcmp(name, "COMBINER_AB_DOT_PRODUCT_NV")) return GL_COMBINER_AB_DOT_PRODUCT_NV;
#endif
#ifdef GL_COMBINER_CD_DOT_PRODUCT_NV
		else if (!strcmp(name, "COMBINER_CD_DOT_PRODUCT_NV")) return GL_COMBINER_CD_DOT_PRODUCT_NV;
#endif
#ifdef GL_CURRENT_FOG_COORDINATE_EXT
		else if (!strcmp(name, "CURRENT_FOG_COORDINATE_EXT")) return GL_CURRENT_FOG_COORDINATE_EXT;
#endif
#ifdef GL_CURRENT_RASTER_NORMAL_SGIX
		else if (!strcmp(name, "CURRENT_RASTER_NORMAL_SGIX")) return GL_CURRENT_RASTER_NORMAL_SGIX;
#endif
#ifdef GL_DEPTH_PASS_INSTRUMENT_SGIX
		else if (!strcmp(name, "DEPTH_PASS_INSTRUMENT_SGIX")) return GL_DEPTH_PASS_INSTRUMENT_SGIX;
#endif
#ifdef GL_DUAL_LUMINANCE_ALPHA4_SGIS
		else if (!strcmp(name, "DUAL_LUMINANCE_ALPHA4_SGIS")) return GL_DUAL_LUMINANCE_ALPHA4_SGIS;
#endif
#ifdef GL_DUAL_LUMINANCE_ALPHA8_SGIS
		else if (!strcmp(name, "DUAL_LUMINANCE_ALPHA8_SGIS")) return GL_DUAL_LUMINANCE_ALPHA8_SGIS;
#endif
#ifdef GL_EDGE_FLAG_ARRAY_STRIDE_EXT
		else if (!strcmp(name, "EDGE_FLAG_ARRAY_STRIDE_EXT")) return GL_EDGE_FLAG_ARRAY_STRIDE_EXT;
#endif
#ifdef GL_EXT_shared_texture_palette
		else if (!strcmp(name, "EXT_SHARED_TEXTURE_PALETTE")) return GL_EXT_shared_texture_palette;
#endif
#ifdef GL_EXT_texture_perturb_normal
		else if (!strcmp(name, "EXT_TEXTURE_PERTURB_NORMAL")) return GL_EXT_texture_perturb_normal;
#endif
#ifdef GL_EYE_DISTANCE_TO_POINT_SGIS
		else if (!strcmp(name, "EYE_DISTANCE_TO_POINT_SGIS")) return GL_EYE_DISTANCE_TO_POINT_SGIS;
#endif
#ifdef GL_LINEAR_CLIPMAP_LINEAR_SGIX
		else if (!strcmp(name, "LINEAR_CLIPMAP_LINEAR_SGIX")) return GL_LINEAR_CLIPMAP_LINEAR_SGIX;
#endif
#ifdef GL_MAX_ASYNC_DRAW_PIXELS_SGIX
		else if (!strcmp(name, "MAX_ASYNC_DRAW_PIXELS_SGIX")) return GL_MAX_ASYNC_DRAW_PIXELS_SGIX;
#endif
#ifdef GL_MAX_ASYNC_READ_PIXELS_SGIX
		else if (!strcmp(name, "MAX_ASYNC_READ_PIXELS_SGIX")) return GL_MAX_ASYNC_READ_PIXELS_SGIX;
#endif
#ifdef GL_MAX_CONVOLUTION_HEIGHT_EXT
		else if (!strcmp(name, "MAX_CONVOLUTION_HEIGHT_EXT")) return GL_MAX_CONVOLUTION_HEIGHT_EXT;
#endif
#ifdef GL_MAX_DEFORMATION_ORDER_SGIX
		else if (!strcmp(name, "MAX_DEFORMATION_ORDER_SGIX")) return GL_MAX_DEFORMATION_ORDER_SGIX;
#endif
#ifdef GL_MAX_PROJECTION_STACK_DEPTH
		else if (!strcmp(name, "MAX_PROJECTION_STACK_DEPTH")) return GL_MAX_PROJECTION_STACK_DEPTH;
#endif
#ifdef GL_MODELVIEW0_STACK_DEPTH_EXT
		else if (!strcmp(name, "MODELVIEW0_STACK_DEPTH_EXT")) return GL_MODELVIEW0_STACK_DEPTH_EXT;
#endif
#ifdef GL_MODELVIEW1_STACK_DEPTH_EXT
		else if (!strcmp(name, "MODELVIEW1_STACK_DEPTH_EXT")) return GL_MODELVIEW1_STACK_DEPTH_EXT;
#endif
#ifdef GL_NATIVE_GRAPHICS_HANDLE_PGI
		else if (!strcmp(name, "NATIVE_GRAPHICS_HANDLE_PGI")) return GL_NATIVE_GRAPHICS_HANDLE_PGI;
#endif
#ifdef GL_PIXEL_TILE_CACHE_SIZE_SGIX
		else if (!strcmp(name, "PIXEL_TILE_CACHE_SIZE_SGIX")) return GL_PIXEL_TILE_CACHE_SIZE_SGIX;
#endif
#ifdef GL_PIXEL_TILE_GRID_DEPTH_SGIX
		else if (!strcmp(name, "PIXEL_TILE_GRID_DEPTH_SGIX")) return GL_PIXEL_TILE_GRID_DEPTH_SGIX;
#endif
#ifdef GL_PIXEL_TILE_GRID_WIDTH_SGIX
		else if (!strcmp(name, "PIXEL_TILE_GRID_WIDTH_SGIX")) return GL_PIXEL_TILE_GRID_WIDTH_SGIX;
#endif
#ifdef GL_POST_COLOR_MATRIX_RED_BIAS
		else if (!strcmp(name, "POST_COLOR_MATRIX_RED_BIAS")) return GL_POST_COLOR_MATRIX_RED_BIAS;
#endif
#ifdef GL_POST_CONVOLUTION_BLUE_BIAS
		else if (!strcmp(name, "POST_CONVOLUTION_BLUE_BIAS")) return GL_POST_CONVOLUTION_BLUE_BIAS;
#endif
#ifdef GL_POST_CONVOLUTION_RED_SCALE
		else if (!strcmp(name, "POST_CONVOLUTION_RED_SCALE")) return GL_POST_CONVOLUTION_RED_SCALE;
#endif
#ifdef GL_PROXY_TEXTURE_CUBE_MAP_ARB
		else if (!strcmp(name, "PROXY_TEXTURE_CUBE_MAP_ARB")) return GL_PROXY_TEXTURE_CUBE_MAP_ARB;
#endif
#ifdef GL_PROXY_TEXTURE_CUBE_MAP_EXT
		else if (!strcmp(name, "PROXY_TEXTURE_CUBE_MAP_EXT")) return GL_PROXY_TEXTURE_CUBE_MAP_EXT;
#endif
#ifdef GL_REPLACEMENT_CODE_ARRAY_SUN
		else if (!strcmp(name, "REPLACEMENT_CODE_ARRAY_SUN")) return GL_REPLACEMENT_CODE_ARRAY_SUN;
#endif
#ifdef GL_SAMPLE_COVERAGE_INVERT_ARB
		else if (!strcmp(name, "SAMPLE_COVERAGE_INVERT_ARB")) return GL_SAMPLE_COVERAGE_INVERT_ARB;
#endif
#ifdef GL_SGIX_calligraphic_fragment
		else if (!strcmp(name, "SGIX_CALLIGRAPHIC_FRAGMENT")) return GL_SGIX_calligraphic_fragment;
#endif
#ifdef GL_SGIX_depth_pass_instrument
		else if (!strcmp(name, "SGIX_DEPTH_PASS_INSTRUMENT")) return GL_SGIX_depth_pass_instrument;
#endif
#ifdef GL_SHARED_TEXTURE_PALETTE_EXT
		else if (!strcmp(name, "SHARED_TEXTURE_PALETTE_EXT")) return GL_SHARED_TEXTURE_PALETTE_EXT;
#endif
#ifdef GL_SPRITE_OBJECT_ALIGNED_SGIX
		else if (!strcmp(name, "SPRITE_OBJECT_ALIGNED_SGIX")) return GL_SPRITE_OBJECT_ALIGNED_SGIX;
#endif
#ifdef GL_TEXTURE_CLIPMAP_DEPTH_SGIX
		else if (!strcmp(name, "TEXTURE_CLIPMAP_DEPTH_SGIX")) return GL_TEXTURE_CLIPMAP_DEPTH_SGIX;
#endif
#ifdef GL_TEXTURE_CLIPMAP_FRAME_SGIX
		else if (!strcmp(name, "TEXTURE_CLIPMAP_FRAME_SGIX")) return GL_TEXTURE_CLIPMAP_FRAME_SGIX;
#endif
#ifdef GL_TEXTURE_CONSTANT_DATA_SUNX
		else if (!strcmp(name, "TEXTURE_CONSTANT_DATA_SUNX")) return GL_TEXTURE_CONSTANT_DATA_SUNX;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_STRIDE
		else if (!strcmp(name, "TEXTURE_COORD_ARRAY_STRIDE")) return GL_TEXTURE_COORD_ARRAY_STRIDE;
#endif
#ifdef GL_TEXTURE_FILTER_CONTROL_EXT
		else if (!strcmp(name, "TEXTURE_FILTER_CONTROL_EXT")) return GL_TEXTURE_FILTER_CONTROL_EXT;
#endif
#ifdef GL_TEXTURE_INTENSITY_SIZE_EXT
		else if (!strcmp(name, "TEXTURE_INTENSITY_SIZE_EXT")) return GL_TEXTURE_INTENSITY_SIZE_EXT;
#endif
#ifdef GL_TEXTURE_LUMINANCE_SIZE_EXT
		else if (!strcmp(name, "TEXTURE_LUMINANCE_SIZE_EXT")) return GL_TEXTURE_LUMINANCE_SIZE_EXT;
#endif
#ifdef GL_TEXTURE_MAX_ANISOTROPY_EXT
		else if (!strcmp(name, "TEXTURE_MAX_ANISOTROPY_EXT")) return GL_TEXTURE_MAX_ANISOTROPY_EXT;
#endif
#ifdef GL_TRANSPOSE_COLOR_MATRIX_ARB
		else if (!strcmp(name, "TRANSPOSE_COLOR_MATRIX_ARB")) return GL_TRANSPOSE_COLOR_MATRIX_ARB;
#endif
#ifdef GL_UNPACK_SUBSAMPLE_RATE_SGIX
		else if (!strcmp(name, "UNPACK_SUBSAMPLE_RATE_SGIX")) return GL_UNPACK_SUBSAMPLE_RATE_SGIX;
#endif
#ifdef GL_UNSIGNED_SHORT_1_5_5_5_REV
		else if (!strcmp(name, "UNSIGNED_SHORT_1_5_5_5_REV")) return GL_UNSIGNED_SHORT_1_5_5_5_REV;
#endif
#ifdef GL_UNSIGNED_SHORT_4_4_4_4_EXT
		else if (!strcmp(name, "UNSIGNED_SHORT_4_4_4_4_EXT")) return GL_UNSIGNED_SHORT_4_4_4_4_EXT;
#endif
#ifdef GL_UNSIGNED_SHORT_4_4_4_4_REV
		else if (!strcmp(name, "UNSIGNED_SHORT_4_4_4_4_REV")) return GL_UNSIGNED_SHORT_4_4_4_4_REV;
#endif
#ifdef GL_UNSIGNED_SHORT_5_5_5_1_EXT
		else if (!strcmp(name, "UNSIGNED_SHORT_5_5_5_1_EXT")) return GL_UNSIGNED_SHORT_5_5_5_1_EXT;
#endif
#ifdef GL_VERTEX_CONSISTENT_HINT_PGI
		else if (!strcmp(name, "VERTEX_CONSISTENT_HINT_PGI")) return GL_VERTEX_CONSISTENT_HINT_PGI;
#endif
		else return _GL_UNDEFINED;
	case 30:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_COLOR_ARRAY_LIST_STRIDE_IBM
		else if (!strcmp(name, "COLOR_ARRAY_LIST_STRIDE_IBM")) return GL_COLOR_ARRAY_LIST_STRIDE_IBM;
#endif
#ifdef GL_COMBINER_COMPONENT_USAGE_NV
		else if (!strcmp(name, "COMBINER_COMPONENT_USAGE_NV")) return GL_COMBINER_COMPONENT_USAGE_NV;
#endif
#ifdef GL_CONVOLUTION_BORDER_COLOR_HP
		else if (!strcmp(name, "CONVOLUTION_BORDER_COLOR_HP")) return GL_CONVOLUTION_BORDER_COLOR_HP;
#endif
#ifdef GL_CONVOLUTION_BORDER_MODE_EXT
		else if (!strcmp(name, "CONVOLUTION_BORDER_MODE_EXT")) return GL_CONVOLUTION_BORDER_MODE_EXT;
#endif
#ifdef GL_CONVOLUTION_FILTER_BIAS_EXT
		else if (!strcmp(name, "CONVOLUTION_FILTER_BIAS_EXT")) return GL_CONVOLUTION_FILTER_BIAS_EXT;
#endif
#ifdef GL_CURRENT_SECONDARY_COLOR_EXT
		else if (!strcmp(name, "CURRENT_SECONDARY_COLOR_EXT")) return GL_CURRENT_SECONDARY_COLOR_EXT;
#endif
#ifdef GL_EDGE_FLAG_ARRAY_POINTER_EXT
		else if (!strcmp(name, "EDGE_FLAG_ARRAY_POINTER_EXT")) return GL_EDGE_FLAG_ARRAY_POINTER_EXT;
#endif
#ifdef GL_EXT_separate_specular_color
		else if (!strcmp(name, "EXT_SEPARATE_SPECULAR_COLOR")) return GL_EXT_separate_specular_color;
#endif
#ifdef GL_HP_convolution_border_modes
		else if (!strcmp(name, "HP_CONVOLUTION_BORDER_MODES")) return GL_HP_convolution_border_modes;
#endif
#ifdef GL_INDEX_ARRAY_LIST_STRIDE_IBM
		else if (!strcmp(name, "INDEX_ARRAY_LIST_STRIDE_IBM")) return GL_INDEX_ARRAY_LIST_STRIDE_IBM;
#endif
#ifdef GL_LINEAR_CLIPMAP_NEAREST_SGIX
		else if (!strcmp(name, "LINEAR_CLIPMAP_NEAREST_SGIX")) return GL_LINEAR_CLIPMAP_NEAREST_SGIX;
#endif
#ifdef GL_NEAREST_CLIPMAP_LINEAR_SGIX
		else if (!strcmp(name, "NEAREST_CLIPMAP_LINEAR_SGIX")) return GL_NEAREST_CLIPMAP_LINEAR_SGIX;
#endif
#ifdef GL_PERSPECTIVE_CORRECTION_HINT
		else if (!strcmp(name, "PERSPECTIVE_CORRECTION_HINT")) return GL_PERSPECTIVE_CORRECTION_HINT;
#endif
#ifdef GL_PIXEL_TILE_GRID_HEIGHT_SGIX
		else if (!strcmp(name, "PIXEL_TILE_GRID_HEIGHT_SGIX")) return GL_PIXEL_TILE_GRID_HEIGHT_SGIX;
#endif
#ifdef GL_POST_COLOR_MATRIX_BLUE_BIAS
		else if (!strcmp(name, "POST_COLOR_MATRIX_BLUE_BIAS")) return GL_POST_COLOR_MATRIX_BLUE_BIAS;
#endif
#ifdef GL_POST_COLOR_MATRIX_RED_SCALE
		else if (!strcmp(name, "POST_COLOR_MATRIX_RED_SCALE")) return GL_POST_COLOR_MATRIX_RED_SCALE;
#endif
#ifdef GL_POST_CONVOLUTION_ALPHA_BIAS
		else if (!strcmp(name, "POST_CONVOLUTION_ALPHA_BIAS")) return GL_POST_CONVOLUTION_ALPHA_BIAS;
#endif
#ifdef GL_POST_CONVOLUTION_BLUE_SCALE
		else if (!strcmp(name, "POST_CONVOLUTION_BLUE_SCALE")) return GL_POST_CONVOLUTION_BLUE_SCALE;
#endif
#ifdef GL_POST_CONVOLUTION_GREEN_BIAS
		else if (!strcmp(name, "POST_CONVOLUTION_GREEN_BIAS")) return GL_POST_CONVOLUTION_GREEN_BIAS;
#endif
#ifdef GL_SEPARATE_SPECULAR_COLOR_EXT
		else if (!strcmp(name, "SEPARATE_SPECULAR_COLOR_EXT")) return GL_SEPARATE_SPECULAR_COLOR_EXT;
#endif
#ifdef GL_TEXTURE_CLIPMAP_CENTER_SGIX
		else if (!strcmp(name, "TEXTURE_CLIPMAP_CENTER_SGIX")) return GL_TEXTURE_CLIPMAP_CENTER_SGIX;
#endif
#ifdef GL_TEXTURE_CLIPMAP_OFFSET_SGIX
		else if (!strcmp(name, "TEXTURE_CLIPMAP_OFFSET_SGIX")) return GL_TEXTURE_CLIPMAP_OFFSET_SGIX;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_POINTER
		else if (!strcmp(name, "TEXTURE_COORD_ARRAY_POINTER")) return GL_TEXTURE_COORD_ARRAY_POINTER;
#endif
#ifdef GL_UNSIGNED_INT_10_10_10_2_EXT
		else if (!strcmp(name, "UNSIGNED_INT_10_10_10_2_EXT")) return GL_UNSIGNED_INT_10_10_10_2_EXT;
#endif
#ifdef GL_UNSIGNED_INT_2_10_10_10_REV
		else if (!strcmp(name, "UNSIGNED_INT_2_10_10_10_REV")) return GL_UNSIGNED_INT_2_10_10_10_REV;
#endif
#ifdef GL_VERTEX_ARRAY_RANGE_VALID_NV
		else if (!strcmp(name, "VERTEX_ARRAY_RANGE_VALID_NV")) return GL_VERTEX_ARRAY_RANGE_VALID_NV;
#endif
		else return _GL_UNDEFINED;
	case 31:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_ARRAY_ELEMENT_LOCK_COUNT_EXT
		else if (!strcmp(name, "ARRAY_ELEMENT_LOCK_COUNT_EXT")) return GL_ARRAY_ELEMENT_LOCK_COUNT_EXT;
#endif
#ifdef GL_ARRAY_ELEMENT_LOCK_FIRST_EXT
		else if (!strcmp(name, "ARRAY_ELEMENT_LOCK_FIRST_EXT")) return GL_ARRAY_ELEMENT_LOCK_FIRST_EXT;
#endif
#ifdef GL_BIAS_BY_NEGATIVE_ONE_HALF_NV
		else if (!strcmp(name, "BIAS_BY_NEGATIVE_ONE_HALF_NV")) return GL_BIAS_BY_NEGATIVE_ONE_HALF_NV;
#endif
#ifdef GL_COLOR_MATRIX_STACK_DEPTH_SGI
		else if (!strcmp(name, "COLOR_MATRIX_STACK_DEPTH_SGI")) return GL_COLOR_MATRIX_STACK_DEPTH_SGI;
#endif
#ifdef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
		else if (!strcmp(name, "COMPRESSED_RGB_S3TC_DXT1_EXT")) return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
#endif
#ifdef GL_CONVOLUTION_FILTER_SCALE_EXT
		else if (!strcmp(name, "CONVOLUTION_FILTER_SCALE_EXT")) return GL_CONVOLUTION_FILTER_SCALE_EXT;
#endif
#ifdef GL_CULL_VERTEX_EYE_POSITION_EXT
		else if (!strcmp(name, "CULL_VERTEX_EYE_POSITION_EXT")) return GL_CULL_VERTEX_EYE_POSITION_EXT;
#endif
#ifdef GL_FRAGMENT_COLOR_MATERIAL_SGIX
		else if (!strcmp(name, "FRAGMENT_COLOR_MATERIAL_SGIX")) return GL_FRAGMENT_COLOR_MATERIAL_SGIX;
#endif
#ifdef GL_HISTOGRAM_LUMINANCE_SIZE_EXT
		else if (!strcmp(name, "HISTOGRAM_LUMINANCE_SIZE_EXT")) return GL_HISTOGRAM_LUMINANCE_SIZE_EXT;
#endif
#ifdef GL_INDEX_MATERIAL_PARAMETER_EXT
		else if (!strcmp(name, "INDEX_MATERIAL_PARAMETER_EXT")) return GL_INDEX_MATERIAL_PARAMETER_EXT;
#endif
#ifdef GL_INSTRUMENT_MEASUREMENTS_SGIX
		else if (!strcmp(name, "INSTRUMENT_MEASUREMENTS_SGIX")) return GL_INSTRUMENT_MEASUREMENTS_SGIX;
#endif
#ifdef GL_MAX_COLOR_MATRIX_STACK_DEPTH
		else if (!strcmp(name, "MAX_COLOR_MATRIX_STACK_DEPTH")) return GL_MAX_COLOR_MATRIX_STACK_DEPTH;
#endif
#ifdef GL_NATIVE_GRAPHICS_END_HINT_PGI
		else if (!strcmp(name, "NATIVE_GRAPHICS_END_HINT_PGI")) return GL_NATIVE_GRAPHICS_END_HINT_PGI;
#endif
#ifdef GL_NEAREST_CLIPMAP_NEAREST_SGIX
		else if (!strcmp(name, "NEAREST_CLIPMAP_NEAREST_SGIX")) return GL_NEAREST_CLIPMAP_NEAREST_SGIX;
#endif
#ifdef GL_NORMAL_ARRAY_LIST_STRIDE_IBM
		else if (!strcmp(name, "NORMAL_ARRAY_LIST_STRIDE_IBM")) return GL_NORMAL_ARRAY_LIST_STRIDE_IBM;
#endif
#ifdef GL_OBJECT_DISTANCE_TO_LINE_SGIS
		else if (!strcmp(name, "OBJECT_DISTANCE_TO_LINE_SGIS")) return GL_OBJECT_DISTANCE_TO_LINE_SGIS;
#endif
#ifdef GL_ONE_MINUS_CONSTANT_ALPHA_EXT
		else if (!strcmp(name, "ONE_MINUS_CONSTANT_ALPHA_EXT")) return GL_ONE_MINUS_CONSTANT_ALPHA_EXT;
#endif
#ifdef GL_ONE_MINUS_CONSTANT_COLOR_EXT
		else if (!strcmp(name, "ONE_MINUS_CONSTANT_COLOR_EXT")) return GL_ONE_MINUS_CONSTANT_COLOR_EXT;
#endif
#ifdef GL_POST_COLOR_MATRIX_ALPHA_BIAS
		else if (!strcmp(name, "POST_COLOR_MATRIX_ALPHA_BIAS")) return GL_POST_COLOR_MATRIX_ALPHA_BIAS;
#endif
#ifdef GL_POST_COLOR_MATRIX_BLUE_SCALE
		else if (!strcmp(name, "POST_COLOR_MATRIX_BLUE_SCALE")) return GL_POST_COLOR_MATRIX_BLUE_SCALE;
#endif
#ifdef GL_POST_COLOR_MATRIX_GREEN_BIAS
		else if (!strcmp(name, "POST_COLOR_MATRIX_GREEN_BIAS")) return GL_POST_COLOR_MATRIX_GREEN_BIAS;
#endif
#ifdef GL_POST_CONVOLUTION_ALPHA_SCALE
		else if (!strcmp(name, "POST_CONVOLUTION_ALPHA_SCALE")) return GL_POST_CONVOLUTION_ALPHA_SCALE;
#endif
#ifdef GL_POST_CONVOLUTION_COLOR_TABLE
		else if (!strcmp(name, "POST_CONVOLUTION_COLOR_TABLE")) return GL_POST_CONVOLUTION_COLOR_TABLE;
#endif
#ifdef GL_POST_CONVOLUTION_GREEN_SCALE
		else if (!strcmp(name, "POST_CONVOLUTION_GREEN_SCALE")) return GL_POST_CONVOLUTION_GREEN_SCALE;
#endif
#ifdef GL_PREFER_DOUBLEBUFFER_HINT_PGI
		else if (!strcmp(name, "PREFER_DOUBLEBUFFER_HINT_PGI")) return GL_PREFER_DOUBLEBUFFER_HINT_PGI;
#endif
#ifdef GL_SAMPLE_ALPHA_TO_COVERAGE_ARB
		else if (!strcmp(name, "SAMPLE_ALPHA_TO_COVERAGE_ARB")) return GL_SAMPLE_ALPHA_TO_COVERAGE_ARB;
#endif
#ifdef GL_SUN_convolution_border_modes
		else if (!strcmp(name, "SUN_CONVOLUTION_BORDER_MODES")) return GL_SUN_convolution_border_modes;
#endif
#ifdef GL_TEXTURE_APPLICATION_MODE_EXT
		else if (!strcmp(name, "TEXTURE_APPLICATION_MODE_EXT")) return GL_TEXTURE_APPLICATION_MODE_EXT;
#endif
#ifdef GL_TEXTURE_BINDING_CUBE_MAP_ARB
		else if (!strcmp(name, "TEXTURE_BINDING_CUBE_MAP_ARB")) return GL_TEXTURE_BINDING_CUBE_MAP_ARB;
#endif
#ifdef GL_TEXTURE_BINDING_CUBE_MAP_EXT
		else if (!strcmp(name, "TEXTURE_BINDING_CUBE_MAP_EXT")) return GL_TEXTURE_BINDING_CUBE_MAP_EXT;
#endif
#ifdef GL_TEXTURE_COLOR_WRITEMASK_SGIS
		else if (!strcmp(name, "TEXTURE_COLOR_WRITEMASK_SGIS")) return GL_TEXTURE_COLOR_WRITEMASK_SGIS;
#endif
#ifdef GL_TEXTURE_COMPRESSION_HINT_ARB
		else if (!strcmp(name, "TEXTURE_COMPRESSION_HINT_ARB")) return GL_TEXTURE_COMPRESSION_HINT_ARB;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_LIST_IBM
		else if (!strcmp(name, "TEXTURE_COORD_ARRAY_LIST_IBM")) return GL_TEXTURE_COORD_ARRAY_LIST_IBM;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_SIZE_EXT
		else if (!strcmp(name, "TEXTURE_COORD_ARRAY_SIZE_EXT")) return GL_TEXTURE_COORD_ARRAY_SIZE_EXT;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_TYPE_EXT
		else if (!strcmp(name, "TEXTURE_COORD_ARRAY_TYPE_EXT")) return GL_TEXTURE_COORD_ARRAY_TYPE_EXT;
#endif
#ifdef GL_TEXTURE_DEFORMATION_BIT_SGIX
		else if (!strcmp(name, "TEXTURE_DEFORMATION_BIT_SGIX")) return GL_TEXTURE_DEFORMATION_BIT_SGIX;
#endif
#ifdef GL_TRANSPOSE_TEXTURE_MATRIX_ARB
		else if (!strcmp(name, "TRANSPOSE_TEXTURE_MATRIX_ARB")) return GL_TRANSPOSE_TEXTURE_MATRIX_ARB;
#endif
#ifdef GL_VERTEX_ARRAY_LIST_STRIDE_IBM
		else if (!strcmp(name, "VERTEX_ARRAY_LIST_STRIDE_IBM")) return GL_VERTEX_ARRAY_LIST_STRIDE_IBM;
#endif
#ifdef GL_VERTEX_ARRAY_RANGE_LENGTH_NV
		else if (!strcmp(name, "VERTEX_ARRAY_RANGE_LENGTH_NV")) return GL_VERTEX_ARRAY_RANGE_LENGTH_NV;
#endif
#ifdef GL_VERTEX_WEIGHT_ARRAY_SIZE_EXT
		else if (!strcmp(name, "VERTEX_WEIGHT_ARRAY_SIZE_EXT")) return GL_VERTEX_WEIGHT_ARRAY_SIZE_EXT;
#endif
#ifdef GL_VERTEX_WEIGHT_ARRAY_TYPE_EXT
		else if (!strcmp(name, "VERTEX_WEIGHT_ARRAY_TYPE_EXT")) return GL_VERTEX_WEIGHT_ARRAY_TYPE_EXT;
#endif
		else return _GL_UNDEFINED;
	case 32:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_3DFX_texture_compression_FXT1
		else if (!strcmp(name, "3DFX_TEXTURE_COMPRESSION_FXT1")) return GL_3DFX_texture_compression_FXT1;
#endif
#ifdef GL_CLIP_VOLUME_CLIPPING_HINT_EXT
		else if (!strcmp(name, "CLIP_VOLUME_CLIPPING_HINT_EXT")) return GL_CLIP_VOLUME_CLIPPING_HINT_EXT;
#endif
#ifdef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
		else if (!strcmp(name, "COMPRESSED_RGBA_S3TC_DXT1_EXT")) return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
#endif
#ifdef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
		else if (!strcmp(name, "COMPRESSED_RGBA_S3TC_DXT3_EXT")) return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
#endif
#ifdef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
		else if (!strcmp(name, "COMPRESSED_RGBA_S3TC_DXT5_EXT")) return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
#endif
#ifdef GL_CURRENT_RASTER_POSITION_VALID
		else if (!strcmp(name, "CURRENT_RASTER_POSITION_VALID")) return GL_CURRENT_RASTER_POSITION_VALID;
#endif
#ifdef GL_CURRENT_RASTER_TEXTURE_COORDS
		else if (!strcmp(name, "CURRENT_RASTER_TEXTURE_COORDS")) return GL_CURRENT_RASTER_TEXTURE_COORDS;
#endif
#ifdef GL_FOG_COORDINATE_ARRAY_LIST_IBM
		else if (!strcmp(name, "FOG_COORDINATE_ARRAY_LIST_IBM")) return GL_FOG_COORDINATE_ARRAY_LIST_IBM;
#endif
#ifdef GL_FOG_COORDINATE_ARRAY_TYPE_EXT
		else if (!strcmp(name, "FOG_COORDINATE_ARRAY_TYPE_EXT")) return GL_FOG_COORDINATE_ARRAY_TYPE_EXT;
#endif
#ifdef GL_GEOMETRY_DEFORMATION_BIT_SGIX
		else if (!strcmp(name, "GEOMETRY_DEFORMATION_BIT_SGIX")) return GL_GEOMETRY_DEFORMATION_BIT_SGIX;
#endif
#ifdef GL_LIGHT_MODEL_COLOR_CONTROL_EXT
		else if (!strcmp(name, "LIGHT_MODEL_COLOR_CONTROL_EXT")) return GL_LIGHT_MODEL_COLOR_CONTROL_EXT;
#endif
#ifdef GL_MAX_CLIENT_ATTRIB_STACK_DEPTH
		else if (!strcmp(name, "MAX_CLIENT_ATTRIB_STACK_DEPTH")) return GL_MAX_CLIENT_ATTRIB_STACK_DEPTH;
#endif
#ifdef GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB
		else if (!strcmp(name, "MAX_CUBE_MAP_TEXTURE_SIZE_ARB")) return GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB;
#endif
#ifdef GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT
		else if (!strcmp(name, "MAX_CUBE_MAP_TEXTURE_SIZE_EXT")) return GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT;
#endif
#ifdef GL_OBJECT_DISTANCE_TO_POINT_SGIS
		else if (!strcmp(name, "OBJECT_DISTANCE_TO_POINT_SGIS")) return GL_OBJECT_DISTANCE_TO_POINT_SGIS;
#endif
#ifdef GL_PIXEL_TRANSFORM_2D_MATRIX_EXT
		else if (!strcmp(name, "PIXEL_TRANSFORM_2D_MATRIX_EXT")) return GL_PIXEL_TRANSFORM_2D_MATRIX_EXT;
#endif
#ifdef GL_POINT_FADE_THRESHOLD_SIZE_EXT
		else if (!strcmp(name, "POINT_FADE_THRESHOLD_SIZE_EXT")) return GL_POINT_FADE_THRESHOLD_SIZE_EXT;
#endif
#ifdef GL_POST_COLOR_MATRIX_ALPHA_SCALE
		else if (!strcmp(name, "POST_COLOR_MATRIX_ALPHA_SCALE")) return GL_POST_COLOR_MATRIX_ALPHA_SCALE;
#endif
#ifdef GL_POST_COLOR_MATRIX_COLOR_TABLE
		else if (!strcmp(name, "POST_COLOR_MATRIX_COLOR_TABLE")) return GL_POST_COLOR_MATRIX_COLOR_TABLE;
#endif
#ifdef GL_POST_COLOR_MATRIX_GREEN_SCALE
		else if (!strcmp(name, "POST_COLOR_MATRIX_GREEN_SCALE")) return GL_POST_COLOR_MATRIX_GREEN_SCALE;
#endif
#ifdef GL_POST_CONVOLUTION_RED_BIAS_EXT
		else if (!strcmp(name, "POST_CONVOLUTION_RED_BIAS_EXT")) return GL_POST_CONVOLUTION_RED_BIAS_EXT;
#endif
#ifdef GL_POST_TEXTURE_FILTER_BIAS_SGIX
		else if (!strcmp(name, "POST_TEXTURE_FILTER_BIAS_SGIX")) return GL_POST_TEXTURE_FILTER_BIAS_SGIX;
#endif
#ifdef GL_PROXY_TEXTURE_COLOR_TABLE_SGI
		else if (!strcmp(name, "PROXY_TEXTURE_COLOR_TABLE_SGI")) return GL_PROXY_TEXTURE_COLOR_TABLE_SGI;
#endif
#ifdef GL_RASTER_POSITION_UNCLIPPED_IBM
		else if (!strcmp(name, "RASTER_POSITION_UNCLIPPED_IBM")) return GL_RASTER_POSITION_UNCLIPPED_IBM;
#endif
#ifdef GL_REFERENCE_PLANE_EQUATION_SGIX
		else if (!strcmp(name, "REFERENCE_PLANE_EQUATION_SGIX")) return GL_REFERENCE_PLANE_EQUATION_SGIX;
#endif
#ifdef GL_SMOOTH_LINE_WIDTH_GRANULARITY
		else if (!strcmp(name, "SMOOTH_LINE_WIDTH_GRANULARITY")) return GL_SMOOTH_LINE_WIDTH_GRANULARITY;
#endif
#ifdef GL_SMOOTH_POINT_SIZE_GRANULARITY
		else if (!strcmp(name, "SMOOTH_POINT_SIZE_GRANULARITY")) return GL_SMOOTH_POINT_SIZE_GRANULARITY;
#endif
#ifdef GL_TEXTURE_COMPARE_OPERATOR_SGIX
		else if (!strcmp(name, "TEXTURE_COMPARE_OPERATOR_SGIX")) return GL_TEXTURE_COMPARE_OPERATOR_SGIX;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_COUNT_EXT
		else if (!strcmp(name, "TEXTURE_COORD_ARRAY_COUNT_EXT")) return GL_TEXTURE_COORD_ARRAY_COUNT_EXT;
#endif
#ifdef GL_VERTEX_ARRAY_RANGE_POINTER_NV
		else if (!strcmp(name, "VERTEX_ARRAY_RANGE_POINTER_NV")) return GL_VERTEX_ARRAY_RANGE_POINTER_NV;
#endif
		else return _GL_UNDEFINED;
	case 33:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_COLOR_TABLE_INTENSITY_SIZE_EXT
		else if (!strcmp(name, "COLOR_TABLE_INTENSITY_SIZE_EXT")) return GL_COLOR_TABLE_INTENSITY_SIZE_EXT;
#endif
#ifdef GL_COLOR_TABLE_INTENSITY_SIZE_SGI
		else if (!strcmp(name, "COLOR_TABLE_INTENSITY_SIZE_SGI")) return GL_COLOR_TABLE_INTENSITY_SIZE_SGI;
#endif
#ifdef GL_COLOR_TABLE_LUMINANCE_SIZE_EXT
		else if (!strcmp(name, "COLOR_TABLE_LUMINANCE_SIZE_EXT")) return GL_COLOR_TABLE_LUMINANCE_SIZE_EXT;
#endif
#ifdef GL_COLOR_TABLE_LUMINANCE_SIZE_SGI
		else if (!strcmp(name, "COLOR_TABLE_LUMINANCE_SIZE_SGI")) return GL_COLOR_TABLE_LUMINANCE_SIZE_SGI;
#endif
#ifdef GL_COMPRESSED_LUMINANCE_ALPHA_ARB
		else if (!strcmp(name, "COMPRESSED_LUMINANCE_ALPHA_ARB")) return GL_COMPRESSED_LUMINANCE_ALPHA_ARB;
#endif
#ifdef GL_COMPRESSED_TEXTURE_FORMATS_ARB
		else if (!strcmp(name, "COMPRESSED_TEXTURE_FORMATS_ARB")) return GL_COMPRESSED_TEXTURE_FORMATS_ARB;
#endif
#ifdef GL_DEPTH_PASS_INSTRUMENT_MAX_SGIX
		else if (!strcmp(name, "DEPTH_PASS_INSTRUMENT_MAX_SGIX")) return GL_DEPTH_PASS_INSTRUMENT_MAX_SGIX;
#endif
#ifdef GL_DETAIL_TEXTURE_2D_BINDING_SGIS
		else if (!strcmp(name, "DETAIL_TEXTURE_2D_BINDING_SGIS")) return GL_DETAIL_TEXTURE_2D_BINDING_SGIS;
#endif
#ifdef GL_EXT_texture_filter_anisotropic
		else if (!strcmp(name, "EXT_TEXTURE_FILTER_ANISOTROPIC")) return GL_EXT_texture_filter_anisotropic;
#endif
#ifdef GL_INSTRUMENT_BUFFER_POINTER_SGIX
		else if (!strcmp(name, "INSTRUMENT_BUFFER_POINTER_SGIX")) return GL_INSTRUMENT_BUFFER_POINTER_SGIX;
#endif
#ifdef GL_MAX_CLIPMAP_VIRTUAL_DEPTH_SGIX
		else if (!strcmp(name, "MAX_CLIPMAP_VIRTUAL_DEPTH_SGIX")) return GL_MAX_CLIPMAP_VIRTUAL_DEPTH_SGIX;
#endif
#ifdef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
		else if (!strcmp(name, "MAX_TEXTURE_MAX_ANISOTROPY_EXT")) return GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT;
#endif
#ifdef GL_NATIVE_GRAPHICS_BEGIN_HINT_PGI
		else if (!strcmp(name, "NATIVE_GRAPHICS_BEGIN_HINT_PGI")) return GL_NATIVE_GRAPHICS_BEGIN_HINT_PGI;
#endif
#ifdef GL_PIXEL_FRAGMENT_RGB_SOURCE_SGIS
		else if (!strcmp(name, "PIXEL_FRAGMENT_RGB_SOURCE_SGIS")) return GL_PIXEL_FRAGMENT_RGB_SOURCE_SGIS;
#endif
#ifdef GL_PIXEL_TILE_BEST_ALIGNMENT_SGIX
		else if (!strcmp(name, "PIXEL_TILE_BEST_ALIGNMENT_SGIX")) return GL_PIXEL_TILE_BEST_ALIGNMENT_SGIX;
#endif
#ifdef GL_POINT_FADE_THRESHOLD_SIZE_SGIS
		else if (!strcmp(name, "POINT_FADE_THRESHOLD_SIZE_SGIS")) return GL_POINT_FADE_THRESHOLD_SIZE_SGIS;
#endif
#ifdef GL_POST_COLOR_MATRIX_RED_BIAS_SGI
		else if (!strcmp(name, "POST_COLOR_MATRIX_RED_BIAS_SGI")) return GL_POST_COLOR_MATRIX_RED_BIAS_SGI;
#endif
#ifdef GL_POST_CONVOLUTION_BLUE_BIAS_EXT
		else if (!strcmp(name, "POST_CONVOLUTION_BLUE_BIAS_EXT")) return GL_POST_CONVOLUTION_BLUE_BIAS_EXT;
#endif
#ifdef GL_POST_CONVOLUTION_RED_SCALE_EXT
		else if (!strcmp(name, "POST_CONVOLUTION_RED_SCALE_EXT")) return GL_POST_CONVOLUTION_RED_SCALE_EXT;
#endif
#ifdef GL_POST_TEXTURE_FILTER_SCALE_SGIX
		else if (!strcmp(name, "POST_TEXTURE_FILTER_SCALE_SGIX")) return GL_POST_TEXTURE_FILTER_SCALE_SGIX;
#endif
#ifdef GL_SECONDARY_COLOR_ARRAY_LIST_IBM
		else if (!strcmp(name, "SECONDARY_COLOR_ARRAY_LIST_IBM")) return GL_SECONDARY_COLOR_ARRAY_LIST_IBM;
#endif
#ifdef GL_SECONDARY_COLOR_ARRAY_SIZE_EXT
		else if (!strcmp(name, "SECONDARY_COLOR_ARRAY_SIZE_EXT")) return GL_SECONDARY_COLOR_ARRAY_SIZE_EXT;
#endif
#ifdef GL_SECONDARY_COLOR_ARRAY_TYPE_EXT
		else if (!strcmp(name, "SECONDARY_COLOR_ARRAY_TYPE_EXT")) return GL_SECONDARY_COLOR_ARRAY_TYPE_EXT;
#endif
#ifdef GL_SPARE0_PLUS_SECONDARY_COLOR_NV
		else if (!strcmp(name, "SPARE0_PLUS_SECONDARY_COLOR_NV")) return GL_SPARE0_PLUS_SECONDARY_COLOR_NV;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_STRIDE_EXT
		else if (!strcmp(name, "TEXTURE_COORD_ARRAY_STRIDE_EXT")) return GL_TEXTURE_COORD_ARRAY_STRIDE_EXT;
#endif
#ifdef GL_TEXTURE_MATERIAL_PARAMETER_EXT
		else if (!strcmp(name, "TEXTURE_MATERIAL_PARAMETER_EXT")) return GL_TEXTURE_MATERIAL_PARAMETER_EXT;
#endif
#ifdef GL_TEXTURE_MULTI_BUFFER_HINT_SGIX
		else if (!strcmp(name, "TEXTURE_MULTI_BUFFER_HINT_SGIX")) return GL_TEXTURE_MULTI_BUFFER_HINT_SGIX;
#endif
#ifdef GL_TRANSPOSE_MODELVIEW_MATRIX_ARB
		else if (!strcmp(name, "TRANSPOSE_MODELVIEW_MATRIX_ARB")) return GL_TRANSPOSE_MODELVIEW_MATRIX_ARB;
#endif
#ifdef GL_VERTEX_WEIGHT_ARRAY_STRIDE_EXT
		else if (!strcmp(name, "VERTEX_WEIGHT_ARRAY_STRIDE_EXT")) return GL_VERTEX_WEIGHT_ARRAY_STRIDE_EXT;
#endif
		else return _GL_UNDEFINED;
	case 34:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_CULL_VERTEX_OBJECT_POSITION_EXT
		else if (!strcmp(name, "CULL_VERTEX_OBJECT_POSITION_EXT")) return GL_CULL_VERTEX_OBJECT_POSITION_EXT;
#endif
#ifdef GL_DETAIL_TEXTURE_FUNC_POINTS_SGIS
		else if (!strcmp(name, "DETAIL_TEXTURE_FUNC_POINTS_SGIS")) return GL_DETAIL_TEXTURE_FUNC_POINTS_SGIS;
#endif
#ifdef GL_EDGE_FLAG_ARRAY_LIST_STRIDE_IBM
		else if (!strcmp(name, "EDGE_FLAG_ARRAY_LIST_STRIDE_IBM")) return GL_EDGE_FLAG_ARRAY_LIST_STRIDE_IBM;
#endif
#ifdef GL_EXT_pixel_transform_color_table
		else if (!strcmp(name, "EXT_PIXEL_TRANSFORM_COLOR_TABLE")) return GL_EXT_pixel_transform_color_table;
#endif
#ifdef GL_FOG_COORDINATE_ARRAY_STRIDE_EXT
		else if (!strcmp(name, "FOG_COORDINATE_ARRAY_STRIDE_EXT")) return GL_FOG_COORDINATE_ARRAY_STRIDE_EXT;
#endif
#ifdef GL_MAT_AMBIENT_AND_DIFFUSE_BIT_PGI
		else if (!strcmp(name, "MAT_AMBIENT_AND_DIFFUSE_BIT_PGI")) return GL_MAT_AMBIENT_AND_DIFFUSE_BIT_PGI;
#endif
#ifdef GL_PIXEL_TILE_CACHE_INCREMENT_SGIX
		else if (!strcmp(name, "PIXEL_TILE_CACHE_INCREMENT_SGIX")) return GL_PIXEL_TILE_CACHE_INCREMENT_SGIX;
#endif
#ifdef GL_POST_COLOR_MATRIX_BLUE_BIAS_SGI
		else if (!strcmp(name, "POST_COLOR_MATRIX_BLUE_BIAS_SGI")) return GL_POST_COLOR_MATRIX_BLUE_BIAS_SGI;
#endif
#ifdef GL_POST_COLOR_MATRIX_RED_SCALE_SGI
		else if (!strcmp(name, "POST_COLOR_MATRIX_RED_SCALE_SGI")) return GL_POST_COLOR_MATRIX_RED_SCALE_SGI;
#endif
#ifdef GL_POST_CONVOLUTION_ALPHA_BIAS_EXT
		else if (!strcmp(name, "POST_CONVOLUTION_ALPHA_BIAS_EXT")) return GL_POST_CONVOLUTION_ALPHA_BIAS_EXT;
#endif
#ifdef GL_POST_CONVOLUTION_BLUE_SCALE_EXT
		else if (!strcmp(name, "POST_CONVOLUTION_BLUE_SCALE_EXT")) return GL_POST_CONVOLUTION_BLUE_SCALE_EXT;
#endif
#ifdef GL_POST_CONVOLUTION_GREEN_BIAS_EXT
		else if (!strcmp(name, "POST_CONVOLUTION_GREEN_BIAS_EXT")) return GL_POST_CONVOLUTION_GREEN_BIAS_EXT;
#endif
#ifdef GL_REPLACEMENT_CODE_ARRAY_TYPE_SUN
		else if (!strcmp(name, "REPLACEMENT_CODE_ARRAY_TYPE_SUN")) return GL_REPLACEMENT_CODE_ARRAY_TYPE_SUN;
#endif
#ifdef GL_TEXTURE_CLIPMAP_LOD_OFFSET_SGIX
		else if (!strcmp(name, "TEXTURE_CLIPMAP_LOD_OFFSET_SGIX")) return GL_TEXTURE_CLIPMAP_LOD_OFFSET_SGIX;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_POINTER_EXT
		else if (!strcmp(name, "TEXTURE_COORD_ARRAY_POINTER_EXT")) return GL_TEXTURE_COORD_ARRAY_POINTER_EXT;
#endif
#ifdef GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB
		else if (!strcmp(name, "TEXTURE_CUBE_MAP_NEGATIVE_X_ARB")) return GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB;
#endif
#ifdef GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT
		else if (!strcmp(name, "TEXTURE_CUBE_MAP_NEGATIVE_X_EXT")) return GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT;
#endif
#ifdef GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB
		else if (!strcmp(name, "TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB")) return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB;
#endif
#ifdef GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT
		else if (!strcmp(name, "TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT")) return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT;
#endif
#ifdef GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB
		else if (!strcmp(name, "TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB")) return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB;
#endif
#ifdef GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT
		else if (!strcmp(name, "TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT")) return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT;
#endif
#ifdef GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB
		else if (!strcmp(name, "TEXTURE_CUBE_MAP_POSITIVE_X_ARB")) return GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB;
#endif
#ifdef GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT
		else if (!strcmp(name, "TEXTURE_CUBE_MAP_POSITIVE_X_EXT")) return GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT;
#endif
#ifdef GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB
		else if (!strcmp(name, "TEXTURE_CUBE_MAP_POSITIVE_Y_ARB")) return GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB;
#endif
#ifdef GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT
		else if (!strcmp(name, "TEXTURE_CUBE_MAP_POSITIVE_Y_EXT")) return GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT;
#endif
#ifdef GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB
		else if (!strcmp(name, "TEXTURE_CUBE_MAP_POSITIVE_Z_ARB")) return GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB;
#endif
#ifdef GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT
		else if (!strcmp(name, "TEXTURE_CUBE_MAP_POSITIVE_Z_EXT")) return GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT;
#endif
#ifdef GL_TRANSPOSE_PROJECTION_MATRIX_ARB
		else if (!strcmp(name, "TRANSPOSE_PROJECTION_MATRIX_ARB")) return GL_TRANSPOSE_PROJECTION_MATRIX_ARB;
#endif
#ifdef GL_VERTEX_WEIGHT_ARRAY_POINTER_EXT
		else if (!strcmp(name, "VERTEX_WEIGHT_ARRAY_POINTER_EXT")) return GL_VERTEX_WEIGHT_ARRAY_POINTER_EXT;
#endif
		else return _GL_UNDEFINED;
	case 35:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_FOG_COORDINATE_ARRAY_POINTER_EXT
		else if (!strcmp(name, "FOG_COORDINATE_ARRAY_POINTER_EXT")) return GL_FOG_COORDINATE_ARRAY_POINTER_EXT;
#endif
#ifdef GL_MAX_COLOR_MATRIX_STACK_DEPTH_SGI
		else if (!strcmp(name, "MAX_COLOR_MATRIX_STACK_DEPTH_SGI")) return GL_MAX_COLOR_MATRIX_STACK_DEPTH_SGI;
#endif
#ifdef GL_PIXEL_FRAGMENT_ALPHA_SOURCE_SGIS
		else if (!strcmp(name, "PIXEL_FRAGMENT_ALPHA_SOURCE_SGIS")) return GL_PIXEL_FRAGMENT_ALPHA_SOURCE_SGIS;
#endif
#ifdef GL_POST_COLOR_MATRIX_ALPHA_BIAS_SGI
		else if (!strcmp(name, "POST_COLOR_MATRIX_ALPHA_BIAS_SGI")) return GL_POST_COLOR_MATRIX_ALPHA_BIAS_SGI;
#endif
#ifdef GL_POST_COLOR_MATRIX_BLUE_SCALE_SGI
		else if (!strcmp(name, "POST_COLOR_MATRIX_BLUE_SCALE_SGI")) return GL_POST_COLOR_MATRIX_BLUE_SCALE_SGI;
#endif
#ifdef GL_POST_COLOR_MATRIX_GREEN_BIAS_SGI
		else if (!strcmp(name, "POST_COLOR_MATRIX_GREEN_BIAS_SGI")) return GL_POST_COLOR_MATRIX_GREEN_BIAS_SGI;
#endif
#ifdef GL_POST_CONVOLUTION_ALPHA_SCALE_EXT
		else if (!strcmp(name, "POST_CONVOLUTION_ALPHA_SCALE_EXT")) return GL_POST_CONVOLUTION_ALPHA_SCALE_EXT;
#endif
#ifdef GL_POST_CONVOLUTION_COLOR_TABLE_SGI
		else if (!strcmp(name, "POST_CONVOLUTION_COLOR_TABLE_SGI")) return GL_POST_CONVOLUTION_COLOR_TABLE_SGI;
#endif
#ifdef GL_POST_CONVOLUTION_GREEN_SCALE_EXT
		else if (!strcmp(name, "POST_CONVOLUTION_GREEN_SCALE_EXT")) return GL_POST_CONVOLUTION_GREEN_SCALE_EXT;
#endif
#ifdef GL_SECONDARY_COLOR_ARRAY_STRIDE_EXT
		else if (!strcmp(name, "SECONDARY_COLOR_ARRAY_STRIDE_EXT")) return GL_SECONDARY_COLOR_ARRAY_STRIDE_EXT;
#endif
#ifdef GL_SHARPEN_TEXTURE_FUNC_POINTS_SGIS
		else if (!strcmp(name, "SHARPEN_TEXTURE_FUNC_POINTS_SGIS")) return GL_SHARPEN_TEXTURE_FUNC_POINTS_SGIS;
#endif
		else return _GL_UNDEFINED;
	case 36:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_FRAGMENT_COLOR_MATERIAL_FACE_SGIX
		else if (!strcmp(name, "FRAGMENT_COLOR_MATERIAL_FACE_SGIX")) return GL_FRAGMENT_COLOR_MATERIAL_FACE_SGIX;
#endif
#ifdef GL_FRAGMENT_LIGHT_MODEL_AMBIENT_SGIX
		else if (!strcmp(name, "FRAGMENT_LIGHT_MODEL_AMBIENT_SGIX")) return GL_FRAGMENT_LIGHT_MODEL_AMBIENT_SGIX;
#endif
#ifdef GL_LIGHT_MODEL_SPECULAR_VECTOR_APPLE
		else if (!strcmp(name, "LIGHT_MODEL_SPECULAR_VECTOR_APPLE")) return GL_LIGHT_MODEL_SPECULAR_VECTOR_APPLE;
#endif
#ifdef GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV
		else if (!strcmp(name, "MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV")) return GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV;
#endif
#ifdef GL_POST_COLOR_MATRIX_ALPHA_SCALE_SGI
		else if (!strcmp(name, "POST_COLOR_MATRIX_ALPHA_SCALE_SGI")) return GL_POST_COLOR_MATRIX_ALPHA_SCALE_SGI;
#endif
#ifdef GL_POST_COLOR_MATRIX_COLOR_TABLE_SGI
		else if (!strcmp(name, "POST_COLOR_MATRIX_COLOR_TABLE_SGI")) return GL_POST_COLOR_MATRIX_COLOR_TABLE_SGI;
#endif
#ifdef GL_POST_COLOR_MATRIX_GREEN_SCALE_SGI
		else if (!strcmp(name, "POST_COLOR_MATRIX_GREEN_SCALE_SGI")) return GL_POST_COLOR_MATRIX_GREEN_SCALE_SGI;
#endif
#ifdef GL_REPLACEMENT_CODE_ARRAY_STRIDE_SUN
		else if (!strcmp(name, "REPLACEMENT_CODE_ARRAY_STRIDE_SUN")) return GL_REPLACEMENT_CODE_ARRAY_STRIDE_SUN;
#endif
#ifdef GL_SECONDARY_COLOR_ARRAY_POINTER_EXT
		else if (!strcmp(name, "SECONDARY_COLOR_ARRAY_POINTER_EXT")) return GL_SECONDARY_COLOR_ARRAY_POINTER_EXT;
#endif
#ifdef GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB
		else if (!strcmp(name, "TEXTURE_COMPRESSED_IMAGE_SIZE_ARB")) return GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB;
#endif
		else return _GL_UNDEFINED;
	case 37:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_SGIX
		else if (!strcmp(name, "FRAGMENT_LIGHT_MODEL_TWO_SIDE_SGIX")) return GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_SGIX;
#endif
#ifdef GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB
		else if (!strcmp(name, "NUM_COMPRESSED_TEXTURE_FORMATS_ARB")) return GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB;
#endif
#ifdef GL_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT
		else if (!strcmp(name, "PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT")) return GL_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT;
#endif
#ifdef GL_PROXY_POST_CONVOLUTION_COLOR_TABLE
		else if (!strcmp(name, "PROXY_POST_CONVOLUTION_COLOR_TABLE")) return GL_PROXY_POST_CONVOLUTION_COLOR_TABLE;
#endif
#ifdef GL_REPLACEMENT_CODE_ARRAY_POINTER_SUN
		else if (!strcmp(name, "REPLACEMENT_CODE_ARRAY_POINTER_SUN")) return GL_REPLACEMENT_CODE_ARRAY_POINTER_SUN;
#endif
#ifdef GL_TEXTURE_CLIPMAP_VIRTUAL_DEPTH_SGIX
		else if (!strcmp(name, "TEXTURE_CLIPMAP_VIRTUAL_DEPTH_SGIX")) return GL_TEXTURE_CLIPMAP_VIRTUAL_DEPTH_SGIX;
#endif
		else return _GL_UNDEFINED;
	case 38:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_COLOR_ARRAY_PARALLEL_POINTERS_INTEL
		else if (!strcmp(name, "COLOR_ARRAY_PARALLEL_POINTERS_INTEL")) return GL_COLOR_ARRAY_PARALLEL_POINTERS_INTEL;
#endif
#ifdef GL_DEPTH_PASS_INSTRUMENT_COUNTERS_SGIX
		else if (!strcmp(name, "DEPTH_PASS_INSTRUMENT_COUNTERS_SGIX")) return GL_DEPTH_PASS_INSTRUMENT_COUNTERS_SGIX;
#endif
#ifdef GL_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP
		else if (!strcmp(name, "POST_IMAGE_TRANSFORM_COLOR_TABLE_HP")) return GL_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP;
#endif
#ifdef GL_POST_TEXTURE_FILTER_BIAS_RANGE_SGIX
		else if (!strcmp(name, "POST_TEXTURE_FILTER_BIAS_RANGE_SGIX")) return GL_POST_TEXTURE_FILTER_BIAS_RANGE_SGIX;
#endif
#ifdef GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE
		else if (!strcmp(name, "PROXY_POST_COLOR_MATRIX_COLOR_TABLE")) return GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_LIST_STRIDE_IBM
		else if (!strcmp(name, "TEXTURE_COORD_ARRAY_LIST_STRIDE_IBM")) return GL_TEXTURE_COORD_ARRAY_LIST_STRIDE_IBM;
#endif
		else return _GL_UNDEFINED;
	case 39:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_FOG_COORDINATE_ARRAY_LIST_STRIDE_IBM
		else if (!strcmp(name, "FOG_COORDINATE_ARRAY_LIST_STRIDE_IBM")) return GL_FOG_COORDINATE_ARRAY_LIST_STRIDE_IBM;
#endif
#ifdef GL_NORMAL_ARRAY_PARALLEL_POINTERS_INTEL
		else if (!strcmp(name, "NORMAL_ARRAY_PARALLEL_POINTERS_INTEL")) return GL_NORMAL_ARRAY_PARALLEL_POINTERS_INTEL;
#endif
#ifdef GL_POST_TEXTURE_FILTER_SCALE_RANGE_SGIX
		else if (!strcmp(name, "POST_TEXTURE_FILTER_SCALE_RANGE_SGIX")) return GL_POST_TEXTURE_FILTER_SCALE_RANGE_SGIX;
#endif
#ifdef GL_VERTEX_ARRAY_PARALLEL_POINTERS_INTEL
		else if (!strcmp(name, "VERTEX_ARRAY_PARALLEL_POINTERS_INTEL")) return GL_VERTEX_ARRAY_PARALLEL_POINTERS_INTEL;
#endif
		else return _GL_UNDEFINED;
	case 40:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_SECONDARY_COLOR_ARRAY_LIST_STRIDE_IBM
		else if (!strcmp(name, "SECONDARY_COLOR_ARRAY_LIST_STRIDE_IBM")) return GL_SECONDARY_COLOR_ARRAY_LIST_STRIDE_IBM;
#endif
		else return _GL_UNDEFINED;
	case 41:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_SGIX
		else if (!strcmp(name, "FRAGMENT_COLOR_MATERIAL_PARAMETER_SGIX")) return GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_SGIX;
#endif
#ifdef GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_SGIX
		else if (!strcmp(name, "FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_SGIX")) return GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_SGIX;
#endif
#ifdef GL_MAX_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT
		else if (!strcmp(name, "MAX_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT")) return GL_MAX_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT;
#endif
#ifdef GL_PROXY_POST_CONVOLUTION_COLOR_TABLE_SGI
		else if (!strcmp(name, "PROXY_POST_CONVOLUTION_COLOR_TABLE_SGI")) return GL_PROXY_POST_CONVOLUTION_COLOR_TABLE_SGI;
#endif
		else return _GL_UNDEFINED;
	case 42:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI
		else if (!strcmp(name, "PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI")) return GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI;
#endif
		else return _GL_UNDEFINED;
	case 44:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_PROXY_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP
		else if (!strcmp(name, "PROXY_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP")) return GL_PROXY_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP;
#endif
		else return _GL_UNDEFINED;
	case 46:
		if (0)  return _GL_UNDEFINED;
#ifdef GL_TEXTURE_COORD_ARRAY_PARALLEL_POINTERS_INTEL
		else if (!strcmp(name, "TEXTURE_COORD_ARRAY_PARALLEL_POINTERS_INTEL")) return GL_TEXTURE_COORD_ARRAY_PARALLEL_POINTERS_INTEL;
#endif
		else return _GL_UNDEFINED;
    case 49:
      if (0)  return _GL_UNDEFINED;
#ifdef GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_SGIX
		else if (!strcmp(name, "FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_SGIX")) return GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_SGIX;
#endif
		else return _GL_UNDEFINED;
    default: return _GL_UNDEFINED;
    }

    return _GL_UNDEFINED;

}
