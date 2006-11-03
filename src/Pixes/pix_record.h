/*
 *  pix_record.h
 *  GEM_darwin
 *
 *  Created by chris clepper on 7/18/05.
 *  Copyright 2005. All rights reserved.
 *
 */
 
 //this will record QT movies
#ifndef INCLUDE_PIX_RECORD_H_
#define INCLUDE_PIX_RECORD_H_

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"

#include "Pixes/record.h"

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

    JMZ: do we need these 2??::
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
	// turn recording on/off
	virtual void	recordMess(bool on);

	////////
	// call up compression dialog
	virtual void	dialogMess();
		
	virtual void	getCodecList();
	
	virtual void	codecMess(t_atom *argv);

	//////////
	// Manual writing
	bool            m_banged;
    	
	//////////
	// Automatic writing
	bool            m_automatic;

	/////////
	// recording start
	bool			m_recordStart;
	/////////
	// recording start
	bool			m_recordStop;
	
	
	//////////
	// a outlet for information like #frames
	t_outlet     *m_outNumFrames;
		
	int           m_currentFrame; //keep track of the number of frames
		
	int                     m_numCodecs;
	//	codecListStorage	codecContainer[64];//anyone with more than 64 codecs can change this

	record        *m_handle;
		
    private:
    	
    	//////////
    	// static member functions
    	static void 	fileMessCallback(void *data, t_symbol *s, int argc, t_atom *argv);
    	static void 	autoMessCallback(void *data, t_floatarg on);
    	static void 	bangMessCallback(void *data);

    	static void 	sizeMessCallback(void *data, t_floatarg width, t_floatarg height );
    	static void 	posMessCallback(void *data, t_floatarg x, t_floatarg y);
	static void 	recordMessCallback(void *data, t_floatarg on);
	static void 	dialogMessCallback(void *data);
	static void 	codeclistMessCallback(void *data);
	static void 	codecMessCallback(void *data, t_symbol *s, int argc, t_atom *argv);


};
#endif	// for header file
