// SeVlaT, 08.02.2015
//
#include <assert.h>
#include <fstream>

#include "Sample.h"

#include "../Fsm.h"
#include "../Input.h"
#include "../Cell.h"
#include "../Source.h"

using namespace TinyCsv;

void Sample(std::istream &is)
{
  typedef TFsm<TFsmDescr> TFsm1;

  typedef TStreamSource<std::istream>   TSource;
  typedef  TCellStr<std::string, false> TCell;

  TSource Source(is);
  TInput<TFsm1, TCell> Input;

  Input.ReadCell(Source);
  Input.NextRow();
}

void Sample()
{
}
