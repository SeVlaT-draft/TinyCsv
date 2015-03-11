#ifndef FsmChecker_h_already_included__01_03_2015__28B5821B
#define FsmChecker_h_already_included__01_03_2015__28B5821B
//
// SeVlaT, 01.03.2015
//

namespace TinyCsv {

template <typename DESCR>
class TChecker {
 public:
  typedef typename DESCR::TState TState;

 public:
  TChecker(): m_nStateCount(0) {}

 public:
  bool CheckState(const TState *ps)
  {
    int iState=m_nStateCount;

    if (!AddState(ps)) return false;

    for ( ; iState<m_nStateCount; ++iState) {
      if (!ProcessState(m_States[iState])) return false;
    }

    return true;
  }

 private:
  bool AddState(const TState *ps)
  {
    if (!ps) return false;

    for (int i=0; i<m_nStateCount; ++i)
      if (m_States[i]==ps) return true;

    if (m_nStateCount==c_nMaxStates) return false;
    m_States[m_nStateCount]=ps;
    ++m_nStateCount;

    return true;
  }

  bool ProcessState(const TState *ps)
  {
    if (!ps) return false;
    const TState &s=*ps;

    if (!CheckState(s)) return false;

    for (int i=0; i<sizeof(s.Next)/sizeof(s.Next[0]); ++i)
      if (!AddState(s.Next[i])) return false;

    return true;
  }

  bool CheckState(const TState &s) const
  {
    return true;
  }

 private:
  static const int c_nMaxStates=100;
  const TState* m_States[c_nMaxStates];
  int m_nStateCount;
};

} // namespace TinyCsv

#endif
