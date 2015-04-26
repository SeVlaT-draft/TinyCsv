#ifndef Cell_h_already_included__22_03_2015__C8F78E0
#define Cell_h_already_included__22_03_2015__C8F78E0
//
// SeVlaT, 22.03.2015
// SeVlaT, 04.03.2015
//

#include "Action.h"

namespace TinyCsv {

// STRING should be a STL-like string class, such as
// std::string, std::wstring, QString

template <typename STRING, bool TRIMSPACES>
class TCellStr;

////////////////////////////////////////////////////////////////////////////////
template <typename STRING>
class TCellStr<STRING, false> {
 public:
  typedef typename STRING::value_type     TChar;
  typedef typename STRING::const_iterator TCIt;

 public:
  TCIt Begin() const { return m_s.begin(); }
  TCIt End()   const { return m_s.end();   }

 public:
  void Next(TChar ch, const TBufAction &BA)
  {
    if (BA.bNew) m_s=STRING();
    if (BA.bAdd) m_s+=ch;
  }

 private:
  STRING m_s;
};

////////////////////////////////////////////////////////////////////////////////
template <typename STRING>
class TCellStr<STRING, true> {
 public:
  typedef typename STRING::value_type     TChar;
  typedef typename STRING::const_iterator TCIt;

 public:
  TCellStr(): m_nLength(0) {}

 public:
  TCIt Begin() const { return m_s.begin();       }
  TCIt End()   const { return Begin()+m_nLength; }

 public:
  void Next(TChar ch, const TBufAction &BA)
  {
    if (BA.bNew) {
      m_s=STRING();
      m_nLength=0;
    }

    if (BA.bAdd) {
      if (BA.bLWsp) {
      } else if (BA.bTWsp) {
        m_s+=ch;
      } else {
        m_s+=ch;
        m_nLength=m_s.length();
      }
    }
  }

 private:
  STRING m_s;
  int m_nLength;
};

}

#endif
