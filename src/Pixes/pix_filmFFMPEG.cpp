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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

extern "C" {
     extern int av_mem;
     extern int av_freed;
}

#ifdef __linux__
#include "Base/config.h"
#endif /* __linux__ */

#include "Pixes/pix_filmFFMPEG.h"

#if defined(HAVE_FFMPEG)


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


CPPEXTERN_NEW_WITH_ONE_ARG(pix_filmFFMPEG, t_symbol *, A_DEFSYM)


#define POST(x, y...) 

/////////////////////////////////////////////////////////
//
// pix_filmFFMPEG
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_filmFFMPEG :: pix_filmFFMPEG(t_symbol *filename)  : 
  pix_film(filename)
{
}

void pix_filmFFMPEG :: closeMess(void)
{
   if (m_haveMovie) {
	POST("close codec");
	avcodec_close(&m_Format->streams[m_track]->codec);
	POST("close input");
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
    struct stat buf;
    POST("opening %s mem %d",filename);

    //    POST("***  Memory  alloc %d, free %d, res %d",av_mem,av_freed,av_mem-av_freed);

    stat(filename,&buf);
      
    err = av_open_input_file(&m_Format,filename,NULL,0,NULL);
    if (err < 0) {
        error("GEM: pix_film: Unable to open file: %s", filename);
        return;
    }

    POST("preread stream");
    err = av_find_stream_info(m_Format);
    if (err < 0) {
        error("pix_film: can't find stream info for %s",filename);
        goto fail;
    }

   POST("searching stream"); 
    m_numTracks = m_Format->nb_streams;

    for (i=0;i<m_Format->nb_streams;i++) { 
       codec = avcodec_find_decoder(m_Format->streams[i]->codec.codec_id);
       if (!codec) continue;
       if (m_Format->streams[i]->codec.codec_type == CODEC_TYPE_VIDEO)
          break;
    }
    POST("pixformat %d",m_Format->streams[i]->codec.pix_fmt);

    m_track = i;  // remember the stream

    if (i == m_Format->nb_streams) {
       error("pix_film: Unsupported Video Codec"); 
       goto fail;
    }   


    POST("opening codec");
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

    //    post("framsesize %f, filesize %f, frames %d",m_Format->streams[m_track]->codec.frame_size,buf.st_size,buf.st_size/m_Format->streams[m_track]->codec.frame_size);

    /* estimate number of frames */

    estimateFramesize(10);
    m_numFrames = buf.st_size/m_framesize;

    return;

   fail:
   closeMess();
}


void pix_filmFFMPEG :: estimateFramesize(int num) {
  int ret,len;
  int gotit;
  int framesize = 0;
  int count = 0;
  unsigned char* ptr;

  while (count < num) {
    if (av_read_packet(m_Format,&m_Pkt) < 0) {
      post("Can't estimate framesize");
      return;
    }
    ptr = m_Pkt.data;
    len = m_Pkt.size;

    ret = avcodec_decode_video(&m_Format->streams[m_track]->codec,
			       &m_avFrame,
			       &gotit,
			       ptr,
			       len);
    if (gotit) {    
      framesize += ret;
      count++;  
    }
  }
  m_framesize = framesize/num;
}


/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////

void pix_filmFFMPEG :: getFrame()
{
  unsigned char* ptr;
  int len;
  int i;
  int gotit = 0;
  int ret;

  // Seek

  if (m_curFrame != m_reqFrame-1) {
    int pos,err;
    POST("seeking to %d",m_reqFrame*m_framesize);
    POST("url is streamed %d",url_is_streamed(&m_Format->pb));

    pos = url_fseek(&m_Format->pb,m_reqFrame*m_framesize,SEEK_SET);
    err = av_find_stream_info(m_Format);
    POST("done pos %d %d seek %d",m_Format->pb.pos,pos,m_Format->pb.seek);

    ptr = m_Pkt.data;
    len = m_Pkt.size;
  }
  if (m_haveMovie) {
       len = m_PacketLen;
       ptr = m_PacketPtr;
       while (!gotit) {
	    if (len == 0) { 
		 POST("read packet");
		 if (av_read_packet(m_Format,&m_Pkt) < 0) {
		      // ?? TODO is this the only way to say goodbye 
		      m_numFrames = m_Format->streams[m_track]->codec.frame_number;
		      m_reqFrame = m_numFrames+1;
		      break;
		 }
		 ptr = m_Pkt.data;
		 len = m_Pkt.size;
	    }
	    POST("decode");
	    ret = avcodec_decode_video(&m_Format->streams[m_track]->codec,
				       &m_avFrame,
				       &gotit,
				       ptr,
				       len);

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
		 if (img_convert(&rgba,dstfmt,(AVPicture*)&m_avFrame,fmt,width,height)<0) 
		      post("pix_film: image conversion failed");
		 
	    }
       }
       //  post("***  Memory  alloc %d, free %d, res %d",av_mem,av_freed,av_mem-av_freed);
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
#endif // HAVE_FFMPEG
