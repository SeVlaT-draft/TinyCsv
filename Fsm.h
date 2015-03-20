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

    TState(const char *szN, TActionFlags sf,
           const TState &sSep, const TState &sQte, const TState &sWsp,
           const TState &sSmb, const TState &sEol, const TState &sEof)
     : szName(szN),
       Action(sf)
    {
      Next[cSep]=&sSep;  Next[cQte]=&sQte;  Next[cWsp]=&sWsp;
      Next[cSmb]=&sSmb;  Next[cEoL]=&sEol;  Next[cEoF]=&sEof;
    }

  };

 private:
  const TState sStart, sEoC, sEoR, sEoCR, sNEoC, sNEoR;
  const TState sWsp0, sWsp, sTxt0, sTxt, sTWsp;
  const TState sQHead, sQTxt, sQuote, sQTail;
  const TState sEoF, sEoCF;
  const TState sErTxt, sErEof, sErQu0, sErQu1;

 private:
  typedef TSuccessiveIndex<TCharTag> Tbl;
  const Tbl Trans;

 public:
  TFsmDescr()
   :
      Trans (Tbl::Head<         cSep,   cQte,   cWsp,   cSmb,   cEoL,   cEoF  >()),

      sStart("sStart", oNone,   sNEoC,  sQHead, sWsp0,  sTxt0,  sNEoR,  sEoF  ),
      sEoC  ("sEoC",   oEoC,    sNEoC,  sQHead, sWsp0,  sTxt0,  sNEoR,  sEoF  ),
      sNEoC ("sNEoC",  oNEoC,   sNEoC,  sQHead, sWsp0,  sTxt0,  sNEoR,  sEoF  ),
      sEoR  ("sEoR",   oEoR,    sNEoC,  sQHead, sWsp0,  sTxt0,  sNEoR,  sEoF  ),
      sNEoR ("sNEoR",  oNEoR,   sNEoC,  sQHead, sWsp0,  sTxt0,  sNEoR,  sEoF  ),
      sEoCR ("sEoCR",  oEoCR,   sNEoC,  sQHead, sWsp0,  sTxt0,  sNEoR,  sEoF  ),

      sWsp0 ("sWsp0",  oNALW,   sEoC,   sQHead, sWsp,   sTxt,   sEoCR,  sEoCF ),
      sWsp  ("sWsp",   oALW,    sEoC,   sQHead, sWsp,   sTxt,   sEoCR,  sEoCF ),
      sTxt0 ("sTxt0",  oNAdd,   sEoC,   sErTxt, sTWsp,  sTxt,   sEoCR,  sEoCF ),
      sTxt  ("sTxt",   oAdd,    sEoC,   sErTxt, sTWsp,  sTxt,   sEoCR,  sEoCF ),
      sTWsp ("sTWsp",  oATW,    sEoC,   sErTxt, sTWsp,  sTxt,   sEoCR,  sEoCF ),

      sQHead("sQHead", oNew,    sQTxt,  sQuote, sQTxt,  sQTxt,  sQTxt,  sErQu0),
      sQTxt ("sQTxt",  oAdd,    sQTxt,  sQuote, sQTxt,  sQTxt,  sQTxt,  sErQu0),
      sQuote("sQuote", oNone,   sEoC,   sQTxt,  sQTail, sErQu1, sEoCR,  sEoCF ),
      sQTail("sQTail", oEoC,    sStart, sErQu1, sQTail, sErQu1, sEoR,   sEoF  ),

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
