/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

    get keyboard/mouse callbacks

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEM_EVENT_H_
#define _INCLUDE__GEM_GEM_EVENT_H_

#include "Gem/ExportDef.h"

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
GEM_EXTERN extern void removeMotionCallback(MOTION_CB callback,
    void *data);


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
GEM_EXTERN extern void removeButtonCallback(BUTTON_CB callback,
    void *data);


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
// Keyboard callback
//
//////////////////////////////////////////////////////////////////
typedef void (*KEYBOARD_CB)(const char *,int, int, void *);
//////////
// Set a keyboard callback
GEM_EXTERN extern void setKeyboardCallback(KEYBOARD_CB callback,
    void *data);

//////////
// Remove a keyboard callback
GEM_EXTERN extern void removeKeyboardCallback(KEYBOARD_CB callback,
    void *data);

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
GEM_EXTERN extern void removeResizeCallback(RESIZE_CB callback,
    void *data);

//////////
// Trigger an event
GEM_EXTERN extern void triggerMotionEvent(int x, int y);
GEM_EXTERN extern void triggerButtonEvent(int which, int state, int x,
    int y);
GEM_EXTERN extern void triggerWheelEvent(int axis, int value);
GEM_EXTERN extern void triggerKeyboardEvent(const char *string, int value,
    int state);
GEM_EXTERN extern void triggerResizeEvent(int xSize, int ySize);

#endif  // for header file
