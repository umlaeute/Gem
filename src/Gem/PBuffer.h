/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003-2004, Alexander Zaprjagaev <frustum@frustum.org>
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*
 * OpenGL pixel buffer
 */

#ifndef __PBUFFER_H__
#define __PBUFFER_H__

struct PBuffer_data;

class PBuffer
{
public:

  enum {
    GEM_PBUFLAG_RGB = 1 << 0,
    GEM_PBUFLAG_RGBA = 1 << 1,
    GEM_PBUFLAG_DEPTH = 1 << 2,
    GEM_PBUFLAG_STENCIL = 1 << 3,
    GEM_PBUFLAG_FLOAT = 1 << 4,
    GEM_PBUFLAG_MULTISAMPLE_2 = 1 << 5,
    GEM_PBUFLAG_MULTISAMPLE_4 = 1 << 6
  };

  PBuffer(int width,int height,
          int flags = GEM_PBUFLAG_RGBA | GEM_PBUFLAG_DEPTH | GEM_PBUFLAG_STENCIL);
  ~PBuffer(void);

  void enable(void);
  void disable(void);

  int width;
  int height;

private:
  struct PBuffer_data *data;

  PBuffer(const PBuffer&);
  PBuffer&operator=(const PBuffer&);
};

#endif /* __PBUFFER_H__ */
