#include "Gem/GemConfig.h"

#include "GemString.h"
#include "Utils/wstring.h"

#ifdef _WIN32
typedef unsigned __int8 uint8_t;
#else
# include <stdint.h>
#endif

#ifdef  HAVE_FRIBIDI_H
# include <fribidi.h>

#include "m_pd.h"

namespace gem
{
namespace string
{
std::wstring getVisualLine(const std::string&instring)
{
  std::wstring outstring;
  const FriBidiCharSet enc= FRIBIDI_CHAR_SET_UTF8;
  FriBidiParType direction=FRIBIDI_PAR_ON;
  size_t len2=instring.size()*2;

  FriBidiChar*str_in  = new FriBidiChar[len2];
  FriBidiChar*str_out = new FriBidiChar[len2];

  /* convert UTF8 to UTF32 */
  FriBidiStrIndex ulen = fribidi_charset_to_unicode(enc, instring.c_str(),
                         instring.size(), str_in);
  /* reshape the UTF32 string */
  FriBidiLevel lvl = fribidi_log2vis(str_in, ulen, &direction, str_out, 0, 0,
                                     0);
  if(lvl) {
    outstring=std::wstring((wchar_t*)str_out);
  } else {
    //outstring=instring;
  }
  delete[]str_in;
  delete[]str_out;
  return outstring;
}

std::wstring getVisualLine(const std::wstring&instring)
{
  std::wstring outstring;

  const FriBidiCharSet enc= FRIBIDI_CHAR_SET_UTF8;
  FriBidiParType direction=FRIBIDI_PAR_ON;
  size_t len2=instring.size()*2;

  const FriBidiChar*str_in  = (const FriBidiChar*)instring.c_str();
  FriBidiChar*str_out = new FriBidiChar[len2];

  /* reshape the UTF32 string */
  FriBidiLevel lvl = fribidi_log2vis(str_in, instring.size(), &direction,
                                     str_out, 0, 0, 0);
  if(lvl) {
    wchar_t*output=(wchar_t*)str_out;
    outstring=std::wstring(output, instring.size());
  } else {
    outstring=instring;
  }
  delete[]str_out;

  return outstring;
}


};
};


#else /* !HAVE_FRIBIDI_H */
namespace gem
{
namespace string
{
std::wstring getVisualLine(const std::string&instring)
{
  return toWstring(instring.c_str());
}
std::wstring getVisualLine(const std::wstring&widestring)
{
  return widestring;
}
};
};
#endif /* !HAVE_FRIBIDI_H */


static const uint8_t utf8d[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

  070,070,070,070,070,070,070,070,070,070,070,070,070,070,070,070,
  050,050,050,050,050,050,050,050,050,050,050,050,050,050,050,050,
  030,030,030,030,030,030,030,030,030,030,030,030,030,030,030,030,
  030,030,030,030,030,030,030,030,030,030,030,030,030,030,030,030,
  204,204,188,188,188,188,188,188,188,188,188,188,188,188,188,188,
  188,188,188,188,188,188,188,188,188,188,188,188,188,188,188,188,
  174,158,158,158,158,158,158,158,158,158,158,158,158,142,126,126,
  111, 95, 95, 95, 79,207,207,207,207,207,207,207,207,207,207,207,

  0,1,1,1,8,7,6,4,5,4,3,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,
  1,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,4,4,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,8,7,6,4,5,4,3,2,1,1,1,1,

};


namespace gem
{
namespace string
{
std::wstring toWstring(const char*str)
{
  std::wstring result;

  wchar_t unic = 0;
  uint8_t byte, stat = 9;

  int len=0;

  while((byte=*str++)) {
    uint8_t data = utf8d[ byte ];
    stat = utf8d[ 256 + (stat << 4) + (data >> 4) ];
    byte = (byte ^ (uint8_t)(data << 4));

    unic = (unic << 6) | byte;

    if (!stat) {
      // unic is now a proper code point, we just print it out.
      result+=unic;
      unic = 0;
    }

    if (stat == 1) {
      // the byte is not allowed here; the state would have to
      // be reset to continue meaningful reading of the string

      throw(len);
    }

    len++;
  }

  return result;
}
  std::wstring toWstring(const std::string&str) {
#ifdef _WIN32
    return gem::string::utf8string_to_wstring(str);
#else
    return toWstring(str.c_str());
#endif
  }
};
};
