////////////////////////////////////////////////////////
//
// pix_hit
//
// hit test over user defined hit_areas...
// 
// Author: Davide Morelli
// http://ww.davidemorelli.it
//
/////////////////////////////////////////////////////////

#include "pix_hit.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_hit,  t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_hit
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_hit :: pix_hit(t_symbol*s) :
  minimum_treshold(DEF_TRESHOLD),
  minimum_pixels(DEF_MINIMUM),
  min_distance(DEF_MIN_DISTANCE),
  show(false)
{
  //inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("symbol"), gensym("pix_hit"));
  // create the new inlet for the messages
  //inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("messages"));
  m_hits = outlet_new(this->x_obj, 0);
  // init the array of active hit_areas
  for (int i=0; i<NUM_hit_areas; i++)
    {
      area_active[i]=0;
      buffer[i]=0;
    }

  if((NULL!=s) && (&s_!=s) && (NULL!=s->s_name))
    load(s->s_name);
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_hit :: ~pix_hit()
{
  outlet_free(m_hits);
}

float pix_hit :: getGreyValue(imageStruct &image, unsigned char *data, float *color, int xPos)
{
  float grayVal = 0.f;

  // is this a gray8 or RGBA?
  switch(image.format)
    {
      // Gray scale
    case(GL_LUMINANCE):
      grayVal=data[chGray] / 256.f;
      color[0] = color[1] = color[2] = grayVal;
      
      break;
      // YUV
    case(GL_YCBCR_422_GEM):

      if ( xPos % 2) { //odd
	color[0] = data[1];
	color[1] = data[-2];
	color[2] = data[0];
      }else{
        color[0] = data[1];
        color[1] = data[0];
        color[2] = data[2];
      }
      grayVal=color[0];
      break;
      // RGB, RGBA
    case(GL_RGB): case (GL_RGBA):
      color[0] = data[chRed] / 256.f;
      color[1] = data[chGreen] / 256.f;
      color[2] = data[chBlue] / 256.f;
      grayVal=(color[0] * 0.3086f + color[1] * 0.06094f + color[2] * 0.0820f);
      break;
    default :
      error("GEM: pix_data: unknown image format");
      return 0;
      //			break;
    }
  // float grayVal = (color[0] + color[1] + color[2]) / 3.f;
  return grayVal;
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_hit :: processImage(imageStruct &image)
{
  /* for each active rectangle test every pixel inside
     and if the number of pixles with grey value is > minimum_treshold  
     is > minimum_pixles send this rectangle id to the outlet 
     which means there has been a hit on that rectangle */
	
  unsigned char *src=image.data;
  unsigned char data[3];
  float color[3];
  int pixels_found;
  bool go_on;

  for (int i=0; i<NUM_hit_areas; i++)
    {
      if (area_active[i])
        {
          switch (hit_areas[i].type)
            {
            case point:
              {
                int xPos = (int)(hit_areas[i].x * (float)image.xsize);
                int yPos = (int)(hit_areas[i].y * (float)image.ysize);
                int position = yPos * image.xsize * image.csize +
                  xPos * image.csize;
                data[0] = src[position];
                data[1] = src[position+1];
                data[2] = src[position+2];
                float grayVal = getGreyValue(image, data, color, xPos);
                if (grayVal >= minimum_treshold)
                  {
                    // hit!
                    if (buffer[i]==0)
                      {
                        // TODO: build list with |i 1(
                        t_atom atom[2];
                        SETFLOAT(&atom[0], (t_int)i);
                        SETFLOAT(&atom[1], (t_int)1);
                        outlet_list(m_hits, gensym("list"), 2, atom);
                        buffer[i]=1;
                      }
                  } else
                  {
                    if (buffer[i]==1)
                      {
                        // TODO: build list with |i 0(
                        t_atom atom[2];
                        SETFLOAT(&atom[0], (t_int)i);
                        SETFLOAT(&atom[1], (t_int)0);
                        outlet_list(m_hits, gensym("list"), 2, atom);
                        buffer[i]=0;
                      }
                  }
                break;
              }
            case rectangle:
              {
                int xPos = (int)(hit_areas[i].x * (float)image.xsize);
                int yPos = (int)(hit_areas[i].y * (float)image.ysize);
                int Width = (int)(hit_areas[i].width * (float)image.xsize);
                int Height = (int)(hit_areas[i].height * (float)image.ysize);
					
                go_on=true;
                bool hit=false;
                pixels_found=0;

                // for each line in this rectangle..
                for (int this_y = yPos; go_on && (this_y < yPos+Height) && (this_y<image.ysize); this_y++)
                  {
                    // for each pixle in this line
                    for (int this_x = xPos; go_on && (this_x < xPos + Width) && (this_x < image.xsize); this_x++)
                      {
                        // get this pixel
                        int position = (this_y * image.xsize + this_x) * image.csize;
                        data[0] = src[position];
                        data[1] = src[position+1];
                        data[2] = src[position+2];
                        if (show) {
                          // src[position]=src[position+1]=src[position+2] = 0;
                          src[position+0]=~src[position+0];
                          src[position+1]=~src[position+1];
                          src[position+2]=~src[position+2];
                        }
                        // get grey val
                        float grayVal = getGreyValue(image, data, color, xPos);
                        if (grayVal >= minimum_treshold)
                          {
                            pixels_found++;
                            if (pixels_found>=minimum_pixels)
                              {
                                // hit!
                                hit=true;
                                // exit the loop..
                                go_on=false;
                              }
                          }
                      }
                  }
                if (hit)
                  {
                    if (buffer[i]==0)
                      {
                        buffer[i]=1;
                        // TODO: build list with |i 1(
                        t_atom atom[2];
                        SETFLOAT(&atom[0], (t_int)i);
                        SETFLOAT(&atom[1], (t_int)1);
                        outlet_list(m_hits, gensym("list"), 2, atom);
                      }
                  } else
                  {
                    if (buffer[i]==1)
                      {
                        buffer[i]=0;
                        // TODO: build list with |i 0(
                        t_atom atom[2];
                        SETFLOAT(&atom[0], (t_int)i);
                        SETFLOAT(&atom[1], (t_int)0);
                        outlet_list(m_hits, gensym("list"), 2, atom);
                      }
                  }
                break;
              }
            case circle:
              //todo
              break;
            case line:
              {
                // i need to get the line equation f(x)
                // then 
                // for each x bewteen x1 and x2
                // get y=f(x)
                // and do the hit test in x,y
                // if grayValue > treshold
                // HIT!

                /*
                  equation of the line passing from p1 and p2:
                  (y-y1)/(y2-y1)=(x-x1)/(x2-x1)
                  so 
                  y = (x-x1)(y2-y1)/(x2-x1) + y1
                  and 
                  x = (y-y1(x2-x1))/(y2-y1) - x1
                */
                int x1 = (int)(hit_areas[i].x * (float)image.xsize);
                int y1 = (int)(hit_areas[i].y * (float)image.ysize);
                int x2 = (int)(hit_areas[i].width * (float)image.xsize);
                int y2 = (int)(hit_areas[i].height * (float)image.ysize);					
					
                int diffx = abs(x2-x1);
                int diffy = abs(y2-y1);

                int counter=0;

                if ((diffx==diffy)&&(diffx==0))
                  break;//faulty line
                if (diffx>diffy)
                  {
                    // x leads the game
                    // y = (x-x1)(y2-y1)/(x2-x1) + y1
                    int incr = 1;
                    if ((x2-x1)<0)
                      incr = -1;
                    int x=x1; 
                    while (x!=x2)
                      {
                        // NB diffx can't be == 0
                        // (i would not be here)
                        int y = (x-x1)*(y2-y1)/(x2-x1) + y1;
                        int position = y * image.xsize * image.csize +
                          x * image.csize;
                        data[0] = src[position];
                        data[1] = src[position+1];
                        data[2] = src[position+2];
                        if (show){
                          // src[position]=src[position+1]=src[position+2] = 0;
                          src[position+0]=~src[position+0];
                          src[position+1]=~src[position+1];
                          src[position+2]=~src[position+2];
                        }
                        float grayVal = getGreyValue(image, data, color, x);
                        if (grayVal >= minimum_treshold)
                          {
                            float where = (float) ((float)counter/(float)diffx);
                            // hit!
                            if (fabs(buffer[i] - where) > min_distance)
                              {
									
                                t_atom atom[2];
                                SETFLOAT(&atom[0], (t_int)i);
                                SETFLOAT(&atom[1], (t_float) where);
                                outlet_list(m_hits, gensym("list"), 2, atom);
                                buffer[i]=where;
                              }
                          } 
                        counter++;
                        x+=incr;
                      }
                  } else
                  {
                    // y rulez
                    // x = (y-y1(x2-x1))/(y2-y1) + x1
                    int incr = 1;
                    if ((y2-y1)<0)
                      incr = -1;
                    int y=y1; 
                    while (y!=y2)
                      {
                        // NB diffy can't be == 0
                        // (i would not be here)
                        int x = (y-y1)*(x2-x1)/(y2-y1) + x1;
                        int position = y * image.xsize * image.csize +
                          x * image.csize;
                        data[0] = src[position];
                        data[1] = src[position+1];
                        data[2] = src[position+2];
                        if (show){
                          // src[position]=src[position+1]=src[position+2] = 0;
                          src[position+0]=~src[position+0];
                          src[position+1]=~src[position+1];
                          src[position+2]=~src[position+2];
                        }
                        float grayVal = getGreyValue(image, data, color, x);
                        if (grayVal >= minimum_treshold)
                          {
                            float where = (float) ((float)counter/(float)diffy);
                            // hit!
                            if (fabs(buffer[i] - where) > min_distance)
                              {
									
                                t_atom atom[2];
                                SETFLOAT(&atom[0], (t_int)i);
                                SETFLOAT(&atom[1], (t_float) where);
                                outlet_list(m_hits, gensym("list"), 2, atom);
                                buffer[i]=where;
                              }
                          } 
                        counter++;
                        y+=incr;
                      }
                  }

                break;
              }
            }
        }
    }
}



void pix_hit :: threshold(float thresh)
{
  if ((thresh<0) || (thresh>1))
    {
      error("threshold must be a float between 0 and 1");
      return;
    }
  minimum_treshold = thresh;
}

void pix_hit :: minimum(int min)
{
  if (min<=0)
    {
      error("min must be an integer > than 0 !");
      return;
    }
  minimum_pixels = min;
}

void pix_hit :: set_min_distance(float min)
{
  if ((min<0)||(min>1))
    {
      error("min_distance must be a float between 0 and 1!");
      return;
    }
  min_distance = min;
}

void pix_hit :: set_show(int val)
{
  show=(val>0);
}

void pix_hit :: createPoint(int n, float x, float y)
{
  if ((n<0) || (n>NUM_hit_areas-1))
    {
      error("the id must be an integer between 0 and %i",NUM_hit_areas);
      return;
    }
  if ((x<0) || (x>1) || (y<0) || (y>1))
    {
      error("invalid input coordinates");
      return;
    }
  hit_areas[n].x = x;
  hit_areas[n].y = y;
  hit_areas[n].type = point;
  area_active[n]=true;

}

void pix_hit :: createRectangle(int n, float x, float y, float w, float h)
{
  if ((n<0) || (n>NUM_hit_areas-1))
    {
      error("the id must be an integer between 0 and %i",NUM_hit_areas);
      return;
    }
  if ((x<0) || (x>1) || (y<0) || (y>1) || (w<0) || (w>1) || (h<0) || (h>1))
    {
      error("invalid input coordinates");
      return;
    }
  hit_areas[n].type = rectangle;
  hit_areas[n].x = x;
  hit_areas[n].y = y;
  hit_areas[n].width = w;
  hit_areas[n].height = h;
  area_active[n]=true;
}

void pix_hit :: createCircle(int n, float x, float y, float r)
{
  if ((n<0) || (n>NUM_hit_areas-1))
    {
      error("the id must be an integer between 0 and %i",NUM_hit_areas);
      return;
    }
  if ((x<0) || (x>1) || (y<0) || (y>1) || (r<0) || (r>1))
    {
      error("invalid input coordinates");
      return;
    }
  hit_areas[n].x = x;
  hit_areas[n].y = y;
  hit_areas[n].width = r;
  hit_areas[n].type = circle;
  area_active[n]=true;

  post("WARNING: circles not implemented.. yet.");
}

void pix_hit :: createLine(int n, float x1, float y1, float x2, float y2)
{
  if ((n<0) || (n>NUM_hit_areas-1))
    {
      error("the id must be an integer between 0 and %i",NUM_hit_areas);
      return;
    }
  if ((x1<0) || (x1>1) || (y1<0) || (y1>1) || (x2<0) || (x2>1) || (y2<0) || (y2>1))
    {
      error("invalid input coordinates");
      return;
    }
  hit_areas[n].type = line;
  hit_areas[n].x = x1;
  hit_areas[n].y = y1;
  hit_areas[n].width = x2;
  hit_areas[n].height = y2;
  area_active[n]=true;

}
void pix_hit :: move(int n, float x, float y, float w, float h)
{
  if ((n<0) || (n>NUM_hit_areas-1))
    {
      error("the id must be an integer between 0 and %i", NUM_hit_areas);
      return;
    }
  hit_areas[n].x = x;
  hit_areas[n].y = y;
  hit_areas[n].width = w;
  hit_areas[n].height = h;
  area_active[n]=true;
}

void pix_hit :: del(int n)
{
  if ((n<0) || (n>NUM_hit_areas-1))
    {
      error("the id must be an integer between 0 and %i",NUM_hit_areas);
      return;
    }
  hit_areas[n].x = 0;
  hit_areas[n].y = 0;
  hit_areas[n].width = 0;
  hit_areas[n].height = 0;
  area_active[n]=false;
  post("deleted area %i", n);
}

void pix_hit :: save(char *filename)
{
  FILE *fp;
  fp=fopen(filename, "w+b");
  int tmp_byte;

  for (int m=0; m<NUM_hit_areas; m++)
    {
      if (area_active[m])
        {
          post("saving index %i", m);
          fputc(1, fp); // first byte tells us if area is active or not
          unsigned short int tipo;
          switch (hit_areas[m].type)
            {
            case point:
              tipo=0;
              break;
            case rectangle:
              tipo=1;
              break;
            case circle:
              tipo=2;
              break;
            case line:
              tipo=3;
            }
          // scrivi tutti i campi di hit_areas[i]
          unsigned short int tmp[5];
          tmp[0] = tipo;
          tmp[1] = (short) (hit_areas[m].x * 65535);
          tmp[2] = (short) (hit_areas[m].y * 65535);
          tmp[3] = (short) (hit_areas[m].width * 65535);
          tmp[4] = (short) (hit_areas[m].height * 65535);
          for (int i=0; i<5; i++)
            {
              // this should avoids problems little/big endian
              // i force little endian (most significant byte first)
              fputc(tmp[0]>>8, fp);
              tmp_byte = tmp[0]<<8;
              tmp_byte = tmp_byte >> 8;
              fputc(tmp_byte, fp);
            }
        } else
        {

          fputc(0, fp);
          // scrivi tanti 0 quanti sono i campi di hit_areas[i]
          for (int i=0; i<5; i++)
            {
              fputc(0, fp);
              fputc(0, fp);
            }
        }
    }
  if (DEBUG)
    post("areas wrote to file %s",filename);
  fclose(fp);
}

void pix_hit :: load(char *filename)
{
  FILE *fp;
  int i, j, m;
  unsigned char tmp[2];
  if ((fp=fopen(filename, "r+b"))==NULL)
    {
      post("error: can't open file %s", filename);
      return;
    }
	
  for (m=0; m<NUM_hit_areas; m++)
    {
		
      area_active[m] = (fgetc(fp) > 0 ? true : false);
      unsigned short int tmp_x, tmp_y, tmp_w, tmp_h;
      // this should avoids problems little/big endian
      // i force little endian (most significant byte first)
      tmp[0] = fgetc(fp);
      tmp[1] = fgetc(fp);
      unsigned short int tipo = (tmp[0]<<8)|(tmp[1]);
      switch (tipo)
        {
        case 0:
          hit_areas[m].type = point;
          break;
        case 1:
          hit_areas[m].type = rectangle;
          break;
        case 2:
          hit_areas[m].type = circle;
          break;
        case 3:
          hit_areas[m].type = line;
        }
      tmp[0] = fgetc(fp);
      tmp[1] = fgetc(fp);
      tmp_x = (tmp[0]<<8)|(tmp[1]);
      hit_areas[m].x = (float) ((float) tmp_x / (float)65535);
      tmp[0] = fgetc(fp);
      tmp[1] = fgetc(fp);
      tmp_y = (tmp[0]<<8)|(tmp[1]);
      hit_areas[m].y = (float) ((float) tmp_y / (float)65535);
      tmp[0] = fgetc(fp);
      tmp[1] = fgetc(fp);
      tmp_w = (tmp[0]<<8)|(tmp[1]);
      hit_areas[m].width = (float) ((float) tmp_w / (float)65535);
      tmp[0] = fgetc(fp);
      tmp[1] = fgetc(fp);
      tmp_h = (tmp[0]<<8)|(tmp[1]);
      hit_areas[m].height = (float) ((float) tmp_h / (float)65535);
      if (area_active[m])
        post("loading index %i hit_areas[m].x=%f hit_areas[m].y=%f", m, hit_areas[m].x, hit_areas[m].y);
    }
  if (DEBUG)
    post("areas from file %s",filename);
  fclose(fp);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_hit :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_hit::thresholdCallback,
                  gensym("threshold"), A_FLOAT, A_NULL); 
  class_addmethod(classPtr, (t_method)&pix_hit::minimumCallback,
                  gensym("min"), A_FLOAT, A_NULL); 
  class_addmethod(classPtr, (t_method)&pix_hit::min_distanceCallback,
                  gensym("min_distance"), A_FLOAT, A_NULL); 
  class_addmethod(classPtr, (t_method)&pix_hit::createPointCallback,
                  gensym("point"), A_GIMME, A_NULL); 
  class_addmethod(classPtr, (t_method)&pix_hit::createRectangleCallback,
                  gensym("rectangle"), A_GIMME, A_NULL); 
  class_addmethod(classPtr, (t_method)&pix_hit::createCircleCallback,
                  gensym("circle"), A_GIMME, A_NULL); 
  class_addmethod(classPtr, (t_method)&pix_hit::createLineCallback,
                  gensym("line"), A_GIMME, A_NULL); 
  class_addmethod(classPtr, (t_method)&pix_hit::moveCallback,
                  gensym("move"), A_GIMME, A_NULL); 
  class_addmethod(classPtr, (t_method)&pix_hit::deleteCallback,
                  gensym("delete"), A_FLOAT, A_NULL); 
  class_addmethod(classPtr, (t_method)&pix_hit::saveCallback,
                  gensym("save"), A_SYMBOL, A_NULL); 
  class_addmethod(classPtr, (t_method)&pix_hit::loadCallback,
                  gensym("load"), A_SYMBOL, A_NULL); 
  class_addmethod(classPtr, (t_method)&pix_hit::showCallback,
                  gensym("show"), A_FLOAT, A_NULL); }

void pix_hit :: thresholdCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->threshold(FLOAT_CLAMP((float)val));
}

