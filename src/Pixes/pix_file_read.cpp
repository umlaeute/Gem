////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
// Copyright (c) 2007, Thomas Holzmann
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_file_read.h"
#include <ctype.h>

#include <stdio.h>



CPPEXTERN_NEW_WITH_ONE_ARG(pix_file_read, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_file_read
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_file_read :: pix_file_read(t_symbol *filename) :
    m_newfilm(false), fileReader(0)
{
  // setting the current frame
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("img_num"));
  // create an outlet to send out how many frames are in the movie + bang when we reached the end
  m_outNumFrames = outlet_new(this->x_obj, 0);
  m_outEnd       = outlet_new(this->x_obj, 0);
  
  /// TODO checken ob filename übergeben, wenn ja öffnen mit openMess
  //if(filename)
  //openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_file_read :: ~pix_file_read()
{
  // Clean up the movie
  closeMess();
}

/////////////////////////////////////////////////////////
// closeMess
//
/////////////////////////////////////////////////////////
void pix_file_read :: closeMess(void)
{
  if(fileReader) fileReader->closeFile();
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
void pix_file_read :: openMess(t_symbol *filename, int format, int codec)
{
  error("begin pix_file_read::open");
  
  closeMess();
  
  char tmp_buff[MAXPDSTRING];
  char *path=tmp_buff;
  // we first try to find the file-to-open with canvas_makefilename
  // if this fails, we just pass the given filename (could be a stream)
  canvas_makefilename(getCanvas(), filename->s_name, tmp_buff, MAXPDSTRING);
  
   if (FILE*fd=fopen(tmp_buff, "r")) fclose(fd);
    else path=filename->s_name;
  
  // get plugin and open file
  fileReader = &m_kernel.getFileReadServer().getPlugin();
  
  if(!(fileReader->openFile(path)))
  {
    error("GEM: pix_file_read: file cannot be loaded");
    return;
  }
  /// TODO error wenn file open nicht funktioniert hat -> dann dementsprechend reagieren
  
  error("end pix_file_read::open");
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_file_read :: render(GemState *state)
{
  error("begin pix_file_read::render");
  
  // check if image size changed
  if(m_image.image.xsize != fileReader->getWidth() ||
     m_image.image.ysize != fileReader->getHeight())
    reallocate_m_image();
  
  int frame=-1;
  
  if(!fileReader)
    return;
  
  if (fileReader->hasVideo() == false)
    return;
  
  if(m_newfilm) m_image.newfilm = true;
  m_newfilm = false;
  m_image.newimage = true;

  // get pointer to the frame data
  unsigned char *vioframe_ptr = fileReader->getFrameData();
  
  if( vioframe_ptr == NULL)
    return;
  
  // read frame data into m_image
  unsigned char *image_ptr = m_image.image.data;
  int i = m_image.image.xsize * m_image.image.ysize * m_image.image.csize ;
  while(i--)
  {
    *(image_ptr++) = *(vioframe_ptr++);
  }
  
  state->image = &m_image;
  
  error("end pix_file_read::render");
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_file_read :: postrender(GemState *state)
{
  if(!fileReader)
    return;
  
  if(fileReader->hasVideo() == false)
    return;
  
  if (state && state->image)
    state->image->newimage = 0;
}

/////////////////////////////////////////////////////////
// setPosition
//
/////////////////////////////////////////////////////////
void pix_file_read :: changeImage(int imgNum, int trackNum)
{
  if (imgNum < 0){
    error("GEM: pix_file_read: selection number must be > 0");
    imgNum=0;
  }
  if (trackNum < 0){
    error("GEM: pix_file_read: track number must be > 0");
    trackNum=0;
  }
  
  if(!fileReader) return;
  
  if (fileReader->hasVideo())
  {
    if (fileReader->setPosition(imgNum, trackNum))
      outlet_bang(m_outEnd);
    /// TODO error checking richtig implementieren (im FileRead), ob er
    ///      wohl dort noch frames hat und schon nicht im Ende
  }
}

void pix_file_read :: autoMess(t_floatarg state)
{
  if(!fileReader) return;
  
  fileReader->setAutoIncrement((float)state);
}

void pix_file_read :: reallocate_m_image()
{
  // allocate memory for m_image
  m_image.image.xsize = fileReader->getWidth();
  m_image.image.ysize = fileReader->getHeight();
  m_image.image.type = GL_UNSIGNED_BYTE; /// TODO schaun ob auf alle OS gleich ?
  m_image.image.csize = 3; /// TODO für alle colorspaces machen
  m_image.image.format = GL_RGB;
  m_image.image.reallocate();
  m_newfilm = true;
  
  t_atom ap[4];
  SETFLOAT(ap, fileReader->getNrOfFrames() );
  SETFLOAT(ap+1, fileReader->getWidth() );
  SETFLOAT(ap+2, fileReader->getHeight() );
  SETFLOAT(ap+3, (float)fileReader->getFPS() );

  post("pix_file_read: loaded file with %d frames (%dx%d) at %f fps", 
      fileReader->getNrOfFrames(), 
      fileReader->getWidth(), 
      fileReader->getHeight(), (float)fileReader->getFPS());
  outlet_list(m_outNumFrames, 0, 4, ap);
}

/////////////////////////////////////////////////////////
// colorSpace
//
/////////////////////////////////////////////////////////
void pix_file_read :: csMess(t_symbol *s)
{
  if(!fileReader) return;
  
//   char c =*s->s_name;
//   switch (c){
//     case 'g': case 'G': fileReader->setMFormat(GL_LUMINANCE, immediately); break;
//     case 'y': case 'Y': fileReader->setMFormat(GL_YCBCR_422_GEM, immediately); break;
//     case 'r': case 'R': 
//       if(gensym("RGB")==s||gensym("rgb")==s)
//         fileReader->setMFormat(GL_RGB, immediately); 
//       else
//         fileReader->setMFormat(GL_RGBA, immediately); 
//       break;
//     default:
//       error("colorspace must be 'RGBA', 'YUV' or 'Gray'");
//   
  
  /// TODO colorspace speicher in der pix_file_read klasse
  ///      dann schauen welchen colorspace das video zurückgibt
  ///      falls unterschiedlich muss video in der render methode konvertiert werden
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_file_read :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_file_read, gensym("pix_file_read"), A_DEFSYM, A_NULL);

  class_addmethod(classPtr, (t_method)&pix_file_read::openMessCallback,
                  gensym("open"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_file_read::changeImageCallback,
                  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_file_read::autoCallback,
                  gensym("auto"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_file_read::csCallback,
                  gensym("colorspace"), A_DEFSYM, A_NULL);
}

void pix_file_read :: openMessCallback(void *data, t_symbol*s,int argc, t_atom*argv)
{
  int codec=-1;
  if (!argc || argc>3)goto illegal_openmess;
  if (argv[0].a_type != A_SYMBOL)goto illegal_openmess;

  if (argc==2){
    if (argv[1].a_type == A_SYMBOL)
      GetMyClass(data)->csMess(atom_getsymbol(argv+1));
    else if (argv[1].a_type == A_FLOAT)
      codec=atom_getint(argv+1);
  } else if (argc==3){
    if ((argv[1].a_type == A_SYMBOL) || (argv[2].a_type == A_FLOAT)) {
      GetMyClass(data)->csMess(atom_getsymbol(argv+1));
      codec=atom_getint(argv+2);    
    } else if ((argv[2].a_type == A_SYMBOL) || (argv[1].a_type == A_FLOAT)) {
      GetMyClass(data)->csMess(atom_getsymbol(argv+2));
      codec=atom_getint(argv+1);  
    }
  }
  
  GetMyClass(data)->openMess(atom_getsymbol(argv), 0, codec);
  return;
 
illegal_openmess:
     GetMyClass(data)->error("open <filename> [<format>] [<preferred codec#>]");
 return;
}

void pix_file_read :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
  GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_file_read :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->autoMess(state);
}

void pix_file_read :: csCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->csMess(s);
}
