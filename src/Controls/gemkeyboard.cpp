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

#include "gemkeyboard.h"

#include "Base/GemEvent.h"

CPPEXTERN_NEW(gemkeyboard)

/////////////////////////////////////////////////////////
//
// gemkeyboard
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemkeyboard :: gemkeyboard()
{
    m_outKeyVal = outlet_new(this->x_obj, 0);

    // register event callback
    setKeyboardCallback(&gemkeyboard::keyboardCallback, this);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemkeyboard :: ~gemkeyboard()
{
    // remove event callback
    removeKeyboardCallback(&gemkeyboard::keyboardCallback, this);

    outlet_free(m_outKeyVal);
}


/////////////////////////////////////////////////////////
// mouseMotion
//
/////////////////////////////////////////////////////////
void gemkeyboard :: KeyBoardPressed(int val, int state)
{
  if (state==0)return;
  char c=0;
  outlet_float(m_outKeyVal, (t_float)val);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemkeyboard :: obj_setupCallback(t_class *)
{ }
void gemkeyboard :: keyboardCallback(char* w, int x, int y, void *data)
{
    ((gemkeyboard *)data)->KeyBoardPressed(x, y);
}