void pix_hit :: minimumCallback(void *data, t_floatarg val)
{
  int n = (int) FLOAT_CLAMP((float)val);
  GetMyClass(data)->minimum(n);
}

void pix_hit :: min_distanceCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->set_min_distance((float) FLOAT_CLAMP((float)val));
}

void pix_hit :: createLineCallback(void *data, t_symbol *sl, int argc, t_atom *argv)
{
  int n;
  float x,y,w,h;
  // check for correct number of floats...
  n = atom_getint(argv++);
  x = atom_getfloat(argv++);
  y = atom_getfloat(argv++);
  w = atom_getfloat(argv++);
  h = atom_getfloat(argv++);

  GetMyClass(data)->createLine(n,x,y,w,h);
}

void pix_hit :: createPointCallback(void *data, t_symbol *sl, int argc, t_atom *argv)
{
  int n;
  float x,y;
  // check for correct number of floats...
  n = atom_getint(argv++);
  x = atom_getfloat(argv++);
  y = atom_getfloat(argv++);


  GetMyClass(data)->createPoint(n,x,y);
}

void pix_hit :: createRectangleCallback(void *data, t_symbol *sl, int argc, t_atom *argv)
{
  int n;
  float x,y,w,h;
  // check for correct number of floats...
  n = atom_getint(argv++);
  x = atom_getfloat(argv++);
  y = atom_getfloat(argv++);
  w = atom_getfloat(argv++);
  h = atom_getfloat(argv++);

  GetMyClass(data)->createRectangle(n,x,y,w,h);
}

