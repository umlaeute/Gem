#ifndef INCLUDE_CAMERA_PROPERTIES_H_
#define INCLUDE_CAMERA_PROPERTIES_H_

#include "pylon/PylonIncludes.h"
#include <pylon/gige/BaslerGigECamera.h>

#include <string>
#include "Base/Properties.h"

namespace gem { namespace pylon { namespace cameraproperties {
                    void init(void);

                    std::vector<std::string>getKeys(void);
                    std::vector<std::string>setKeys(void);

                    void get(Pylon::CBaslerGigECamera*device, 
                             std::string key,
                             gem::any&result);

                    bool set(Pylon::CBaslerGigECamera*device, 
                             std::string key,
                             gem::Properties&props);

                  };};};

#endif /* INCLUDE_CAMERA_PROPERTIES_H_ */
