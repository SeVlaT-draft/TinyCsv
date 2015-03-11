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

using TinyCsv::TCsvCharTraitsA;
using TinyCsv::TOutputParamsA;

struct TSrcOptions {
  string sFile;

  TCsvCharTraitsA  cct;
};

struct TTrgOptions {
  string sFile;

  TCsvCharTraitsA  cct;
  TOutputParamsA paramsTrg;
};

struct TOptions {
  TSrcOptions Src;
  TTrgOptions Trg;
};


#endif
