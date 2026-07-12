/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2010, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  include Realtime-Environments headers

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEM_RTE_H_
#define _INCLUDE__GEM_GEM_RTE_H_

#if defined _MSC_VER
/* data conversion with possible loss of data */
# pragma warning(push)
# pragma warning(disable : 4091)
#endif

#include <m_pd.h>

#ifdef _MSC_VER
# pragma warning( pop )
#endif

#define GEMMARK() verbose(2, "%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__)


#endif /* _INCLUDE__GEM_GEM_RTE_H_ */
