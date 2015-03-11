#ifndef Input_h_already_included__18_02_2015__27A8A72B
#define Input_h_already_included__18_02_2015__27A8A72B
//
// SeVlaT, 18.02.2015
//

#include "Char.h"
#include "Fsm.h"

#include "Debug.h"

namespace TinyCsv {

// <Fld><Fld><Fld><EoR>
// <Fld><Fld><Fld><Fld><EoR>
//                     <EoF>

// c    c    c    c    c    c    S      c    c    c    S    c    S   
// c    c    c    S    c    S      c    c    c    S    c    S   
//   0                               1                             
//   0              1         x      0              1         x
//  [It] [It] [NF] [It] [NF] [NR]   [It] [It] [NF] [It] [NF] [NR]
//
//  [It] [It] [NF] [It] [NF] [NR]   [It] [It] [NF] [It] [NF] [NR]
//     T    F    T    F    F    T      T    F    T    F    F    F   
class TCounter {
 public:
  TCounter()
   :  m_n(0),
      m_bSep(false) {}

 public:
  bool IsStopped() const { return m_bSep; }
  int Cur() const { return m_n; }

 public:
  void Next(bool bSep)
   {
     TINY_CSV_ASSERT(!m_bSep);
     m_bSep=bSep;
   }

  void Over()
   {
     TINY_CSV_ASSERT(m_bSep);
     if (m_bSep) ++m_n;
     m_bSep=false;
   }

 private:
  int m_n;
  bool m_bSep;
};

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
    if (m_Stream >> ch) return CharTag(ch, m_bct, m_cct);
    return cEof;
  }

 private:
  STREAM &m_Stream;

 private:
  BASE_CHAR_TRAITS m_bct;
  CSV_CHAR_TRAITS  m_cct;
};

////////////////////////////////////////////////////////////////////////////////
template <typename FSM, typename SOURCE, typename FIELD>
class TInput {
  typedef typename SOURCE::TChar TChar;

 public:
  TInput(SOURCE &Source, FIELD &Field)
   : m_Source(Source),
     m_Field(Field) {}

 public:
  bool NextField()
  {
    if (m_cntRec.IsStopped()) return false;

    m_cntFld.Over();
    return true;
  }

  bool NextRecord()
  {
    if (m_Fsm.Cur().bStop) return false;

    m_cntRec.Over();
    m_cntFld=TCounter();

    return true;
  }

 public:
  bool IsEoR() const   { return m_cntRec.IsStopped(); }

  int GetRecordNumber() const { return m_cntRec.Cur(); }
  int GetFieldNumber()  const { return m_cntFld.Cur(); }

 public:
  bool Iteration()
  {
    TChar ch=0;
    const TCharTag ct=m_Source.GetChar(ch);
    return Iteration(ch, ct);
  }

  bool Iteration(TChar ch, TCharTag ct)
  {
    m_Fsm.Next(ct);
    return Iteration(ch, ct, m_Fsm.Cur());
  }

 private:
  bool Iteration(      TChar     ch,
                       TCharTag  ct,
                 const TAction  &Action)
  {
    m_Field.Next(ch, ct, Action.bNew, Action.bAdd);

    m_cntFld.Next(Action.bEoC);
    m_cntRec.Next(Action.bEoR);

    return !m_cntFld.IsStopped() &&
           !m_cntRec.IsStopped() &&
           !Action.bStop;
  }

 private:
  SOURCE &m_Source;

 private:
  FSM m_Fsm;

 private:
  FIELD &m_Field;

 private:
  TCounter m_cntFld;
  TCounter m_cntRec;
};

} // namespace TinyCsv

#endif
