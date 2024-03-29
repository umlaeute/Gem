////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "teapot.h"
#include "Gem/State.h"

/* Rim, body, lid, and bottom data must be reflected in x and
   y; handle and spout data across the y axis only.  */

static int teapot_patchdata[][16] = {
  /* rim */
  {102, 103, 104, 105, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
  /* body */
  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27},
  {24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40},
  /* lid */
  {96, 96, 96, 96, 97, 98, 99, 100, 101, 101, 101, 101, 0, 1, 2, 3,},
  {0, 1, 2, 3, 106, 107, 108, 109, 110, 111, 112,  113, 114, 115, 116, 117},
  /* bottom */
  {118, 118, 118, 118, 124, 122, 119, 121, 123, 126, 125, 120, 40, 39, 38, 37},
  /* handle */
  {41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56},
  {53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 28, 65, 66, 67},
  /* spout */
  {68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83},
  {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95}
};
/* *INDENT-OFF* */

static float teapot_cpdata[][3] =
{
    {0.2, 0, 2.7}, {0.2, -0.112, 2.7},
    {0.112, -0.2, 2.7}, {0, -0.2, 2.7},
    {1.3375, 0, 2.53125}, {1.3375, -0.749, 2.53125},
    {0.749, -1.3375, 2.53125}, {0, -1.3375, 2.53125},
    {1.4375, 0, 2.53125}, {1.4375, -0.805, 2.53125},
    {0.805, -1.4375,  2.53125}, {0, -1.4375, 2.53125},
    {1.5, 0, 2.4}, {1.5, -0.84, 2.4}, {0.84, -1.5, 2.4},
    {0, -1.5, 2.4}, {1.75, 0, 1.875},  {1.75, -0.98, 1.875},
    {0.98, -1.75, 1.875}, {0, -1.75, 1.875}, {2, 0, 1.35},
    {2, -1.12, 1.35}, {1.12, -2, 1.35},  {0, -2, 1.35},
    {2, 0, 0.9}, {2, -1.12, 0.9}, {1.12, -2,  0.9},
    {0, -2, 0.9}, {-2, 0, 0.9}, {2, 0, 0.45}, {2, -1.12, 0.45},
    {1.12, -2, 0.45}, {0, -2, 0.45}, {1.5, 0, 0.225},
    {1.5, -0.84, 0.225}, {0.84, -1.5, 0.225}, {0, -1.5, 0.225},
    {1.5, 0, 0.15}, {1.5, -0.84, 0.15}, {0.84, -1.5, 0.15},
    {0,  -1.5, 0.15}, {-1.6, 0, 2.025}, {-1.6, -0.3, 2.025},
    {-1.5, -0.3, 2.25}, {-1.5, 0, 2.25}, {-2.3, 0, 2.025},
    {-2.3, -0.3, 2.025}, {-2.5, -0.3, 2.25}, {-2.5, 0, 2.25},
    {-2.7, 0,  2.025}, {-2.7, -0.3, 2.025}, {-3, -0.3, 2.25},
    {-3, 0,  2.25}, {-2.7, 0, 1.8}, {-2.7, -0.3, 1.8}, {-3, -0.3, 1.8},
    {-3, 0, 1.8}, {-2.7, 0, 1.575}, {-2.7, -0.3, 1.575},
    {-3, -0.3, 1.35}, {-3, 0, 1.35}, {-2.5, 0, 1.125},
    {-2.5, -0.3, 1.125}, {-2.65, -0.3, 0.9375}, {-2.65, 0, 0.9375},
    {-2,  -0.3, 0.9}, {-1.9, -0.3, 0.6}, {-1.9, 0, 0.6},
    {1.7, 0, 1.425}, {1.7, -0.66, 1.425}, {1.7, -0.66, 0.6},
    {1.7, 0,  0.6}, {2.6, 0, 1.425}, {2.6, -0.66, 1.425},
    {3.1, -0.66, 0.825}, {3.1, 0, 0.825}, {2.3, 0, 2.1}, {2.3, -0.25, 2.1},
    {2.4, -0.25, 2.025}, {2.4, 0, 2.025}, {2.7, 0, 2.4}, {2.7, -0.25, 2.4},
    {3.3, -0.25, 2.4}, {3.3, 0, 2.4}, {2.8, 0,  2.475},
    {2.8, -0.25, 2.475}, {3.525, -0.25, 2.49375},
    {3.525, 0, 2.49375}, {2.9, 0, 2.475}, {2.9, -0.15, 2.475},
    {3.45, -0.15, 2.5125}, {3.45, 0, 2.5125}, {2.8, 0, 2.4},
    {2.8, -0.15, 2.4}, {3.2, -0.15, 2.4}, {3.2, 0, 2.4},
    {0, 0, 3.15}, {0.8, 0, 3.15}, {0.8, -0.45, 3.15},
    {0.45, -0.8, 3.15}, {0, -0.8, 3.15}, {0, 0, 2.85},
    {1.4, 0, 2.4}, {1.4, -0.784, 2.4}, {0.784, -1.4, 2.4},
    {0, -1.4, 2.4}, {0.4, 0, 2.55}, {0.4, -0.224, 2.55},
    {0.224, -0.4, 2.55}, {0, -0.4, 2.55}, {1.3, 0, 2.55},
    {1.3, -0.728, 2.55}, {0.728, -1.3, 2.55}, {0, -1.3, 2.55},
    {1.3, 0, 2.4}, {1.3, -0.728, 2.4}, {0.728, -1.3, 2.4},
    {0, -1.3, 2.4}, {0, 0, 0}, {1.425, -0.798, 0}, {1.5, 0, 0.075},
    {1.425, 0, 0}, {0.798, -1.425, 0}, {0, -1.5, 0.075},
    {0, -1.425, 0}, {1.5, -0.84, 0.075}, {0.84, -1.5, 0.075}
};


