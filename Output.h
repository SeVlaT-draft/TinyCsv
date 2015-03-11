#ifndef Output_h_already_included__18_02_2015__29DCF3EE
#define Output_h_already_included__18_02_2015__29DCF3EE
//
// SeVlaT, 18.02.2015
//

#include "Csv.h"

namespace TinyCsv {

template<typename CH>
struct TOutputParams {
  CH chPad;   // 0 if no padding

  bool bPadLeft;
  bool bPadRight;

  bool bPadLeftQuote;
  bool bPadRightQuote;

  bool bEscapeEdgeWhitespace;
  bool bEscapeAlways;
};

typedef TOutputParams<char>    TOutputParamsA;
typedef TOutputParams<wchar_t> TOutputParamsW;

template <typename ITO, typename CH>
ITO Pad(ITO io, CH chPad, int nPad)
{
  for (int i=0; i<nPad; ++i) {
    *io=chPad;
    ++io;
  }

  return io;
}

template <typename ITI, typename ITO, typename CH, typename COPIER>
ITO AlignLeft(      ITI     iiB,
                    ITI     iiE,
                    ITO     io,
              const COPIER &copier,
                    CH      chPadRight,
                    int     nPlaceWidth)
{
  ITO oiCur=copier(iiB, iiE, io);
  const int nFieldWidth=oiCur-io;
  const int nExtraSpace=nPlaceWidth-nFieldWidth;

  return Pad(oiCur, chPadRight, nExtraSpace);
}

template <typename ITI, typename ITO, typename CH, typename COPIER>
ITO AlignCenterRight(      ITI     iiB,
                           ITI     iiE,
                           ITO     io,
                     const COPIER &copier,
                           CH      chPad,
                           int     nPlaceWidth,
                           int     nFieldWidth,
                           bool    bCenter)
{
  const int nExtraSpace=nPlaceWidth-nFieldWidth;
  const int nExtraSpace0=bCenter? nExtraSpace/2: nExtraSpace;

  ITO oiCur=Pad(io, chPad, nExtraSpace0);
  oiCur=copier(iiB, iiE, oiCur);
  const int nCur=oiCur-io;
  return Pad(oiCur, chPad, nPlaceWidth-nCur);
}


// SQE - Sep, Quote, Eol
// LTS - leading or trailing space
//                                                 FF   FT   TF   TT
//   No SQE, no LTS                                -    +    -    +
//      SQE, no LTS                                +    +    +    +
//   no SQE,    LTS                                -    +    +    +
//      SQE,    LTS                                +    +    +    +

} // namespace TinyCsv

#endif
