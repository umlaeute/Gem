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

#include "Pixes/record.h"

//#if defined (__WIN32__) || defined(__APPLE__)


#if defined HAVE_QUICKTIME && defined __WIN32__ 
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
  virtual int	size(int width, int height);
  //////////
  // When a position message is received
  virtual int	position(int x, int y);
		
  ////////
  // call up compression dialog
  virtual int	dialog();
  virtual int	getNumCodecs();
  virtual int	codecMess(int num);
  virtual int	codecMess(char*name);

 private:
		
  virtual void	setupQT();
		
  virtual void	compressFrame();

  ////////
  // recording start
  bool			m_recordStart;
	
  ////////
  // recording start
  bool			m_recordStop;
	
  //////
  // is recording setup and ready to go?
  bool			m_recordSetup;
	
  bool			m_dialog;
    	
  //////////
  // current file to write to
  char	    	m_filename[80];

  //////////
  // previous dimensions to check
  int		m_prevHeight;
  int		m_prevWidth;

  imageStruct	*m_compressImage;

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
  CodecQ					m_spatialQuality;
  //set these to reflect if the codec settings are good or not
  bool				m_codecSet;
  bool				m_codecQualitySet;
  short		nFileRefNum;
  short		nResID;
		
  //this will hold the ctype value of the codecs listed by getCodecList()
  typedef struct codecListStorage{
    int		position;
    int		ctype;
    CodecComponent		codec;
  };
  
  codecListStorage	codecContainer[64];//anyone with more than 64 codecs can change this
#endif
};
#endif	// for header file
