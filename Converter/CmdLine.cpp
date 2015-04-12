// SeVlaT, 12.03.2015
//
#include "CmdLine.h"

#include "Utils.h"
#include "Options.h"

template<typename SOURCE, typename VALUE>
class TCmdOptionParser {
 public:
  typedef typename SOURCE::TChar TChar;
  typedef basic_string<TChar>    TString;

 public:
  TCmdOptionParser(SOURCE &Src, const TString &sKey, VALUE &Val)
   : m_Src(Src),
     m_Val(Val),
     m_sKey(sKey),
     m_nCount(1) {}

 public:
  bool operator()(const TString &s)
  {
    if (!CheckKeyCount(s)) return false;
    DoParse();
    return true;
  }

 private:
  bool CheckKeyCount(const TString &s)
  {
    if (!StringIsEqual(s, m_sKey)) return false;
    if (m_nCount==0) throw std::exception("Count is zero");
    --m_nCount;
    return true;
  }

 private:
  void DoParse();

  void ThrowError(const char *sz) const
  {
    throw std::exception(sz);
  }

 private:
  SOURCE &m_Src;
  VALUE  &m_Val;

  const TString m_sKey;

 private:
  int m_nCount;
};

template<typename SOURCE>
void TCmdOptionParser<SOURCE, bool>::DoParse()
{
  m_Val=true;
}

template<typename SOURCE>
void TCmdOptionParser<SOURCE, int>::DoParse()
{
  TString s;
  if (!m_Src.Get(s)) return;

  int n=0;
  if (!String2Int(s, n)) return;
  m_Val=n;
}

template<typename SOURCE>
void TCmdOptionParser<SOURCE, TString>::DoParse()
{
  TString s;
  if (!m_Src.Get(s)) return;
  m_Val=s;
}

template<typename SOURCE>
void TCmdOptionParser<SOURCE, TChar>::DoParse()
{
  TString s;
  if (!m_Src.Get(s)) return;
  if (s.length()!=1) return;
  m_Val=s;
}

template<typename SOURCE>
class TCmdLineParser {
 public:
  typedef typename SOURCE::TChar TChar;
  typedef basic_string<TChar>    TString;

  typedef TCmdOptionParser<SOURCE, bool>    TOpBool;
  typedef TCmdOptionParser<SOURCE, int>     TOpInt;
  typedef TCmdOptionParser<SOURCE, TChar>   TOpChar;
  typedef TCmdOptionParser<SOURCE, TString> TOpString;

 private:
 public:
  TCmdLineParser()

 public:
  bool Parse(SOURCE &src, TOptions &opt)
  {
    TOpString osI (src, "/I",  opt.Src.sFile                          );
    TOpChar   osIS(src, "/IS", opt.Src.cct.chSep                      );
    TOpChar   osIQ(src, "/IQ", opt.Src.cct.chQte                      );

    TOpString osO (src, "/O ", opt.Trg.sFile                          );
    TOpChar   osOS(src, "/OS", opt.Trg.cct.chSep                      );
    TOpChar   osOQ(src, "/OQ", opt.Trg.cct.chQte                      );

    TOpBool   osEW(src, "/EW", opt.Trg.paramsTrg.bEscapeEdgeWhitespace);
    TOpBool   osEA(src, "/EA", opt.Trg.paramsTrg.bEscapeAlways        );

    TString s;
    while(src.Get(s)) {
      if (osI (s)) continue;
      if (osIS(s)) continue;
      if (osIQ(s)) continue;

      if (osO (s)) continue;
      if (osOS(s)) continue;
      if (osOQ(s)) continue;

      if (osEW(s)) continue;
      if (osEA(s)) continue;
      return false;
    }
    return true;
  }
};

struct TDummiSrc {
  typedef char TChar;
  bool Get(string &s)  { return true; }
  bool Get(wstring &s) { return true; }
};

bool ParseCmdLine(int       argc,
                  const char* const argv[],
                  TOptions &Options)
{
  TDummiSrc src;
  TCmdLineParser<TDummiSrc> parser;
  parser.Parse(src, Options);
  return false;
}

bool ParseCmdLine(const char *szCmdLine,
                  TOptions &Options)
{
  return false;
}

