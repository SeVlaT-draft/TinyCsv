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

  bool bPadQuote;

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


////////////////////////////////////////////////////////////////////////////////
template <typename CH,
          typename OITERATOR,
          typename BASE_CHAR_TRAITS=TBaseCharTraitsA,
          typename CSV_CHAR_TRAITS=TCsvCharTraitsA,
          typename COPIER=CopierIfNeedEscape<BASE_CHAR_TRAITS,
                                             CSV_CHAR_TRAITS> >
class TOutput {
 public:
  TOutput(      OITERATOR        &oit,
          const BASE_CHAR_TRAITS &bct=BASE_CHAR_TRAITS(),
          const CSV_CHAR_TRAITS  &cct=CSV_CHAR_TRAITS(),
          const COPIER           &Copier=COPIER())
   : m_oit(oit),
     m_bct(bct),
     m_cct(cct),
     m_Copier(Copier),
     m_nCell(0),
     m_nRow(0)  {}

 public:
  template <typename ITI>
  void Cell(ITI iiB, ITI iiE) { BoC(); m_Copier(iiB, iiE, m_oit); ++m_nCell; }

  void Cell() { BoC(); ++m_nCell; }

  void Cell(CH ch) { BoC(); m_oit++=ch; ++m_nCell; }

 public:
  template<int N>
  void Cell(const CH (&str)[N]) { Cell(str+0, str+N); }

  template <typename STRING>
  void Cell(const STRING &s) { Cell(s.begin(), s.end()); }

 public:
  void EndOfLine()
  {
    m_nCell=0;
    ++m_nRow;
    m_oit++=m_bct.chEoL;
  }

 private:
  void BoC()
  { if (m_nCell>0) m_oit++=m_cct.chSep; }

 private:
  OITERATOR &m_oit;

 private:
  COPIER m_Copier;

 private:
  const BASE_CHAR_TRAITS m_bct;
  const CSV_CHAR_TRAITS  m_cct;

 private:
  int m_nCell;
  int m_nRow;

};
// SQE - Sep, Quote, Eol
// LTS - leading or trailing space
//                                                 FF   FT   TF   TT
//   No SQE, no LTS                                -    +    -    +
//      SQE, no LTS                                +    +    +    +
//   no SQE,    LTS                                -    +    +    +
//      SQE,    LTS                                +    +    +    +

} // namespace TinyCsv

#endif
