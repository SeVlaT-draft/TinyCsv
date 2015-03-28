#ifndef Options_h_already_included__28_03_2015__32E1D452
#define Options_h_already_included__28_03_2015__32E1D452
//
// SeVlaT, 28.03.2015
//

#include "Types.h"

#include "../Input.h"
#include "../Output.h"

using TinyCsv::TCsvCharTraits;
using TinyCsv::TOutputParams;
using TinyCsv::TOutputParams;

template<typename CH>
struct TSrcOptions {
  basic_string<CH> sFile;

  TCsvCharTraits<CH> cct;
};

template<typename CH>
struct TTrgOptions {
  basic_string<CH> sFile;

  TCsvCharTraits<CH> cct;
  TOutputParams<CH> paramsTrg;
};

template<typename CH>
struct TOptions {
  TSrcOptions<CH> Src;
  TTrgOptions<CH> Trg;
};

#endif
