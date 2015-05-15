#ifndef Input_h_already_included__18_02_2015__27A8A72B
#define Input_h_already_included__18_02_2015__27A8A72B
//
// SeVlaT, 18.02.2015
//

#include "Char.h"

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
//  <Cell>       <Cell>       <Cell>             <Cell>                         
//         <EoC>        <EoC>        <EoC>              <EoC> <EoC>             
//                                         <EoR>                    <EoR> <EoR> 
//         ----|        ----|        ----|              ----| ----|             
//                                         ----|                    ----| ----| 
//*    --|       -----|       -----|             -----|      |                  
//*    --|------------|------------|------------------|------|------------------
//*    --|------------|------------|------|      -----|------|-----|            
//     --------|------------|------------|------------------|-----|-------      
//     --------|------------|------------||      -----------|-----||     |      
//*  -------------------------------------|------------------------|-----|---   
//        -------------------------------------|------------------------|-----| 
//                                         <EoR>                    <EoR> <EoR> 
//         <EoC>        <EoC>        <EoC>              <EoC> <EoC>             
//  <Cell>       <Cell>       <Cell>             <Cell>                         
template <typename FSM, typename CELL>
class TInput {
 public:
  const CELL& Cell() const { return m_Cell; }

 public:
  bool IsStop() const { return m_Fsm.Action().bStop; }

  bool IsEoF() const { return m_Fsm.Action().bEoF; }
  bool IsEoC() const { return m_cntFld.IsStopped(); }
  bool IsEoR() const { return m_cntRec.IsStopped(); }

  int GetRowNumber()  const { return m_cntRec.Cur(); }
  int GetCellNumber() const { return m_cntFld.Cur(); }

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
  template <typename SOURCE>
  bool ProcessChar(SOURCE &Source)
  {
    typedef typename SOURCE::TChar TChar;
    TChar ch=0;
    const TCharTag ct=Source.GetChar(ch);
    return ProcessChar(ch, ct);
  }

  template <typename CH>
  bool ProcessChar(CH ch, TCharTag ct)
  {
    m_Fsm.Next(ct);

    const TAction &Action=m_Fsm.Action();

    m_Cell.Next(ch, Action.BufAction);

    m_cntFld.Next(Action.bEoC);
    m_cntRec.Next(Action.bEoR);

    return !m_cntFld.IsStopped() &&
           !m_cntRec.IsStopped() &&
           !Action.bStop;
  }

 public:
  template <typename SOURCE>
  bool ReadCell(SOURCE &Source)
  {
    if (IsStop()) return false;
    if (IsEoR()) return false;

    if (IsEoC()) NextCell();

    while (ProcessChar(Source));

    return IsEoC();
  }

  template <typename SOURCE>
  bool ReadCellThroughRows(SOURCE &Source)
  {
    while (!ReadCell(Source)) {
      if (IsStop()) return false;
      if (IsEoR()) NextRow();
    }

    return true;
  }

  template <typename SOURCE>
  bool ReadCell(SOURCE &Source, bool bThroughRows)
  {
    if (bThroughRows) return ReadCellThroughRows(Source);
                 else return ReadCell(Source);
  }

  template <typename SOURCE, typename VISITOR>
  bool ReadCell(SOURCE &Source, VISITOR &Visitor, bool bThroughRows)
  {
    if (!ReadCell(Source, bThroughRows)) return false;
    Visitor.Cell(m_Cell.Begin(), m_Cell.End(), GetCellNumber());
    return true;
  }

 public:
  template <typename SOURCE, typename VISITOR>
  bool ReadRow(SOURCE &Source, VISITOR &Visitor)
  {
    if (IsStop()) return false;
    if (IsEoR()) NextRow();
    while (ReadCell(Source, Visitor, false)) ;
    return true;
  }

 public:
  template <typename SOURCE, typename VISITOR>
  bool ReadFullRow(SOURCE &Source, VISITOR &Visitor)
  {
    if (IsStop()) return false;

    Visitor.RowBegin(GetRowNumber());
    const bool bRes=ReadRow(Source, Visitor);
    Visitor.RowEnd();
    return bRes;
  }

  template <typename SOURCE, typename VISITOR>
  void ReadAll(SOURCE &Source, VISITOR &Visitor)
  {
    while (ReadFullRow(Source, Visitor)) ;
  }

 public:
  template <typename VISITOR>
  bool operator()(int       ch,
                  TCharTag  ct,
                  VISITOR  &Visitor)
  {
    if (ProcessChar(ch, ct)) return true;

    const int nRow=GetRowNumber();
    const int nCell=GetCellNumber();

    if (nCell==0) Visitor.RowBegin(nRow);

    if (IsEoC())
      Visitor.Cell(m_Cell.Begin(), m_Cell.End(), nCell);

    if (NextCell()) return true;

    Visitor.RowEnd();

    return NextRow();
  }

 private:
  FSM  m_Fsm;
  CELL m_Cell;

 private:
  TCounter m_cntFld;
  TCounter m_cntRec;
};


} // namespace TinyCsv

#endif
