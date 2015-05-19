#ifndef Fsm_h_already_included__08_02_2015__24320F89
#define Fsm_h_already_included__08_02_2015__24320F89
//
// SeVlaT, 08.02.2015
//

#include "Char.h"
#include "Action.h"

#include "Debug.h"

namespace TinyCsv {

class TFsmDescr {
 public:
  typedef TCharTag TInput;
  static const int nCharCatCount=cLast;

  typedef TAction TOutput;

  struct TState {
    const char *szName;
    TAction Action;
    const TState *Next[nCharCatCount];

    template <typename ST>
    TState(const char *szN, TActionFlags sf,
           const ST &sEof, const ST &sEol, const ST &sSep,
           const ST &sQte, const ST &sWsp, const ST &sSmb)
     : szName(szN),
       Action(sf)
    {
      Set(cEoF, sEof);
      Set(cEoL, sEol);
      Set(cSep, sSep);
      Set(cQte, sQte);
      Set(cWsp, sWsp);
      Set(cSmb, sSmb);
    }

   private:
    void Set(TCharTag ct, const TState &s)
    {
      Next[ct]=&s;
    }

    void Set(TCharTag ct0, TCharTag ct1)
    {
      TINY_CSV_ASSERT(ct0==ct1);
      Next[ct0]=this;
    }
  };

 private:
  const TState sHead; // A dummy state, just to be a header for transition table

  const TState sStart;                          // Start state
  const TState sEc, sEr, sEcr, sNEc, sNEcr;     // End-of-Cell, End-of-Row states
  const TState sWsp0, sWsp, sTxt0, sTxt, sTWsp; // States in ordinary cells
  const TState sQHead, sQTxt, sQuote, sQTail;   // States in quoted cells
  const TState sEf, sEcf, sNEcf;                // End-of-File states
  const TState sFTxt, sFEof, sFQuo0, sFQuo1;    // Fault states

 public:
  TFsmDescr()
   :
     sHead ("",        oNone,   cEoF,   cEoL,   cSep,   cQte,   cWsp,   cSmb  ),

     sStart("sStart",  oNone,   sEf,    sNEcr,  sNEc,   sQHead, sWsp0,  sTxt0 ),
     sEc   ("sEc",     oEc,     sNEcf,  sNEcr,  sNEc,   sQHead, sWsp0,  sTxt0 ),
     sNEc  ("sNEc",    oNEc,    sEf,    sNEcr,  sNEc,   sQHead, sWsp0,  sTxt0 ),
     sEr   ("sEr",     oEr,     sEf,    sNEcr,  sNEc,   sQHead, sWsp0,  sTxt0 ),
     sNEcr ("sNEcr",   oNEcr,   sEf,    sNEcr,  sNEc,   sQHead, sWsp0,  sTxt0 ),
     sEcr  ("sEcr",    oEcr,    sEf,    sNEcr,  sNEc,   sQHead, sWsp0,  sTxt0 ),

     sWsp0 ("sWsp0",   oNALW,   sEcf,   sEcr,   sEc,    sQHead, sWsp,   sTxt  ),
     sWsp  ("sWsp",    oALW,    sEcf,   sEcr,   sEc,    sQHead, sWsp,   sTxt  ),
     sTxt0 ("sTxt0",   oNAdd,   sEcf,   sEcr,   sEc,    sFTxt,  sTWsp,  sTxt  ),
     sTxt  ("sTxt",    oAdd,    sEcf,   sEcr,   sEc,    sFTxt,  sTWsp,  sTxt  ),
     sTWsp ("sTWsp",   oATW,    sEcf,   sEcr,   sEc,    sFTxt,  sTWsp,  sTxt  ),

     sQHead("sQHead",  oNew,    sFQuo0, sQTxt,  sQTxt,  sQuote, sQTxt,  sQTxt ),
     sQTxt ("sQTxt",   oAdd,    sFQuo0, sQTxt,  sQTxt,  sQuote, sQTxt,  sQTxt ),
     sQuote("sQuote",  oNone,   sEcf,   sEcr,   sEc,    sQTxt,  sQTail, sFQuo1),
     sQTail("sQTail",  oEc,     sEf,    sEr,    sStart, sFQuo1, sQTail, sFQuo1),

     sEf   ("sEf",     oEf,     sFEof,  sFEof,  sFEof,  sFEof,  sFEof,  sFEof ),
     sEcf  ("sEcf",    oEcf,    sFEof,  sFEof,  sFEof,  sFEof,  sFEof,  sFEof ),
     sNEcf ("sNEcf",   oNEcf,   sFEof,  sFEof,  sFEof,  sFEof,  sFEof,  sFEof ),

     sFTxt ("sFTxt",   oFTxt,   sFEof,  sFEof,  sFEof,  sFEof,  sFEof,  sFEof ),
     sFQuo0("sFQuo0",  oFQu0,   sFEof,  sFEof,  sFEof,  sFEof,  sFEof,  sFEof ),
     sFQuo1("sFQuo1",  oFQu1,   sFEof,  sFEof,  sFEof,  sFEof,  sFEof,  sFEof ),
     sFEof ("sFEof",   oFStp,   sFEof,  sFEof,  sFEof,  sFEof,  sFEof,  sFEof )
  {}

 public:
  const TState* GetFirstState() const { return &sStart; }
};

////////////////////////////////////////////////////////////////////////////////
template <typename DESCR>
class TFsm {
 public:
  typedef typename DESCR::TState TState;

 public:
  TFsm()
   : m_pState(GetDescr().GetFirstState()) {}

 public:
  void Next(TCharTag cc) { m_pState=m_pState->Next[cc]; }

 public:
  const TAction& Action() const { return m_pState->Action; }

 public:
  const TState* GetCurState() const { return m_pState; }

 private:
  static const DESCR& GetDescr() {
    static const DESCR Descr;
    return Descr;
  }

 private:
  const TState *m_pState;
};

} // namespace TinyCsv

#endif
