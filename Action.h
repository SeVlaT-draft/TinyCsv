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
  oEc   = 0x0100,  // End-of-Cell, buffer contains cell's text
  oEr   = 0x0200,  // End-of-Row
  oEf   = 0x0400,  // End-of-File

// Errors
  oFTxt = 0x1000,  // Error in text cell
  oFStp = 0x2000,  // Error in stopped state
  oFQu0 = 0x4000,  // Error in escaped cell (inside quotes)
  oFQu1 = 0x8000,  // Error in escaped cell (after closing quote)

// Combined flags
  oALW  = oAdd | oLWsp,
  oATW  = oAdd | oTWsp,

  oNAdd = oNew | oAdd,
  oNALW = oNew | oALW,
  oNATW = oNew | oATW,

  oNEc  = oNew | oEc,
  oNEcr = oNew | oEc | oEr,
  oNEcf = oNew | oEc | oEf,

  oEcr  = oEc  | oEr,
  oEcf  = oEc  | oEf,

// Masks. 
  oMskFault= oFTxt | oFStp | oFQu0 | oFQu1,
  oMskStop = oMskFault | oEf
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
   : bErTxt((sf & oFTxt)!=0),
     bErQu0((sf & oFQu0)!=0),
     bErQu1((sf & oFQu1)!=0),
     bErEof((sf & oFStp)!=0),

     bError((sf & oMskFault )!=0)  {} //!!!!!

  TActionFlags Flags() const
  {
    const unsigned int dw = (bErTxt? oFTxt: 0) |
                            (bErQu0? oFQu0: 0) |
                            (bErQu1? oFQu1: 0) |
                            (bErEof? oFStp: 0);

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

     bEoC  ((sf & oEc )!=0),
     bEoR  ((sf & oEr )!=0),
     bEoF  ((sf & oEf )!=0),
     bStop ((sf & oMskStop)!=0)  {} //!!!!!

  TActionFlags Flags() const
  {
    const unsigned int dwBA = BufAction.Flags();
    const unsigned int dwEA = ErrorAction.Flags();

    const unsigned int dw = (bEoC  ? oEc : 0) |
                            (bEoR  ? oEr : 0) |
                            (bEoF  ? oEf : 0);

    return static_cast<TActionFlags>(dw | dwBA | dwEA);
  }
};

} // namespace TinyCsv

#endif
