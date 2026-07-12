/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2010, fredoverflow <https://stackoverflow.com/a/3177723/1169096>
 * SPDX-FileCopyrightText: © 2019, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef GEM_IS_POINTER_H_
#define GEM_IS_POINTER_H_

namespace gem
{
template <typename T>
struct is_pointer_type {
  enum { value = false };
};

template <typename T>
struct is_pointer_type<T*> {
  enum { value = true };
};

template <typename T>
bool is_pointer(void)
{
  return is_pointer_type<T>::value;
}
template <typename T>
bool is_pointer(const T&)
{
  return is_pointer_type<T>::value;
}
};
#endif // GEM_IS_POITNER_H_
