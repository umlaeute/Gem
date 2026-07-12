/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2009, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  - Runtime Configuration of Gem

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEM_SETTINGS_H_
#define _INCLUDE__GEM_GEM_SETTINGS_H_

#include <string>
#include <vector>

typedef struct _atom t_atom;
namespace gem
{
namespace Settings
{
void init(void);
void print(void);
void save(void);

t_atom*get(const std::string&key);
void set(const std::string&key, t_atom*value=NULL);

void get(const std::string&key, int&value);
void get(const std::string&key, float&value);
void get(const std::string&key, double&value);
void get(const std::string&key, std::string&value);

std::vector<std::string> keys();
};
};
#endif
