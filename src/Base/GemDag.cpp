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

#include "GemDag.h"

#include <stddef.h>

/////////////////////////////////////////////////////////
//
// GemDag
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemDag :: GemDag()
    	: m_list(NULL), m_numChildren(0), m_current(NULL)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemDag :: ~GemDag()
{
    // special case
    if (!m_list) return;
    
    // clean up the list
    for (int i = 0; i < m_numChildren; i++) delete m_list[i];
	delete [] m_list;
}

/////////////////////////////////////////////////////////
// addChild
//
/////////////////////////////////////////////////////////
void GemDag :: addChild(GemBase *data, void (*renderFunc)(GemBase *, GemState *), void (*postrenderFunc)(GemBase *, GemState *), void (*stoprenderFunc)(GemBase*))

{
    // a new entry at the top
    if (m_current == NULL)
    {
    	// copy over the list
    	gemBaseLink **newList = new gemBaseLink*[m_numChildren + 1];
    	for (int i = 0; i < m_numChildren; i++) newList[i] = m_list[i];

    	delete [] m_list;
    	m_list = newList;
    	m_current = m_list[m_numChildren] = new gemBaseLink;
     	m_list[m_numChildren]->setData(data, renderFunc, postrenderFunc, stoprenderFunc);
    	m_numChildren++;
    }
    // else put it into the current link
    else
    {
     	m_current = m_current->addChild(data, renderFunc, postrenderFunc, stoprenderFunc);
    }
}

/////////////////////////////////////////////////////////
// childDone
//
/////////////////////////////////////////////////////////
void GemDag :: childDone(GemBase *)
{
    m_current = m_current->parent;
}




///////////////////////////////////////////////////////////////////////////
// gemBaseLink
//
///////////////////////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemBaseLink :: gemBaseLink()
    	     : data(NULL), children(NULL), parent(NULL), numChildren(0)
{ }

///////////////////////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemBaseLink :: ~gemBaseLink()
{
    // special case
    if (!children) return;
    
    // clean up the list
    for (int i = 0; i < numChildren; i++) delete children[i];
	delete [] children;
}

/////////////////////////////////////////////////////////
// setData
//
/////////////////////////////////////////////////////////
void gemBaseLink :: setData(GemBase *_data, void (*_renderFunc)(GemBase *, GemState *), void (*_postrenderFunc)(GemBase *, GemState *), void (*_stoprenderFunc)(GemBase*))
{
    data = _data;
    renderFunc = _renderFunc;
    postrenderFunc = _postrenderFunc;
    stoprenderFunc = _stoprenderFunc;
}

/////////////////////////////////////////////////////////
// addChild
//
/////////////////////////////////////////////////////////
gemBaseLink *gemBaseLink :: addChild(GemBase *data, void (*renderFunc)(GemBase *, GemState *), void (*postrenderFunc)(GemBase *, GemState *), void (*stoprenderFunc)(GemBase*))
{
    // the very first entry
    if (children == NULL)
    {
    	children = new gemBaseLink*[1];
    	children[0] = new gemBaseLink;
	children[0]->setData(data, renderFunc, postrenderFunc, stoprenderFunc);
    	children[0]->parent = this;
   		numChildren = 1;
   		return(children[0]);
    }
    // else put it into the current link
    else
    {
    	// copy over the list
    	gemBaseLink **newList = new gemBaseLink*[numChildren + 1];
    	for (int i = 0; i < numChildren; i++) newList[i] = children[i];

    	delete [] children;
    	children = newList;
    	
    	children[numChildren] = new gemBaseLink;
     	children[numChildren]->setData(data, renderFunc, postrenderFunc, stoprenderFunc);
    	children[numChildren]->parent = this;
    	numChildren++;
    	return(children[numChildren - 1]);
    }
//    return(NULL);
}
