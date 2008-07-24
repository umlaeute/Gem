/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Respond to mouse events
    
  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_GEMRECEIVE_H_
#define INCLUDE_GEMRECEIVE_H_

#include "Base/CPPExtern.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemreceive
    
  Respond to keyboard events

  DESCRIPTION
    
    
  -----------------------------------------------------------------*/


typedef struct _bind_element {
  t_pd   *object;
  t_float priority;
  struct _bind_element*next;
} t_bind_element;

typedef struct _gemreceive_proxy {
  t_object p_obj;

  t_symbol*key;
  t_bind_element*elements;
  struct _gemreceive_proxy*next;
} t_gemreceive_proxy;

class GEM_EXTERN gemreceive : public CPPExtern
{
  CPPEXTERN_HEADER(gemreceive, CPPExtern)

    public:

  //////////
  // Constructor
  gemreceive(t_symbol*s, t_floatarg f=50.f);
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~gemreceive();
        
  //////////
  // keyboard-button
  void            receive(t_symbol*s, int argc, t_atom*argv);

  //////////
  // the symbol we are bound to
  void nameMess(t_symbol*s);
  t_symbol*m_name;

  //////////
  // the receive priority
  void priorityMess(t_float f);
  t_float m_priority;

  //////////
  // The receive outlet
  t_outlet    	*m_outlet;

  //////////
  // inlet for priority
  t_inlet*m_fltin;


 private:

  //////////
  // Static member functions
  static void     receiveCallback(void*data, t_symbol*s, int argc, t_atom*argv);
  static void     proxyCallback(void*data, t_symbol*s, int argc, t_atom*argv);

  //////////
  static void     nameCallback(void *data, t_symbol*s);
  static void     priorityCallback(void *data, t_float f);

  static t_gemreceive_proxy*find_key(t_symbol*);
  static t_gemreceive_proxy*add_key(t_symbol*);
  static void add_element(t_gemreceive_proxy*, t_bind_element*);

  static t_gemreceive_proxy*proxy_list;


 public:
  static void bind(t_pd*x, t_symbol*name, t_float priority);
  static void unbind(t_pd*x, t_symbol*name);


};

#endif  // for header file
