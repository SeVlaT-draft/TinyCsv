#ifndef Types_h_already_included__02_03_2015__E4C44BF
#define Types_h_already_included__02_03_2015__E4C44BF
//
// SeVlaT, 02.03.2015
//

#include <string>
#include <fstream>

#include <cassert>

#include "../Input.h"
#include "../Output.h"

using std::string;
using std::ifstream;
using std::ofstream;

using TinyCsv::TCsvCharTraits;
using TinyCsv::TOutputParams;
using TinyCsv::TOutputParams;

template<typename CH>
struct TSrcOptions {
  string sFile;

  TCsvCharTraits<CH> cct;
};

template<typename CH>
struct TTrgOptions {
  string sFile;

  TCsvCharTraits<CH> cct;
  TOutputParams<CH> paramsTrg;
};

template<typename CH>
struct TOptions {
  TSrcOptions<CH> Src;
  TTrgOptions<CH> Trg;
};


#endif
