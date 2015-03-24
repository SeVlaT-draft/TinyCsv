#ifndef Source_h_already_included__23_03_2015__118A9678
#define Source_h_already_included__23_03_2015__118A9678
//
// SeVlaT, 23.03.2015
//

#include "Char.h"

namespace TinyCsv {

////////////////////////////////////////////////////////////////////////////////
template <typename STREAM,
          typename CH=char,
          typename BASE_CHAR_TRAITS=TBaseCharTraits<CH>,
          typename CSV_CHAR_TRAITS=TCsvCharTraits<CH> >
class TStreamSource {
 public:
  typedef CH TChar;

 public:
  explicit TStreamSource(      STREAM           &Stream,
                         const BASE_CHAR_TRAITS &bct=BASE_CHAR_TRAITS(),
                         const CSV_CHAR_TRAITS  &cct=CSV_CHAR_TRAITS())
   : m_Stream(Stream),
     m_bct(bct),
     m_cct(cct) {}

 public:
  TCharTag GetChar(TChar &ch)
  {
    const int nCh=m_Stream.get();
    if (!m_Stream) return cEoF;
    ch=static_cast<CH>(nCh);
    return CharTag(ch, m_bct, m_cct);
  }

 private:
  STREAM &m_Stream;

 private:
  BASE_CHAR_TRAITS m_bct;
  CSV_CHAR_TRAITS  m_cct;
};

} // namespace TinyCsv

#endif
