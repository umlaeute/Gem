/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block
(OS independant parent-class)

Copyright (c) 2005-2010 Chris Clepper
Copyright (c) 2009-2010 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#if 1
 //this will record QT movies
#ifndef INCLUDE_PIX_RECORD_H_
#define INCLUDE_PIX_RECORD_H_

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"

#include "plugins/record.h"

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
  // another outlet for extra information (like list of codecs...)
	t_outlet     *m_outInfo;
		
	int           m_currentFrame; //keep track of the number of frames

        //////////
        //
        int           m_minFrames, m_maxFrames;
		
	int                     m_numCodecs;
	//	codecListStorage	codecContainer[64];//anyone with more than 64 codecs can change this

	gem::record *m_handle;
	
	
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

	static void 	minMessCallback(void *data, t_floatarg min);
	static void 	maxMessCallback(void *data, t_floatarg max);

};
#endif	// for header file
#endif //removes pix_record
