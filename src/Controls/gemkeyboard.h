/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Respond to mouse events
    
    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMKEYBOARD_H_
#define INCLUDE_GEMKEYBOARD_H_

#ifndef HELPSYMBOL_BASE
# define HELPSYMBOL_BASE "Gem/"
#endif

#include "Base/CPPExtern.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    gemkeyboard
    
    Respond to keyboard events

DESCRIPTION
    
    
-----------------------------------------------------------------*/
class GEM_EXTERN gemkeyboard : public CPPExtern
{
    CPPEXTERN_HEADER(gemkeyboard, CPPExtern)

    public:

        //////////
        // Constructor
        gemkeyboard();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~gemkeyboard();

        //////////
        // keyboard-button
        void            KeyBoardPressed(int val, int state);

        //////////
        // The key-val outlet
        t_outlet    	*m_outKeyVal;

    private:

        //////////
        // Static member functions
        static void     keyboardCallback(char *w, int x, int y, void *data);
};

#endif  // for header file
