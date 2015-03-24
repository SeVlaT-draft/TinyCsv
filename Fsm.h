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
  const TState sStart, sEoC, sEoR, sEoCR, sNEoC, sNEoR;
  const TState sWsp0, sWsp, sTxt0, sTxt, sTWsp;
  const TState sQHead, sQTxt, sQuote, sQTail;
  const TState sEoF, sEoCF;
  const TState sErTxt, sErEof, sErQu0, sErQu1;

 public:
  TFsmDescr()
   :
      sHead ("sHead",  oNone,   cEoF,   cEoL,   cSep,   cQte,   cWsp,   cSmb  ),

      sStart("sStart", oNone,   sEoF,   sNEoR,  sNEoC,  sQHead, sWsp0,  sTxt0 ),
      sEoC  ("sEoC",   oEoC,    sEoF,   sNEoR,  sNEoC,  sQHead, sWsp0,  sTxt0 ),
      sNEoC ("sNEoC",  oNEoC,   sEoF,   sNEoR,  sNEoC,  sQHead, sWsp0,  sTxt0 ),
      sEoR  ("sEoR",   oEoR,    sEoF,   sNEoR,  sNEoC,  sQHead, sWsp0,  sTxt0 ),
      sNEoR ("sNEoR",  oNEoR,   sEoF,   sNEoR,  sNEoC,  sQHead, sWsp0,  sTxt0 ),
      sEoCR ("sEoCR",  oEoCR,   sEoF,   sNEoR,  sNEoC,  sQHead, sWsp0,  sTxt0 ),

      sWsp0 ("sWsp0",  oNALW,   sEoCF,  sEoCR,  sEoC,   sQHead, sWsp,   sTxt  ),
      sWsp  ("sWsp",   oALW,    sEoCF,  sEoCR,  sEoC,   sQHead, sWsp,   sTxt  ),
      sTxt0 ("sTxt0",  oNAdd,   sEoCF,  sEoCR,  sEoC,   sErTxt, sTWsp,  sTxt  ),
      sTxt  ("sTxt",   oAdd,    sEoCF,  sEoCR,  sEoC,   sErTxt, sTWsp,  sTxt  ),
      sTWsp ("sTWsp",  oATW,    sEoCF,  sEoCR,  sEoC,   sErTxt, sTWsp,  sTxt  ),

      sQHead("sQHead", oNew,    sErQu0, sQTxt,  sQTxt,  sQuote, sQTxt,  sQTxt ),
      sQTxt ("sQTxt",  oAdd,    sErQu0, sQTxt,  sQTxt,  sQuote, sQTxt,  sQTxt ),
      sQuote("sQuote", oNone,   sEoCF,  sEoCR,  sEoC,   sQTxt,  sQTail, sErQu1),
      sQTail("sQTail", oEoC,    sEoF,   sEoR,   sStart, sErQu1, sQTail, sErQu1),

      sEoF  ("sEoF",   oEoF,    sErEof, sErEof, sErEof, sErEof, sErEof, sErEof),
      sEoCF ("sEoCF",  oEoCF,   sErEof, sErEof, sErEof, sErEof, sErEof, sErEof),

      sErTxt("sErTxt", oErTx,   sErEof, sErEof, sErEof, sErEof, sErEof, sErEof),
      sErQu0("sErQu0", oErQ0,   sErEof, sErEof, sErEof, sErEof, sErEof, sErEof),
      sErQu1("sErQu1", oErQ1,   sErEof, sErEof, sErEof, sErEof, sErEof, sErEof),
      sErEof("sErEof", oErSt,   sErEof, sErEof, sErEof, sErEof, sErEof, sErEof)
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
