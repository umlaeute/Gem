/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    get keyboard/mouse/tablet callbacks

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMEVENT_H_
#define INCLUDE_GEMEVENT_H_

#include "Base/GemExportDef.h"
//#include "Base/CPPExtern.h"

//////////////////////////////////////////////////////////////////
//
// Mouse motion callback
//
//////////////////////////////////////////////////////////////////
typedef void (*MOTION_CB)(int, int, void *);
//////////
// Set a mouse motion callback
GEM_EXTERN extern void setMotionCallback(MOTION_CB callback, void *data);

//////////
// Remove a mouse motion callback
GEM_EXTERN extern void removeMotionCallback(MOTION_CB callback, void *data);


//////////////////////////////////////////////////////////////////
//
// Mouse button callback
//
//////////////////////////////////////////////////////////////////
typedef void (*BUTTON_CB)(int, int, int, int, void *);
//////////
// Set a button callback
// which == 0 == left
// which == 1 == middle
// which == 2 == right
GEM_EXTERN extern void setButtonCallback(BUTTON_CB callback, void *data);

//////////
// Remove a button callback
GEM_EXTERN extern void removeButtonCallback(BUTTON_CB callback, void *data);


//////////////////////////////////////////////////////////////////
//
// Mouse wheel callback
//
//////////////////////////////////////////////////////////////////
typedef void (*WHEEL_CB)(int, int, void *);
//////////
// Set a wheel callback
GEM_EXTERN extern void setWheelCallback(WHEEL_CB callback, void *data);

//////////
// Remove a wheel callback
GEM_EXTERN extern void removeWheelCallback(WHEEL_CB callback, void *data);


//////////////////////////////////////////////////////////////////
//
// Tablet motion callback
//
//////////////////////////////////////////////////////////////////
typedef void (*TABMOTION_CB)(int, int, float, void *);
//////////
// Set a tablet motion callback
GEM_EXTERN extern void setTabletMotionCallback(TABMOTION_CB callback, void *data);

//////////
// Remove a tablet motion callback
GEM_EXTERN extern void removeTabletMotionCallback(TABMOTION_CB callback, void *data);


//////////////////////////////////////////////////////////////////
//
// Tablet rotation callback
//
//////////////////////////////////////////////////////////////////
typedef void (*TABROTATION_CB)(int, int, int, void *);
//////////
// Set a tablet rotation callback
GEM_EXTERN extern void setTabletRotationCallback(TABROTATION_CB callback, void *data);

//////////
// Remove a tablet rotation callback
GEM_EXTERN extern void removeTabletRotationCallback(TABROTATION_CB callback, void *data);


//////////////////////////////////////////////////////////////////
//
// Tablet button callback
//
//////////////////////////////////////////////////////////////////
typedef void (*TABBUTTON_CB)(int, int, int, int, void *);
//////////
// Set a tablet button callback
// which == 0 == left
// which == 1 == middle
// which == 2 == right
GEM_EXTERN extern void setTabletButtonCallback(TABBUTTON_CB callback, void *data);

//////////
// Remove a tablet button callback
GEM_EXTERN extern void removeTabletButtonCallback(TABBUTTON_CB callback, void *data);

//////////////////////////////////////////////////////////////////
//
// Keyboard callback
//
//////////////////////////////////////////////////////////////////
typedef void (*KEYBOARD_CB)(char *,int, int, void *);
//////////
// Set a keyboard callback
GEM_EXTERN extern void setKeyboardCallback(KEYBOARD_CB callback, void *data);

//////////
// Remove a keyboard callback
GEM_EXTERN extern void removeKeyboardCallback(KEYBOARD_CB callback, void *data);

//////////////////////////////////////////////////////////////////
//
// Resize callback
//
//////////////////////////////////////////////////////////////////
typedef void (*RESIZE_CB)(int, int, void *);
//////////
// Set a resize callback
GEM_EXTERN extern void setResizeCallback(RESIZE_CB callback, void *data);

//////////
// Remove a resize callback
GEM_EXTERN extern void removeResizeCallback(RESIZE_CB callback, void *data);

//////////
// Trigger an event
GEM_EXTERN extern void triggerMotionEvent(int x, int y);
GEM_EXTERN extern void triggerButtonEvent(int which, int state, int x, int y);
GEM_EXTERN extern void triggerWheelEvent(int axis, int value);
GEM_EXTERN extern void triggerTabletMotionEvent(int x, int y, float pressure);
GEM_EXTERN extern void triggerTabletRotationEvent(int az, int alt, int twist);
GEM_EXTERN extern void triggerTabletButtonEvent(int which, int state, int x, int y);
GEM_EXTERN extern void triggerKeyboardEvent(char *string, int value, int state);
GEM_EXTERN extern void triggerResizeEvent(int xSize, int ySize);

#endif  // for header file