CPPEXTERN_NEW_WITH_TWO_ARGS(teapot, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// teapot
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
teapot :: teapot(t_floatarg size, t_floatarg slice)
      : GemGluObj(size)
{
  m_drawType=GL_FILL;
  m_numSlices=static_cast<int>(slice);
  if(m_numSlices<=0)m_numSlices=14;

  m_texCoords[0][0]=0;m_texCoords[0][1]=0;
  m_texCoords[1][0]=1;m_texCoords[1][1]=0;
  m_texCoords[2][0]=0;m_texCoords[2][1]=1;
  m_texCoords[3][0]=1;m_texCoords[3][1]=1;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
teapot :: ~teapot()
{}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void teapot :: renderShape(GemState *state)
{
  GLenum type = m_drawType;
  switch(m_drawType){
  case GL_LINE_LOOP: type=GL_LINE;  break;
  case GL_POINTS   : type=GL_POINT; break;
  case GL_DEFAULT_GEM: // default
  case GL_POLYGON  : type=GL_FILL;  break;
  }
#ifdef GLU_TRUE
  switch(m_drawType){
  case GLU_LINE : type=GL_LINE;  break;
  case GLU_POINT: type=GL_POINT; break;
  case GLU_FILL : type=GL_FILL;  break;
  }
#endif

  float p[4][4][3], q[4][4][3], r[4][4][3], s[4][4][3];
  long i, j, k, l;

  TexCoord*texCoords=NULL;
  int texType=0;
  int texNum=0;
  bool lighting=false;
  state->get(GemState::_GL_TEX_COORDS, texCoords);
  state->get(GemState::_GL_TEX_TYPE, texType);
  state->get(GemState::_GL_TEX_NUMCOORDS, texNum);
  state->get(GemState::_GL_LIGHTING, lighting);

  if (texType && texNum>=4) {
    m_texCoords[0][0]=texCoords[0].s;m_texCoords[0][1]=texCoords[0].t;
    m_texCoords[1][0]=texCoords[1].s;m_texCoords[1][1]=texCoords[1].t;
    m_texCoords[2][0]=texCoords[2].s;m_texCoords[2][1]=texCoords[2].t;
    m_texCoords[3][0]=texCoords[3].s;m_texCoords[3][1]=texCoords[3].t;
  }

  glPushAttrib(GL_ENABLE_BIT | GL_EVAL_BIT);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_MAP2_VERTEX_3);
  glEnable(GL_MAP2_TEXTURE_COORD_2);
  glPushMatrix();
  glRotatef(270.0, 1.0, 0.0, 0.0);
  glScalef(0.5 * m_size, -0.5 * m_size, 0.5 * m_size);
  glTranslatef(0.0, 0.0, -1.5);
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 4; j++) {
      for (k = 0; k < 4; k++) {
        for (l = 0; l < 3; l++) {
          p[j][k][l] = teapot_cpdata[teapot_patchdata[i][j * 4 + k]][l];
          q[j][k][l] = teapot_cpdata[teapot_patchdata[i][j * 4 + (3 - k)]][l];
          if (l == 1)
            q[j][k][l] *= -1.0;
          if (i < 6) {
            r[j][k][l] =
              teapot_cpdata[teapot_patchdata[i][j * 4 + (3 - k)]][l];
            if (l == 0)
              r[j][k][l] *= -1.0;
            s[j][k][l] = teapot_cpdata[teapot_patchdata[i][j * 4 + k]][l];
            if (l == 0)
              s[j][k][l] *= -1.0;
            if (l == 1)
              s[j][k][l] *= -1.0;
          }
        }
      }
    }
    glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2,
      &m_texCoords[0][0]);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
      &p[0][0][0]);
    glMapGrid2f(m_numSlices, 0.0, 1.0, m_numSlices, 0.0, 1.0);
    glEvalMesh2(type, 0, m_numSlices, 0, m_numSlices);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
      &q[0][0][0]);
    glEvalMesh2(type, 0, m_numSlices, 0, m_numSlices);
    if (i < 6) {
      glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
        &r[0][0][0]);
      glEvalMesh2(type, 0, m_numSlices, 0, m_numSlices);
      glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
        &s[0][0][0]);
      glEvalMesh2(type, 0, m_numSlices, 0, m_numSlices);
    }
  }
  glPopMatrix();
  glPopAttrib();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void teapot :: obj_setupCallback(t_class *)
{ }
