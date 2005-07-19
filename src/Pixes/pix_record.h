/*
 *  pix_record.h
 *  GEM_darwin
 *
 *  Created by chris clepper on 7/18/05.
 *  Copyright 2005. All rights reserved.
 *
 */
 
 //this will record QT movies
#ifndef INCLUDE_pix_record_H_
#define INCLUDE_pix_record_H_


#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"
#include "Base/GemPixImageSave.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_record
    
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
class GEM_EXTERN pix_record : public GemBase
{
    CPPEXTERN_HEADER(pix_record, GemBase)

    public:

        //////////
        // Constructor
    	pix_record(int argc, t_atom *argv);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_record();

    	//////////
    	// Write to the current filename
    	virtual void	doWrite();
		
		virtual void	setupQT();
		
		virtual void	compressFrame();
		
		//virtual void	startRecording();
		
		virtual void	stopRecording();
    	
    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

    	//////////
    	// Clear the dirty flag on the pixBlock
    	virtual void 	postrender(GemState *state) {};

    	//////////
    	// Set the filename and filetype
    	virtual void	fileMess(int argc, t_atom *argv);
 
    	//////////
    	// When a size message is received
    	virtual void	sizeMess(int width, int height);
    	
    	//////////
    	// When a position message is received
    	virtual void	posMess(int x, int y);
    	
    	//////////
    	// Clean up the image
    	void	    	cleanImage();
    	
    	//////////
    	// The original pix_record
    	imageStruct 	*m_originalImage;


	//////////
	// Manual writing
	bool            m_banged;
    	
	//////////
	// Automatic writing
	bool            m_automatic;

	//////////
	// Counter for automatic writing
	int             m_autocount;
	
	/////////
	// recording start
	bool			m_recordStart;
	
	/////////
	// recording start
	bool			m_recordStop;
	
	//////
	// is recording setup and ready to go?
	bool			m_recordSetup;
    	
    	//////////
	// path to write to
    	char	    	m_pathname[80];
    	//////////
	// current file to write to
    	char	    	m_filename[80];

    	//////////
	// current file to write to
    	int	    	m_filetype; // 0=tiff, [1..6=jpeg]

    	//////////
    	// The x position
    	int     	m_xoff;
    	
    	//////////
    	// The y position
    	int     	m_yoff;
    	
    	//////////
    	// The width
    	int     	m_width;
    	
    	//////////
    	// The height
    	int     	m_height;
		
		//////////
		// previous dimensions to check
		int			m_prevHeight;
		
		int			m_prevWidth;
		
		pixBlock	*m_pixBlock;
		
		imageStruct	m_compressImage;
		
		///////////
		/// QT stuff
		
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
		
		short		nFileRefNum;
		short		nResID;
		
    private:
    	
    	//////////
    	// static member functions
    	static void 	fileMessCallback(void *data, t_symbol *s, int argc, t_atom *argv);
    	static void 	autoMessCallback(void *data, t_floatarg on);
    	static void 	bangMessCallback(void *data);
    	static void 	sizeMessCallback(void *data, t_floatarg width, t_floatarg height );
    	static void 	posMessCallback(void *data, t_floatarg x, t_floatarg y);
		static void 	recordMessCallback(void *data, t_floatarg on);
};

#endif	// for header file
