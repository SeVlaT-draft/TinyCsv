// SeVlaT, 20.02.2015
//
#include <assert.h>

#include "Test.h"

#include "../Fsm.h"

#include "../Tests/FsmChecker.h"

using namespace TinyCsv;

void TestC0()
{
  TFsm<TFsmDescr> fsm;
  TChecker<TFsmDescr> Checker;

  assert(Checker(fsm.GetCurState()));
}

void TestC()
{
  TestC0();
}
