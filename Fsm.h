#ifndef Fsm_h_already_included__08_02_2015__24320F89
#define Fsm_h_already_included__08_02_2015__24320F89
//
// SeVlaT, 08.02.2015
//

#include "Char.h"

namespace TinyCsv {

// Line - in text file

// CSV terminology:
//
// Main    Alternative       Abbrev
//       |                |
// Row,    Record,           R
// Cell,   Column, Field,    C
// File                      F

// New                       N
// Add,    Append            A 

enum TFlags {
  oNone = 0x0000,  // Nothing to do

// Buffer operations
  oNew  = 0x0001,  // Clear buffer
  oAdd  = 0x0002,  // Add symbol

  oLWsp = 0x0020,  // Leading whitespace
  oTWsp = 0x0040,  // Whitespace, that may be trailing

// Field
  oEoC  = 0x0100,  // End-of-Cell, buffer contains field's text
                
// End marks    
  oEoR  = 0x0200,  // End-of-Row
                
// Stopped      
// End-of-File  
  oEoF  = 0x0400,  // End-of-File
                
// Errors       
  oErTx = 0x1000,  // Error in text field
  oErSt = 0x2000,  // Error in stopped state
  oErQ0 = 0x4000,  // Error in escaped field (inside quotes)
  oErQ1 = 0x8000,  // Error in escaped field (after closing quote)

// Combined flags
  oALW  = oAdd | oLWsp,
  oATW  = oAdd | oTWsp,

  oNAdd = oNew | oAdd,
  oNALW = oNew | oALW,
  oNATW = oNew | oATW,

  oNEoC = oNew | oEoC,
  oNEoR = oNew | oEoR,

  oEoCR = oEoC | oEoR,
  oEoCF = oEoC | oEoF,

// Combined flags
  oErr  = oErTx | oErSt | oErQ0 | oErQ1,
  oStop = oErr  | oEoF
};

////////////////////////////////////////////////////////////////////////////////
struct TBufAction {
  bool bNew;
  bool bAdd;

  bool bLWsp;
  bool bTWsp;

  explicit TBufAction(TFlags o)
   : bNew  ((o & oNew )!=0),
     bAdd  ((o & oAdd )!=0),
     bLWsp ((o & oLWsp)!=0),
     bTWsp ((o & oTWsp)!=0)  {} //!!!!!

  TFlags O() const
  {
    const unsigned int dw = (bNew  ? oNew : 0) |
                            (bAdd  ? oAdd : 0) |
                            (bLWsp ? oLWsp: 0) |
                            (bTWsp ? oTWsp: 0);

    return static_cast<TFlags>(dw);
  }
};

struct TAction {
  TBufAction BufAction;

  bool bEoC;
  bool bEoR;
  bool bEoF;

  bool bErTxt;
  bool bErQu0;
  bool bErQu1;
  bool bErEof;

  bool bError;
  bool bStop;

  explicit TAction(TFlags o)
   : BufAction(o),
     bEoC  ((o & oEoC )!=0),
     bEoR  ((o & oEoR )!=0),
     bEoF  ((o & oEoF )!=0),
     bErTxt((o & oErTx)!=0),
     bErQu0((o & oErQ0)!=0),
     bErQu1((o & oErQ1)!=0),
     bErEof((o & oErSt)!=0),
     bError((o & oErr )!=0),
     bStop ((o & oStop)!=0)  {} //!!!!!

  TFlags O() const
  {
    const unsigned int dwBA = BufAction.O();

    const unsigned int dw = (bEoC  ? oEoC : 0) |
                            (bEoR  ? oEoR : 0) |
                            (bEoF  ? oEoF : 0) |
                            (bErTxt? oErTx: 0) |
                            (bErQu0? oErQ0: 0) |
                            (bErQu1? oErQ1: 0) |
                            (bErEof? oErSt: 0);

    return static_cast<TFlags>(dw | dwBA);
  }
};

class TFsmDescr {
 public:
  typedef TCharTag TInput;
  static const int nCharCatCount=cLast;

  typedef TAction TOutput;

  struct TState {
    const char *szName;
    TAction Action;
    const TState *Next[nCharCatCount];

    TState(const char *szN, TFlags f,
           const TState &sSep, const TState &sQte, const TState &sWsp,
           const TState &sSmb, const TState &sEol, const TState &sEof)
     : szName(szN),
       Action(f)
    {
      Next[cSep]=&sSep;  Next[cQte]=&sQte;  Next[cWsp]=&sWsp;
      Next[cSmb]=&sSmb;  Next[cEoL]=&sEol;  Next[cEoF]=&sEof;
    }

  };

 private:
  TState sStart, sEoC, sEoR, sEoCR, sNEoC, sNEoR;
  TState sWsp0, sWsp, sTxt0, sTxt, sTWsp;
  TState sQHead, sQTxt, sQuote, sQTail;
  TState sEoF, sEoCF;
  TState sErTxt, sErEof, sErQu0, sErQu1;

 public:
  TFsmDescr()
   :
   //                           cSep    cQte    cWsp    cSmb    cEol    cEof
      sStart("sStart", oNone,   sNEoC,  sQHead, sWsp0,  sTxt0,  sNEoR,  sEoF  ), // Remove?
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
  const TAction& Cur() const { return m_pState->Action; }

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
