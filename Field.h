#ifndef Field_h_already_included__04_03_2015__F60E2D0
#define Field_h_already_included__04_03_2015__F60E2D0
//
// SeVlaT, 04.03.2015
//

#include "Char.h"

namespace TinyCsv {

// STRING should be a STL-like string class, such as
// std::string, std::wstring, QString

template <typename STRING, bool TRIMSPACES>
class TField;

////////////////////////////////////////////////////////////////////////////////
template <typename STRING>
class TField<STRING, false> {
 public:
  typedef typename STRING::value_type     TChar;
  typedef typename STRING::const_iterator TCIt;

 public:
  TCIt Begin() const { return m_s.begin(); }
  TCIt End()   const { return m_s.end();   }

 public:
  void Next(TChar    ch,
            TCharTag,
            bool     bClear,
            bool     bAdd)
  {
    if (bClear) m_s=STRING();
    if (bAdd)   m_s+=ch;
  }

 private:
  STRING m_s;
};

////////////////////////////////////////////////////////////////////////////////
template <typename STRING>
class TField<STRING, true> {
 public:
  typedef typename STRING::value_type     TChar;
  typedef typename STRING::const_iterator TCIt;

 public:
  TField(): m_nLength(0) {}

 public:
  TCIt Begin() const { return m_s.begin();       }
  TCIt End()   const { return Begin()+m_nLength; }

 public:
  void Next(TChar    ch,
            TCharTag ct,
            bool     bClear,
            bool     bAdd)
  {
    if (bClear) {
      m_s=STRING();
      m_nLength=0;
    }

    if (bAdd) {
      if (ct==cWsp) {
        if (!m_s.empty()) m_s+=ch;
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
