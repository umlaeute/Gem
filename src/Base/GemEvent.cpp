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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemEvent.h"

#include <stdlib.h>

/////////////////////////////////////////////////////////
// The callbacks
//
/////////////////////////////////////////////////////////
struct CallbackList
{
    CallbackList() : data(NULL), func(NULL), next(NULL) {}
    void *data;
    void *func;
    CallbackList *next;
};

static CallbackList *s_motionList = NULL;
static CallbackList *s_buttonList = NULL;
static CallbackList *s_wheelList = NULL;
static CallbackList *s_tabmotionList = NULL;
static CallbackList *s_tabrotationList = NULL;
static CallbackList *s_tabbuttonList = NULL;
static CallbackList *s_keyboardList = NULL;
static CallbackList *s_resizeList = NULL;

/////////////////////////////////////////////////////////
// Motion callbacks
//
/////////////////////////////////////////////////////////
GEM_EXTERN void setMotionCallback(MOTION_CB callback, void *data)
{
    CallbackList *newCallback = new CallbackList;
    newCallback->data = data;
    newCallback->func = (void *)callback;

    if (!s_motionList)
        s_motionList = newCallback;
    else
    {
        CallbackList *theList = s_motionList;
        while(theList->next)
            theList = theList->next;
        theList->next = newCallback;
    }
}
GEM_EXTERN void removeMotionCallback(MOTION_CB callback, void *data)
{
    CallbackList *theList = s_motionList;
    if (!theList)
        return;
    else if (theList->func == (void *)callback &&
             theList->data == data)
    {
        s_motionList = theList->next;
        delete theList;
    }
    else
    {
        while(theList->next)
        {
            if (theList->next->func == (void *)callback &&
                theList->next->data == data)
            {
                CallbackList *holder = theList->next;
                theList->next = holder->next;
                delete holder;
                return;
            }
            theList = theList->next;
        }
    }
}
/////////////////////////////////////////////////////////
// Button callbacks
//
/////////////////////////////////////////////////////////
GEM_EXTERN void setButtonCallback(BUTTON_CB callback, void *data)
{
    CallbackList *newCallback = new CallbackList;

    newCallback->data = data;
    newCallback->func = (void *)callback;

    if (!s_buttonList)
        s_buttonList = newCallback;
    else
    {
        CallbackList *theList = s_buttonList;
        while(theList->next)
            theList = theList->next;
        theList->next = newCallback;
    }
}
GEM_EXTERN void removeButtonCallback(BUTTON_CB callback, void *data)
{
    CallbackList *theList = s_buttonList;
    if (!theList)
        return;
    else if (theList->func == (void *)callback &&
             theList->data == data)
    {
        s_buttonList = theList->next;
        delete theList;
    }
    else
    {
        while(theList->next)
        {
            if (theList->next->func == (void *)callback &&
                theList->next->data == data)
            {
                CallbackList *holder = theList->next;
                theList->next = holder->next;
                delete holder;
                return;
            }
            theList = theList->next;
        }
    }
}
/////////////////////////////////////////////////////////
// Wheel callbacks
//
/////////////////////////////////////////////////////////
GEM_EXTERN void setWheelCallback(WHEEL_CB callback, void *data)
{
    CallbackList *newCallback = new CallbackList;
    newCallback->data = data;
    newCallback->func = (void *)callback;

    if (!s_wheelList)
        s_wheelList = newCallback;
    else
    {
        CallbackList *theList = s_wheelList;
        while(theList->next)
            theList = theList->next;
        theList->next = newCallback;
    }
}
GEM_EXTERN void removeWheelCallback(WHEEL_CB callback, void *data)
{
    CallbackList *theList = s_wheelList;
    if (!theList)
        return;
    else if (theList->func == (void *)callback &&
             theList->data == data)
    {
        s_wheelList = theList->next;
        delete theList;
    }
    else
    {
        while(theList->next)
        {
            if (theList->next->func == (void *)callback &&
                theList->next->data == data)
            {
                CallbackList *holder = theList->next;
                theList->next = holder->next;
                delete holder;
                return;
            }
            theList = theList->next;
        }
    }
}
/////////////////////////////////////////////////////////
// Tablet motion callbacks
//
/////////////////////////////////////////////////////////
GEM_EXTERN void setTabletMotionCallback(TABMOTION_CB callback, void *data)
{
    CallbackList *newCallback = new CallbackList;
    newCallback->data = data;
    newCallback->func = (void *)callback;

    if (!s_tabmotionList)
        s_tabmotionList = newCallback;
    else
    {
        CallbackList *theList = s_tabmotionList;
        while(theList->next)
            theList = theList->next;
        theList->next = newCallback;
    }
}
GEM_EXTERN void removeTabletMotionCallback(TABMOTION_CB callback, void *data)
{
    CallbackList *theList = s_tabmotionList;
    if (!theList)
        return;
    else if (theList->func == (void *)callback &&
             theList->data == data)
    {
        s_tabmotionList = theList->next;
        delete theList;
    }
    else
    {
        while(theList->next)
        {
            if (theList->next->func == (void *)callback &&
                theList->next->data == data)
            {
                CallbackList *holder = theList->next;
                theList->next = holder->next;
                delete holder;
                return;
            }
            theList = theList->next;
        }
    }
}
/////////////////////////////////////////////////////////
// Tablet rotation callbacks
//
/////////////////////////////////////////////////////////
GEM_EXTERN void setTabletRotationCallback(TABROTATION_CB callback, void *data)
{
    CallbackList *newCallback = new CallbackList;
    newCallback->data = data;
    newCallback->func = (void *)callback;

    if (!s_tabrotationList)
        s_tabrotationList = newCallback;
    else
    {
        CallbackList *theList = s_tabrotationList;
        while(theList->next)
            theList = theList->next;
        theList->next = newCallback;
    }
}
GEM_EXTERN void removeTabletRotationCallback(TABROTATION_CB callback, void *data)
{
    CallbackList *theList = s_tabrotationList;
    if (!theList)
        return;
    else if (theList->func == (void *)callback &&
             theList->data == data)
    {
        s_tabrotationList = theList->next;
        delete theList;
    }
    else
    {
        while(theList->next)
        {
            if (theList->next->func == (void *)callback &&
                theList->next->data == data)
            {
                CallbackList *holder = theList->next;
                theList->next = holder->next;
                delete holder;
                return;
            }
            theList = theList->next;
        }
    }
}
/////////////////////////////////////////////////////////
// Tablet button callbacks
//
/////////////////////////////////////////////////////////
GEM_EXTERN void setTabletButtonCallback(TABBUTTON_CB callback, void *data)
{
    CallbackList *newCallback = new CallbackList;
    newCallback->data = data;
    newCallback->func = (void *)callback;

    if (!s_tabbuttonList)
        s_tabbuttonList = newCallback;
    else
    {
        CallbackList *theList = s_tabbuttonList;
        while(theList->next)
            theList = theList->next;
        theList->next = newCallback;
    }
}
GEM_EXTERN void removeTabletButtonCallback(TABBUTTON_CB callback, void *data)
{
    CallbackList *theList = s_tabbuttonList;
    if (!theList)
        return;
    else if (theList->func == (void *)callback &&
             theList->data == data)
    {
        s_tabbuttonList = theList->next;
        delete theList;
    }
    else
    {
        while(theList->next)
        {
            if (theList->next->func == (void *)callback &&
                theList->next->data == data)
            {
                CallbackList *holder = theList->next;
                theList->next = holder->next;
                delete holder;
                return;
            }
            theList = theList->next;
        }
    }
}
/////////////////////////////////////////////////////////
// Keyboard callbacks
//
/////////////////////////////////////////////////////////
GEM_EXTERN void setKeyboardCallback(KEYBOARD_CB callback, void *data)
{
    CallbackList *newCallback = new CallbackList;
    newCallback->data = data;
    newCallback->func = (void *)callback;

    if (!s_keyboardList)
        s_keyboardList = newCallback;
    else
    {
        CallbackList *theList = s_keyboardList;
        while(theList->next)
            theList = theList->next;
        theList->next = newCallback;
    }
}
GEM_EXTERN void removeKeyboardCallback(KEYBOARD_CB callback, void *data)
{
    CallbackList *theList = s_keyboardList;
    if (!theList)
        return;
    else if (theList->func == (void *)callback &&
             theList->data == data)
    {
        s_keyboardList = theList->next;
        delete theList;
    }
    else
    {
        while(theList->next)
        {
            if (theList->next->func == (void *)callback &&
                theList->next->data == data)
            {
                CallbackList *holder = theList->next;
                theList->next = holder->next;
                delete holder;
                return;
            }
            theList = theList->next;
        }
    }
}
/////////////////////////////////////////////////////////
// Resize callbacks
//
/////////////////////////////////////////////////////////
GEM_EXTERN void setResizeCallback(RESIZE_CB callback, void *data)
{
    CallbackList *newCallback = new CallbackList;
    newCallback->data = data;
    newCallback->func = (void *)callback;

    if (!s_resizeList)
        s_resizeList = newCallback;
    else
    {
        CallbackList *theList = s_resizeList;
        while(theList->next)
            theList = theList->next;
        theList->next = newCallback;
    }
}
GEM_EXTERN void removeResizeCallback(RESIZE_CB callback, void *data)
{
    CallbackList *theList = s_resizeList;
    if (!theList)
        return;
    else if (theList->func == (void *)callback &&
             theList->data == data)
    {
        s_resizeList = theList->next;
        delete theList;
    }
    else
    {
        while(theList->next)
        {
            if (theList->next->func == (void *)callback &&
                theList->next->data == data)
            {
                CallbackList *holder = theList->next;
                theList->next = holder->next;
                delete holder;
                return;
            }
            theList = theList->next;
        }
    }
}

