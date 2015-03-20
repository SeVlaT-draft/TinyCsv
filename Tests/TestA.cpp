// SeVlaT, 12.02.2015
//
#include <assert.h>

#include "Test.h"

#include "../Fsm.h"

#include "../Tests/FsmChecker.h"

using namespace TinyCsv;


template <typename DESCR, int N>
void TestArr0(const TCharTag     (&In) [N],
              const TActionFlags (&Out)[N])
{
  TFsm<DESCR> fsm;
  TChecker<DESCR> Checker;

  for (int i=0; i<N; ++i) {
    const TCharTag    I=In[i];
    const TActionFlags O=Out[i];

    assert(Checker(fsm.GetCurState()));
    fsm.Next(I);
    const TAction &Action=fsm.Action();
    assert(Checker(fsm.GetCurState()));
    const TActionFlags O1=Action.Flags();

    if (O1!=O) {
      assert(O1==O);
    }
  }
}

template <int N>
void TestArr(const TCharTag     (&In) [N],
             const TActionFlags (&Out)[N])
{
  TestArr0<TFsmDescr>(In, Out);
}

void Test0()
{
  TCharTag     I[]={cSmb,  cSmb, cSep, cEoF };
  TActionFlags O[]={oNAdd, oAdd, oEoC, oEoF };

  TestArr(I, O);
}

void Test1()
{
  TCharTag     I[]={cSmb,  cSmb, cEoF };
  TActionFlags O[]={oNAdd, oAdd, oEoCF};

  TestArr(I, O);
}

void Test2()
{
  TCharTag     I[]={cWsp,  cQte, cSmb, cSmb, cQte,  cEoF };
  TActionFlags O[]={oNALW, oNew, oAdd, oAdd, oNone, oEoCF};

  TestArr(I, O);
}

void Test3()
{
  TCharTag     I[]={cWsp,  cQte, cSmb, cQte,  cQte, cSmb, cQte,  cEoF };
  TActionFlags O[]={oNALW, oNew, oAdd, oNone, oAdd, oAdd, oNone, oEoCF};

  TestArr(I, O);
}

void Test4()
{
  TCharTag     I[]={cQte, cSmb, cQte,  cEoL,  cEoF};
  TActionFlags O[]={oNew, oAdd, oNone, oEoCR, oEoF};

  TestArr(I, O);
}

void Test5()
{
  TCharTag     I[]={cQte, cSmb, cQte,  cWsp, cEoL, cEoF};
  TActionFlags O[]={oNew, oAdd, oNone, oEoC, oEoR, oEoF};

  TestArr(I, O);
}

void TestA()
{
  Test0();
  Test1();
  Test2();
  Test3();
  Test4();
  Test5();
}
