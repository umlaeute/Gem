#include "Base/GemConfig.h"

#ifdef _WIN32
#define _WIN32_WINNT 0x0400
#include <windows.h>
#else
# include <glob.h>
#endif

#include "Base/GemFiles.h"


namespace gem {
  namespace files {

    std::vector<std::string>getFilenameListing(const std::string pattern) {
      std::vector<std::string>result;
#ifdef _WIN32
      WIN32_FIND_DATA findData;
      DWORD errorNumber;
      HANDLE hFind;
      LPVOID lpErrorMessage;
      
      hFind = FindFirstFile(pattern.c_str(), &findData);
      if (hFind == INVALID_HANDLE_VALUE) 
        {
          errorNumber = GetLastError();
          switch (errorNumber)
            {
            case ERROR_FILE_NOT_FOUND:
            case ERROR_PATH_NOT_FOUND:
              break;
            default:
              FormatMessage(
                            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                            FORMAT_MESSAGE_FROM_SYSTEM,
                            NULL,
                            errorNumber,
                            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                            (LPTSTR) &lpErrorMessage,
                            0, NULL );
              //pd_error(x,"[folder_list] %s", (char *)lpErrorMessage);
            }
          return result;
        }
      do {
        result.push_back(findData.cFileName);
      } while (FindNextFile(hFind, &findData) != 0);

      FindClose(hFind);
#else
      /* use glob */
      glob_t glob_buffer;
      int i=0;
      switch( glob( pattern.c_str(), GLOB_TILDE, NULL, &glob_buffer ) ) {
      case GLOB_NOSPACE: 
        //        error("out of memory for \"%s\"",pattern.c_str()); 
        return result;
# ifdef GLOB_ABORTED
      case GLOB_ABORTED: 
        //error("aborted \"%s\"",pattern.c_str()); 
        return result;
# endif
# ifdef GLOB_NOMATCH
      case GLOB_NOMATCH: 
        //error("nothing found for \"%s\"",pattern.c_str()); 
        return result;
# endif
      }
      for(i=0; i<glob_buffer.gl_pathc; i++) {
        result.push_back(glob_buffer.gl_pathv[i]);
      }
      globfree( &(glob_buffer) );
#endif

      return result;
    }
  };
};
