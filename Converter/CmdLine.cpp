// SeVlaT, 12.03.2015
//
#include <sstream>

#include "CmdLine.h"

#include "Utils.h"
#include "Options.h"

template<typename SOURCE, typename VALUE>
class TCmdOptionParser {
 public:
  typedef typename SOURCE::TChar TChar;
  typedef basic_string<TChar>    TString;

 public:
  TCmdOptionParser(SOURCE &Src, const string &sKey, VALUE &Val)
   : m_Src(Src),
     m_Val(Val),
     m_sKey(sKey),
     m_nCount(1) {}

 public:
  bool operator()(const TString &s)
  {
    if (!CheckKeyCount(s)) return false;
    DoParse(m_Val);
    return true;
  }

 private:
  bool CheckKeyCount(const TString &s)
  {
    if (!StringsAreEqual(m_sKey, s)) return false;
    if (m_nCount==0) throw std::exception("Count is zero");
    --m_nCount;
    return true;
  }

 private:
  void DoParse(bool &bVal);
  void DoParse(int &nVal);
  void DoParse(TString &sVal);
  void DoParse(TChar &chVal);

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

template<typename SOURCE, typename VALUE>
void TCmdOptionParser<SOURCE, VALUE>::DoParse(bool &bVal)
{
  bVal=true;
}

template<typename SOURCE, typename VALUE>
void TCmdOptionParser<SOURCE, VALUE>::DoParse(int &nVal)
{
  TString s;
  if (!m_Src.Get(s)) return;

  int n=0;
  if (!String2Int(s, n)) return;
  nVal=n;
}

template<typename SOURCE, typename VALUE>
void TCmdOptionParser<SOURCE, VALUE>::DoParse(TString &sVal)
{
  TString s;
  if (!m_Src.Get(s)) return;
  sVal=s;
}

template<typename SOURCE, typename VALUE>
void TCmdOptionParser<SOURCE, VALUE>::DoParse(TChar &chVal)
{
  TString s;
  if (!m_Src.Get(s)) return;
  if (s.length()!=1) return;
  chVal=s[0];
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
  TCmdLineParser();

 public:
  bool Parse(SOURCE &src, TOptions<TChar> &opt)
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

template <typename CH>
class TCmdLineSrc {
 public:
  typedef CH TChar;
  typedef basic_string<CH> TString;

 public:
  TCmdLineSrc(std::basic_istream<CH> &stream)
   : m_stream(stream),
     m_cct(' ', '\"') {}

 public:
  bool Get(TString &s)
  {
    TinyCsv::TCsvIterator<TinyCsv::TCellStr<std::string, true> > It;
    It.Cell();
    return true;
  }

 private:
  std::basic_istream<CH> &m_stream;
  TinyCsv::TBaseCharTraits<CH> m_bct;
  TinyCsv::TCsvCharTraits<CH> m_cct;
/*


  TCharTag ct=cEoF;
  do {
    const int ch=is.get();
    ct=CharTagEof(ch, bct, cct);

    if (!It(ch, ct, Tester)) return;
  } while (ct!=cEoF);

*/
  
};

bool ParseCmdLine(int       argc,
                  const char* const argv[],
                  TOptions<char> &Options)
{
  TDummiSrc src;
  TCmdLineParser<TDummiSrc> parser;
  parser.Parse(src, Options);
  return false;
}

template <typename CH>
bool TCmdLine<CH>::Parse(const CH           *szCmdLine,
                               TOptions<CH> &Options)
{
  std::basic_istringstream<CH> ss(szCmdLine);

  TCmdLineSrc<CH> src(ss); 
  TCmdLineParser<TCmdLineSrc<CH> > parser;
  parser.Parse(src, Options);

  return false;
}

template TCmdLine<char>;
template TCmdLine<wchar_t>;