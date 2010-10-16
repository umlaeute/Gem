#include <map>

#include "Properties.h"

namespace gem {

  struct Properties::PIMPL {
    std::map<std::string, any> map;

    PIMPL(void) {;}
  };


  Properties::Properties() :
    pimpl(new PIMPL())
  { }
  Properties::~Properties() {
    delete pimpl;
  }

  any&Properties::operator[](const std::string&key) {
    return pimpl->map[key]; 
  }

  any Properties::at(const std::string&key) {
    return pimpl->map[key]; 
  }

  void Properties::set(const std::string&key, gem::any value) {
    pimpl->map[key]=value; 
  } 

  std::vector<std::string>Properties::keys() {
    std::vector<std::string>result;
    for(std::map<std::string,gem::any>::iterator it = pimpl->map.begin(); it != pimpl->map.end(); ++it)
      result.push_back(it->first);
    return result;
  }

};
