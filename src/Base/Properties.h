/*
 * (c) IOhannes m zmölnig 2010
 *
 * map that stores "any"thing
 * 
 * USAGE:
 * 
 *   Properties am;                                                     // create a new Properties
 *   am["a"] = 42;                                                      // assign value "42" to key "a"
 *   int i=0;
 *   try { i=any_cast<int>am["a"]; } catch(bad_any_cast ex) { ; }       // retrieve value from key "a"; might throw a bad_any_cast exception
 *   am.get("a", i);                                                    // retrieve value at key "a"; if there was an error (e.g. typeof(i) does not match, then i is left untouched and "false" is returned
 *
 * NOTE:
 *   this simplilstic approach has some problems with type-conversion
 *   e.g. this will fail:
 *      am["a"]=12.0f; any_cast<int>am["a"];
 */

#ifndef GEM_PROPERTIES_H
#define GEM_PROPERTIES_H

#include "Gem/any.h"
#include <vector>
#include <string>

namespace gem 
{
  class Properties {
  private:
    class PIMPL;
    PIMPL*pimpl;
  public:
    Properties(void);
    virtual ~Properties(void);

    virtual gem::any&operator[](const std::string&key);
    virtual gem::any at(const std::string&key);
    
    virtual void set(const std::string&key, gem::any value);

    template<class Class>
    bool get(const std::string&key, Class&value) { 
       try {
	value=at(key);
      } catch (gem::bad_any_cast e) {
	return false;
      }
      return true;
    };

    /* get all keys */
    virtual std::vector<std::string>keys(void);
  };
};
#endif /* GEM_PROPERTIES_H */
