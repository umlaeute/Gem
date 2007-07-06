//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   Utilities for the VideoIO-Framework.
//
//   vioutils
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
    
    
#ifndef VIOUTILS_H_
#define VIOUTILS_H_
    
    
/*!
 * \file VIOUtils.h
 * Some global defines for der VideoIO-Framework
 */

#include "m_pd.h"
    

// color spaces defines:
const int RGB = 0;       //! RGB colorspace
const int RGBA = 1;      //! RGBA colorspace
const int YUV422 = 2;    //! YUV422 colorspace
const int GRAY = 3;      //! GRAY colorspace


// color component defines:
// RGBA
const int chRed = 0;     //! channel Red
const int chGreen = 1;   //! channel Green
const int chBlue = 2;    //! channel Blue
const int chAlpha = 3;   //! channel Alpha

// YUV 422
const int chU = 0;       //! channel U
const int chY0 = 1;      //! channel Y0
const int chV = 2;       //! channel V
const int chY1 = 3;      //! channel Y1

// Gray
const int chGray = 0;    //! channel Gray


#endif
