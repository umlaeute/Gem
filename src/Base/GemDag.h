/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Establish a GEM Dag

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMDAG_H_
#define INCLUDE_GEMDAG_H_

#include "Base/GemExportDef.h"

class gemBaseLink;
class GemBase;
class GemState;

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemDag
    
    Establish a GEM Dag

DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN GemDag
{
    public:

    	//////////
        //   	void	    	    addChild(GemBase *data, void (*renderFunc)(GemBase *, GemState *), void (*postrenderFunc)(GemBase *, GemState *));
        void			addChild(GemBase *data, void (*renderFunc)(GemBase *, GemState *), 								void (*postrenderFunc)(GemBase *, GemState *), 									void (*stoprenderFunc)(GemBase*));	//DH	

    	//////////
    	void	    	    childDone(GemBase *data);
    	
    private:
    
    	gemBaseLink         **m_list;
    	int 	    	    m_numChildren;
    	
    	gemBaseLink 	    *m_current;
    	
    	GemDag();
    	~GemDag();

    	friend class gemhead;
};

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    gemBaseLink
    
	Link between GemDags

DESCRIPTION
    
-----------------------------------------------------------------*/
class gemBaseLink
{
    public:

    	gemBaseLink();
       	~gemBaseLink();
     	//void	    	    setData(GemBase *data, void (*renderFunc)(GemBase *, GemState *), void (*postrenderFunc)(GemBase *, GemState *));
     	//gemBaseLink			*addChild(GemBase *data, void (*renderFunc)(GemBase *, GemState *), void (*postrenderFunc)(GemBase *, GemState *));
 
     	void	    	    setData(GemBase *data, void (*renderFunc)(GemBase *, GemState *), void (*postrenderFunc)(GemBase *, GemState *), void (*stoprenderFunc)(GemBase*));
     	gemBaseLink	   *addChild(GemBase *data, void (*renderFunc)(GemBase *, GemState *), void (*postrenderFunc)(GemBase *, GemState *), void (*stoprenderFunc)(GemBase*));
 
    	GemBase     	    *data;
    	gemBaseLink 	    **children;
    	gemBaseLink 	    *parent;
    	int 	    	    numChildren;
    	void	    	    (*renderFunc)(GemBase *, GemState *);
    	void	    	    (*postrenderFunc)(GemBase *, GemState *);
	void	    	    (*stoprenderFunc)(GemBase *);
};

#endif	// for header file
