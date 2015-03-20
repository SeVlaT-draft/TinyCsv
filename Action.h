#ifndef Action_h_already_included__15_03_2015__318A4D49
#define Action_h_already_included__15_03_2015__318A4D49
//
// SeVlaT, 15.03.2015
//

namespace TinyCsv {

// Line - in text file

// CSV terminology:
//
// TinyCsv |  Abbrev |  Alternative
//         |         |
// Row,       R         Record,
// Cell,      C         Column, Field,
// File       F
// Quote      Q, Qte
// Separator     Sep
// Whitespace W, Wsp

// New        N
// Add        A           Append

// Leading    L
// Trailing   T

enum TActionFlags {
  oNone = 0x0000,  // Nothing to do

// Test buffer operations
  oNew  = 0x0001,  // Clear buffer
  oAdd  = 0x0002,  // Add symbol

  oLWsp = 0x0020,  // Leading whitespace
  oTWsp = 0x0040,  // Whitespace, that may be trailing

// End marks
  oEoC  = 0x0100,  // End-of-Cell, buffer contains cell's text
  oEoR  = 0x0200,  // End-of-Row
  oEoF  = 0x0400,  // End-of-File

// Errors
  oErTx = 0x1000,  // Error in text cell
  oErSt = 0x2000,  // Error in stopped state
  oErQ0 = 0x4000,  // Error in escaped cell (inside quotes)
  oErQ1 = 0x8000,  // Error in escaped cell (after closing quote)

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
  const bool bNew;
  const bool bAdd;

  const bool bLWsp;
  const bool bTWsp;

  explicit TBufAction(TActionFlags sf)
   : bNew  ((sf & oNew )!=0),
     bAdd  ((sf & oAdd )!=0),
     bLWsp ((sf & oLWsp)!=0),
     bTWsp ((sf & oTWsp)!=0)  {}

  TActionFlags Flags() const
  {
    const unsigned int dw = (bNew  ? oNew : 0) |
                            (bAdd  ? oAdd : 0) |
                            (bLWsp ? oLWsp: 0) |
                            (bTWsp ? oTWsp: 0);

    return static_cast<TActionFlags>(dw);
  }
};

struct TErrorAction {
  const bool bErTxt;
  const bool bErQu0;
  const bool bErQu1;
  const bool bErEof;

  const bool bError;

  explicit TErrorAction(TActionFlags sf)
   : bErTxt((sf & oErTx)!=0),
     bErQu0((sf & oErQ0)!=0),
     bErQu1((sf & oErQ1)!=0),
     bErEof((sf & oErSt)!=0),

     bError((sf & oErr )!=0)  {} //!!!!!

  TActionFlags Flags() const
  {
    const unsigned int dw = (bErTxt? oErTx: 0) |
                            (bErQu0? oErQ0: 0) |
                            (bErQu1? oErQ1: 0) |
                            (bErEof? oErSt: 0);

    return static_cast<TActionFlags>(dw);
  }
};

struct TAction {
  const TBufAction   BufAction;
  const TErrorAction ErrorAction;

  const bool bEoC;
  const bool bEoR;
  const bool bEoF;

  const bool bStop;

  explicit TAction(TActionFlags sf)
   : BufAction(sf),
     ErrorAction(sf),

     bEoC  ((sf & oEoC )!=0),
     bEoR  ((sf & oEoR )!=0),
     bEoF  ((sf & oEoF )!=0),
     bStop ((sf & oStop)!=0)  {} //!!!!!

  TActionFlags Flags() const
  {
    const unsigned int dwBA = BufAction.Flags();
    const unsigned int dwEA = ErrorAction.Flags();

    const unsigned int dw = (bEoC  ? oEoC : 0) |
                            (bEoR  ? oEoR : 0) |
                            (bEoF  ? oEoF : 0);

    return static_cast<TActionFlags>(dw | dwBA | dwEA);
  }
};

} // namespace TinyCsv

#endif
