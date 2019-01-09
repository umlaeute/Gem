/*
 * Copyright © 2010 fredoverflow https://stackoverflow.com/a/3177723/1169096
 * Copyright © 2019 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
 */

#ifndef GEM_IS_POINTER_H_
#define GEM_IS_POINTER_H_

namespace gem
{
  template <typename T>
    struct is_pointer_type
    {
      enum { value = false };
    };

  template <typename T>
    struct is_pointer_type<T*>
    {
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
