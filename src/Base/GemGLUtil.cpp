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

#define _GL_UNDEFINED -10

GEM_EXTERN int getGLdefine(char *name)
{
    char *c=name;
    int count=0;
    while (*c){*c++=toupper(*c);count++;}
    if (count<4)return _GL_UNDEFINED;
    if (!(name[0]=='G' && name[1]=='L' && name[2]=='_'))return _GL_UNDEFINED;

    switch (count){
    case 4:
      switch (name[3]){
#ifdef GL_Q
      case 'Q': return GL_Q;
#endif
#ifdef GL_R
      case 'R': return GL_R;
#endif
#ifdef GL_S
      case 'S': return GL_S;
#endif
#ifdef GL_T
      case 'T': return GL_T;
#endif
      default: return _GL_UNDEFINED;
      }
    case 5:
      switch (name[3]){
#ifdef GL_2D
      case '2': return GL_2D;
#endif
#ifdef GL_3D
      case '3': return GL_3D;
#endif
#ifdef GL_CW
      case 'C': return GL_CW;
#endif
#ifdef GL_OR
      case 'O': return GL_OR;
#endif
      default: return _GL_UNDEFINED;
      }
    case 6:
      switch (name[3]){
      case 'A':
	switch (name[4]){
#ifdef GL_ADD
	case 'D': return GL_ADD;
#endif
#ifdef GL_AND
	case 'N': return GL_AND;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_BGR
      case 'B': return GL_BGR;
#endif
#ifdef GL_CCW
      case 'C': return GL_CCW;
#endif
#ifdef GL_EXP
      case 'E': return GL_EXP;
#endif
#ifdef GL_FOG
      case 'F': return GL_FOG;
#endif
#ifdef GL_INT
      case 'I': return GL_INT;
#endif
      case 'M':
	switch (name[4]){
#ifdef GL_MAX
	case 'A': return GL_MAX;
#endif
#ifdef GL_MIN
	case 'I': return GL_MIN;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_NOR
      case 'N': return GL_NOR;
#endif
#ifdef GL_ONE
      case 'O': return GL_ONE;
#endif
      case 'R':
	switch (name[4]){
#ifdef GL_RED
	case 'E': return GL_RED;
#endif
#ifdef GL_RGB
	case 'G': return GL_RGB;
#endif
	default: return _GL_UNDEFINED;
	} 
#ifdef GL_SET
      case 'S': return GL_SET;
#endif
      case 'V':
	switch (name[4]){
#ifdef GL_V2F
	case '2': return GL_V2F;
#endif
#ifdef GL_V3F
	case '3': return GL_V3F;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_XOR
      case 'X': return GL_XOR;
#endif
      default: return _GL_UNDEFINED;
      }
    case 7:
      switch (name[3]){
      case 'A':
	switch (name[6]){
#ifdef GL_AUX0
	case '0': return GL_AUX0;
#endif
#ifdef GL_AUX1
	case '1': return GL_AUX1;
#endif
#ifdef GL_AUX2
	case '2': return GL_AUX2;
#endif
#ifdef GL_AUX3
	case '3': return GL_AUX3;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'B':
	switch (name[4]){
#ifdef GL_BACK
	case 'A': return GL_BACK;
#endif
#ifdef GL_BGRA
	case 'G': return GL_BGRA;
#endif
#ifdef GL_BLUE
	case 'L': return GL_BLUE;
#endif
#ifdef GL_BYTE
	case 'Y': return GL_BYTE;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_COPY
      case 'C': return GL_COPY;
#endif
#ifdef GL_DECR
      case 'D': return GL_DECR;
#endif
#ifdef GL_EXP2
      case 'E': return GL_EXP2;
#endif
      case 'F':
	switch (name[4]){
#ifdef GL_FILL
	case 'I': return GL_FILL;
#endif
#ifdef GL_FLAT
	case 'L': return GL_FLAT;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_INCR
      case 'I': return GL_INCR;
#endif
#ifdef GL_KEEP
      case 'K': return GL_KEEP;
#endif
      case 'L':
	switch (name[5]){
#ifdef GL_LEFT
	case 'F': return GL_LEFT;
#endif
#ifdef GL_LESS
	case 'S': return GL_LESS;
#endif
#ifdef GL_LINE
	case 'N': return GL_LINE;
#endif
#ifdef GL_LOAD
	case 'A': return GL_LOAD;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_MULT
      case 'M': return GL_MULT;
#endif
      case 'N':
	switch (name[4]){
#ifdef GL_NAND
	case 'A': return GL_NAND;
#endif
	case 'O':
	  switch (name[5]){
#ifdef GL_NONE
	  case 'N': return GL_NONE;
#endif
#ifdef GL_NOOP
	  case 'O': return GL_NOOP;
#endif
	  default: return _GL_UNDEFINED;
	  }
	default: return _GL_UNDEFINED;
	}
      case 'R':
	switch (name[6]){
#ifdef GL_RGB4
	case '4': return GL_RGB4;
#endif
#ifdef GL_RGB5
	case '5': return GL_RGB5;
#endif
#ifdef GL_RGB8
	case '8': return GL_RGB8;
#endif
#ifdef GL_RGBA
	case 'A': return GL_RGBA;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_TRUE
      case 'T': return GL_TRUE;
#endif
#ifdef GL_ZERO
      case 'Z': return GL_ZERO;
#endif
      default: return _GL_UNDEFINED;
      }
    case 8:
      switch (name[3]){
      case 'A':
	switch (name[4]){
#ifdef GL_ACCUM
	case 'C': return GL_ACCUM;
#endif
#ifdef GL_ALPHA
	case 'L': return GL_ALPHA;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_BLEND
      case 'B': return GL_BLEND;
#endif
      case 'C':
	switch (name[6]){
#ifdef GL_CLAMP
	case 'M': return GL_CLAMP;
#endif
#ifdef GL_CLEAR
	case 'A': return GL_CLEAR;
#endif
#ifdef GL_COEFF
	case 'F': return GL_COEFF;
#endif
#ifdef GL_COLOR
	case 'O': return GL_COLOR;
#endif
      default: return _GL_UNDEFINED;
      }
      case 'D':
	switch (name[5]){
#ifdef GL_DECAL
      case 'C': return GL_DECAL;
#endif
#ifdef GL_DEPTH
      case 'P': return GL_DEPTH;
#endif
      default: return _GL_UNDEFINED;
      }
      case 'E':
	switch (name[6]){
#ifdef GL_EQUAL
	case 'A': return GL_EQUAL;
#endif
#ifdef GL_EQUIV
	case 'I': return GL_EQUIV;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'F':
	switch (name[4]){
#ifdef GL_FALSE
	case 'A': return GL_FALSE;
#endif
#ifdef GL_FLOAT
	case 'L': return GL_FLOAT;
#endif
#ifdef GL_FRONT
	case 'R': return GL_FRONT;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_GREEN
      case 'G': return GL_GREEN;
#endif
#ifdef GL_LINES
      case 'L': return GL_LINES;
#endif
#ifdef GL_NEVER
      case 'N': return GL_NEVER;
#endif
#ifdef GL_ORDER
      case 'O': return GL_ORDER;
#endif
#ifdef GL_POINT
      case 'P': return GL_POINT;
#endif
#ifdef GL_QUADS
      case 'Q': return GL_QUADS;
#endif
      case 'R':
	switch (name[6]){
	case '1':
	  switch (name[7]){
#ifdef GL_RGB10
	  case '0': return GL_RGB10;
#endif
#ifdef GL_RGB12
	  case '2': return GL_RGB12;
#endif
#ifdef GL_RGB16
	  case '6': return GL_RGB16;
#endif
	  default: return _GL_UNDEFINED;
	  }
	case 'A':
	  switch (name[7]){
#ifdef GL_RGBA2
	  case '2': return GL_RGBA2;
#endif
#ifdef GL_RGBA4
	  case '4': return GL_RGBA4;
#endif
#ifdef GL_RGBA8
	  case '8': return GL_RGBA8;
#endif
	  default: return _GL_UNDEFINED;
	  }
#ifdef GL_RIGHT
	case 'H': return GL_RIGHT;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_SHORT
      case 'S': return GL_SHORT;
#endif
      default: return _GL_UNDEFINED;
      }
    case 9:
      switch (name[8]){
      case 'E': 
	switch (name[3]){
#ifdef GL_DOUBLE
	case 'D': return GL_DOUBLE;
#endif
#ifdef GL_REDUCE
	case 'R': return GL_REDUCE;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_SMOOTH
      case 'H': return GL_SMOOTH;
#endif
      case 'L': 
	switch (name[3]){
#ifdef GL_GEQUAL
	case 'G': return GL_GEQUAL;
#endif
#ifdef GL_LEQUAL
	case 'L': return GL_LEQUAL;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'N': 
	switch (name[3]){
#ifdef GL_DOMAIN
	case 'D': return GL_DOMAIN;
#endif
#ifdef GL_RETURN
	case 'R': return GL_RETURN;
#endif
 	default: return _GL_UNDEFINED;
	}
#ifdef GL_STEREO
      case 'O': return GL_STEREO;
#endif
#ifdef GL_BITMAP
      case 'P': return GL_BITMAP;
#endif
      case 'R': 
	switch (name[3]){
#ifdef GL_DITHER
	case 'D': return GL_DITHER;
#endif
#ifdef GL_LINEAR
	case 'L': return GL_LINEAR;
#endif
#ifdef GL_RENDER
	case 'R': return GL_RENDER;
#endif
#ifdef GL_VENDOR
	case 'V': return GL_VENDOR;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'S': 
	switch (name[3]){
#ifdef GL_ALWAYS
	case 'A': return GL_ALWAYS;
#endif
#ifdef GL_POINTS
	case 'P': return GL_POINTS;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'T': 
	switch (name[3]){
#ifdef GL_INVERT
	case 'I': return GL_INVERT;
#endif
#ifdef GL_NICEST
	case 'N': return GL_NICEST;
#endif
#ifdef GL_SELECT
	case 'S': return GL_SELECT;
#endif
#ifdef GL_REPEAT
	case 'R': return GL_REPEAT;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'X': 
	switch (name[3]){
#ifdef GL_MINMAX
	case 'M': return GL_MINMAX;
#endif
#ifdef GL_ZOOM_X
	case 'Z': return GL_ZOOM_X;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_ZOOM_Y
      case 'Y': return GL_ZOOM_Y;
#endif
#ifdef GL_LIGHT0
      case '0': return GL_LIGHT0;
#endif
#ifdef GL_LIGHT1
      case '1': return GL_LIGHT1;
#endif
      case '2':
	switch (name[3]){
#ifdef GL_LIGHT2
	case 'L': return GL_LIGHT2;
#endif
#ifdef GL_RGBA12
	case 'R': return GL_RGBA12;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_LIGHT3
      case '3': return GL_LIGHT3;
#endif
      case '4': 
	switch (name[3]){
#ifdef GL_LIGHT4
	case 'L': return GL_LIGHT4;
#endif
#ifdef GL_ALPHA4
	case 'A': return GL_ALPHA4;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_LIGHT5
      case '5': return GL_LIGHT5;
#endif
      case '6': 
	switch (name[3]){
#ifdef GL_LIGHT6
	case 'L': return GL_LIGHT6;
#endif
#ifdef GL_RGBA16
	case 'R': return GL_RGBA16;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_LIGHT7
      case '7': return GL_LIGHT7;
#endif
#ifdef GL_ALPHA8
      case '8': return GL_ALPHA8;
#endif
      default: return _GL_UNDEFINED;
      }
    case 10:
      switch (name[3]){
#ifdef GL_2_BYTES
      case '2': return GL_2_BYTES;
#endif
#ifdef GL_3_BYTES
      case '3': return GL_3_BYTES;
#endif
#ifdef GL_4_BYTES
      case '4': return GL_4_BYTES;
#endif
      case 'A': 
	switch (name[9]){
#ifdef GL_ALPHA12
	case '2': return GL_ALPHA12;
#endif
#ifdef GL_ALPHA16
	case '6': return GL_ALPHA16;
#endif
#ifdef GL_AMBIENT
	case 'T': return GL_AMBIENT;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_BGR_EXT
      case 'B': return GL_BGR_EXT;
#endif
      case 'C': 
	switch (name[4]){
#ifdef GL_C3F_V3F
	case '3': return GL_C3F_V3F;
#endif
#ifdef GL_COMPILE
	case 'O': return GL_COMPILE;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_DIFFUSE
      case 'D': return GL_DIFFUSE;
#endif
      case 'F': 
	switch (name[8]){
#ifdef GL_FASTEST
	case 'S': return GL_FASTEST;
#endif
#ifdef GL_FOG_BIT
	case 'I': return GL_FOG_BIT;
#endif
#ifdef GL_FOG_END
	case 'N': return GL_FOG_END;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_GREATER
      case 'G': return GL_GREATER;
#endif
      case 'M': 
	switch (name[4]){
#ifdef GL_MAX_EXT
	case 'A': return GL_MAX_EXT;
#endif
#ifdef GL_MIN_EXT
	case 'I': return GL_MIN_EXT;
#endif
	default: return _GL_UNDEFINED;
	}
       case 'N': 
	 switch (name[4]){
#ifdef GL_N3F_V3F
	 case '3': return GL_N3F_V3F;
#endif
#ifdef GL_NEAREST
	 case 'E': return GL_NEAREST;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_POLYGON
       case 'P': return GL_POLYGON;
#endif
      case 'R': 
	switch (name[4]){
#ifdef GL_REPLACE
	case 'E': return GL_REPLACE;
#endif
#ifdef GL_RGB5_A1
	case 'G': return GL_RGB5_A1;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_STENCIL
      case 'S': return GL_STENCIL;
#endif
      case 'T': 
	switch (name[4]){
#ifdef GL_T2F_V3F
	case '2': return GL_T2F_V3F;
#endif
#ifdef GL_T4F_V4F
	case '4': return GL_T4F_V4F;
#endif
#ifdef GL_TEXTURE
	case 'E': return GL_TEXTURE;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_VERSION
      case 'V': return GL_VERSION;
#endif
      default: return _GL_UNDEFINED;
      }
    case 11:
      switch (name[3]){
#ifdef GL_3D_COLOR
      case '3': return GL_3D_COLOR;
#endif
#ifdef GL_ABGR_EXT
      case 'A': return GL_ABGR_EXT;
#endif
#ifdef GL_BGRA_EXT
      case 'B': return GL_BGRA_EXT;
#endif
      case 'C': 
	switch (name[9]){
#ifdef GL_C4UB_V2F
	case '2': return GL_C4UB_V2F;
#endif
#ifdef GL_C4UB_V3F
	case '3': return GL_C4UB_V3F;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'E': 
	switch (name[9]){
#ifdef GL_EMISSION
	case 'O': return GL_EMISSION;
#endif
#ifdef GL_EVAL_BIT
	case 'I': return GL_EVAL_BIT;
#endif
#ifdef GL_EXT_abgr
	case 'G': return GL_EXT_abgr;
#endif
#ifdef GL_EXT_bgra
	case 'R': return GL_EXT_bgra;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'F': 
	switch (name[9]){
#ifdef GL_FEEDBACK
	case 'C': return GL_FEEDBACK;
#endif
#ifdef GL_FOG_HINT
	case 'N': return GL_FOG_HINT;
#endif
	case 'D': 
	  switch (name[4]){
#ifdef GL_FOG_MODE
	  case 'O': return GL_FOG_MODE;
#endif
#ifdef GL_FUNC_ADD
	  case 'U': return GL_FUNC_ADD;
#endif
	  default: return _GL_UNDEFINED;
	  }
	default: return _GL_UNDEFINED;
	}
#ifdef GL_HINT_BIT
      case 'H': return GL_HINT_BIT;
#endif
      case 'L': 
	switch (name[6]){
#ifdef GL_LIGHTING
	case 'H': return GL_LIGHTING;
#endif
#ifdef GL_LINE_BIT
	case 'E': return GL_LINE_BIT;
#endif
#ifdef GL_LIST_BIT
	case 'T': return GL_LIST_BIT;
#endif
#ifdef GL_LOGIC_OP
	case 'I': return GL_LOGIC_OP;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_MODULATE
      case 'M': return GL_MODULATE;
#endif
      case 'N': 
	switch (name[5]){
#ifdef GL_NOTEQUAL
	case 'T': return GL_NOTEQUAL;
#endif
#ifdef GL_NO_ERROR
	case '_': return GL_NO_ERROR;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_POSITION
      case 'P': return GL_POSITION;
#endif
      case 'R': 
	switch (name[9]){
#ifdef GL_R3_G3_B2
	case 'B': return GL_R3_G3_B2;
#endif
#ifdef GL_RED_BITS
	case 'T': return GL_RED_BITS;
#endif
#ifdef GL_RENDERER
	case 'E': return GL_RENDERER;
#endif
	case 'A':
	  switch (name[4]){
#ifdef GL_RED_BIAS
	  case 'E': return GL_RED_BIAS;
#endif
#ifdef GL_RGB10_A2
	  case 'G': return GL_RGB10_A2;
#endif
	  default: return _GL_UNDEFINED;
	  }
	default: return _GL_UNDEFINED;
	}
#ifdef GL_SPECULAR
      case 'S': return GL_SPECULAR;
#endif
#ifdef GL_VIEWPORT
      case 'V': return GL_VIEWPORT;
#endif
      default: return _GL_UNDEFINED;
      }
    case 12:
      switch (name[3]){
      case 'B':
	switch (name[10]){
#ifdef GL_BACK_LEFT
	case 'F': return GL_BACK_LEFT;
#endif
#ifdef GL_BLEND_DST
	case 'S': return GL_BLEND_DST;
#endif
#ifdef GL_BLEND_SRC
	case 'R': return GL_BLEND_SRC;
#endif
#ifdef GL_BLUE_BIAS
	case 'A': return GL_BLUE_BIAS;
#endif
#ifdef GL_BLUE_BITS
	case 'T': return GL_BLUE_BITS;
#endif
      default: return _GL_UNDEFINED;
      }
#ifdef GL_CULL_FACE
      case 'C': return GL_CULL_FACE;
#endif
      case 'D':
	switch (name[8]){
#ifdef GL_DONT_CARE
	case 'C': return GL_DONT_CARE;
#endif
#ifdef GL_DST_ALPHA
	case 'L': return GL_DST_ALPHA;
#endif
#ifdef GL_DST_COLOR
	case 'O': return GL_DST_COLOR;
#endif
      default: return _GL_UNDEFINED;
      }
      case 'E':
	switch (name[4]){
#ifdef GL_EDGE_FLAG
	case 'D': return GL_EDGE_FLAG;
#endif
#ifdef GL_EYE_PLANE
	case 'Y': return GL_EYE_PLANE;
#endif
      default: return _GL_UNDEFINED;
      }
      case 'F':
	switch (name[7]){
#ifdef GL_FOG_COLOR
	case 'C': return GL_FOG_COLOR;
#endif
#ifdef GL_FOG_INDEX
	case 'I': return GL_FOG_INDEX;
#endif
#ifdef GL_FOG_START
	case 'S': return GL_FOG_START;
#endif
      default: return _GL_UNDEFINED;
      }
#ifdef GL_HISTOGRAM
      case 'H': return GL_HISTOGRAM;
#endif
#ifdef GL_INTENSITY
      case 'I': return GL_INTENSITY;
#endif
      case 'L':
	switch (name[8]){
#ifdef GL_LINE_LOOP
	case 'L': return GL_LINE_LOOP;
#endif
#ifdef GL_LIST_BASE
	case 'B': return GL_LIST_BASE;
#endif
#ifdef GL_LIST_MODE
	case 'M': return GL_LIST_MODE;
#endif
#ifdef GL_LUMINANCE
	case 'A': return GL_LUMINANCE;
#endif
      default: return _GL_UNDEFINED;
      }
      case 'M':
	switch (name[4]){
#ifdef GL_MAP_COLOR
	case 'A': return GL_MAP_COLOR;
#endif
#ifdef GL_MODELVIEW
	case 'O': return GL_MODELVIEW;
#endif
      default: return _GL_UNDEFINED;
      }
#ifdef GL_NORMALIZE
      case 'N': return GL_NORMALIZE;
#endif
      case 'P':
	switch (name[4]){
#ifdef GL_PHONG_WIN
	case 'H': return GL_PHONG_WIN;
#endif
#ifdef GL_POINT_BIT
	case 'O': return GL_POINT_BIT;
#endif
      default: return _GL_UNDEFINED;
      }
      case 'R':
	switch (name[4]){
#ifdef GL_RED_SCALE
	case 'E': return GL_RED_SCALE;
#endif
#ifdef GL_RGBA_MODE
	case 'G': return GL_RGBA_MODE;
#endif
      default: return _GL_UNDEFINED;
      }
      case 'S':
	switch (name[7]){
#ifdef GL_SHININESS
	case 'I': return GL_SHININESS;
#endif
#ifdef GL_SRC_ALPHA
	case 'A': return GL_SRC_ALPHA;
#endif
#ifdef GL_SRC_COLOR
	case 'C': return GL_SRC_COLOR;
#endif
      default: return _GL_UNDEFINED;
      }
#ifdef GL_TRIANGLES
      case 'T': return GL_TRIANGLES;
#endif
      default: return _GL_UNDEFINED;
      }
    case 13:
      switch (name[3]){
      case 'A':
	switch (name[11]){
#ifdef GL_ALPHA_BIAS
	case 'A': return GL_ALPHA_BIAS;
#endif
#ifdef GL_ALPHA_BITS
	case 'T': return GL_ALPHA_BITS;
#endif
#ifdef GL_ALPHA_TEST
	case 'S': return GL_ALPHA_TEST;
#endif
	default: return _GL_UNDEFINED;
      }
      case 'B':
	switch (name[4]){
#ifdef GL_BACK_RIGHT
	case 'A': return GL_BACK_RIGHT;
#endif
#ifdef GL_BLUE_SCALE
	case 'L': return GL_BLUE_SCALE;
#endif
	default: return _GL_UNDEFINED;
      }
      case 'D':
	switch (name[11]){
#ifdef GL_DEPTH_BIAS
	case 'A': return GL_DEPTH_BIAS;
#endif
#ifdef GL_DEPTH_BITS
	case 'T': return GL_DEPTH_BITS;
#endif
#ifdef GL_DEPTH_FUNC
	case 'N': return GL_DEPTH_FUNC;
#endif
#ifdef GL_DEPTH_TEST
	case 'S': return GL_DEPTH_TEST;
#endif
#ifdef GL_DOUBLE_EXT
	case 'X': return GL_DOUBLE_EXT;
#endif
	default: return _GL_UNDEFINED;
      }
      case 'E':
	switch (name[4]){
#ifdef GL_ENABLE_BIT
	case 'N': return GL_ENABLE_BIT;
#endif
#ifdef GL_EXTENSIONS
	case 'X': return GL_EXTENSIONS;
#endif
#ifdef GL_EYE_LINEAR
	case 'Y': return GL_EYE_LINEAR;
#endif
	default: return _GL_UNDEFINED;
      }
      case 'F':
	switch (name[9]){
#ifdef GL_FRONT_FACE
	case 'F': return GL_FRONT_FACE;
#endif
#ifdef GL_FRONT_LEFT
	case 'L': return GL_FRONT_LEFT;
#endif
	default: return _GL_UNDEFINED;
      }
      case 'G':
	switch (name[11]){
#ifdef GL_GREEN_BIAS
	case 'A': return GL_GREEN_BIAS;
#endif
#ifdef GL_GREEN_BITS
	case 'T': return GL_GREEN_BITS;
#endif
	default: return _GL_UNDEFINED;
      }
      case 'I':
	switch (name[12]){
#ifdef GL_INDEX_BITS
	case 'S': return GL_INDEX_BITS;
#endif
#ifdef GL_INDEX_MODE
	case 'E': return GL_INDEX_MODE;
#endif
#ifdef GL_INTENSITY4
	case '4': return GL_INTENSITY4;
#endif
#ifdef GL_INTENSITY8
	case '8': return GL_INTENSITY8;
#endif
	default: return _GL_UNDEFINED;
      }
      case 'L':
	switch (name[12]){
#ifdef GL_LINE_STRIP
	case 'P': return GL_LINE_STRIP;
#endif
#ifdef GL_LINE_TOKEN
	case 'N': return GL_LINE_TOKEN;
#endif
#ifdef GL_LINE_WIDTH
	case 'H': return GL_LINE_WIDTH;
#endif
#ifdef GL_LIST_INDEX
	case 'X': return GL_LIST_INDEX;
#endif
#ifdef GL_LUMINANCE4
	case '4': return GL_LUMINANCE4;
#endif
#ifdef GL_LUMINANCE8
	case '8': return GL_LUMINANCE8;
#endif
	default: return _GL_UNDEFINED;
      }	
      case 'M':
	switch (name[6]){
#ifdef GL_MAP1_INDEX
	case '1': return GL_MAP1_INDEX;
#endif
#ifdef GL_MAP2_INDEX
	case '2': return GL_MAP2_INDEX;
#endif
#ifdef GL_MAX_LIGHTS
	case '_': return GL_MAX_LIGHTS;
#endif
	default: return _GL_UNDEFINED;
      }
#ifdef GL_OR_REVERSE
      case 'O': return GL_OR_REVERSE;
#endif

      case 'P':
	switch (name[4]){
#ifdef GL_POINT_SIZE
	case 'O': return GL_POINT_SIZE;
#endif
#ifdef GL_PROJECTION
	case 'R': return GL_PROJECTION;
#endif
	default: return _GL_UNDEFINED;
      }
#ifdef GL_QUAD_STRIP
      case 'Q': return GL_QUAD_STRIP;
#endif
#ifdef GL_SPHERE_MAP
      case 'S': return GL_SPHERE_MAP;
#endif
      case 'T':
	switch (name[11]){
#ifdef GL_TEXTURE_1D
	case '1': return GL_TEXTURE_1D;
#endif
#ifdef GL_TEXTURE_2D
	case '2': return GL_TEXTURE_2D;
#endif
#ifdef GL_TEXTURE_3D
	case '3': return GL_TEXTURE_3D;
#endif
	default: return _GL_UNDEFINED;
	}
      default: return _GL_UNDEFINED;
      }
    case 14:
      switch (name[3]){
      case 'A':
	switch (name[5]){
#ifdef GL_ALPHA_SCALE
	case 'P': return GL_ALPHA_SCALE;
#endif
#ifdef GL_AND_REVERSE
	case 'D': return GL_AND_REVERSE;
#endif
#ifdef GL_AUTO_NORMAL
	case 'T': return GL_AUTO_NORMAL;
#endif
#ifdef GL_AUX_BUFFERS
	case 'X': return GL_AUX_BUFFERS;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_BLEND_COLOR
      case 'B': return GL_BLEND_COLOR;
#endif
      case 'C':
	switch (name[13]){
#ifdef GL_C4F_N3F_V3F
	case 'F': return GL_C4F_N3F_V3F;
#endif
#ifdef GL_CLIP_PLANE0
	case '0': return GL_CLIP_PLANE0;
#endif
#ifdef GL_CLIP_PLANE1
	case '1': return GL_CLIP_PLANE1;
#endif
#ifdef GL_CLIP_PLANE2
	case '2': return GL_CLIP_PLANE2;
#endif
#ifdef GL_CLIP_PLANE3
	case '3': return GL_CLIP_PLANE3;
#endif
#ifdef GL_CLIP_PLANE4
	case '4': return GL_CLIP_PLANE4;
#endif
#ifdef GL_CLIP_PLANE5
	case '5': return GL_CLIP_PLANE5;
#endif
#ifdef GL_COLOR_ARRAY
	case 'Y': return GL_COLOR_ARRAY;
#endif
#ifdef GL_COLOR_INDEX
	case 'X': return GL_COLOR_INDEX;
#endif
#ifdef GL_COLOR_TABLE
	case 'E': return GL_COLOR_TABLE;
#endif
#ifdef GL_CURRENT_BIT
	case 'T': return GL_CURRENT_BIT;
#endif
	default: return _GL_UNDEFINED;
      }
      case 'D':
	switch (name[9]){
#ifdef GL_DEPTH_RANGE
	case 'R': return GL_DEPTH_RANGE;
#endif
#ifdef GL_DEPTH_SCALE
	case 'S': return GL_DEPTH_SCALE;
#endif
#ifdef GL_DRAW_BUFFER
	case 'U': return GL_DRAW_BUFFER;
#endif
	default: return _GL_UNDEFINED;
      }
      case 'F':
	switch (name[5]){
#ifdef GL_FOG_DENSITY
	case 'G': return GL_FOG_DENSITY;
#endif
#ifdef GL_FRONT_RIGHT
	case 'O': return GL_FRONT_RIGHT;
#endif
	default: return _GL_UNDEFINED;
      }
#ifdef GL_GREEN_SCALE
      case 'G': return GL_GREEN_SCALE;
#endif
      case 'I':
	switch (name[13]){
#ifdef GL_INDEX_ARRAY
	case 'Y': return GL_INDEX_ARRAY;
#endif
#ifdef GL_INDEX_SHIFT
	case 'T': return GL_INDEX_SHIFT;
#endif
#ifdef GL_INTENSITY12
	case '2': return GL_INTENSITY12;
#endif
#ifdef GL_INTENSITY16
	case '6': return GL_INTENSITY16;
#endif
	default: return _GL_UNDEFINED;
      }
      case 'L':
	switch (name[13]){
#ifdef GL_LINE_SMOOTH
	case 'H': return GL_LINE_SMOOTH;
#endif
#ifdef GL_LUMINANCE12
	case '2': return GL_LUMINANCE12;
#endif
#ifdef GL_LUMINANCE16
	case '6': return GL_LUMINANCE16;
#endif
	default: return _GL_UNDEFINED;
      }
      case 'M':
	switch (name[6]){
#ifdef GL_MAP1_NORMAL
	case '1': return GL_MAP1_NORMAL;
#endif
#ifdef GL_MAP2_NORMAL
	case '2': return GL_MAP2_NORMAL;
#endif
#ifdef GL_MAP_STENCIL
	case '_': return GL_MAP_STENCIL;
#endif
#ifdef GL_MATRIX_MODE
	case 'R': return GL_MATRIX_MODE;
#endif
#ifdef GL_MINMAX_SINK
	case 'M': return GL_MINMAX_SINK;
#endif
	default: return _GL_UNDEFINED;
      }
#ifdef GL_OR_INVERTED
      case 'O': return GL_OR_INVERTED;
#endif
      case 'P':
	switch (name[5]){
#ifdef GL_POINT_TOKEN
	case 'I': return GL_POINT_TOKEN;
#endif
#ifdef GL_POLYGON_BIT
	case 'L': return GL_POLYGON_BIT;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'R':
	switch (name[5]){
#ifdef GL_READ_BUFFER
	case 'A': return GL_READ_BUFFER;
#endif
#ifdef GL_RENDER_MODE
	case 'N': return GL_RENDER_MODE;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'S':
	switch (name[4]){
	case 'C':
	  switch (name[13]){
#ifdef GL_SCISSOR_BIT
	  case 'T': return GL_SCISSOR_BIT;
#endif
#ifdef GL_SCISSOR_BOX
	  case 'X': return GL_SCISSOR_BOX;
#endif
	  default: return _GL_UNDEFINED;
	  }
#ifdef GL_SHADE_MODEL
	case 'H': return GL_SHADE_MODEL;
#endif
#ifdef GL_SPOT_CUTOFF
	case 'P': return GL_SPOT_CUTOFF;
#endif
#ifdef GL_STENCIL_REF
	case 'T': return GL_STENCIL_REF;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'T':
	switch (name[7]){
#ifdef GL_T2F_C3F_V3F
	case 'C': return GL_T2F_C3F_V3F;
#endif
#ifdef GL_T2F_N3F_V3F
	case 'N': return GL_T2F_N3F_V3F;
#endif
	case 'U': 
	  switch (name[11]){
	  case 'B' : return GL_TEXTURE_BIT;
	  case 'E' : return GL_TEXTURE_ENV;
	  default: return _GL_UNDEFINED;
	  }
 	default: return _GL_UNDEFINED;
	}
      case 'V':
	switch (name[13]){
#ifdef GL_VERSION_1_1
      case '1': return GL_VERSION_1_1;
#endif
#ifdef GL_VERSION_1_2
      case '2': return GL_VERSION_1_2;
#endif
 	default: return _GL_UNDEFINED;
      }
      default: return _GL_UNDEFINED;
      }
    case 15:
      switch (name[3]){
#ifdef GL_AND_INVERTED
      case 'A': return GL_AND_INVERTED;
#endif
#ifdef GL_BITMAP_TOKEN
      case 'B': return GL_BITMAP_TOKEN;
#endif
#ifdef GL_COLOR_MATRIX
      case 'C': return GL_COLOR_MATRIX;
#endif
      case 'D':
	switch (name[6]){
#ifdef GL_DOT3_RGB_EXT
	case '3': return GL_DOT3_RGB_EXT;
#endif
#ifdef GL_DOUBLEBUFFER
	case 'B': return GL_DOUBLEBUFFER;
#endif
	default: return _GL_UNDEFINED;
      }
#ifdef GL_FUNC_ADD_EXT
      case 'F': return GL_FUNC_ADD_EXT;
#endif
      case 'I':
	switch (name[5]){
#ifdef GL_INDEX_OFFSET
	case 'D': return GL_INDEX_OFFSET;
#endif
#ifdef GL_INVALID_ENUM
	case 'V': return GL_INVALID_ENUM;
#endif
	default: return _GL_UNDEFINED;
      }
      case 'L':
	switch (name[5]){
#ifdef GL_LIGHTING_BIT
	case 'G': return GL_LIGHTING_BIT;
#endif
#ifdef GL_LINE_STIPPLE
	case 'N': return GL_LINE_STIPPLE;
#endif
	default: return _GL_UNDEFINED;
      }
      case 'M':
	switch (name[6]){
#ifdef GL_MAP1_COLOR_4
	case '1': return GL_MAP1_COLOR_4;
#endif
#ifdef GL_MAP2_COLOR_4
	case '2': return GL_MAP2_COLOR_4;
#endif
	default: return _GL_UNDEFINED;
      }
#ifdef GL_NORMAL_ARRAY
      case 'N': return GL_NORMAL_ARRAY;
#endif
#ifdef GL_OBJECT_PLANE
      case 'O': return GL_OBJECT_PLANE;
#endif
      case 'P':
	switch (name[5]){
#ifdef GL_POINT_SMOOTH
	case 'I': return GL_POINT_SMOOTH;
#endif
#ifdef GL_POLYGON_MODE
	case 'L': return GL_POLYGON_MODE;
#endif
	default: return _GL_UNDEFINED;
      }
      case 'S':
	switch (name[13]){
#ifdef GL_SEPARABLE_2D
	case '2': return GL_SEPARABLE_2D;
#endif
#ifdef GL_SINGLE_COLOR
	case 'O': return GL_SINGLE_COLOR;
#endif
#ifdef GL_STENCIL_BITS
	case 'T': return GL_STENCIL_BITS;
#endif
#ifdef GL_STENCIL_FAIL
	case 'I': return GL_STENCIL_FAIL;
#endif
#ifdef GL_STENCIL_FUNC
	case 'N': return GL_STENCIL_FUNC;
#endif
	case 'S': 
	  switch (name[4]){
#ifdef GL_STENCIL_TEST
	  case 'T': return GL_STENCIL_TEST;
#endif
#ifdef GL_SCISSOR_TEST
	  case 'C': return GL_SCISSOR_TEST;
#endif
	  default: return _GL_UNDEFINED;
	  } 
	default: return _GL_UNDEFINED;
	}
      case 'T':
	switch (name[10]){
#ifdef GL_T2F_C4UB_V3F
	case 'B': return GL_T2F_C4UB_V3F;
#endif
#ifdef GL_TEXTURE0_ARB
	case '0': return GL_TEXTURE0_ARB;
#endif
#ifdef GL_TEXTURE1_ARB
	case '1': return GL_TEXTURE1_ARB;
#endif
#ifdef GL_TEXTURE2_ARB
	case '2': return GL_TEXTURE2_ARB;
#endif
#ifdef GL_TEXTURE3_ARB
	case '3': return GL_TEXTURE3_ARB;
#endif
#ifdef GL_TEXTURE4_ARB
	case '4': return GL_TEXTURE4_ARB;
#endif
#ifdef GL_TEXTURE5_ARB
	case '5': return GL_TEXTURE5_ARB;
#endif
#ifdef GL_TEXTURE6_ARB
	case '6': return GL_TEXTURE6_ARB;
#endif
#ifdef GL_TEXTURE7_ARB
	case '7': return GL_TEXTURE7_ARB;
#endif
#ifdef GL_TEXTURE8_ARB
	case '8': return GL_TEXTURE8_ARB;
#endif
#ifdef GL_TEXTURE9_ARB
	case '9': return GL_TEXTURE9_ARB;
#endif
#ifdef GL_TRIANGLE_FAN
	case 'E': return GL_TRIANGLE_FAN;
#endif
	default: return _GL_UNDEFINED;
      }
#ifdef GL_UNSIGNED_INT
      case 'U': return GL_UNSIGNED_INT;
#endif
      case 'V':
	switch (name[4]){
#ifdef GL_VERTEX_ARRAY
	case 'E': return GL_VERTEX_ARRAY;
#endif
#ifdef GL_VIEWPORT_BIT
	case 'I': return GL_VIEWPORT_BIT;
#endif
	default: return _GL_UNDEFINED;
      }
      default: return _GL_UNDEFINED;
      }
    case 16:
      switch (name[3]){
     case 'C':
	switch (name[11]){
#ifdef GL_CLAMP_TO_EDGE
	case '_': return GL_CLAMP_TO_EDGE;
#endif
#ifdef GL_COLOR_INDEXES
	case 'D': return GL_COLOR_INDEXES;
#endif
#ifdef GL_COPY_INVERTED
	case 'E': return GL_COPY_INVERTED;
#endif
#ifdef GL_CURRENT_COLOR
	case 'C': return GL_CURRENT_COLOR;
#endif
#ifdef GL_CURRENT_INDEX
	case 'I': return GL_CURRENT_INDEX;
#endif
	default: return _GL_UNDEFINED;
      }
     case 'D':
	switch (name[4]){
#ifdef GL_DECR_WRAP_EXT
	case 'E': return GL_DECR_WRAP_EXT;
#endif
#ifdef GL_DOT3_RGBA_EXT
	case 'O': return GL_DOT3_RGBA_EXT;
#endif
	default: return _GL_UNDEFINED;
      }
#ifdef GL_EXT_texture3D
      case 'E': return GL_EXT_texture3D;
#endif
#ifdef GL_FUNC_SUBTRACT
      case 'F': return GL_FUNC_SUBTRACT;
#endif
      case 'I':
	switch (name[5]){
#ifdef GL_INCR_WRAP_EXT
	case 'C': return GL_INCR_WRAP_EXT;
#endif
#ifdef GL_INVALID_VALUE
	case 'V': return GL_INVALID_VALUE;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_LOGIC_OP_MODE
      case 'L': return GL_LOGIC_OP_MODE;
#endif
      case 'M':
	switch (name[6]){
	case '1':
	  switch (name[15]){
#ifdef GL_MAP1_VERTEX_3
	  case '3': return GL_MAP1_VERTEX_3;
#endif
#ifdef GL_MAP1_VERTEX_4
	  case '4': return GL_MAP1_VERTEX_4;
#endif
	  default: return _GL_UNDEFINED;
	  }
	case '2':
	  switch (name[15]){
#ifdef GL_MAP2_VERTEX_3
	  case '3': return GL_MAP2_VERTEX_3;
#endif
#ifdef GL_MAP2_VERTEX_4
	  case '4': return GL_MAP2_VERTEX_4;
#endif
	  default: return _GL_UNDEFINED;
	  }
#ifdef GL_MINMAX_FORMAT
	case 'M': return GL_MINMAX_FORMAT;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_NORMAL_MAP_NV
      case 'N': return GL_NORMAL_MAP_NV;
#endif
      case 'O':
	switch (name[4]){
#ifdef GL_OBJECT_LINEAR
	case 'B': return GL_OBJECT_LINEAR;
#endif
#ifdef GL_OUT_OF_MEMORY
	case 'U': return GL_OUT_OF_MEMORY;
#endif
	default: return _GL_UNDEFINED;
      }
#ifdef GL_POLYGON_TOKEN
      case 'P': return GL_POLYGON_TOKEN;
#endif
     case 'S':
	switch (name[4]){
#ifdef GL_SPOT_EXPONENT
	case 'P': return GL_SPOT_EXPONENT;
#endif
#ifdef GL_STENCIL_INDEX
	case 'T': return GL_STENCIL_INDEX;
#endif
#ifdef GL_SUBPIXEL_BITS
	case 'U': return GL_SUBPIXEL_BITS;
#endif
	default: return _GL_UNDEFINED;
      }
     case 'T':
       switch (name[10]){
	case '1':
	  switch (name[11]){
#ifdef GL_TEXTURE10_ARB
	  case '0': return GL_TEXTURE10_ARB;
#endif
#ifdef GL_TEXTURE11_ARB
	  case '1': return GL_TEXTURE11_ARB;
#endif
#ifdef GL_TEXTURE12_ARB
	  case '2': return GL_TEXTURE12_ARB;
#endif
#ifdef GL_TEXTURE13_ARB
	  case '3': return GL_TEXTURE13_ARB;
#endif
#ifdef GL_TEXTURE14_ARB
	  case '4': return GL_TEXTURE14_ARB;
#endif
#ifdef GL_TEXTURE15_ARB
	  case '5': return GL_TEXTURE15_ARB;
#endif
#ifdef GL_TEXTURE16_ARB
	  case '6': return GL_TEXTURE16_ARB;
#endif
#ifdef GL_TEXTURE17_ARB
	  case '7': return GL_TEXTURE17_ARB;
#endif
#ifdef GL_TEXTURE18_ARB
	  case '8': return GL_TEXTURE18_ARB;
#endif
#ifdef GL_TEXTURE19_ARB
	  case '9': return GL_TEXTURE19_ARB;
#endif
	  default: return _GL_UNDEFINED;
	  }
       case '2':
	 switch (name[11]){
#ifdef GL_TEXTURE20_ARB
	 case '0': return GL_TEXTURE20_ARB;
#endif
#ifdef GL_TEXTURE21_ARB
	 case '1': return GL_TEXTURE21_ARB;
#endif
#ifdef GL_TEXTURE22_ARB
	 case '2': return GL_TEXTURE22_ARB;
#endif
#ifdef GL_TEXTURE23_ARB
	 case '3': return GL_TEXTURE23_ARB;
#endif
#ifdef GL_TEXTURE24_ARB
	 case '4': return GL_TEXTURE24_ARB;
#endif
#ifdef GL_TEXTURE25_ARB
	 case '5': return GL_TEXTURE25_ARB;
#endif
#ifdef GL_TEXTURE26_ARB
	 case '6': return GL_TEXTURE26_ARB;
#endif
#ifdef GL_TEXTURE27_ARB
	 case '7': return GL_TEXTURE27_ARB;
#endif
#ifdef GL_TEXTURE28_ARB
	 case '8': return GL_TEXTURE28_ARB;
#endif
#ifdef GL_TEXTURE29_ARB
	 case '9': return GL_TEXTURE29_ARB;
#endif
	  default: return _GL_UNDEFINED;
	  }
       case '3':
	 switch (name[11]){
#ifdef GL_TEXTURE30_ARB
	 case '0': return GL_TEXTURE30_ARB;
#endif
#ifdef GL_TEXTURE31_ARB
	 case '1': return GL_TEXTURE31_ARB;
#endif
	 default: return _GL_UNDEFINED;
	 }
       case '_':
	 switch (name[11]){
#ifdef GL_TEXTURE_DEPTH
	 case 'D': return GL_TEXTURE_DEPTH;
#endif
#ifdef GL_TEXTURE_WIDTH
	 case 'W': return GL_TEXTURE_WIDTH;
#endif
	 case 'G':
	   switch (name[15]){
#ifdef GL_TEXTURE_GEN_Q
	   case 'Q': return GL_TEXTURE_GEN_Q;
#endif
#ifdef GL_TEXTURE_GEN_R
	   case 'R': return GL_TEXTURE_GEN_R;
#endif
#ifdef GL_TEXTURE_GEN_S
	   case 'S': return GL_TEXTURE_GEN_S;
#endif
#ifdef GL_TEXTURE_GEN_T
	   case 'T': return GL_TEXTURE_GEN_T;
#endif
	   default: return _GL_UNDEFINED;
	   }
	 default: return _GL_UNDEFINED;
	 }
#ifdef GL_TRANSFORM_BIT
       case 'R': return GL_TRANSFORM_BIT;
#endif
       default: return _GL_UNDEFINED;
       }
#ifdef GL_UNSIGNED_BYTE
      case 'U': return GL_UNSIGNED_BYTE;
#endif
#ifdef GL_WIN_swap_hint
      case 'W': return GL_WIN_swap_hint;
#endif
      default: return _GL_UNDEFINED;
      }
    case 17:
      switch (name[3]){
      case 'A':
	switch (name[4]){
#ifdef GL_ACCUM_RED_BITS
	case 'C': return GL_ACCUM_RED_BITS;
#endif
#ifdef GL_ALPHA_TEST_REF
	case 'L': return GL_ALPHA_TEST_REF;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_BLEND_EQUATION
      case 'B': return GL_BLEND_EQUATION;
#endif
      case 'C':
	switch (name[16]){
#ifdef GL_COLOR_LOGIC_OP
	case 'P': return GL_COLOR_LOGIC_OP;
#endif
	case 'L':
	  switch (name[4]){
#ifdef GL_COLOR_MATERIAL
	  case 'O': return GL_COLOR_MATERIAL;
#endif
#ifdef GL_CURRENT_NORMAL
	  case 'U': return GL_CURRENT_NORMAL;
#endif
	  default: return _GL_UNDEFINED;
	  }
#ifdef GL_CONSTANT_ALPHA
	case 'A': return GL_CONSTANT_ALPHA;
#endif
#ifdef GL_CONSTANT_COLOR
	case 'R': return GL_CONSTANT_COLOR;
#endif
	case 'D':
	  switch (name[15]){
#ifdef GL_CONVOLUTION_1D
	  case '1': return GL_CONVOLUTION_1D;
#endif
#ifdef GL_CONVOLUTION_2D
	  case '2': return GL_CONVOLUTION_2D;
#endif
	  default: return _GL_UNDEFINED;
	  }
#ifdef GL_CULL_FACE_MODE
	case 'E': return GL_CULL_FACE_MODE;
#endif
	default: return _GL_UNDEFINED;
      }
#ifdef GL_FRONT_AND_BACK
      case 'F': return GL_FRONT_AND_BACK;
#endif
#ifdef GL_HISTOGRAM_SINK
      case 'H': return GL_HISTOGRAM_SINK;
#endif
#ifdef GL_INDEX_LOGIC_OP
      case 'I': return GL_INDEX_LOGIC_OP;
#endif
#ifdef GL_MAX_EVAL_ORDER
      case 'M': return GL_MAX_EVAL_ORDER;
#endif
      case 'P':
	switch (name[12]){
#ifdef GL_PACK_ALIGNMENT
	case 'N': return GL_PACK_ALIGNMENT;
#endif
#ifdef GL_PACK_LSB_FIRST
	case 'F': return GL_PACK_LSB_FIRST;
#endif
#ifdef GL_PACK_SKIP_ROWS
	case '_': return GL_PACK_SKIP_ROWS;
#endif
#ifdef GL_PHONG_HINT_WIN
	case 'T': return GL_PHONG_HINT_WIN;
#endif
#ifdef GL_PIXEL_MODE_BIT
	case 'E': return GL_PIXEL_MODE_BIT;
#endif
#ifdef GL_POLYGON_SMOOTH
	case 'M': return GL_POLYGON_SMOOTH;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_RESCALE_NORMAL
      case 'R': return GL_RESCALE_NORMAL;
#endif
      case 'S':
	switch (name[4]){
#ifdef GL_SPOT_DIRECTION
	case 'P': return GL_SPOT_DIRECTION;
#endif
#ifdef GL_STACK_OVERFLOW
	case 'T': return GL_STACK_OVERFLOW;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'T':
	switch (name[12]){
#ifdef GL_TEXTURE_3D_EXT
	case 'D': return GL_TEXTURE_3D_EXT;
#endif
#ifdef GL_TEXTURE_BORDER
	case 'O': return GL_TEXTURE_BORDER;
#endif
#ifdef GL_TEXTURE_HEIGHT
	case 'E': return GL_TEXTURE_HEIGHT;
#endif
#ifdef GL_TEXTURE_MATRIX
	case 'A': return GL_TEXTURE_MATRIX;
#endif
	case 'R':
	  switch (name[16]){
#ifdef GL_TEXTURE_WRAP_R
	  case 'R': return GL_TEXTURE_WRAP_R;
#endif
#ifdef GL_TEXTURE_WRAP_S
	  case 'S': return GL_TEXTURE_WRAP_S;
#endif
#ifdef GL_TEXTURE_WRAP_T
	  case 'T': return GL_TEXTURE_WRAP_T;
#endif
	  default: return _GL_UNDEFINED;
	  }
#ifdef GL_TRIANGLE_STRIP
	case 'S': return GL_TRIANGLE_STRIP;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_UNSIGNED_SHORT
      case 'U': return GL_UNSIGNED_SHORT;
#endif
      default: return _GL_UNDEFINED;
      }
    case 18:
      switch (name[3]){
      case 'A':
	switch (name[5]){
#ifdef GL_ACCUM_BLUE_BITS
	case 'C': return GL_ACCUM_BLUE_BITS;
#endif
#ifdef GL_ALL_ATTRIB_BITS
	case 'L': return GL_ALL_ATTRIB_BITS;
#endif
#ifdef GL_ALPHA_TEST_FUNC
	case 'P': return GL_ALPHA_TEST_FUNC;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_BLEND_COLOR_EXT
      case 'B': return GL_BLEND_COLOR_EXT;
#endif
      case 'C':
	switch (name[9]){
#ifdef GL_COLOR_ARRAY_EXT
	case 'A': return GL_COLOR_ARRAY_EXT;
#endif
#ifdef GL_COLOR_WRITEMASK
	case 'W': return GL_COLOR_WRITEMASK;
#endif
#ifdef GL_CONSTANT_BORDER
	case 'N': return GL_CONSTANT_BORDER;
#endif
	default: return _GL_UNDEFINED;
	}
     case 'D':
       switch (name[9]){
#ifdef GL_DEPTH_COMPONENT
       case 'C': return GL_DEPTH_COMPONENT;
#endif
#ifdef GL_DEPTH_WRITEMASK
       case 'W': return GL_DEPTH_WRITEMASK;
#endif
       default: return _GL_UNDEFINED;
       }
     case 'E':
	switch (name[4]){
#ifdef GL_EDGE_FLAG_ARRAY
	case 'D': return GL_EDGE_FLAG_ARRAY;
#endif
#ifdef GL_EXT_blend_color
	case 'X': return GL_EXT_blend_color;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_HISTOGRAM_WIDTH
      case 'H': return GL_HISTOGRAM_WIDTH;
#endif
      case 'I':
	switch (name[9]){
#ifdef GL_INDEX_ARRAY_EXT
	case 'A': return GL_INDEX_ARRAY_EXT;
#endif
#ifdef GL_INDEX_WRITEMASK
	case 'W': return GL_INDEX_WRITEMASK;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_LUMINANCE_ALPHA
      case 'L': return GL_LUMINANCE_ALPHA;
#endif
      case 'M':
	switch (name[4]){
#ifdef GL_MAX_CLIP_PLANES
	case 'A': return GL_MAX_CLIP_PLANES;
#endif
#ifdef GL_MESA_window_pos
	case 'E': return GL_MESA_window_pos;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'P':
	switch (name[8]){
#ifdef GL_PACK_ROW_LENGTH
	case 'R': return GL_PACK_ROW_LENGTH;
#endif
#ifdef GL_PACK_SWAP_BYTES
	case 'S': return GL_PACK_SWAP_BYTES;
#endif
#ifdef GL_POLYGON_STIPPLE
	case 'O': return GL_POLYGON_STIPPLE;
#endif
#ifdef GL_PROXY_HISTOGRAM
	case '_': return GL_PROXY_HISTOGRAM;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_STACK_UNDERFLOW
      case 'S': return GL_STACK_UNDERFLOW;
#endif
      case 'T':
	switch (name[4]){
#ifdef GL_T2F_C4F_N3F_V3F
	case '2': return GL_T2F_C4F_N3F_V3F;
#endif
#ifdef GL_T4F_C4F_N3F_V4F
	case '4': return GL_T4F_C4F_N3F_V4F;
#endif
#ifdef GL_TABLE_TOO_LARGE
	case 'A': return GL_TABLE_TOO_LARGE;
#endif
	case 'E':
	  switch (name[12]){
#ifdef GL_TEXTURE_MAX_LOD
	  case 'A': return GL_TEXTURE_MAX_LOD;
#endif
#ifdef GL_TEXTURE_MIN_LOD
	  case 'I': return GL_TEXTURE_MIN_LOD;
#endif
	  default: return _GL_UNDEFINED;
	  }
	default: return _GL_UNDEFINED;
	}
      default: return _GL_UNDEFINED;
      }
    case 19:
      switch (name[3]){
#ifdef GL_3D_COLOR_TEXTURE
      case '3': return GL_3D_COLOR_TEXTURE;
#endif
#ifdef GL_4D_COLOR_TEXTURE
      case '4': return GL_4D_COLOR_TEXTURE;
#endif
      case 'A':
	switch (name[9]){
#ifdef GL_ACCUM_ALPHA_BITS
	case 'A': return GL_ACCUM_ALPHA_BITS;
#endif
#ifdef GL_ACCUM_BUFFER_BIT
	case 'B': return GL_ACCUM_BUFFER_BIT;
#endif
#ifdef GL_ACCUM_GREEN_BITS
	case 'G': return GL_ACCUM_GREEN_BITS;
#endif
#ifdef GL_ARB_multitexture
	case 'L': return GL_ARB_multitexture;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'C':
	switch (name[9]){
	case 'A':
	  switch (name[15]){
#ifdef GL_COLOR_ARRAY_SIZE
	  case 'S': return GL_COLOR_ARRAY_SIZE;
#endif
#ifdef GL_COLOR_ARRAY_TYPE
	  case 'T': return GL_COLOR_ARRAY_TYPE;
#endif
	  default: return _GL_UNDEFINED;
	  }
#ifdef GL_COLOR_BUFFER_BIT
	case 'B': return GL_COLOR_BUFFER_BIT;
#endif
#ifdef GL_COLOR_TABLE_BIAS
	case 'T': return GL_COLOR_TABLE_BIAS;
#endif
	case 'I':
	  switch (name[14]){
#ifdef GL_COPY_PIXEL_TOKEN
	  case 'T': return GL_COPY_PIXEL_TOKEN;
#endif
#ifdef GL_COLOR_INDEX1_EXT
	  case '1': return GL_COLOR_INDEX1_EXT;
#endif
#ifdef GL_COLOR_INDEX2_EXT
	  case '2': return GL_COLOR_INDEX2_EXT;
#endif
#ifdef GL_COLOR_INDEX4_EXT
	  case '4': return GL_COLOR_INDEX4_EXT;
#endif
#ifdef GL_COLOR_INDEX8_EXT
	  case '8': return GL_COLOR_INDEX8_EXT;
#endif
	  default: return _GL_UNDEFINED;
	  }
	default: return _GL_UNDEFINED;
	}
      case 'D':
	switch (name[4]){
#ifdef GL_DEPTH_BUFFER_BIT
      case 'E': return GL_DEPTH_BUFFER_BIT;
#endif
#ifdef GL_DRAW_PIXEL_TOKEN
      case 'R': return GL_DRAW_PIXEL_TOKEN;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'E':
	switch (name[7]){
#ifdef GL_EXT_blend_minmax
      case 'B': return GL_EXT_blend_minmax;
#endif
#ifdef GL_EXT_stencil_wrap
      case 'S': return GL_EXT_stencil_wrap;
#endif
#ifdef GL_EXT_vertex_array
      case 'V': return GL_EXT_vertex_array;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_HISTOGRAM_FORMAT
      case 'H': return GL_HISTOGRAM_FORMAT;
#endif
#ifdef GL_INDEX_ARRAY_TYPE
      case 'I': return GL_INDEX_ARRAY_TYPE;
#endif
      case 'L':
	switch (name[8]){
#ifdef GL_LINE_RESET_TOKEN
      case 'R': return GL_LINE_RESET_TOKEN;
#endif
#ifdef GL_LINE_SMOOTH_HINT
      case 'S': return GL_LINE_SMOOTH_HINT;
#endif
#ifdef GL_LINE_WIDTH_RANGE
      case 'W': return GL_LINE_WIDTH_RANGE;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'M':
	switch (name[6]){
#ifdef GL_MAP1_GRID_DOMAIN
	case '1': return GL_MAP1_GRID_DOMAIN;
#endif
#ifdef GL_MAP2_GRID_DOMAIN
	case '2': return GL_MAP2_GRID_DOMAIN;
#endif
	case '_':
	  switch (name[7]){
#ifdef GL_MAX_LIST_NESTING
	  case 'L': return GL_MAX_LIST_NESTING;
#endif
#ifdef GL_MAX_TEXTURE_SIZE
	  case 'T': return GL_MAX_TEXTURE_SIZE;
#endif
	  default: return _GL_UNDEFINED;
	  }
#ifdef GL_MODELVIEW_MATRIX
	case 'E': return GL_MODELVIEW_MATRIX;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'N':
	switch (name[4]){
#ifdef GL_NAME_STACK_DEPTH
      case 'A': return GL_NAME_STACK_DEPTH;
#endif
#ifdef GL_NORMAL_ARRAY_EXT
      case 'O': return GL_NORMAL_ARRAY_EXT;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'P':
	switch (name[17]){
#ifdef GL_PACK_SKIP_IMAGES
	case 'E': return GL_PACK_SKIP_IMAGES;
#endif
#ifdef GL_PACK_SKIP_PIXELS
	case 'L': return GL_PACK_SKIP_PIXELS;
#endif
	case '_':
	  switch (name[13]){
#ifdef GL_PIXEL_MAP_A_TO_A
	  case 'A': return GL_PIXEL_MAP_A_TO_A;
#endif
#ifdef GL_PIXEL_MAP_B_TO_B
	  case 'B': return GL_PIXEL_MAP_B_TO_B;
#endif
#ifdef GL_PIXEL_MAP_G_TO_G
	  case 'G': return GL_PIXEL_MAP_G_TO_G;
#endif
	  case 'I':
	    switch (name[18]){
#ifdef GL_PIXEL_MAP_I_TO_A
	    case 'A': return GL_PIXEL_MAP_I_TO_A;
#endif
#ifdef GL_PIXEL_MAP_I_TO_B
	    case 'B': return GL_PIXEL_MAP_I_TO_B;
#endif
#ifdef GL_PIXEL_MAP_I_TO_G
	    case 'G': return GL_PIXEL_MAP_I_TO_G;
#endif
#ifdef GL_PIXEL_MAP_I_TO_I
	    case 'I': return GL_PIXEL_MAP_I_TO_I;
#endif
#ifdef GL_PIXEL_MAP_I_TO_R
	    case 'R': return GL_PIXEL_MAP_I_TO_R;
#endif
	    default: return _GL_UNDEFINED;
	    }
#ifdef GL_PIXEL_MAP_R_TO_R
	  case 'R': return GL_PIXEL_MAP_R_TO_R;
#endif
#ifdef GL_PIXEL_MAP_S_TO_S
	  case 'S': return GL_PIXEL_MAP_S_TO_S;
#endif
	  default: return _GL_UNDEFINED;
	  }
#ifdef GL_POINT_SIZE_RANGE
	case 'G': return GL_POINT_SIZE_RANGE;
#endif
#ifdef GL_PROXY_TEXTURE_1D
	case '1': return GL_PROXY_TEXTURE_1D;
#endif
#ifdef GL_PROXY_TEXTURE_2D
	case '2': return GL_PROXY_TEXTURE_2D;
#endif
#ifdef GL_PROXY_TEXTURE_3D
	case '3': return GL_PROXY_TEXTURE_3D;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_REPLICATE_BORDER
      case 'R': return GL_REPLICATE_BORDER;
#endif
      case 'T':
	switch (name[13]){
#ifdef GL_TEXTURE_ENV_MODE
	case 'V': return GL_TEXTURE_ENV_MODE;
#endif
#ifdef GL_TEXTURE_GEN_MODE
	case 'N': return GL_TEXTURE_GEN_MODE;
#endif
#ifdef GL_TEXTURE_PRIORITY
	case 'I': return GL_TEXTURE_PRIORITY;
#endif
#ifdef GL_TEXTURE_RED_SIZE
	case 'D': return GL_TEXTURE_RED_SIZE;
#endif
#ifdef GL_TEXTURE_RESIDENT
	case 'S': return GL_TEXTURE_RESIDENT;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'U':
	switch (name[10]){
#ifdef GL_UNPACK_ALIGNMENT
	case 'A': return GL_UNPACK_ALIGNMENT;
#endif
#ifdef GL_UNPACK_LSB_FIRST
	case 'L': return GL_UNPACK_LSB_FIRST;
#endif
#ifdef GL_UNPACK_SKIP_ROWS
	case 'S': return GL_UNPACK_SKIP_ROWS;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_VERTEX_ARRAY_EXT
      case 'V': return GL_VERTEX_ARRAY_EXT;
#endif
      default: return _GL_UNDEFINED;
      }
    case 20:
      switch (name[3]){
#ifdef GL_ACCUM_CLEAR_VALUE
      case 'A': return GL_ACCUM_CLEAR_VALUE;
#endif
      case 'C':
	switch (name[15]){
#ifdef GL_COLOR_CLEAR_VALUE
	case 'V': return GL_COLOR_CLEAR_VALUE;
#endif
#ifdef GL_COLOR_INDEX12_EXT
	case '2': return GL_COLOR_INDEX12_EXT;
#endif
#ifdef GL_COLOR_INDEX16_EXT
	case '6': return GL_COLOR_INDEX16_EXT;
#endif
#ifdef GL_COLOR_TABLE_SCALE
	case 'S': return GL_COLOR_TABLE_SCALE;
#endif
	case 'W':
	  switch (name[5]){
#ifdef GL_COLOR_TABLE_WIDTH
	  case 'L': return GL_COLOR_TABLE_WIDTH;
#endif
#ifdef GL_CONVOLUTION_WIDTH
	  case 'N': return GL_CONVOLUTION_WIDTH;
#endif
	  default: return _GL_UNDEFINED;
	  }
	default: return _GL_UNDEFINED;
	}
#ifdef GL_DEPTH_CLEAR_VALUE
      case 'D': return GL_DEPTH_CLEAR_VALUE;
#endif
#ifdef GL_FUNC_SUBTRACT_EXT
      case 'F': return GL_FUNC_SUBTRACT_EXT;
#endif
#ifdef GL_HP_occlusion_test
      case 'H': return GL_HP_occlusion_test;
#endif
      case 'I':
	switch (name[5]){
#ifdef GL_INDEX_CLEAR_VALUE
	case 'D': return GL_INDEX_CLEAR_VALUE;
#endif
#ifdef GL_INVALID_OPERATION
	case 'V': return GL_INVALID_OPERATION;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'L':
	switch (name[12]){
#ifdef GL_LUMINANCE4_ALPHA4
	case '4': return GL_LUMINANCE4_ALPHA4;
#endif
#ifdef GL_LUMINANCE6_ALPHA2
	case '6': return GL_LUMINANCE6_ALPHA2;
#endif
#ifdef GL_LUMINANCE8_ALPHA8
	case '8': return GL_LUMINANCE8_ALPHA8;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_MAX_VIEWPORT_DIMS
      case 'M': return GL_MAX_VIEWPORT_DIMS;
#endif
#ifdef GL_NORMAL_ARRAY_TYPE
      case 'N': return GL_NORMAL_ARRAY_TYPE;
#endif
#ifdef GL_OCCLUSION_TEST_HP
      case 'O': return GL_OCCLUSION_TEST_HP;
#endif
      case 'P':
	switch (name[6]){
#ifdef GL_PACK_IMAGE_HEIGHT
	case 'K': return GL_PACK_IMAGE_HEIGHT;
#endif
#ifdef GL_POINT_SMOOTH_HINT
	case 'N': return GL_POINT_SMOOTH_HINT;
#endif
#ifdef GL_PROJECTION_MATRIX
	case 'J': return GL_PROJECTION_MATRIX;
#endif
#ifdef GL_PROXY_COLOR_TABLE
	case 'X': return GL_PROXY_COLOR_TABLE;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_REFLECTION_MAP_NV
      case 'R': return GL_REFLECTION_MAP_NV;
#endif
#ifdef GL_STENCIL_WRITEMASK
      case 'S': return GL_STENCIL_WRITEMASK;
#endif
      case 'T':
	switch (name[11]){
#ifdef GL_TEXTURE_BLUE_SIZE
	case 'B': return GL_TEXTURE_BLUE_SIZE;
#endif
#ifdef GL_TEXTURE_DEPTH_EXT
	case 'D': return GL_TEXTURE_DEPTH_EXT;
#endif
#ifdef GL_TEXTURE_ENV_COLOR
	case 'E': return GL_TEXTURE_ENV_COLOR;
#endif
#ifdef GL_TEXTURE_MAX_LEVEL
	case 'M': return GL_TEXTURE_MAX_LEVEL;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'U':
	switch (name[10]){
#ifdef GL_UNPACK_ROW_LENGTH
	case 'R': return GL_UNPACK_ROW_LENGTH;
#endif
#ifdef GL_UNPACK_SWAP_BYTES
	case 'S': return GL_UNPACK_SWAP_BYTES;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'V':
	switch (name[16]){
#ifdef GL_VERTEX_ARRAY_SIZE
	case 'S': return GL_VERTEX_ARRAY_SIZE;
#endif
#ifdef GL_VERTEX_ARRAY_TYPE
	case 'T': return GL_VERTEX_ARRAY_TYPE;
#endif
	default: return _GL_UNDEFINED;
	}
      default: return _GL_UNDEFINED;
      }
    case 21:
      switch (name[3]){
      case 'A':
	switch (name[4]){
#ifdef GL_ACTIVE_TEXTURE_ARB
	case 'C': return GL_ACTIVE_TEXTURE_ARB;
#endif
#ifdef GL_ATTRIB_STACK_DEPTH
	case 'T': return GL_ATTRIB_STACK_DEPTH;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_BLEND_EQUATION_EXT
      case 'B': return GL_BLEND_EQUATION_EXT;
#endif
      case 'C':
	switch (name[13]){
#ifdef GL_CLAMP_TO_EDGE_SGIS
	case 'D': return GL_CLAMP_TO_EDGE_SGIS;
#endif
#ifdef GL_COLOR_ARRAY_STRIDE
	case 'Y': return GL_COLOR_ARRAY_STRIDE;
#endif
#ifdef GL_COLOR_TABLE_FORMAT
	case 'E': return GL_COLOR_TABLE_FORMAT;
#endif
#ifdef GL_CONSTANT_ALPHA_EXT
	case 'L': return GL_CONSTANT_ALPHA_EXT;
#endif
#ifdef GL_CONSTANT_COLOR_EXT
	case 'O': return GL_CONSTANT_COLOR_EXT;
#endif
	case 'N':
	  switch (name[15]){
#ifdef GL_CONVOLUTION_FORMAT
	  case 'F': return GL_CONVOLUTION_FORMAT;
#endif
#ifdef GL_CONVOLUTION_HEIGHT
	  case 'H': return GL_CONVOLUTION_HEIGHT;
#endif
	  default: return _GL_UNDEFINED;
	  }
	default: return _GL_UNDEFINED;
	}
      case 'E':
	switch (name[16]){
#ifdef GL_EXT_blend_logic_op
	case 'I': return GL_EXT_blend_logic_op;
#endif
#ifdef GL_EXT_blend_subtract
	case 'T': return GL_EXT_blend_subtract;
#endif
#ifdef GL_EXT_polygon_offset
	case 'F': return GL_EXT_polygon_offset;
#endif
#ifdef GL_EXT_rescale_normal
	case 'O': return GL_EXT_rescale_normal;
#endif
#ifdef GL_EXT_texture_object
	case 'B': return GL_EXT_texture_object;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_HISTOGRAM_RED_SIZE
      case 'H': return GL_HISTOGRAM_RED_SIZE;
#endif
#ifdef GL_INDEX_ARRAY_STRIDE
      case 'I': return GL_INDEX_ARRAY_STRIDE;
#endif
      case 'L':
	switch (name[4]){
#ifdef GL_LINEAR_ATTENUATION
	case 'I': return GL_LINEAR_ATTENUATION;
#endif
#ifdef GL_LUMINANCE12_ALPHA4
	case 'U': return GL_LUMINANCE12_ALPHA4;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'M':
	switch (name[6]){
#ifdef GL_MAP1_GRID_SEGMENTS
	case '1': return GL_MAP1_GRID_SEGMENTS;
#endif
#ifdef GL_MAP2_GRID_SEGMENTS
	case '2': return GL_MAP2_GRID_SEGMENTS;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'P':
	switch (name[15]){
#ifdef GL_PASS_THROUGH_TOKEN
	case '_': return GL_PASS_THROUGH_TOKEN;
#endif
#ifdef GL_POINT_SIZE_MAX_EXT
	case 'A': return GL_POINT_SIZE_MAX_EXT;
#endif
#ifdef GL_POINT_SIZE_MIN_EXT
	case 'I': return GL_POINT_SIZE_MIN_EXT;
#endif
#ifdef GL_POLYGON_OFFSET_EXT
	case 'E': return GL_POLYGON_OFFSET_EXT;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_RESCALE_NORMAL_EXT
      case 'R': return GL_RESCALE_NORMAL_EXT;
#endif
      case 'S':
	switch (name[11]){
#ifdef GL_SRC_ALPHA_SATURATE
      case 'A': return GL_SRC_ALPHA_SATURATE;
#endif
#ifdef GL_STENCIL_BUFFER_BIT
      case 'B': return GL_STENCIL_BUFFER_BIT;
#endif
#ifdef GL_STENCIL_VALUE_MASK
      case 'V': return GL_STENCIL_VALUE_MASK;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'T':
	switch (name[19]){
	case 'Z':
	  switch (name[11]){
#ifdef GL_TEXTURE_ALPHA_SIZE
	  case 'A': return GL_TEXTURE_ALPHA_SIZE;
#endif
#ifdef GL_TEXTURE_GREEN_SIZE
	  case 'G': return GL_TEXTURE_GREEN_SIZE;
#endif
	  default: return _GL_UNDEFINED;
	  }
#ifdef GL_TEXTURE_BINDING_1D
	case '1': return GL_TEXTURE_BINDING_1D;
#endif
#ifdef GL_TEXTURE_BINDING_2D
	case '2': return GL_TEXTURE_BINDING_2D;
#endif
#ifdef GL_TEXTURE_BINDING_3D
	case '3': return GL_TEXTURE_BINDING_3D;
#endif
#ifdef GL_TEXTURE_COMPONENTS
	case 'T': return GL_TEXTURE_COMPONENTS;
#endif
	case 'E':
	  switch (name[13]){
#ifdef GL_TEXTURE_BASE_LEVEL
	  case 'S': return GL_TEXTURE_BASE_LEVEL;
#endif
#ifdef GL_TEXTURE_MAG_FILTER
	  case 'G': return GL_TEXTURE_MAG_FILTER;
#endif
#ifdef GL_TEXTURE_MIN_FILTER
	  case 'N': return GL_TEXTURE_MIN_FILTER;
#endif
	  default: return _GL_UNDEFINED;
	  }
#ifdef GL_TEXTURE_WRAP_R_EXT
	case 'X': return GL_TEXTURE_WRAP_R_EXT;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'U':
	switch (name[15]){
#ifdef GL_UNPACK_SKIP_IMAGES
	case 'I': return GL_UNPACK_SKIP_IMAGES;
#endif
#ifdef GL_UNPACK_SKIP_PIXELS
	case 'P': return GL_UNPACK_SKIP_PIXELS;
#endif
	default: return _GL_UNDEFINED;
	}
      default: return _GL_UNDEFINED;
      }
    case 22:
      switch (name[3]){
#ifdef GL_AMBIENT_AND_DIFFUSE
      case 'A': return GL_AMBIENT_AND_DIFFUSE;
#endif
      case 'C':
	switch (name[9]){
#ifdef GL_COLOR_ARRAY_POINTER
	case 'A': return GL_COLOR_ARRAY_POINTER;
#endif
#ifdef GL_COLOR_MATERIAL_FACE
	case 'M': return GL_COLOR_MATERIAL_FACE;
#endif
#ifdef GL_COMPILE_AND_EXECUTE
	case 'E': return GL_COMPILE_AND_EXECUTE;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'E':
	switch (name[4]){
#ifdef GL_EDGE_FLAG_ARRAY_EXT
	case 'D': return GL_EDGE_FLAG_ARRAY_EXT;
#endif
#ifdef GL_EXT_texture_env_add
	case 'X': return GL_EXT_texture_env_add;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_HISTOGRAM_BLUE_SIZE
      case 'H': return GL_HISTOGRAM_BLUE_SIZE;
#endif
#ifdef GL_INDEX_ARRAY_POINTER
      case 'I': return GL_INDEX_ARRAY_POINTER;
#endif
      case 'L':
	switch (name[5]){
#ifdef GL_LIGHT_MODEL_AMBIENT
	case 'G': return GL_LIGHT_MODEL_AMBIENT;
#endif
#ifdef GL_LINE_STIPPLE_REPEAT
	case 'N': return GL_LINE_STIPPLE_REPEAT;
#endif
	case 'M':
	  switch (name[21]){
#ifdef GL_LUMINANCE12_ALPHA12
	  case '2': return GL_LUMINANCE12_ALPHA12;
#endif
#ifdef GL_LUMINANCE16_ALPHA16
	  case '6': return GL_LUMINANCE16_ALPHA16;
#endif
	  default: return _GL_UNDEFINED;
	  }
	default: return _GL_UNDEFINED;
	}
      case 'M':
	switch (name[7]){
#ifdef GL_MAX_3D_TEXTURE_SIZE
	case '3': return GL_MAX_3D_TEXTURE_SIZE;
#endif
#ifdef GL_MAX_PIXEL_MAP_TABLE
	case 'P': return GL_MAX_PIXEL_MAP_TABLE;
#endif
#ifdef GL_MESA_resize_buffers
	case '_': return GL_MESA_resize_buffers;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_NORMAL_ARRAY_STRIDE
      case 'N': return GL_NORMAL_ARRAY_STRIDE;
#endif
      case 'O':
	switch (name[13]){
	case 'D':
	  switch (name[17]){
#ifdef GL_ONE_MINUS_DST_ALPHA
	  case 'A': return GL_ONE_MINUS_DST_ALPHA;
#endif
#ifdef GL_ONE_MINUS_DST_COLOR
	  case 'C': return GL_ONE_MINUS_DST_COLOR;
#endif
	  default: return _GL_UNDEFINED;
	  }
	case 'S':
	  switch (name[17]){
#ifdef GL_ONE_MINUS_SRC_ALPHA
	  case 'A': return GL_ONE_MINUS_SRC_ALPHA;
#endif
#ifdef GL_ONE_MINUS_SRC_COLOR
	  case 'C': return GL_ONE_MINUS_SRC_COLOR;
#endif
	  default: return _GL_UNDEFINED;
	  }
	default: return _GL_UNDEFINED;
	}
      case 'P':
	switch (name[18]){
#ifdef GL_POLYGON_OFFSET_FILL
	case 'F': return GL_POLYGON_OFFSET_FILL;
#endif
#ifdef GL_POLYGON_OFFSET_LINE
	case 'L': return GL_POLYGON_OFFSET_LINE;
#endif
#ifdef GL_POLYGON_SMOOTH_HINT
	case 'H': return GL_POLYGON_SMOOTH_HINT;
#endif
#ifdef GL_POLYGON_STIPPLE_BIT
	case '_': return GL_POLYGON_STIPPLE_BIT;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_STENCIL_CLEAR_VALUE
      case 'S': return GL_STENCIL_CLEAR_VALUE;
#endif
      case 'T':
	switch (name[11]){
#ifdef GL_TABLE_TOO_LARGE_EXT
	case 'O': return GL_TABLE_TOO_LARGE_EXT;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY
	case 'C': return GL_TEXTURE_COORD_ARRAY;
#endif
#ifdef GL_TEXTURE_STACK_DEPTH
	case 'S': return GL_TEXTURE_STACK_DEPTH;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'U':
	switch (name[6]){
#ifdef GL_UNPACK_IMAGE_HEIGHT
	case 'A': return GL_UNPACK_IMAGE_HEIGHT;
#endif
#ifdef GL_UNSIGNED_BYTE_3_3_2
	case 'I': return GL_UNSIGNED_BYTE_3_3_2;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_VERTEX_ARRAY_STRIDE
      case 'V': return GL_VERTEX_ARRAY_STRIDE;
#endif
      default: return _GL_UNDEFINED;
      }
    case 23:
      switch (name[3]){
       case 'C':
	 switch (name[15]){
#ifdef GL_COLOR_ARRAY_SIZE_EXT
	 case 'S': return GL_COLOR_ARRAY_SIZE_EXT;
#endif
#ifdef GL_COLOR_ARRAY_TYPE_EXT
	 case 'T': return GL_COLOR_ARRAY_TYPE_EXT;
#endif
#ifdef GL_COLOR_TABLE_RED_SIZE
	 case 'R': return GL_COLOR_TABLE_RED_SIZE;
#endif
	 case 'E':
	   switch (name[18]){
#ifdef GL_CONSTANT_ATTENUATION
	   case 'A': return GL_CONSTANT_ATTENUATION;
#endif
#ifdef GL_CURRENT_RASTER_COLOR
	   case 'C': return GL_CURRENT_RASTER_COLOR;
#endif
#ifdef GL_CURRENT_RASTER_INDEX
	   case 'I': return GL_CURRENT_RASTER_INDEX;
#endif
	   default: return _GL_UNDEFINED;
	   }
	 }
      case 'E':
	switch (name[8]){
#ifdef GL_EXT_clip_volume_hint
	case 'L': return GL_EXT_clip_volume_hint;
#endif
#ifdef GL_EXT_paletted_texture
	case 'A': return GL_EXT_paletted_texture;
#endif
#ifdef GL_EXT_point_parameters
	case 'O': return GL_EXT_point_parameters;
#endif
#ifdef GL_EXT_texture_env_dot3
	case 'E': return GL_EXT_texture_env_dot3;
#endif
	default: return _GL_UNDEFINED;
	}
       case 'F':
	switch (name[19]){
#ifdef GL_FEEDBACK_BUFFER_SIZE
	case 'S': return GL_FEEDBACK_BUFFER_SIZE;
#endif
#ifdef GL_FEEDBACK_BUFFER_TYPE
	case 'T': return GL_FEEDBACK_BUFFER_TYPE;
#endif
	default: return _GL_UNDEFINED;
	}
       case 'H':
	switch (name[13]){
#ifdef GL_HISTOGRAM_ALPHA_SIZE
	case 'A': return GL_HISTOGRAM_ALPHA_SIZE;
#endif
#ifdef GL_HISTOGRAM_GREEN_SIZE
	case 'G': return GL_HISTOGRAM_GREEN_SIZE;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_INDEX_ARRAY_TYPE_EXT
      case 'I': return GL_INDEX_ARRAY_TYPE_EXT;
#endif
      case 'L':
	switch (name[7]){
#ifdef GL_LIGHT_MODEL_TWO_SIDE
	case 'T': return GL_LIGHT_MODEL_TWO_SIDE;
#endif
#ifdef GL_LINEAR_MIPMAP_LINEAR
	case 'A': return GL_LINEAR_MIPMAP_LINEAR;
#endif
#ifdef GL_LINE_STIPPLE_PATTERN
	case '_': return GL_LINE_STIPPLE_PATTERN;
#endif
	default: return _GL_UNDEFINED;
	}
       case 'M':
	switch (name[6]){
	case '1':
	  switch (name[22]){
#ifdef GL_MAP1_TEXTURE_COORD_1
	  case '1': return GL_MAP1_TEXTURE_COORD_1;
#endif
#ifdef GL_MAP1_TEXTURE_COORD_2
	  case '2': return GL_MAP1_TEXTURE_COORD_2;
#endif
#ifdef GL_MAP1_TEXTURE_COORD_3
	  case '3': return GL_MAP1_TEXTURE_COORD_3;
#endif
#ifdef GL_MAP1_TEXTURE_COORD_4
	  case '4': return GL_MAP1_TEXTURE_COORD_4;
#endif
	  default: return _GL_UNDEFINED;
	 }
	case '2':
	  switch (name[22]){
#ifdef GL_MAP2_TEXTURE_COORD_1
	  case '1': return GL_MAP2_TEXTURE_COORD_1;
#endif
#ifdef GL_MAP2_TEXTURE_COORD_2
	  case '2': return GL_MAP2_TEXTURE_COORD_2;
#endif
#ifdef GL_MAP2_TEXTURE_COORD_3
	  case '3': return GL_MAP2_TEXTURE_COORD_3;
#endif
#ifdef GL_MAP2_TEXTURE_COORD_4
	  case '4': return GL_MAP2_TEXTURE_COORD_4;
#endif
	  default: return _GL_UNDEFINED;
	}
	case '_':
	  switch (name[7]){
#ifdef GL_MAX_ELEMENTS_INDICES
	  case 'E': return GL_MAX_ELEMENTS_INDICES;
#endif
#ifdef GL_MAX_NAME_STACK_DEPTH
	  case 'N': return GL_MAX_NAME_STACK_DEPTH;
#endif
	  default: return _GL_UNDEFINED;
	  }
	default: return _GL_UNDEFINED;
	}
       case 'N':
	switch (name[4]){
#ifdef GL_NORMAL_ARRAY_POINTER
	case 'O': return GL_NORMAL_ARRAY_POINTER;
#endif
#ifdef GL_NV_texgen_reflection
	case 'V': return GL_NV_texgen_reflection;
#endif
	default: return _GL_UNDEFINED;
	}
       case 'P':
	switch (name[18]){
#ifdef GL_PACK_SKIP_IMAGES_EXT
	case 'S': return GL_PACK_SKIP_IMAGES_EXT;
#endif
#ifdef GL_POLYGON_OFFSET_POINT
	case 'P': return GL_POLYGON_OFFSET_POINT;
#endif
#ifdef GL_POLYGON_OFFSET_UNITS
	case 'U': return GL_POLYGON_OFFSET_UNITS;
#endif
#ifdef GL_PROXY_TEXTURE_3D_EXT
	case 'D': return GL_PROXY_TEXTURE_3D_EXT;
#endif
	default: return _GL_UNDEFINED;
	}
       case 'T':
	switch (name[11]){
#ifdef GL_TEXTURE_BORDER_COLOR
	case 'B': return GL_TEXTURE_BORDER_COLOR;
#endif
#ifdef GL_TEXTURE_PRIORITY_EXT
	case 'P': return GL_TEXTURE_PRIORITY_EXT;
#endif
#ifdef GL_TEXTURE_RESIDENT_EXT
	case 'R': return GL_TEXTURE_RESIDENT_EXT;
#endif
	default: return _GL_UNDEFINED;
	}
       case 'U':
	switch (name[12]){
#ifdef GL_UNSIGNED_INT_8_8_8_8
	case 'I': return GL_UNSIGNED_INT_8_8_8_8;
#endif
#ifdef GL_UNSIGNED_SHORT_5_6_5
	case 'S': return GL_UNSIGNED_SHORT_5_6_5;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_VERTEX_ARRAY_POINTER
      case 'V': return GL_VERTEX_ARRAY_POINTER;
#endif
      default: return _GL_UNDEFINED;
      }
    case 24:
      switch (name[3]){
      case 'C':
	switch (name[15]){
#ifdef GL_COLOR_ARRAY_COUNT_EXT
	case 'C': return GL_COLOR_ARRAY_COUNT_EXT;
#endif
#ifdef GL_COLOR_TABLE_BLUE_SIZE
	case 'B': return GL_COLOR_TABLE_BLUE_SIZE;
#endif
#ifdef GL_COLOR_TABLE_WIDTH_EXT
	case 'W': return GL_COLOR_TABLE_WIDTH_EXT;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_FUNC_REVERSE_SUBTRACT
      case 'F': return GL_FUNC_REVERSE_SUBTRACT;
#endif
#ifdef GL_INDEX_ARRAY_COUNT_EXT
      case 'I': return GL_INDEX_ARRAY_COUNT_EXT;
#endif
#ifdef GL_LINEAR_MIPMAP_NEAREST
      case 'L': return GL_LINEAR_MIPMAP_NEAREST;
#endif
      case 'M':
	switch (name[7]){
#ifdef GL_MAX_CONVOLUTION_WIDTH
	case 'C': return GL_MAX_CONVOLUTION_WIDTH;
#endif
#ifdef GL_MAX_ELEMENTS_VERTICES
	case 'E': return GL_MAX_ELEMENTS_VERTICES;
#endif
#ifdef GL_MAX_TEXTURE_UNITS_ARB
	case 'T': return GL_MAX_TEXTURE_UNITS_ARB;
#endif
#ifdef GL_MODELVIEW_STACK_DEPTH
	case 'L': return GL_MODELVIEW_STACK_DEPTH;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'N':
	switch (name[4]){
#ifdef GL_NEAREST_MIPMAP_LINEAR
	case 'E': return GL_NEAREST_MIPMAP_LINEAR;
#endif
#ifdef GL_NORMAL_ARRAY_TYPE_EXT
	case 'O': return GL_NORMAL_ARRAY_TYPE_EXT;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'P':
	switch (name[13]){
#ifdef GL_PACK_IMAGE_HEIGHT_EXT
	case '_': return GL_PACK_IMAGE_HEIGHT_EXT;
#endif
#ifdef GL_PIXEL_MAP_A_TO_A_SIZE
	case 'A': return GL_PIXEL_MAP_A_TO_A_SIZE;
#endif
#ifdef GL_PIXEL_MAP_B_TO_B_SIZE
	case 'B': return GL_PIXEL_MAP_B_TO_B_SIZE;
#endif
#ifdef GL_PIXEL_MAP_G_TO_G_SIZE
	case 'G': return GL_PIXEL_MAP_G_TO_G_SIZE;
#endif
	case 'I':
	  switch (name[18]){
#ifdef GL_PIXEL_MAP_I_TO_A_SIZE
	  case 'A': return GL_PIXEL_MAP_I_TO_A_SIZE;
#endif
#ifdef GL_PIXEL_MAP_I_TO_B_SIZE
	  case 'B': return GL_PIXEL_MAP_I_TO_B_SIZE;
#endif
#ifdef GL_PIXEL_MAP_I_TO_G_SIZE
	  case 'G': return GL_PIXEL_MAP_I_TO_G_SIZE;
#endif
#ifdef GL_PIXEL_MAP_I_TO_I_SIZE
	  case 'I': return GL_PIXEL_MAP_I_TO_I_SIZE;
#endif
#ifdef GL_PIXEL_MAP_I_TO_R_SIZE
	  case 'R': return GL_PIXEL_MAP_I_TO_R_SIZE;
#endif
	  default: return _GL_UNDEFINED;
	  }
#ifdef GL_PIXEL_MAP_R_TO_R_SIZE
	case 'R': return GL_PIXEL_MAP_R_TO_R_SIZE;
#endif
#ifdef GL_PIXEL_MAP_S_TO_S_SIZE
	case 'S': return GL_PIXEL_MAP_S_TO_S_SIZE;
#endif
#ifdef GL_POLYGON_OFFSET_FACTOR
	case 'F': return GL_POLYGON_OFFSET_FACTOR;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_QUADRATIC_ATTENUATION
      case 'Q': return GL_QUADRATIC_ATTENUATION;
#endif
#ifdef GL_SELECTION_BUFFER_SIZE
      case 'S': return GL_SELECTION_BUFFER_SIZE;
#endif
      case 'V':
	switch (name[16]){
#ifdef GL_VERTEX_ARRAY_SIZE_EXT
	case 'S': return GL_VERTEX_ARRAY_SIZE_EXT;
#endif
#ifdef GL_VERTEX_ARRAY_TYPE_EXT
	case 'T': return GL_VERTEX_ARRAY_TYPE_EXT;
#endif
	default: return _GL_UNDEFINED;
	}
      default: return _GL_UNDEFINED;
      }
    case 25:
      switch (name[3]){
#ifdef GL_ALL_CLIENT_ATTRIB_BITS
      case 'A': return GL_ALL_CLIENT_ATTRIB_BITS;
#endif
      case 'C':
	switch (name[15]){
#ifdef GL_CLIENT_ALL_ATTRIB_BITS
	case 'T': return GL_CLIENT_ALL_ATTRIB_BITS;
#endif
#ifdef GL_CLIENT_PIXEL_STORE_BIT
	case '_': return GL_CLIENT_PIXEL_STORE_BIT;
#endif
#ifdef GL_COLOR_ARRAY_STRIDE_EXT
	case 'S': return GL_COLOR_ARRAY_STRIDE_EXT;
#endif
#ifdef GL_COLOR_TABLE_ALPHA_SIZE
	case 'A': return GL_COLOR_TABLE_ALPHA_SIZE;
#endif
#ifdef GL_COLOR_TABLE_FORMAT_EXT
	case 'F': return GL_COLOR_TABLE_FORMAT_EXT;
#endif
#ifdef GL_COLOR_TABLE_GREEN_SIZE
	case 'G': return GL_COLOR_TABLE_GREEN_SIZE;
#endif
#ifdef GL_CURRENT_TEXTURE_COORDS
	case 'U': return GL_CURRENT_TEXTURE_COORDS;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_EDGE_FLAG_ARRAY_STRIDE
      case 'E': return GL_EDGE_FLAG_ARRAY_STRIDE;
#endif
#ifdef GL_INDEX_ARRAY_STRIDE_EXT
      case 'I': return GL_INDEX_ARRAY_STRIDE_EXT;
#endif
#ifdef GL_LINE_WIDTH_GRANULARITY
      case 'L': return GL_LINE_WIDTH_GRANULARITY;
#endif
      case 'M':
	switch (name[7]){
#ifdef GL_MAX_ATTRIB_STACK_DEPTH
	case 'A': return GL_MAX_ATTRIB_STACK_DEPTH;
#endif
#ifdef GL_MAX_CONVOLUTION_HEIGHT
	case 'C': return GL_MAX_CONVOLUTION_HEIGHT;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'N':
	switch (name[4]){
#ifdef GL_NEAREST_MIPMAP_NEAREST
	case 'E': return GL_NEAREST_MIPMAP_NEAREST;
#endif
#ifdef GL_NORMAL_ARRAY_COUNT_EXT
	case 'O': return GL_NORMAL_ARRAY_COUNT_EXT;
#endif
	default: return _GL_UNDEFINED;
	}
       case 'P':
	switch (name[4]){
#ifdef GL_POINT_SIZE_GRANULARITY
	case 'O': return GL_POINT_SIZE_GRANULARITY;
#endif
#ifdef GL_PROJECTION_STACK_DEPTH
	case 'R': return GL_PROJECTION_STACK_DEPTH;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'T':
	switch (name[11]){
#ifdef GL_TEXTURE_1D_BINDING_EXT
	case '1': return GL_TEXTURE_1D_BINDING_EXT;
#endif
#ifdef GL_TEXTURE_2D_BINDING_EXT
	case '2': return GL_TEXTURE_2D_BINDING_EXT;
#endif
#ifdef GL_TEXTURE_3D_BINDING_EXT
	case '3': return GL_TEXTURE_3D_BINDING_EXT;
#endif
	case 'I':
	  switch (name[13]){
#ifdef GL_TEXTURE_INDEX_SIZE_EXT
	  case 'D': return GL_TEXTURE_INDEX_SIZE_EXT;
#endif
#ifdef GL_TEXTURE_INTENSITY_SIZE
	  case 'T': return GL_TEXTURE_INTENSITY_SIZE;
#endif
	  default: return _GL_UNDEFINED;
	  }
#ifdef GL_TEXTURE_LUMINANCE_SIZE
	case 'L': return GL_TEXTURE_LUMINANCE_SIZE;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'U':
	switch (name[24]){
#ifdef GL_UNPACK_SKIP_IMAGES_EXT
	case 'T': return GL_UNPACK_SKIP_IMAGES_EXT;
#endif
#ifdef GL_UNSIGNED_SHORT_4_4_4_4
	case '4': return GL_UNSIGNED_SHORT_4_4_4_4;
#endif
#ifdef GL_UNSIGNED_SHORT_5_5_5_1
	case '1': return GL_UNSIGNED_SHORT_5_5_5_1;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_VERTEX_ARRAY_COUNT_EXT
      case 'V': return GL_VERTEX_ARRAY_COUNT_EXT;
#endif
      default: return _GL_UNDEFINED;
      }
    case 26:
      switch (name[3]){
      case 'C':
	switch (name[24]){
#ifdef GL_CLIENT_VERTEX_ARRAY_BIT
	case 'I': return GL_CLIENT_VERTEX_ARRAY_BIT;
#endif
#ifdef GL_COLOR_ARRAY_POINTER_EXT
	case 'X': return GL_COLOR_ARRAY_POINTER_EXT;
#endif
#ifdef GL_CONVOLUTION_BORDER_MODE
	case 'D': return GL_CONVOLUTION_BORDER_MODE;
#endif
#ifdef GL_CONVOLUTION_FILTER_BIAS
	case 'A': return GL_CONVOLUTION_FILTER_BIAS;
#endif
#ifdef GL_CURRENT_RASTER_DISTANCE
	case 'C': return GL_CURRENT_RASTER_DISTANCE;
#endif
#ifdef GL_CURRENT_RASTER_POSITION
	case 'O': return GL_CURRENT_RASTER_POSITION;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_EDGE_FLAG_ARRAY_POINTER
      case 'E': return GL_EDGE_FLAG_ARRAY_POINTER;
#endif
#ifdef GL_FEEDBACK_BUFFER_POINTER
      case 'F': return GL_FEEDBACK_BUFFER_POINTER;
#endif
#ifdef GL_INDEX_ARRAY_POINTER_EXT
      case 'I': return GL_INDEX_ARRAY_POINTER_EXT;
#endif
      case 'M':
	switch (name[7]){
#ifdef GL_MAX_3D_TEXTURE_SIZE_EXT
	case '3': return GL_MAX_3D_TEXTURE_SIZE_EXT;
#endif
#ifdef GL_MAX_TEXTURE_STACK_DEPTH
	case 'T': return GL_MAX_TEXTURE_STACK_DEPTH;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_NORMAL_ARRAY_STRIDE_EXT
      case 'N': return GL_NORMAL_ARRAY_STRIDE_EXT;
#endif
#ifdef GL_POLYGON_OFFSET_BIAS_EXT
      case 'P': return GL_POLYGON_OFFSET_BIAS_EXT;
#endif
      case 'S':
	switch (name[18]){
#ifdef GL_SEPARATE_SPECULAR_COLOR
	case 'A': return GL_SEPARATE_SPECULAR_COLOR;
#endif
#ifdef GL_SGIS_texture_edge_clamp
	case 'G': return GL_SGIS_texture_edge_clamp;
#endif
#ifdef GL_SMOOTH_LINE_WIDTH_RANGE
	case 'T': return GL_SMOOTH_LINE_WIDTH_RANGE;
#endif
#ifdef GL_SMOOTH_POINT_SIZE_RANGE
	case 'Z': return GL_SMOOTH_POINT_SIZE_RANGE;
#endif
	case 'P':
	  switch (name[22]){
#ifdef GL_STENCIL_PASS_DEPTH_FAIL
	  case 'F': return GL_STENCIL_PASS_DEPTH_FAIL;
#endif
#ifdef GL_STENCIL_PASS_DEPTH_PASS
	  case 'P': return GL_STENCIL_PASS_DEPTH_PASS;
#endif
	  default: return _GL_UNDEFINED;
	  }
	default: return _GL_UNDEFINED;
	}
      case 'T':
	switch (name[11]){
#ifdef GL_TEXTURE_COORD_ARRAY_EXT
	case 'C': return GL_TEXTURE_COORD_ARRAY_EXT;
#endif
#ifdef GL_TEXTURE_INTERNAL_FORMAT
	case 'I': return GL_TEXTURE_INTERNAL_FORMAT;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'U':
	switch (name[12]){
#ifdef GL_UNPACK_IMAGE_HEIGHT_EXT
	case 'A': return GL_UNPACK_IMAGE_HEIGHT_EXT;
#endif
#ifdef GL_UNSIGNED_BYTE_2_3_3_REV
	case 'B': return GL_UNSIGNED_BYTE_2_3_3_REV;
#endif
#ifdef GL_UNSIGNED_INT_10_10_10_2
	case 'I': return GL_UNSIGNED_INT_10_10_10_2;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_VERTEX_ARRAY_STRIDE_EXT
      case 'V': return GL_VERTEX_ARRAY_STRIDE_EXT;
#endif
#ifdef GL_WIN_draw_range_elements
      case 'W': return GL_WIN_draw_range_elements;
#endif
      default: return _GL_UNDEFINED;
      }
    case 27:
      switch (name[3]){
      case 'A':
	switch (name[11]){
#ifdef GL_ALIASED_LINE_WIDTH_RANGE
	case 'L': return GL_ALIASED_LINE_WIDTH_RANGE;
#endif
#ifdef GL_ALIASED_POINT_SIZE_RANGE
	case 'P': return GL_ALIASED_POINT_SIZE_RANGE;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'C':
	switch (name[15]){
#ifdef GL_COLOR_MATERIAL_PARAMETER
	case 'A': return GL_COLOR_MATERIAL_PARAMETER;
#endif
#ifdef GL_COLOR_MATRIX_STACK_DEPTH
	case '_': return GL_COLOR_MATRIX_STACK_DEPTH;
#endif
#ifdef GL_COLOR_TABLE_RED_SIZE_EXT
	case 'R': return GL_COLOR_TABLE_RED_SIZE_EXT;
#endif
#ifdef GL_CONVOLUTION_BORDER_COLOR
	case 'B': return GL_CONVOLUTION_BORDER_COLOR;
#endif
#ifdef GL_CONVOLUTION_FILTER_SCALE
	case 'F': return GL_CONVOLUTION_FILTER_SCALE;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_DISTANCE_ATTENUATION_EXT
      case 'D': return GL_DISTANCE_ATTENUATION_EXT;
#endif
#ifdef GL_FOG_SPECULAR_TEXTURE_WIN
      case 'F': return GL_FOG_SPECULAR_TEXTURE_WIN;
#endif
#ifdef GL_HISTOGRAM_LUMINANCE_SIZE
      case 'H': return GL_HISTOGRAM_LUMINANCE_SIZE;
#endif
#ifdef GL_LIGHT_MODEL_LOCAL_VIEWER
      case 'L': return GL_LIGHT_MODEL_LOCAL_VIEWER;
#endif
#ifdef GL_MAX_ELEMENTS_INDICES_WIN
      case 'M': return GL_MAX_ELEMENTS_INDICES_WIN;
#endif
#ifdef GL_NORMAL_ARRAY_POINTER_EXT
      case 'N': return GL_NORMAL_ARRAY_POINTER_EXT;
#endif
      case 'O':
	switch (name[22]){
#ifdef GL_OCCLUSION_TEST_RESULT_HP
	case 'L': return GL_OCCLUSION_TEST_RESULT_HP;
#endif
#ifdef GL_ONE_MINUS_CONSTANT_ALPHA
	case 'A': return GL_ONE_MINUS_CONSTANT_ALPHA;
#endif
#ifdef GL_ONE_MINUS_CONSTANT_COLOR
	case 'C': return GL_ONE_MINUS_CONSTANT_COLOR;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_SELECTION_BUFFER_POINTER
      case 'S': return GL_SELECTION_BUFFER_POINTER;
#endif
      case 'T':
	switch (name[23]){
#ifdef GL_TEXTURE_COORD_ARRAY_SIZE
	case 'S': return GL_TEXTURE_COORD_ARRAY_SIZE;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_TYPE
	case 'T': return GL_TEXTURE_COORD_ARRAY_TYPE;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'U':
	switch (name[12]){
#ifdef GL_UNSIGNED_INT_8_8_8_8_REV
	case 'I': return GL_UNSIGNED_INT_8_8_8_8_REV;
#endif
#ifdef GL_UNSIGNED_SHORT_5_6_5_REV
	case 'S': return GL_UNSIGNED_SHORT_5_6_5_REV;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_VERTEX_ARRAY_POINTER_EXT
      case 'V': return GL_VERTEX_ARRAY_POINTER_EXT;
#endif
      default: return _GL_UNDEFINED;
      }
    case 28:
      switch (name[3]){
      case 'C':
	switch (name[11]){
#ifdef GL_CLIENT_ACTIVE_TEXTURE_ARB
	case 'C': return GL_CLIENT_ACTIVE_TEXTURE_ARB;
#endif
#ifdef GL_CLIENT_ATTRIB_STACK_DEPTH
	case 'T': return GL_CLIENT_ATTRIB_STACK_DEPTH;
#endif
#ifdef GL_COLOR_TABLE_BLUE_SIZE_EXT
	case 'B': return GL_COLOR_TABLE_BLUE_SIZE_EXT;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'E':
	switch (name[4]){
#ifdef GL_EDGE_FLAG_ARRAY_COUNT_EXT
	case 'D': return GL_EDGE_FLAG_ARRAY_COUNT_EXT;
#endif
#ifdef GL_EXT_compiled_vertex_array
	case 'X': return GL_EXT_compiled_vertex_array;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_FUNC_REVERSE_SUBTRACT_EXT
      case 'F': return GL_FUNC_REVERSE_SUBTRACT_EXT;
#endif
#ifdef GL_LIGHT_MODEL_COLOR_CONTROL
      case 'L': return GL_LIGHT_MODEL_COLOR_CONTROL;
#endif
      case 'M':
	switch (name[7]){
#ifdef GL_MAX_ELEMENTS_VERTICES_WIN
	case 'E': return GL_MAX_ELEMENTS_VERTICES_WIN;
#endif
#ifdef GL_MAX_MODELVIEW_STACK_DEPTH
	case 'M': return GL_MAX_MODELVIEW_STACK_DEPTH;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'P':
	switch (name[5]){
#ifdef GL_POLYGON_OFFSET_FACTOR_EXT
	case 'L': return GL_POLYGON_OFFSET_FACTOR_EXT;
#endif
#ifdef GL_POST_CONVOLUTION_RED_BIAS
	case 'S': return GL_POST_CONVOLUTION_RED_BIAS;
#endif
	default: return _GL_UNDEFINED;
	}
      default: return _GL_UNDEFINED;
      }
    case 29:
      switch (name[3]){
      case 'C':
	switch (name[15]){
#ifdef GL_COLOR_TABLE_ALPHA_SIZE_EXT
	case 'A': return GL_COLOR_TABLE_ALPHA_SIZE_EXT;
#endif
#ifdef GL_COLOR_TABLE_GREEN_SIZE_EXT
	case 'G': return GL_COLOR_TABLE_GREEN_SIZE_EXT;
#endif
#ifdef GL_COLOR_TABLE_INTENSITY_SIZE
	case 'I': return GL_COLOR_TABLE_INTENSITY_SIZE;
#endif
#ifdef GL_COLOR_TABLE_LUMINANCE_SIZE
	case 'L': return GL_COLOR_TABLE_LUMINANCE_SIZE;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'E':
	switch (name[4]){
#ifdef GL_EDGE_FLAG_ARRAY_STRIDE_EXT
	case 'D': return GL_EDGE_FLAG_ARRAY_STRIDE_EXT;
#endif
#ifdef GL_EXT_shared_texture_palette
	case 'X': return GL_EXT_shared_texture_palette;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_MAX_PROJECTION_STACK_DEPTH
      case 'M': return GL_MAX_PROJECTION_STACK_DEPTH;
#endif
      case 'P':
	switch (name[20]){
#ifdef GL_POST_COLOR_MATRIX_RED_BIAS
	case '_': return GL_POST_COLOR_MATRIX_RED_BIAS;
#endif
#ifdef GL_POST_CONVOLUTION_BLUE_BIAS
	case 'B': return GL_POST_CONVOLUTION_BLUE_BIAS;
#endif
#ifdef GL_POST_CONVOLUTION_RED_SCALE
	case 'R': return GL_POST_CONVOLUTION_RED_SCALE;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_SHARED_TEXTURE_PALETTE_EXT
      case 'S': return GL_SHARED_TEXTURE_PALETTE_EXT;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_STRIDE
      case 'T': return GL_TEXTURE_COORD_ARRAY_STRIDE;
#endif
      case 'U':
	switch (name[24]){
#ifdef GL_UNSIGNED_SHORT_1_5_5_5_REV
	case '5': return GL_UNSIGNED_SHORT_1_5_5_5_REV;
#endif
#ifdef GL_UNSIGNED_SHORT_4_4_4_4_REV
	case '4': return GL_UNSIGNED_SHORT_4_4_4_4_REV;
#endif
	default: return _GL_UNDEFINED;
	}
      default: return _GL_UNDEFINED;
      }
    case 30:
      switch (name[3]){
#ifdef GL_EDGE_FLAG_ARRAY_POINTER_EXT
      case 'E': return GL_EDGE_FLAG_ARRAY_POINTER_EXT;
#endif
      case 'P':
	switch (name[23]){
#ifdef GL_PERSPECTIVE_CORRECTION_HINT
	case 'O': return GL_PERSPECTIVE_CORRECTION_HINT;
#endif
#ifdef GL_POST_COLOR_MATRIX_BLUE_BIAS
	case 'U': return GL_POST_COLOR_MATRIX_BLUE_BIAS;
#endif
#ifdef GL_POST_COLOR_MATRIX_RED_SCALE
	case 'D': return GL_POST_COLOR_MATRIX_RED_SCALE;
#endif
#ifdef GL_POST_CONVOLUTION_ALPHA_BIAS
	case 'H': return GL_POST_CONVOLUTION_ALPHA_BIAS;
#endif
	case 'E':
	  switch (name[20]){
#ifdef GL_POST_CONVOLUTION_BLUE_SCALE
	  case 'B': return GL_POST_CONVOLUTION_BLUE_SCALE;
#endif
#ifdef GL_POST_CONVOLUTION_GREEN_BIAS
	  case 'G': return GL_POST_CONVOLUTION_GREEN_BIAS;
#endif
	  default: return _GL_UNDEFINED;
	  }
	default: return _GL_UNDEFINED;
	}
#ifdef GL_TEXTURE_COORD_ARRAY_POINTER
      case 'T': return GL_TEXTURE_COORD_ARRAY_POINTER;
#endif
#ifdef GL_UNSIGNED_INT_2_10_10_10_REV
      case 'U': return GL_UNSIGNED_INT_2_10_10_10_REV;
#endif
      default: return _GL_UNDEFINED;
      }
    case 31:
      switch (name[3]){
      case 'A':
	switch (name[22]){
#ifdef GL_ARRAY_ELEMENT_LOCK_COUNT_EXT
	case 'C': return GL_ARRAY_ELEMENT_LOCK_COUNT_EXT;
#endif
#ifdef GL_ARRAY_ELEMENT_LOCK_FIRST_EXT
	case 'F': return GL_ARRAY_ELEMENT_LOCK_FIRST_EXT;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_MAX_COLOR_MATRIX_STACK_DEPTH
      case 'M': return GL_MAX_COLOR_MATRIX_STACK_DEPTH;
#endif
      case 'O':
	switch (name[22]){
#ifdef GL_ONE_MINUS_CONSTANT_ALPHA_EXT
	case 'A': return GL_ONE_MINUS_CONSTANT_ALPHA_EXT;
#endif
#ifdef GL_ONE_MINUS_CONSTANT_COLOR_EXT
	case 'C': return GL_ONE_MINUS_CONSTANT_COLOR_EXT;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'P':
	switch (name[21]){
#ifdef GL_POST_COLOR_MATRIX_ALPHA_BIAS
	case 'A': return GL_POST_COLOR_MATRIX_ALPHA_BIAS;
#endif
#ifdef GL_POST_COLOR_MATRIX_BLUE_SCALE
	case 'B': return GL_POST_COLOR_MATRIX_BLUE_SCALE;
#endif
#ifdef GL_POST_COLOR_MATRIX_GREEN_BIAS
	case 'G': return GL_POST_COLOR_MATRIX_GREEN_BIAS;
#endif
#ifdef GL_POST_CONVOLUTION_ALPHA_SCALE
	case 'L': return GL_POST_CONVOLUTION_ALPHA_SCALE;
#endif
#ifdef GL_POST_CONVOLUTION_COLOR_TABLE
	case 'O': return GL_POST_CONVOLUTION_COLOR_TABLE;
#endif
#ifdef GL_POST_CONVOLUTION_GREEN_SCALE
	case 'R': return GL_POST_CONVOLUTION_GREEN_SCALE;
#endif
	default: return _GL_UNDEFINED;
	}
       case 'T':
	switch (name[23]){
#ifdef GL_TEXTURE_COORD_ARRAY_SIZE_EXT
	case 'S': return GL_TEXTURE_COORD_ARRAY_SIZE_EXT;
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_TYPE_EXT
	case 'T': return GL_TEXTURE_COORD_ARRAY_TYPE_EXT;
#endif
	default: return _GL_UNDEFINED;
	}
      default: return _GL_UNDEFINED;
      }
    case 32:
      switch (name[3]){
      case 'C':
	switch (name[19]){
#ifdef GL_CLIP_VOLUME_CLIPPING_HINT_EXT
	case 'P': return GL_CLIP_VOLUME_CLIPPING_HINT_EXT;
#endif
#ifdef GL_CURRENT_RASTER_POSITION_VALID
	case 'O': return GL_CURRENT_RASTER_POSITION_VALID;
#endif
#ifdef GL_CURRENT_RASTER_TEXTURE_COORDS
	case 'E': return GL_CURRENT_RASTER_TEXTURE_COORDS;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_MAX_CLIENT_ATTRIB_STACK_DEPTH
      case 'M': return GL_MAX_CLIENT_ATTRIB_STACK_DEPTH;
#endif
      case 'P':
	switch (name[21]){
#ifdef GL_POINT_FADE_THRESHOLD_SIZE_EXT
	case 'L': return GL_POINT_FADE_THRESHOLD_SIZE_EXT;
#endif
#ifdef GL_POST_COLOR_MATRIX_ALPHA_SCALE
	case 'A': return GL_POST_COLOR_MATRIX_ALPHA_SCALE;
#endif
#ifdef GL_POST_COLOR_MATRIX_COLOR_TABLE
	case 'C': return GL_POST_COLOR_MATRIX_COLOR_TABLE;
#endif
#ifdef GL_POST_COLOR_MATRIX_GREEN_SCALE
	case 'G': return GL_POST_COLOR_MATRIX_GREEN_SCALE;
#endif
	default: return _GL_UNDEFINED;
	}
      case 'S':
	switch (name[10]){
#ifdef GL_SMOOTH_LINE_WIDTH_GRANULARITY
	case 'L': return GL_SMOOTH_LINE_WIDTH_GRANULARITY;
#endif
#ifdef GL_SMOOTH_POINT_SIZE_GRANULARITY
	case 'P': return GL_SMOOTH_POINT_SIZE_GRANULARITY;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_TEXTURE_COORD_ARRAY_COUNT_EXT
      case 'T': return GL_TEXTURE_COORD_ARRAY_COUNT_EXT;
#endif
      default: return _GL_UNDEFINED;
      }
    case 33:
      switch (name[3]){
      case 'C':
	switch (name[15]){
#ifdef GL_COLOR_TABLE_INTENSITY_SIZE_EXT
	case 'I': return GL_COLOR_TABLE_INTENSITY_SIZE_EXT;
#endif
#ifdef GL_COLOR_TABLE_LUMINANCE_SIZE_EXT
	case 'L': return GL_COLOR_TABLE_LUMINANCE_SIZE_EXT;
#endif
	default: return _GL_UNDEFINED;
	}
#ifdef GL_TEXTURE_COORD_ARRAY_STRIDE_EXT
      case 'T': return GL_TEXTURE_COORD_ARRAY_STRIDE_EXT;
#endif
      default: return _GL_UNDEFINED;
      }
    case 34:
      switch (name[3]){
#ifdef GL_TEXTURE_COORD_ARRAY_POINTER_EXT
      case 'T': return GL_TEXTURE_COORD_ARRAY_POINTER_EXT;
#endif
      default: return _GL_UNDEFINED;
      }
    case 37:
      switch (name[3]){
#ifdef GL_PROXY_POST_CONVOLUTION_COLOR_TABLE
      case 'P': return GL_PROXY_POST_CONVOLUTION_COLOR_TABLE;
#endif
      default: return _GL_UNDEFINED;
      }
    case 38:
      switch (name[3]){
#ifdef GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE
      case 'P': return GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE;
#endif
      default: return _GL_UNDEFINED;
      }
    default: return _GL_UNDEFINED;
    }
    return _GL_UNDEFINED;
} 
