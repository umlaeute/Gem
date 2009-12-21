////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_coordinate.h"

#include "Base/GemState.h"
#include "Base/GemPixUtil.h"

CPPEXTERN_NEW(pix_coordinate)

/////////////////////////////////////////////////////////
//
// pix_coordinate
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_coordinate :: pix_coordinate()
  : m_coords(NULL), m_rectcoords(NULL), m_numCoords(0),
    m_oldTexCoords(NULL), m_oldNumCoords(0)
{
    // the size inlet
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("coords"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_coordinate :: ~pix_coordinate()
{
    if(m_coords)delete [] m_coords;
    if(m_rectcoords)delete [] m_rectcoords;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_coordinate :: render(GemState *state)
{
  m_oldTexCoords=state->texCoords;
  m_oldNumCoords=state->numTexCoords;

  if (state->texture && m_numCoords){
    state->numTexCoords = m_numCoords;
    
    state->texCoords = m_coords;
  }
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_coordinate :: postrender(GemState *state)
{
  state->texCoords= m_oldTexCoords;
  state->numTexCoords=  m_oldNumCoords;
}

/////////////////////////////////////////////////////////
// coordsMess
//
/////////////////////////////////////////////////////////
void pix_coordinate :: coordsMess(int argc, t_atom *argv)
{
    if (argc % 2)
    {
    	error("must have a even number of coordinates");
    	return;
    }
    
    int numVals = argc / 2;
    
    if (numVals != m_numCoords)
    {
      if(m_coords)delete [] m_coords;	    m_coords = NULL;
      if(m_rectcoords)delete[]m_rectcoords; m_rectcoords=NULL;
      m_numCoords = numVals;

      if (m_numCoords == 0)return;
      m_coords = new TexCoord[m_numCoords];
      m_rectcoords=new TexCoord[m_numCoords];
    }
    
    for (int i = 0; i < numVals; i++)
    {
    	m_coords[i].s = atom_getfloat(&argv[2 * i]);
    	m_coords[i].t = atom_getfloat(&argv[2 * i + 1]);
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_coordinate :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_coordinate::coordsMessCallback,
    	    gensym("coords"), A_GIMME, A_NULL);
}
void pix_coordinate :: coordsMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->coordsMess(argc, argv);
}