/////////////////////////////////////////////////////////
// Trigger events
//
/////////////////////////////////////////////////////////
GEM_EXTERN void triggerMotionEvent(int x, int y)
{
    CallbackList *theList = s_motionList;
    while(theList)
    {
        MOTION_CB callback = (MOTION_CB)theList->func;
        (*callback)(x, y, theList->data);
        theList = theList->next;
    }
}
GEM_EXTERN void triggerButtonEvent(int which, int state, int x, int y)
{
    CallbackList *theList = s_buttonList;
    while(theList)
    {
        BUTTON_CB callback = (BUTTON_CB)theList->func;
        (*callback)(which, state, x, y, theList->data);
        theList = theList->next;
    }
}
GEM_EXTERN void triggerWheelEvent(int axis, int value)
{
    CallbackList *theList = s_wheelList;
    while(theList)
    {
        WHEEL_CB callback = (WHEEL_CB)theList->func;
        (*callback)(axis, value, theList->data);
        theList = theList->next;
    }
}
GEM_EXTERN void triggerTabletMotionEvent(int x, int y, float pressure)
{
    CallbackList *theList = s_tabmotionList;
    while(theList)
    {
        TABMOTION_CB callback = (TABMOTION_CB)theList->func;
        (*callback)(x, y, pressure, theList->data);
        theList = theList->next;
    }
}
GEM_EXTERN void triggerTabletRotationEvent(int azimuth, int altitude, int twist)
{
    CallbackList *theList = s_tabrotationList;
    while(theList)
    {
        TABROTATION_CB callback = (TABROTATION_CB)theList->func;
        (*callback)(azimuth, altitude, twist, theList->data);
        theList = theList->next;
    }
}
GEM_EXTERN void triggerTabletButtonEvent(int which, int state, int x, int y)
{
    CallbackList *theList = s_tabbuttonList;
    while(theList)
    {
        TABBUTTON_CB callback = (TABBUTTON_CB)theList->func;
        (*callback)(which, state, x, y, theList->data);
        theList = theList->next;
    }
}
GEM_EXTERN void triggerKeyboardEvent(char *string, int value, int state)
{
    CallbackList *theList = s_keyboardList;
    while(theList)
    {
        KEYBOARD_CB callback = (KEYBOARD_CB)theList->func;
        (*callback)(string, value, state, theList->data);
        theList = theList->next;
    }
}
GEM_EXTERN void triggerResizeEvent(int xSize, int ySize)
{
    CallbackList *theList = s_resizeList;
    while(theList)
    {
        RESIZE_CB callback = (RESIZE_CB)theList->func;
        (*callback)(xSize, ySize, theList->data);
        theList = theList->next;
    }
}
