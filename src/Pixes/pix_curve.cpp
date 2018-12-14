////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
// pix_curve
//
//   IOhannes m zmoelnig
//   mailto:zmoelnig@iem.kug.ac.at
//
//   this code is published under the Gnu GeneralPublicLicense that should be distributed with gem & pd
//
/////////////////////////////////////////////////////////

#include "pix_curve.h"
#include <string.h>
#include "Utils/Functions.h"
#include "RTE/Array.h"

CPPEXTERN_NEW_WITH_GIMME(pix_curve);


/////////////////////////////////////////////////////////
//
// pix_curve
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_curve :: pix_curve(int argc, t_atom *argv):
  name_R(0), name_G(0), name_B(0), name_A(0),
  m_mode(0)
{
  if(argc) {
    setMess(0, argc, argv);
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_curve :: ~pix_curve()
{
}


////////////////////////////////////
// Set Message
//
///////////////////////////////////
void pix_curve :: setMess(t_symbol*,int argc, t_atom *argv)
{
  t_atom *ap=argv;
  int n=argc;

  if (!(argc==1 || argc==3 || argc==4)) {
    error("only 1, 3 or 4 arguments are allowed");
    m_mode=0;
    return;
  }

  while(n--) {
    if (ap->a_type != A_SYMBOL) {
      error("only symbolic table-names are accepted");
      return;
    }
    ap++;
  }

  m_mode=3;

  ap=argv;
  switch (argc) {
  case 1:
    name_R=name_G=name_B=name_A=atom_getsymbol(ap);
    m_mode=1;
    break;
  /* coverity[unterminated_case] */
  case 4:
    name_A=atom_getsymbol(ap+3);
    m_mode=4;
  default:
    name_R=atom_getsymbol(ap);
    name_G=atom_getsymbol(ap+1);
    name_B=atom_getsymbol(ap+2);
  }
  setPixModified();
}

#define setTable(tab, nam) if (nam)tab.name(nam->s_name)
#define checkTable(tab) bool use_##tab=tab.isValid(); int n_##tab=tab.size()
#define applyTable(tab, chan) if (use_##tab) base[chan]=CLAMP(static_cast<int>(tab[ n_##tab*base[chan]>>8 ]));

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_curve :: processRGBAImage(imageStruct &image)
{
  int i=image.xsize*image.ysize;
  unsigned char *base = image.data;

  switch(m_mode) {
  case 4:
  case 3:
  case 1:
    break;
  default:
    return;
  }

  gem::RTE::Array tabR, tabG, tabB, tabA;

  switch(m_mode) {
  case 4:
    setTable(tabA, name_A);
  case 3:
    setTable(tabR, name_R);
    setTable(tabG, name_G);
    setTable(tabB, name_B);
    break;
  case 1:
    setTable(tabR, name_R);
    setTable(tabG, name_R);
    setTable(tabB, name_R);
    setTable(tabA, name_R);
    break;
  default:
    error("invalid mode %d", m_mode);
    return;
  }

  checkTable(tabR);
  checkTable(tabG);
  checkTable(tabB);
  checkTable(tabA);

  switch (m_mode) {
  case 3: // only RGB
    while (i--) {
      applyTable(tabR, chRed);
      applyTable(tabG, chGreen);
      applyTable(tabB, chBlue);
      base+=4;
    }
    break;
  case 4: // RGBA
  case 1: // one table for all
    while (i--) {
      applyTable(tabR, chRed);
      applyTable(tabG, chGreen);
      applyTable(tabB, chBlue);
      applyTable(tabA, chBlue);

      base+=4;
    }
  default:
    break;
  }


}
/////////////////////////////////////////////////////////
// processImage
void pix_curve :: processGrayImage(imageStruct &image)
{
  int i=image.xsize*image.ysize;
  unsigned char *base = image.data;
  switch(m_mode) {
  case 1:
    break;
  default:
    return;
  }

  gem::RTE::Array tabY;
  setTable(tabY, name_R);
  checkTable(tabY);

  while (i--) {
    applyTable(tabY, chGray);
    base++;
  }
}

void pix_curve :: processYUVImage(imageStruct &image)
{
  int i=image.xsize*image.ysize/2;
  unsigned char *base = image.data;
  switch(m_mode) {
  case 3:
  case 1:
    break;
  default:
    return;
  }

  gem::RTE::Array tabY, tabU, tabV;

  setTable(tabY, name_R);
  setTable(tabU, name_G);
  setTable(tabV, name_B);
  checkTable(tabY);
  checkTable(tabU);
  checkTable(tabV);

  switch (m_mode) {
  case 4: // ignore 4th table
  case 3: // YUV
    while (i--) {
      applyTable(tabU, chU );
      applyTable(tabY, chY0);
      applyTable(tabV, chV );
      applyTable(tabY, chY1);
      base+=4;
    }
    break;
  case 1: // only Y
    while (i--) {
      applyTable(tabY, chY0);
      applyTable(tabY, chY1);
      base+=4;
    }
  default:
    break;
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_curve :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG(classPtr, "set", setMess);
}
