/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    utf16.h
       - contains wstring functions/macros
       - part of GEM
       - header-only implementation (to be used in plugins,...)

    Copyright (c) 2018 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_UTILS_UTF16_H_
#define _INCLUDE__GEM_UTILS_UTF16_H_

#include <string>
#ifdef _WIN32
# include <windows.h>
#endif

namespace gem { namespace string {
#ifdef _WIN32
/* convert a multibyte encoded string (as defined by codepage) to a UTF-16 (wchar) encoded string */
    static inline std::wstring mbstring_to_wstring(const std::string&s, unsigned int codepage) {
  if (s.empty()){
    return std::wstring();
  }
  int n = MultiByteToWideChar(codepage, 0, s.data(), s.size(), NULL, 0);
  std::wstring buf;
  buf.resize(n);
  MultiByteToWideChar(codepage, 0, s.data(), s.size(), &buf[0], n);
  return buf;
}
/* convert a UTF-8 encoded string to a multibyte encoded string (as defined by codepage) */
static inline std::string wstring_to_mbstring(const std::wstring&s, unsigned int codepage) {
  if (s.empty()){
    return std::string();
  }
  int n = WideCharToMultiByte(codepage, 0, s.data(), s.size(), NULL, 0, NULL, NULL);
  std::string buf;
  buf.resize(n);
  WideCharToMultiByte(codepage, 0, s.data(), s.size(), &buf[0], n, NULL, NULL);
  return buf;
}


/* convert a standard UTF-8 encoded string to a UTF-16 (wchar) encoded string */
static inline std::wstring utf8string_to_wstring(const std::string&s) {
  return mbstring_to_wstring(s, CP_UTF8);
}
/* convert a UTF-16 (wchar) encoded string to a standard UTF-8 encoded string */
static inline std::string wstring_to_utf8string(const std::wstring&s) {
  return wstring_to_mbstring(s, CP_UTF8);
}
/* convert a UTF-8 encoded string to a native windows multibyte string (using the default Windows 'ANSI' code page) */
static inline std::string utf8string_to_nativestring(const std::string&s) {
  return gem::string::wstring_to_mbstring(gem::string::utf8string_to_wstring(s), CP_ACP);
}
#else
/* convert a UTF-8 encoded string to a native string (UTF-8 on all platforms but windows) */
static inline std::string utf8string_to_nativestring(const std::string&s) {
  return std::string(s);
}
#endif /* OS */
};};
#endif  // for header file
