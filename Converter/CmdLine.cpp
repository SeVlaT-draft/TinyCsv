// SeVlaT, 12.03.2015
//
#include "CmdLine.h"

#include "Utils.h"
#include "Options.h"

template<typename SOURCE>
class TCmdLineParser {
 public:
  typedef typename SOURCE::TChar TChar;
  typedef basic_string<TChar>    TString;

 public:
  TCmdLineParser(SOURCE &Src, TOptions &Opt)
   : m_Src(Src),
     m_Opt(Opt),
     m_nI(1),
     m_nIS(1),
     m_nIQ(1),

     m_nO(1),
     m_nOS(1),
     m_nOQ(1),

     m_nPQ(1),

     m_nEW(1),
     m_nEA(1) {}

 public:
  bool ParseOption(const TString &s, const char *szKey, bool &bVal, int &nCount)
  {
    if (StringIsEqual(s, szKey)) return false;

    if (nCount==0) return;
    bVal=true;
  }

  bool ParseOption(const TString &s, const char *szKey, int &nVal, int &nCount)
  {
    if (StringIsEqual(s, szKey)) return false;

    if (nCount==0) return;

    TString s1;
    if (!m_Src.Get(s1)) return;

    int n=0;
    if (!String2Int(s1, n) return;
    nVal=n;
  }

  bool ParseOption(const TString &s, const char *szKey, TString &sVal, int &nCount)
  {
    if (StringIsEqual(s, szKey)) return false;

    if (nCount==0) return;

    TString s1;
    if (!m_Src.Get(s1)) return;
    sVal=s1;
  }

  bool ParseOption(const TString &s, const char *szKey, TChar &chVal, int &nCount)
  {
    if (StringIsEqual(s, szKey)) return false;

    if (nCount==0) return;

    TString s;
    if (!m_Src.Get(s)) return;
    if (s.length()!=1) return;
    chVal=s;
  }

 public:
  bool ParseNext()
  {
    TString s;

    if (!m_Src.Get(s)) return false;

    if (StringIsEqual(s, "/I"))  ParseOption(m_Opt.Src.sFile,                           m_nI);
    if (StringIsEqual(s, "/IS")) ParseOption(m_Opt.Src.cct.chSep,                       m_nIS);
    if (StringIsEqual(s, "/IQ")) ParseOption(m_Opt.Src.cct.chQte,                       m_nIQ);

    if (StringIsEqual(s, "/O ")) ParseOption(m_Opt.Trg.sFile,                           m_nO);
    if (StringIsEqual(s, "/OS")) ParseOption(m_Opt.Trg.cct.chSep,                       m_nOS);
    if (StringIsEqual(s, "/OQ")) ParseOption(m_Opt.Trg.cct.chQte,                       m_nOQ);

    if (StringIsEqual(s, "/PQ")) ParseOption(m_Opt.Trg.paramsTrg,                       m_nPQ);

    if (StringIsEqual(s, "/EW")) ParseOption(m_Opt.Trg.paramsTrg.bEscapeEdgeWhitespace, m_nEW);
    if (StringIsEqual(s, "/EA")) ParseOption(m_Opt.Trg.paramsTrg.bEscapeAlways,         m_nEA);
  }

 private:
  SOURCE   &m_Src;
  TOptions &m_Opt; 

 private:
  int m_nI;
  int m_nIS;
  int m_nIQ;

  int m_nO;
  int m_nOS;
  int m_nOQ;

  int m_nPQ;

  int m_nEW;
  int m_nEA;
}

bool ParseCmdLine(int       argc,
                  const char* const argv[],
                  TOptions &Options)
{
  return false;
}

bool ParseCmdLine(const char *szCmdLine,
                  TOptions &Options)
{
  return false;
}

