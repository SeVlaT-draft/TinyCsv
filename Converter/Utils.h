#ifndef Utils_h_already_included__28_03_2015__F10172E
#define Utils_h_already_included__28_03_2015__F10172E
//
// SeVlaT, 28.03.2015
//

#include "Types.h"

bool String2Int(const string &s, int &n)
{
  return sscanf(s.c_str(), "%d", &n)==1;
}

bool String2Int(const wstring &s, int &n)
{
  return swscanf(s.c_str(), L"%d", &n)==1;
}

template <typename CH>
bool StringsAreEqual(const char *sz, const basic_string<CH> &s)
{
  if (!sz) return false;

  for (string::const_iterator I=s.begin(); I!=s.end(); ++I, ++sz) {
    if (!*sz) return false;
    if (*I!=*sz) return false;
  }
  return true;
}

template <typename CH>
bool StringsAreEqual(const string &s0, const basic_string<CH> &s1)
{
  if (s0.length()!=s1.length()) return false;

  return std::equal(s0.begin(), s0.end(), s1.begin());
}

#endif
