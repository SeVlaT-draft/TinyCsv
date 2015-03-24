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
template <typename FSM>
class TInput {
 public:
  bool NextCell()
  {
    if (m_Fsm.Action().bStop) return false;
    if (m_cntRec.IsStopped()) return false;

    m_cntFld.Over();
    return true;
  }

  bool NextRow()
  {
    if (m_Fsm.Action().bStop) return false;

    m_cntRec.Over();
    m_cntFld=TCounter();

    return true;
  }

 public:
  bool IsEoC() const   { return m_cntFld.IsStopped(); }
  bool IsEoR() const   { return m_cntRec.IsStopped(); }

  int GetRowNumber() const { return m_cntRec.Cur(); }
  int GetCellNumber()  const { return m_cntFld.Cur(); }

 public:
  template <typename SOURCE, typename CELL>
  bool Iteration(SOURCE &Source, CELL &Cell)
  {
    typedef typename SOURCE::TChar TChar;
    TChar ch=0;
    const TCharTag ct=Source.GetChar(ch);
    return Iteration(ch, ct, Cell);
  }

  template <typename CH, typename CELL>
  bool Iteration(CH ch, TCharTag ct, CELL &Cell)
  {
    m_Fsm.Next(ct);

    const TAction &Action=m_Fsm.Action();

    Cell.Next(ch, Action.BufAction);

    m_cntFld.Next(Action.bEoC);
    m_cntRec.Next(Action.bEoR);

    return !m_cntFld.IsStopped() &&
           !m_cntRec.IsStopped() &&
           !Action.bStop;
  }

 private:
  FSM m_Fsm;

 private:
  TCounter m_cntFld;
  TCounter m_cntRec;
};

} // namespace TinyCsv

#endif
