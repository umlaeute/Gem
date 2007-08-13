//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   Writes a digital video (like AVI, Mpeg, Quicktime) to the harddisc.
//
//   FileWrite
//   header file
//
//   copyright            : (C) 2007 by Thomas Holzmann
//   email                : holzi1@gmx.at
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
///////////////////////////////////////////////////////////////////////////
 
 //this will record QT movies
#ifndef INCLUDE_PIX_FILE_WRITE_H_
#define INCLUDE_PIX_FILE_WRITE_H_

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"
#include "../videoIO/VIOKernel.h"
#include "../videoIO/VIOFrame.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_file_write
    
    Writes a pix with the VideoIO Framework
-----------------------------------------------------------------*/

class GEM_EXTERN pix_file_write : public GemBase
{
    CPPEXTERN_HEADER(pix_file_write, GemBase)

    public:

        //////////
        // Constructor
    	pix_file_write(int argc, t_atom *argv);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_file_write();
		
    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

    	//////////
    	// Clear the dirty flag on the pixBlock
    	virtual void 	postrender(GemState *state) {};

    	//////////
    	// Set the filename and filetype
    	virtual void	openFile(t_symbol *filename);
 
	//////////
	// a outlet for information like #frames
	t_outlet     *m_outNumFrames;
		
    private:
      
        // the file reader
        VideoIO_::FileWrite *m_filewriter;
        /// TODO kernel should be used only once ?
        VideoIO_::VIOKernel m_kernel;
    	
        //////////////
        // helper vars
        
        // stores the current frame
        VideoIO_::VIOFrame m_frame;
        
        // true if we should push frames
        bool m_recording;
        
        // first frame of the recording, used
        // to initialize the format
        bool m_first_time;
        
    	//////////
    	// static member functions
    	static void 	fileMessCallback(void *data, t_symbol *s, int argc, t_atom *argv);
	static void 	recordMessCallback(void *data, t_floatarg on);

};
#endif	// for header file
