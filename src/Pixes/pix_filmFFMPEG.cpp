////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file 
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#ifdef MEMDEBUG
extern "C" {
     extern int av_mem;
     extern int av_freed;
}
#endif


#include "Base/config.h"
#include "Pixes/pix_filmFFMPEG.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_filmFFMPEG, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_filmFFMPEG
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_filmFFMPEG :: pix_filmFFMPEG(t_symbol *filename)  : 
  pix_film(filename), m_PacketLen(0)
{
  
}

void pix_filmFFMPEG :: closeMess(void)
{
   if (m_haveMovie) {
	post("close codec");
	avcodec_close(&m_Format->streams[m_track]->codec);
	post("close input");
	av_close_input_file(m_Format);
   }
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
void pix_filmFFMPEG :: realOpen(char *filename)
{
    int err;
    AVCodec* codec;
    int i;
    post("opening %s mem %d",filename);
#ifdef MEMDEBUG
    post("***  Memory  alloc %d, free %d, res %d",av_mem,av_freed,av_mem-av_freed);
#endif
    err = av_open_input_file(&m_Format,filename,NULL,0,NULL);
    if (err < 0) {
        error("GEM: pix_film: Unable to open file: %s", filename);
        return;
    }

    post("preread stream");
    err = av_find_stream_info(m_Format);
    if (err < 0) {
        error("pix_film: can't find stream info for %s",filename);
        goto fail;
    }

   post("searching stream"); 
    m_numTracks = m_Format->nb_streams;

    for (i=0;i<m_Format->nb_streams;i++) { 
       codec = avcodec_find_decoder(m_Format->streams[i]->codec.codec_id);
       if (!codec) continue;
       if (m_Format->streams[i]->codec.codec_type == CODEC_TYPE_VIDEO)
          break;
    }
    post("pixformat %d",m_Format->streams[i]->codec.pix_fmt);
    m_track = i;  // remember the stream

    if (i == m_Format->nb_streams) {
       error("pix_film: Unsupported Video Codec"); 
       goto fail;
    }   
   post("opening codec");
    err = avcodec_open(&m_Format->streams[i]->codec,codec);
    if (err < 0) {
        error("pix_film: Can't open codec");
        goto fail;
    }

    m_reqFrame = 0;
    m_curFrame = 0;

    // get all of the information about the stream

    // Get the length of the movie
    m_numFrames = -1;

    m_xsize = m_Format->streams[i]->codec.width;
    m_ysize = m_Format->streams[i]->codec.height;

    m_csize=3;
    m_format=GL_RGB;
    m_haveMovie = 2;
    
    if (m_PacketLen)
	 av_free_packet(&m_Pkt);

    m_PacketLen = 0;
    m_PacketPtr = NULL;
    m_Pkt.data = NULL;

//    outlet_float(m_outFramerate,m_Format->streams[i]->codec.frame_rate/FRAME_RATE_BASE);
    return;

   fail:
   closeMess();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_filmFFMPEG :: getFrame()
{
  UINT8* ptr;
  int len;
  int i;
  int gotit = 0;
  int ret;
  
  if (m_haveMovie) {
       len = m_PacketLen;
       ptr = m_PacketPtr;
       while (!gotit) {
	    if (len == 0) { 
		 post("read packet");
		 if (av_read_packet(m_Format,&m_Pkt) < 0) {
		      // ?? TODO is this the only way to say goodbye 
		      m_numFrames = m_Format->streams[m_track]->codec.frame_number;
		      m_reqFrame = m_numFrames+1;
		      break;
		 }
		 ptr = m_Pkt.data;
		 len = m_Pkt.size;
	    }
	    post("decode");
	    ret = avcodec_decode_video(&m_Format->streams[m_track]->codec,&m_Picture,&gotit,ptr,len);
	    if (ret < 0 ) { // TODO recover gracefully 
		 post("error while decoding");
		 break;
	    }
	    len-=ret;
	    ptr+=ret;
	    if (!len) av_free_packet(&m_Pkt);

	    if (gotit) {
		 AVPicture rgba;
		 int dstfmt = PIX_FMT_RGB24;
		 int width = m_Format->streams[m_track]->codec.width;
		 int height = m_Format->streams[m_track]->codec.height;
		 int fmt = m_Format->streams[m_track]->codec.pix_fmt;
		 
		 m_curFrame = m_Format->streams[m_track]->codec.frame_number;
		 avpicture_fill(&rgba,m_data,dstfmt,width,height);
		 if (img_convert(&rgba,dstfmt,&m_Picture,fmt,width,height)<0) 
		      post("pix_film: image conversion failed");
		 
	    }
       }
#ifdef MEMDEBUG
  post("***  Memory  alloc %d, free %d, res %d",av_mem,av_freed,av_mem-av_freed);
#endif
       m_PacketLen = len;
       m_PacketPtr = ptr;
  }
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_filmFFMPEG :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_filmFFMPEG, gensym("pix_film"), A_DEFSYM, A_NULL);
  pix_film::real_obj_setupCallback(classPtr);

  class_addmethod(classPtr, (t_method)&pix_filmFFMPEG::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmFFMPEG::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmFFMPEG::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
  av_register_all();
}

void pix_filmFFMPEG :: openMessCallback(void *data, t_symbol *filename)
{
  GetMyClass(data)->openMess(filename);
}

void pix_filmFFMPEG :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_filmFFMPEG :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=!(!(int)state);
}

