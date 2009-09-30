/*
 *  recordQT.h
 *  GEM_darwin
 *
 *  Created by chris clepper on 7/18/05.
 *  Copyright 2005. All rights reserved.
 *
 */
 
//this will record QT movies
#ifndef INCLUDE_PIXES_RECORDQT_H_
#define INCLUDE_PIXES_RECORDQT_H_
#include "Base/GemConfig.h"

#define QT_MAX_FILENAMELENGTH 256

#include "Pixes/record.h"

#if defined HAVE_QUICKTIME && defined _WIN32 
# include <QTML.h>
# include <Movies.h>
# include <QuicktimeComponents.h>
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
class GEM_EXTERN recordQT : public record
{
 public:

  //////////
  // Constructor
  recordQT(int x, int y, int w, int h);
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~recordQT();
  
#ifdef HAVE_QUICKTIME
  virtual void close();
  virtual bool open(char*filename);
    	
  //////////
  // Do the rendering
  virtual int 	putFrame(imageStruct*img);
 
  //////////
  // When a size message is received
  virtual bool	size(int width, int height);
  //////////
  // When a position message is received
  virtual bool	position(int x, int y);
		
  ////////
  // call up compression dialog
  virtual bool	dialog();
  virtual int	getNumCodecs();
  virtual char* getCodecName(int n);
  //virtual char* getCodecDescription(int n);

  virtual bool	setCodec(int num);
  virtual bool	setCodec(char*name);

 private:
		
  virtual void	setupQT();
		
  virtual void	compressFrame();
	
  //////
  // is recording setup and ready to go?
  bool			m_recordSetup;
    	
  //////////
  // current file to write to
  char	    	m_filename[QT_MAX_FILENAMELENGTH];

  //////////
  // previous dimensions to check
  int		m_prevHeight;
  int		m_prevWidth;

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
};
#endif	// for header file
