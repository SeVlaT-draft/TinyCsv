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
  bool IsEoF() const { return m_Fsm.Action().bEoF; }
  bool IsEoC() const { return m_cntFld.IsStopped(); }
  bool IsEoR() const { return m_cntRec.IsStopped(); }

  int GetRowNumber()  const { return m_cntRec.Cur(); }
  int GetCellNumber() const { return m_cntFld.Cur(); }

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

////////////////////////////////////////////////////////////////////////////////
//    <Cell>     <Cell>     <Cell>          <Cell>     <Cell>          
//          <EoC>      <EoC>      <EoC>           <EoC>      <EoC>     
//                                     <EoR>                      <EoR>
//       --|----------|----------|     ----------|----------|
//       -------|----------|----------|     ----------|----------|
//          -------------------------------|--------------------------|
template <typename CELL>
class TCsvIterator {
 public:
  template <typename SOURCE>
  bool Cell(SOURCE &Source)
  {
    if (m_Input.IsEoR()) return false;
    if (m_Input.IsEoF()) return false;

    while (m_Input.Iteration(Source, m_Cell));

    m_Input.NextCell();

    return true;
  }

 public:
  template <typename SOURCE, typename VISITOR>
  bool Cell(SOURCE &Source, VISITOR &Visitor)
  {
    while (m_Input.Iteration(Source, m_Cell));

    if (m_Input.IsEoC())
      Visitor.Cell(m_Cell.Begin(), m_Cell.End(), m_Input.GetCellNumber());

    return m_Input.NextCell();
  }

  template <typename SOURCE, typename VISITOR>
  bool Row(SOURCE &Source, VISITOR &Visitor)
  {
    while (Cell(Source, Visitor)) ;
    return m_Input.NextRow();
  }

  template <typename SOURCE, typename VISITOR>
  bool ReadRow(SOURCE &Source, VISITOR &Visitor) // return false if failed to read row
  {
    if (m_Input.IsEoF()) return false;

    while (Cell(Source, Visitor)) ;
    m_Input.NextRow();
    return true;
  }

 public:
  template <typename SOURCE, typename VISITOR>
  bool FullRow(SOURCE &Source, VISITOR &Visitor)
  {
    Visitor.RowBegin(m_Input.GetRowNumber());
    const bool bRes=Row(Source, Visitor);
    Visitor.RowEnd();
    return bRes;
  }

  template <typename SOURCE, typename VISITOR>
  void operator()(SOURCE &Source, VISITOR &Visitor)
  {
    while (FullRow(Source, Visitor)) ;
  }

 public:
  template <typename VISITOR>
  bool operator()(int       ch,
                  TCharTag  ct,
                  VISITOR  &Visitor)
  {
    if (m_Input.Iteration(ch, ct, m_Cell)) return true;

    const int nRow=m_Input.GetRowNumber();
    const int nCell=m_Input.GetCellNumber();

    if (nCell==0) Visitor.RowBegin(nRow);

    if (m_Input.IsEoC())
      Visitor.Cell(m_Cell.Begin(), m_Cell.End(), nCell);

    if (m_Input.NextCell()) return true;

    Visitor.RowEnd();

    return m_Input.NextRow();
  }


 private:
  typedef TFsm<TFsmDescr> TFsm1;
  TInput<TFsm1> m_Input;

 private:
  CELL m_Cell;
};


} // namespace TinyCsv

#endif
