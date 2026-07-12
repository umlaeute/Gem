////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2009, IOhannes m zmölnig and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////

#include "ContextData.h"
#include "Base/GemContext.h"

using namespace gem;

/* LATER, when we have multiple contexts, this should really be "-1" or such
 */
const unsigned int ContextDataBase::INVALID_CONTEXT=0;

unsigned int ContextDataBase::getCurContext(void) const
{
  /* this should get an integer-index of the current context from GemContext */
  unsigned int id=0;
  id=gem::Context::getContextId();
  return id;
}

ContextDataBase::~ContextDataBase(void)
{
}
