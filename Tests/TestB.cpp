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

template <int N>
class TTester {
 public:
  typedef const char* TStrArray[N];

 public:
  TTester(const TStrArray &szRes)
   : m_szRes(szRes),
     m_nCellTotal(0) {}

  ~TTester()
  {
    assert(m_szRes[m_nCellTotal]==0);
  }

 public:
  template <typename ITI>
  void Cell(ITI iiB, ITI iiE, int nCellInRow)
  {
    const std::string sField(iiB, iiE);
    std::cout << "|" << nCellInRow << ":" << sField << "|";

    assert(m_szRes[m_nCellTotal]);
    assert(sField==m_szRes[m_nCellTotal]);
    ++m_nCellTotal;
  }

 private:
  const TStrArray &m_szRes;

 private:
  int m_nCellTotal;
};

template <bool TRIMSPACES, int N>
void TestStream1(      std::istream  &is,
                 const char* const  (&szRes)[N])
{
  typedef TFsm<TFsmDescr> TFsm1;

  typedef TStreamSource<std::istream> TSource;
  typedef TField<std::string, TRIMSPACES>  TField1;

  TSource Source(is);
  TField1 Field;
  TInput<TFsm1> Input;

  TTester<N> Tester(szRes);

  TCharTag ct=cEoF;
  do {
    char ch=is.get();
  } while (ct!=cEoF);
};

template <bool TRIMSPACES, int N>
void TestStream0(      std::istream  &is,
                 const char* const  (&szRes)[N])
{
  typedef TFsm<TFsmDescr> TFsm1;

  typedef TStreamSource<std::istream> TSource;
  typedef TField<std::string, TRIMSPACES>  TField1;

  TSource Source(is);
  TField1 Field;
  TInput<TFsm1> Input;

  TTester<N> Tester(szRes);

  do {
    const int nRow=Input.GetRecordNumber();
    std::cout << nRow << ":|";

    do {
      while (Input.Iteration(Source, Field));

      if (Input.IsEoC()) {
        Tester.Cell(Field.Begin(), Field.End(), Input.GetFieldNumber());
      }
    } while (Input.NextField());
    std::cout << std::endl;
  } while (Input.NextRecord());
  std::cout << std::endl;
}

template <bool TRIMSPACES, int N>
void TestStream(const std::string  &sSrc,
                const char* const (&szRes)[N])
{
  std::stringstream ss(sSrc);
  TestStream0<TRIMSPACES, N>(ss, szRes);
}

template <int N>
void TestStream(const std::string  &sSrc,
                const char* const (&szRes) [N],
                const char* const (&szResT)[N])
{
  std::cout << sSrc << std::endl;
  std::cout << std::endl;

  TestStream<false, N>(sSrc, szRes);
  TestStream<true, N> (sSrc, szResT);

  std::cout << std::endl;
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

void TestB4()
{
  const char szCsv[]= "a; b; c\nd ;e;f";
  const char* szRes[] ={"a"," b"," c", "d ", "e", "f", 0};
  const char* szResT[]={"a", "b", "c", "d",  "e", "f", 0};

  TestStream(szCsv, szRes, szResT);
}

void TestB5()
{
  const char szCsv[]= "a; \n\nb\n";
  const char* szRes[] ={"a", " ","b", 0};
  const char* szResT[]={"a", "", "b", 0};

  TestStream(szCsv, szRes, szResT);
}

void TestB()
{
  TestB0();
  TestB1();
  TestB2();
  TestB3();
  TestB4();
  TestB5();
}
