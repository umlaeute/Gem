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
  : m_coords(NULL), m_rectcoords(NULL), m_numCoords(0)
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
    if (state->texture && m_numCoords){
        state->numTexCoords = m_numCoords;

	if(state->texture==2 && state->image!=NULL){ 
	  // since we are using rectangle-textures (state->texture==2), 
	  // we want to scale the coordinates by the image-dimensions if they are available
	  t_float xsize = (t_float)state->image->image.xsize;
	  t_float ysize = (t_float)state->image->image.ysize;

	  for (int i = 0; i <  m_numCoords; i++)
	    {
	      m_rectcoords[i].s = xsize*m_coords[i].s;
	      m_rectcoords[i].t = ysize*m_coords[i].t;
	    }
	  state->texCoords=m_rectcoords;

	} else
        state->texCoords = m_coords;
    }
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_coordinate :: postrender(GemState *state)
{
   // state->numTexCoords = 0;
   // state->texCoords = NULL;
}

/////////////////////////////////////////////////////////
// coordsMess
//
/////////////////////////////////////////////////////////
void pix_coordinate :: coordsMess(int argc, t_atom *argv)
{
    if (argc % 2)
    {
    	post("GEM: pix_coordinate must have a even number of coordinates");
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


