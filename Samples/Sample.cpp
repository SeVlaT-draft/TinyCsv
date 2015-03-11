// SeVlaT, 08.02.2015
//
#include <assert.h>
#include <fstream>

#include "Sample.h"

#include "../Fsm.h"
#include "../Input.h"
#include "../Field.h"

using namespace TinyCsv;

void Sample(std::istream &is)
{
  typedef TFsm<TFsmDescr> TFsm1;

  typedef TStreamSource<std::istream> TSource;
  typedef TField<std::string, false>  TField1;

  TSource Source(is);
  TField1 Field;
  TInput<TFsm1, TSource, TField1> Input(Source, Field);

  Input.ReadField();
  Input.NextRecord();
}

void Sample()
{
}
