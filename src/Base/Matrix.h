/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	Matrix class

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_MATRIX_H_
#define INCLUDE_MATRIX_H_

#include "Base/GemExportDef.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    Matrix
    
    4x4 Matrix class

DESCRIPTION

	Post-concatenation
	Column-major
    
-----------------------------------------------------------------*/
class GEM_EXTERN Matrix
{
    public:
    	
        //////////
        // Constructor
		// Sets the matrix to identity
		Matrix();

        //////////
		// Set the matrix to the identity
		void identity();
        
		//////////
		// Post mulitply the matrix
		void multiply(Matrix *pMatrix);
        
		//////////
		void scale(float x, float y, float z);
        
		//////////
		void translate(float x, float y, float z);
        
		//////////
		void rotateX(float degrees);
        
		//////////
		void rotateY(float degrees);
        
		//////////
		void rotateZ(float degrees);
        
		//////////
		void transform(float srcX, float srcY, float srcZ, float *dstX, float *dstY, float *dstZ) const;

        //////////
		// The actual matrix values
		float				mat[4][4];
		
        //////////
		// Utility functions
		static void generateNormal(const float *v1, const float *v2, const float *v3, float *dst);
};


#endif	// for header file
