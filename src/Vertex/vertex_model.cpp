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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "vertex_model.h"

#include "Base/GemState.h"
#include "string.h"
#ifdef __APPLE__
#include <AGL/agl.h>
extern bool HaveValidContext (void);
#endif

#define T(x) (m_model->triangles[(x)])

CPPEXTERN_NEW(vertex_model)

/////////////////////////////////////////////////////////
//
// vertex_model
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
vertex_model :: vertex_model()
{
    m_VertexArray = new float [16];
    m_ColorArray = new float [16];
    m_TexCoordArray = new float [16];
    m_NormalArray = new float [16];
    m_tempTA = new float [16];
    m_tempVA = new float [16];
    m_tempCA = new float [16];
    m_tempNA = new float [16];
    m_vertcount = 4;
    m_rescaleModel = 1;
    m_haveModel = 0;
    m_oldVSize = m_oldCSize = 0;
    maxX = 1;
    maxY = 1;
    oldmaxX = 1;
    oldmaxY = 1;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
vertex_model :: ~vertex_model()
{ }

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
void vertex_model :: openMess(t_symbol *filename)
{

GLMgroup*  	group;
GLMtriangle*  	triangle;
float*	tritext;  //texture coords for a triangle
float*	trivert;  //vertices of one point in a triangle
float*  trinorm;
 int src2,src3, src4,numvertices;
 unsigned int i;
m_haveModel = 0;
#ifdef __APPLE__
  if (!HaveValidContext ()) {
    post("GEM: geo: model - need window to load model");
    return;
  }
#endif
  //cleanModel();
 
  char buf[MAXPDSTRING];
  canvas_makefilename(getCanvas(), filename->s_name, buf, MAXPDSTRING);
  // read the object in
  m_model = glmReadOBJ(buf);

  if (!m_model) return;    

  // set the size to -1 to 1
  if (m_rescaleModel)
    glmUnitize(m_model);

  group = m_model->groups;
  glmFacetNormals (m_model);
  glmVertexNormals(m_model, 90);
  glmLinearTexture(m_model);
 
  post("vertex_model: model->numtriangles %d",m_model->numtriangles);
  post("vertex_model: model->numgroups %d",m_model->numgroups);
  post("vertex_model: model->numvertices %d",m_model->numvertices);
  post("vertex_model: model->numnormals %d",m_model->numnormals);
  post("vertex_model: model->numtexcoords %d",m_model->numtexcoords);
 
  numvertices = (int)(m_model->numtriangles * m_model->numgroups * 3);
  m_vertcount = numvertices;

  // would it be a bad idea to make all arrays equally sized ?
  // (fill up the missing elements with zeros

  delete [] m_VertexArray;
  m_VertexArray = new float[numvertices * 4];  // x, y, z, w
  delete [] m_ColorArray;
  m_ColorArray = new float[numvertices * 4];   // r, g, b, a
  delete [] m_TexCoordArray;
  m_TexCoordArray = new float[numvertices * 2];// u, v
  delete [] m_NormalArray;
  m_NormalArray = new float[numvertices * 3];  // x, y, z

  delete [] m_tempVA;
  m_tempVA = new float[numvertices * 4];
  delete [] m_tempCA;
  m_tempCA = new float[numvertices * 4];
  delete [] m_tempTA;
  m_tempTA = new float[numvertices * 2];
  delete [] m_tempNA;
  m_tempNA = new float[numvertices * 3];
  
  src2 = 0;
  src3 = 0;
  src4 = 0;
  while(group){
    for (i = 0; i < group->numtriangles; i++) {
        triangle = &T(group->triangles[i]);
        trivert = &m_model->vertices[3 * triangle->vindices[0]];
        m_VertexArray[src4] = trivert[0];
        m_VertexArray[src4+1] = trivert[1];
        m_VertexArray[src4+2] = trivert[2];
        m_VertexArray[src4+3] = 1;
        m_ColorArray[src4] = 1;
        m_ColorArray[src4+1] = 1;
        m_ColorArray[src4+2] = 1;
        m_ColorArray[src4+3] = 1;
        tritext = &m_model->texcoords[2 * triangle->tindices[0]];
        m_TexCoordArray[src2] = tritext[0];
        m_TexCoordArray[src2+1] = tritext[1];
        trinorm = &m_model->normals[3 * triangle->nindices[0]];
        m_NormalArray[src3] = trinorm[0];
        m_NormalArray[src3+1] = trinorm[1];
        m_NormalArray[src3+2] = trinorm[2];
        src3 += 3;
        src2 += 2;
        src4 += 4;
        trivert = &m_model->vertices[3 * triangle->vindices[1]];
        m_VertexArray[src4] = trivert[0];
        m_VertexArray[src4+1] = trivert[1];
        m_VertexArray[src4+2] = trivert[2];
        m_VertexArray[src4+3] = 1;
        m_ColorArray[src4] = 1;
        m_ColorArray[src4+1] = 1;
        m_ColorArray[src4+2] = 1;
        m_ColorArray[src4+3] = 1;
        tritext = &m_model->texcoords[2 * triangle->tindices[1]];
        m_TexCoordArray[src2] = tritext[0];
        m_TexCoordArray[src2+1] = tritext[1];
        trinorm = &m_model->normals[3 * triangle->nindices[1]];
        m_NormalArray[src3] = trinorm[0];
        m_NormalArray[src3+1] = trinorm[1];
        m_NormalArray[src3+2] = trinorm[2];
        src3 += 3;
        src2 += 2;
        src4 += 4;
        trivert = &m_model->vertices[3 * triangle->vindices[2]];
        m_VertexArray[src4] = trivert[0];
        m_VertexArray[src4+1] = trivert[1];
        m_VertexArray[src4+2] = trivert[2];
        m_VertexArray[src4+3] = 1;
        m_ColorArray[src4] = 1;
        m_ColorArray[src4+1] = 1;
        m_ColorArray[src4+2] = 1;
        m_ColorArray[src4+3] = 1;
        tritext = &m_model->texcoords[2 * triangle->tindices[2]];
        m_TexCoordArray[src2] = tritext[0];
        m_TexCoordArray[src2+1] = tritext[1];
        trinorm = &m_model->normals[3 * triangle->nindices[2]];
        m_NormalArray[src3] = trinorm[0];
        m_NormalArray[src3+1] = trinorm[1];
        m_NormalArray[src3+2] = trinorm[2];
        src3 += 3;
        src2 += 2;
        src4 += 4;
        
        }
    group = group->next;
   }
  
  src4 = src4/4;
  src2 = src2/4;
  post("vertex_model: i %d",i);
  post("vertex_model: src2 %d",src2);
  post("vertex_model: src4 %d",src4);

  m_haveModel = 1;
  setModified();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void vertex_model :: render(GemState *state)
{

    int size,i,src,length;

    if (!m_haveModel) return;  
    //this will get the appropriate maximum texcoord size for use with the model
    src = 0;
    
    
        
    size = m_vertcount * 4 * 4;
   // post("vertex_model: m_vertcount %d",m_vertcount);
    memcpy(m_tempVA, m_VertexArray, size);
    memcpy(m_tempCA, m_ColorArray, size); 
    size = m_vertcount * 4 * 3;
    memcpy(m_tempNA, m_NormalArray, size);
    /*
    if (state->numTexCoords) {
    
        if (maxX != state->texCoordX(1) || maxY != state->texCoordY(1)){
            post("vertex_model: changing texcoords");
            
            post("vertex_model: state->texCoordX(1) %f",state->texCoordX(1));
            post("vertex_model: state->texCoordY(1) %f",state->texCoordY(1));
            maxX = state->texCoordX(1);
            maxY = state->texCoordY(1);
            post("vertex_model: maxX %f",maxX);
            post("vertex_model: maxY %f",maxY);
            //resize the texcoords
            //length = sizeof(m_TexCoordArray)/2;
            length = m_vertcount;
            post("vertex_model: length %d",length);
            post("vertex_model: m_tempTA[0] start %f",m_TexCoordArray[0]);
            post("vertex_model: m_tempTA[1] start %f",m_TexCoordArray[1]);
            post("vertex_model: m_tempTA[2] start %f",m_TexCoordArray[2]);
            post("vertex_model: m_tempTA[length] start %f",m_TexCoordArray[length]);
            for(i=0; i < length; i++){
                m_TexCoordArray[src] = m_TexCoordArray[src] * maxX;
                m_TexCoordArray[src+1] = m_TexCoordArray[src+1] * maxY;
                src+=2;
                }
            post("vertex_model: src %d",src);
            post("vertex_model: m_tempTA[0] end %f",m_TexCoordArray[0]);
            post("vertex_model: m_tempTA[1] end %f",m_TexCoordArray[1]);
            post("vertex_model: m_tempTA[src] end %f",m_TexCoordArray[src]);
            }
        } */
    
    size = m_vertcount * 4 * 2;
   // memcpy(m_tempTA, m_TexCoordArray, size); 
    
     if (state->numTexCoords) {
    
        if (maxX != state->texCoordX(1) || maxY != state->texCoordY(1)){
         //   post("vertex_model: changing texcoords");
            
         //   post("vertex_model: state->texCoordX(1) %f",state->texCoordX(1));
        //    post("vertex_model: state->texCoordY(1) %f",state->texCoordY(1));
            maxX = state->texCoordX(1);
            maxY = state->texCoordY(1);
        //    post("vertex_model: maxX %f",maxX);
        //    post("vertex_model: maxY %f",maxY);
            //resize the texcoords
            //length = sizeof(m_TexCoordArray)/2;
            length = m_vertcount;
        //    post("vertex_model: length %d",length);
        //    post("vertex_model: m_tempTA[0] start %f",m_tempTA[0]);
        //    post("vertex_model: m_tempTA[1] start %f",m_tempTA[1]);
        //    post("vertex_model: m_tempTA[2] start %f",m_tempTA[2]);
        //    post("vertex_model: m_tempTA[length] start %f",m_tempTA[length]);
        //can this be unrolled??
            for(i=0; i < length; i++){
                m_tempTA[src] = m_TexCoordArray[src] * maxX;
                m_tempTA[src+1] = m_TexCoordArray[src+1] * maxY;
                src+=2;
                }
        //    post("vertex_model: src %d",src);
        //    post("vertex_model: m_tempTA[0] end %f",m_tempTA[0]);
        //    post("vertex_model: m_tempTA[1] end %f",m_tempTA[1]);
        //    post("vertex_model: m_tempTA[src] end %f",m_tempTA[src]);
        //    }
        }
        }else{
            memcpy(m_tempTA, m_TexCoordArray, size); 
            }
    
           
    state->VertexArray = m_tempVA;
    state->ColorArray = m_tempCA;
    state->TexCoordArray = m_tempTA;
    state->NormalArray = m_tempNA;
    state->HaveColorArray = 1;
    state->HaveTexCoordArray = 1;
    state->HaveNormalArray = 1;
    state->VertexArrayStride = 4;
    state->VertexArraySize = m_vertcount;
      
    
}
 
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void vertex_model :: obj_setupCallback(t_class *classPtr)
{       class_addmethod(classPtr, (t_method)&vertex_model::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
}

void vertex_model :: openMessCallback(void *data, t_symbol *filename)
{
  GetMyClass(data)->openMess(filename);
}
