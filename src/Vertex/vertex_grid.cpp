////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) G¸nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "vertex_grid.h"

#include "Base/GemState.h"
#include "string.h"
CPPEXTERN_NEW_WITH_TWO_ARGS(vertex_grid, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// vertex_grid
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
vertex_grid :: vertex_grid(t_floatarg f1, t_floatarg f2)
{
    m_x = 4;
    m_y = 4;

  if(f1>=1.0)m_x=(int)f1;
  if(f2>=1.0)m_y=(int)f1;


    maxX = 0;
    maxY = 0;
    m_spacex = 1;
    m_spacey = 1;
    m_oldx = 0;
    m_oldy = 0;
    m_VertexArray = new float [16];
    m_ColorArray = new float [16];
    m_TexCoordArray = new float [8];
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
vertex_grid :: ~vertex_grid()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void vertex_grid :: render(GemState *state)
{
  
   int h,w,src,x,y,t;
   
  // m_x = 4;
  // m_y = 4;
  // m_spacex = 0.5f;
  // m_spacey = 0.5f;
   
   if(m_x < 1)m_x = 1;
   if(m_y < 1)m_y = 1;
  
    //texcoords s,t 0..state->texCoordX,texCoordY
    //ratios on OSX are on texcoord[1].s,t
    //
    if (maxX != state->texCoordX(1) || maxY != state->texCoordY(1)){
        //get the maximum x,y texcoord values
        maxX = state->texCoordX(1); //bottom right of image
        maxY = state->texCoordY(1);
    
        //make a ratio between the max and number of vertices on each axis for interpolation
        ratioX = maxX / m_x;
        ratioY = maxY / m_y;
        post("vertex_grid: maxX %f",maxX);
        post("vertex_grid: maxY %f",maxY);
        post("vertex_grid: ratioX %f",ratioX);
        post("vertex_grid: ratioY %f",ratioY);
        }
      
    ratioX = maxX / m_x;
    ratioY = maxY / m_y;
   if (m_x != m_oldx || m_y != m_oldy){
        
        post("vertex_grid : resizing arrays");
        m_x += 1; //to give the correct number of columns;
        delete [] m_VertexArray;
        delete [] m_ColorArray;
	delete [] m_TexCoordArray;
        m_VertexArray = new float [m_x * m_y * 4 * 2];
        m_ColorArray = new float [m_x * m_y * 4 * 2];
        m_TexCoordArray = new float [m_x * m_y * 2 * 2];
        
        m_oldx = m_x;
        m_oldy = m_y;
        post("vertex_grid : resizing arrays done");
   }
    
    //this has to do two rows at once for TRIANGLE_STRIPS to draw correctly
    src=0;
    y = 0;
    t = 0;
    
    
    
    //this could be put in a separate function and only done when the size changes
    //use memcpy() for render passes that don't change the size
    for (h=0;h<m_y;h++){
       // y = y *h;
        for(w=0;w<m_x;w++){
            m_VertexArray[src]= (w * m_spacex);
            m_VertexArray[src+1]= (h * m_spacey); 
            m_VertexArray[src+2]= 0.f;
            m_VertexArray[src+3]= 1.f;
            m_VertexArray[src+4]= (w * m_spacex);
            m_VertexArray[src+5]= (h * m_spacey + m_spacey); 
            m_VertexArray[src+6]= 0.f;
            m_VertexArray[src+7]= 1.f;
            m_ColorArray[src]= 1.f;
            m_ColorArray[src+1]= 1.f;
            m_ColorArray[src+2]= 1.f;
            m_ColorArray[src+3]= 1.f;
            m_ColorArray[src+4]= 1.f;
            m_ColorArray[src+5]= 1.f;
            m_ColorArray[src+6]= 1.f;
            m_ColorArray[src+7]= 1.f;
            src+=8;
            m_TexCoordArray[t] = (w * ratioX);
            m_TexCoordArray[t+1] = (maxY - (h * ratioY));
            m_TexCoordArray[t+2] = (w * ratioX);
            m_TexCoordArray[t+3] = ((maxY - (h * ratioY)) - ratioY);
           // post("vertex_grid:  %f %f",m_TexCoordArray[t],m_TexCoordArray[t+1]);
           // post("vertex_grid:  %f %f",m_TexCoordArray[t+2],m_TexCoordArray[t+3]);
            
            t+=4;
        }
        
    }
    //post("");
   //post("vertex_grid: m_TexCoordArray[t] %f",m_TexCoordArray[t]);
    state->VertexArray = m_VertexArray;
    state->ColorArray = m_ColorArray;
    state->TexCoordArray = m_TexCoordArray;

    state->HaveColorArray = 1;
    state->HaveTexCoordArray = 1;

    state->VertexArrayStride = 4;
    state->VertexArraySize = src / 4;
    state->drawType = GL_TRIANGLE_STRIP;
   
}
 
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void vertex_grid :: obj_setupCallback(t_class *classPtr)
{     class_addmethod(classPtr, (t_method)&vertex_grid::sizeMessCallback,
    	    gensym("size"), A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&vertex_grid::spacingMessCallback,
    	    gensym("spacing"), A_FLOAT, A_FLOAT, A_NULL);
}

void vertex_grid :: sizeMessCallback(void *data, t_floatarg x, t_floatarg y)
{
    GetMyClass(data)->m_x=((int)x);
    GetMyClass(data)->m_y=((int)y);
}

void vertex_grid :: spacingMessCallback(void *data, t_floatarg x, t_floatarg y)
{
    GetMyClass(data)->m_spacex=((float)x);
    GetMyClass(data)->m_spacey=((float)y);
}
