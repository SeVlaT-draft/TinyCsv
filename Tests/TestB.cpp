// SeVlaT, 18.02.2015
//
#include <assert.h>

#include <iostream>
#include <sstream>

#include "Test.h"

#include "../Fsm.h"
#include "../Input.h"
#include "../Field.h"

using namespace TinyCsv;

template <bool TRIMSPACES, int N>
void TestStream(      std::istream  &is,
                const char* const  (&szRes)[N])
{
  typedef TFsm<TFsmDescr> TFsm1;

  typedef TStreamSource<std::istream> TSource;
  typedef TField<std::string, TRIMSPACES>  TField1;

  TSource Source(is);
  TField1 Field;
  TInput<TFsm1, TSource, TField1> Input(Source, Field);

  do {
    do {
      while (Input.Iteration());

      if (Input.IsEoC()) {
        const std::string sField(Field.Begin(), Field.End());
        const int nField=Input.GetFieldNumber();
        std::cout << nField << ": " << sField << std::endl;

        assert(szRes[nField]);
        assert(sField==szRes[nField]);
      }
    } while (Input.NextField());
  } while (Input.NextRecord());
}

template <bool TRIMSPACES, int N>
void TestStream(const std::string  &sSrc,
                const char* const (&szRes)[N])
{
  std::stringstream ss(sSrc);
  TestStream<TRIMSPACES, N>(ss, szRes);
}

template <int N>
void TestStream(const std::string  &sSrc,
                const char* const (&szRes) [N],
                const char* const (&szResT)[N])
{
  TestStream<false, N>(sSrc, szRes);
  TestStream<true, N> (sSrc, szResT);
}
////////////////////////////////////////////////////////////////////////////////
void TestB0()
{
  const char szCsv[]= "a;";
  const char* szRes[]={"a", 0};

  TestStream(szCsv, szRes, szRes);
}

void TestB1()
{
  const char szCsv[]= "a; b; c";
  const char* szRes[] ={"a"," b"," c", 0};
  const char* szResT[]={"a", "b", "c", 0};

  TestStream(szCsv, szRes, szResT);
}

void TestB2()
{
  const char szCsv[]= "\"a b\";";
  const char* szRes[]={"a b", 0};

  TestStream(szCsv, szRes, szRes);
}

void TestB3()
{
  const char szCsv[]= "a; b; c ; \"a a\" ; \" a\"\"b\"\"\"";
  const char* szRes[] ={"a"," b"," c ","a a"," a\"b\"", 0};
  const char* szResT[]={"a", "b", "c" ,"a a"," a\"b\"", 0};

  TestStream(szCsv, szRes, szResT);
}

void TestB()
{
  TestB0();
  TestB1();
  TestB2();
  TestB3();
}
