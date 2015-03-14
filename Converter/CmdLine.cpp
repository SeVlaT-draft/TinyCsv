// SeVlaT, 12.03.2015
//
#include "CmdLine.h"

template <typename CH>
class TCmdOption {
 public:
  typedef basic_string<CH> TString;

 public:
  enum TState {
    sNotSet,
    sProcess,
    sDone
  };
  
 public:
  explicit TCmdOption(const TString &sKey)
   : m_sKey(szKey),
     m_bStarted(false) {}

 public:
  bool Start(const CH *szKey)
   { if 

 public:
  virtual TState State() const { return m_bStarted? sDone; sNotSet; }

 private:
  const TString m_sKey;
  bool m_bStarted;
};

template <typename CH>
class TCmdOption1: public TCmdOption0 {
 public:
  explicit TCmdOption0(const CH *szKey)
   : TCmdOption0(szKey),
     m_bDone(false) {}

 public:
  virtual TState State() const { return m_bDone? sDone; sNotSet; }

 private:
  const TString m_sKey;
  bool m_bDone;
};

template <typename CH>
class TCmdLineParser {
 public:
  typedef basic_string<CH> TString;

 public:
  TCmdLineParser()
   {}
   
 public:
  bool Add(const TString &s)
  
};

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

