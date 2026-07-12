/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_UTILS_MATRIX_H_
#define _INCLUDE__GEM_UTILS_MATRIX_H_

#include "Gem/ExportDef.h"

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
  Matrix(void);

  //////////
  // Set the matrix to the identity
  void identity(void);

  //////////
  // Post multiply the matrix
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
  void transform(float srcX, float srcY, float srcZ, float *dstX,
                 float *dstY, float *dstZ) const;

  //////////
  // The actual matrix values
  float                           mat[4][4];

  //////////
  // Utility functions
  static void generateNormal(const float *v1, const float *v2,
                             const float *v3, float *dst);
};


#endif  // for header file
