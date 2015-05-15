// SeVlaT, 18.02.2015
//
#include <assert.h>

#include <iostream>
#include <sstream>

#include "Test.h"

#include "../Fsm.h"
#include "../Input.h"
#include "../Cell.h"
#include "../Source.h"

using namespace TinyCsv;

template <int N>
class TTestVisitor {
 public:
  typedef const char* TStrArray[N];

 public:
  TTestVisitor(const TStrArray &szRes)
   : m_szRes(szRes),
     m_nCurRes(0) {}

  ~TTestVisitor()
  {
    std::cout << std::endl;
    assert(m_szRes[m_nCurRes]==0);
  }

 public:
  void RowBegin(int nRow)
    { std::cout << nRow << ":|"; }

  void RowEnd()
    {
      std::cout << std::endl;
      assert(m_szRes[m_nCurRes]==0);
      ++m_nCurRes;
    }

  template <typename ITI>
  void Cell(ITI iiB, ITI iiE, int nCellInRow)
  {
    const std::string sField(iiB, iiE);
    std::cout << "|" << nCellInRow << ":" << sField << "|";

    assert(m_szRes[m_nCurRes]);
    assert(sField==m_szRes[m_nCurRes]);
    ++m_nCurRes;
  }

 private:
  const TStrArray &m_szRes;

 private:
  int m_nCurRes;
};

template <bool TRIMSPACES, int N>
class TStreamTester {
  typedef TFsm<TFsmDescr>                   TFsm1;
  typedef TCellStr<std::string, TRIMSPACES> TCell;

  typedef TInput<TFsm1, TCell> TCsvIt;

 public:
  explicit TStreamTester(const char* const (&szRes)[N])
   : m_szRes(szRes) {}

 public:
  void Test0(std::istream &is)
  {
    TCsvIt It;
    TTestVisitor<N> Tester(m_szRes);

    TStreamSource<std::istream> Source(is);

    It(Source, Tester);
  }

  void Test1(std::istream &is)
  {
    TCsvIt It;
    TTestVisitor<N> Tester(m_szRes);

    TBaseCharTraitsA bct;
    TCsvCharTraitsA  cct;

    TCharTag ct=cEoF;
    do {
      const int ch=is.get();
      ct=CharTagEof(ch, bct, cct);

      if (!It(ch, ct, Tester)) return;
    } while (ct!=cEoF);
  };

  void Test2(std::istream &is)
  {
    TCsvIt It;
    TStreamSource<std::istream> Source(is);

    int i=0;
    while (It.ReadCell(Source, true)) {
      while (i<N && !m_szRes[i]) ++i;
      if (i>=N) {
        assert(false);
        return;
      }

      const std::string sCell(It.Cell().Begin(), It.Cell().End());
      assert(sCell==m_szRes[i]);

      ++i;
    }
  }

  void Test3(std::istream &is)
  {
    TCsvIt It;
    TStreamSource<std::istream> Source(is);

    int i=0;
    do {
      while (It.ReadCell(Source, false)) {
        const std::string sCell(It.Cell().Begin(), It.Cell().End());
        if (i>=N) {
          assert(false);
          return;
        }
        assert(sCell==m_szRes[i]);

        ++i;
      }

      if (i>=N) {
        assert(false);
        return;
      }

      assert(!m_szRes[i]);
      ++i;
    } while(It.NextRow());
  }

 private:
  const char* const (&m_szRes)[N];
};


template <bool TRIMSPACES, int N>
void TestStream(const std::string  &sSrc,
                const char* const (&szRes)[N])
{
  TStreamTester<TRIMSPACES, N> Tester(szRes);

  Tester.Test0(std::stringstream(sSrc));
  Tester.Test1(std::stringstream(sSrc));
  Tester.Test2(std::stringstream(sSrc));
  Tester.Test3(std::stringstream(sSrc));
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
  const char* szRes[]={"a", "", 0, 0};

  TestStream(szCsv, szRes, szRes);
}

void TestB1()
{
  const char szCsv[]= "a; b; c";
  const char* szRes[] ={"a"," b"," c", 0, 0};
  const char* szResT[]={"a", "b", "c", 0, 0};

  TestStream(szCsv, szRes, szResT);
}

void TestB2()
{
  const char szCsv[]= "\"a b\";";
  const char* szRes[]={"a b", "", 0, 0};

  TestStream(szCsv, szRes, szRes);
}

void TestB3()
{
  const char szCsv[]= "a; b; c ; \"a a\" ; \" a\"\"b\"\"\"";
  const char* szRes[] ={"a"," b"," c ","a a"," a\"b\"", 0, 0};
  const char* szResT[]={"a", "b", "c" ,"a a"," a\"b\"", 0, 0};

  TestStream(szCsv, szRes, szResT);
}

void TestB4()
{
  const char szCsv[]= "a; b; c\nd ;e;f";
  const char* szRes[] ={"a"," b"," c", 0, "d ", "e", "f", 0, 0};
  const char* szResT[]={"a", "b", "c", 0, "d",  "e", "f", 0, 0};

  TestStream(szCsv, szRes, szResT);
}

void TestB5()
{
  const char szCsv[]= "a; \n\nb\n";
  const char* szRes[] ={"a", " ",0, 0, "b", 0, 0, 0};
  const char* szResT[]={"a", "", 0, 0, "b", 0, 0, 0};

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
