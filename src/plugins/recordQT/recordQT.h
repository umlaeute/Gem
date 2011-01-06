/*
 *  recordQT.h
 *  GEM_darwin
 *
 *  Created by chris clepper on 7/18/05.
 *  Copyright 2005. All rights reserved.
 *
 */
 
//this will record QT movies
#ifndef INCLUDE_PLUGINS_RECORDQT_H_
#define INCLUDE_PLUGINS_RECORDQT_H_
#include "plugins/record.h"



#if defined __APPLE__ 
# if !defined __x86_64__
// with OSX10.6, apple has removed loads of Carbon functionality (in 64bit mode)
// LATER make this a real check in configure
#  define HAVE_CARBONQUICKTIME
# elif defined HAVE_QUICKTIME
#  undef HAVE_QUICKTIME
# endif
#endif


#define QT_MAX_FILENAMELENGTH 256

#if defined HAVE_QUICKTIME
# if defined _WIN32 
#  include <QTML.h>
#  include <Movies.h>
#  include <QuicktimeComponents.h>
# endif
# ifdef __APPLE__
#  include <QuickTime/QuickTime.h>
# endif // __APPLE__
#endif


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  recordQT
    
  Writes a pix of the render buffer
    
  KEYWORDS
  pix
    
  DESCRIPTION

  Inlet for a list - "vert_size"
  Inlet for a list - "vert_pos"

  "file" - filename to write to
  "bang" - do write now
  "auto 0/1" - stop/start writing automatically

  "vert_size" - Set the size of the pix
  "vert_pos" - Set the position of the pix
    
  -----------------------------------------------------------------*/
namespace gem { class GEM_EXPORT recordQT : public record
{
 public:

  //////////
  // Constructor
  recordQT();

  //////////
  // Destructor
  virtual ~recordQT();
  
#ifdef HAVE_QUICKTIME
  virtual void close();
  virtual bool open(const std::string filename);
    	
  //////////
  // Do the rendering
  virtual bool putFrame(imageStruct*img);
 
  ////////
  // call up compression dialog
  virtual bool	dialog();
  virtual int	getNumCodecs();
  virtual const char* getCodecName(int n);
  virtual bool	setCodec(int num);
  virtual bool	setCodec(const std::string name);

 private:
		
  virtual void	setupQT();
		
  virtual void	compressFrame();
	
  //////
  // is recording setup and ready to go?
  bool			m_recordSetup;

  bool		m_recordStart;
  bool		m_recordStop;

    	
  //////////
  // current file to write to
  char	    	m_filename[QT_MAX_FILENAMELENGTH];

  //////////
  // (previous) dimensions to check
  int m_width, m_height;
  int		m_prevHeight,m_prevWidth;


  imageStruct	*m_compressImage;


# ifdef __APPLE__
  UnsignedWide startTime, endTime;
# endif

# ifdef _WIN32
  LARGE_INTEGER freq, startTime, endTime;
# endif
  float seconds;

  //number of QT ticks for a frame 600/frameDuration (used by AddMediaSample)
  int					m_ticks;
		
  bool	m_firstRun;

  //////////
  // QT stuff

  GWorldPtr				m_srcGWorld;
  Rect					m_srcRect;
  int						m_rowBytes;
  Movie					m_movie;
  Track					track;
  Media					media;
  ComponentInstance		stdComponent;
  SCTemporalSettings		TemporalSettings;
  SCSpatialSettings		SpatialSettings;
  SCDataRateSettings		DataRateSetting;
  SCDataRateSettings		datarate;
  long					dataSize;
  ImageDescriptionHandle	hImageDesc;

  //these are for the programmatic setting of the compressor
  CodecType				m_codecType;
  CodecComponent			m_codec;
  short					m_depth;
  float m_frameRate;
  float m_keyFrameRate;
  CodecQ					m_spatialQuality;
  short		nFileRefNum;
  short		nResID;

  void resetCodecSettings(void);
		
  //this will hold the ctype value of the codecs listed by getCodecList()
  typedef struct codecListStorage{
    int		position;
    int		ctype;
    char* name;
    CodecComponent		codec;
  };
  
  codecListStorage *codecContainer;
  int numCodecContainer;
#endif /* QT */
}; };
#endif	// for header file