void pix_hit :: createCircleCallback(void *data, t_symbol *sl, int argc, t_atom *argv)
{
  int n;
  float x,y,r;
  // check for correct number of floats...
  n = atom_getint(argv++);
  x = atom_getfloat(argv++);
  y = atom_getfloat(argv++);
  r = atom_getfloat(argv++);


  GetMyClass(data)->createCircle(n,x,y,r);
}

void pix_hit :: moveCallback(void *data, t_symbol *sl, int argc, t_atom *argv)
{
  int n;
  float x,y,w,h;
  // check for correct number of floats...
  n = atom_getint(argv++);
  x = atom_getfloat(argv++);
  y = atom_getfloat(argv++);
  w = atom_getfloat(argv++);
  h = atom_getfloat(argv++);

  GetMyClass(data)->move(n,x,y,w,h);
}

void pix_hit :: deleteCallback(void *data, t_floatarg id)
{
  //int n = (int) FLOAT_CLAMP((float)id);
  //post("n=%i id =%f", n, id);
  GetMyClass(data)->del((int)id);
}

void pix_hit :: saveCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->save(s->s_name);
}

void pix_hit :: loadCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->load(s->s_name);
}

void pix_hit :: showCallback(void *data, t_floatarg val)
{
  //int n = (int) FLOAT_CLAMP((float)id);
  //post("n=%i id =%f", n, id);
  GetMyClass(data)->set_show((int)val);
}

