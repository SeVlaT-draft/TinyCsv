#ifndef Escape_h_already_included__05_03_2015__7A015F5
#define Escape_h_already_included__05_03_2015__7A015F5
//
// SeVlaT, 05.03.2015
//

namespace TinyCsv {

template<typename CH,
         typename BASE_CHAR_TRAITS,
         typename CSV_CHAR_TRAITS>
bool NeedEscape(      CH                ch,
                const BASE_CHAR_TRAITS &bct,
                const CSV_CHAR_TRAITS  &cct)
{
  if (bct.IsSep(ch)) return true;
  if (bct.IsQte(ch)) return true;
  if (cct.IsEol(ch)) return true;

  return false;
}


template <typename II,
          typename BASE_CHAR_TRAITS,
          typename CSV_CHAR_TRAITS>
bool NeedEscape(      II                iiB,
                      II                iiE,
                const BASE_CHAR_TRAITS &bct,
                const CSV_CHAR_TRAITS  &cct,
                      bool              bEscapeEdgeWhitespace)
{
  if (iiB==iiE) return false;

  II ii=iiB;
  CH chCur=*ii;

  if (bEscapeEdgeWhitespace && bct.IsWsp(chCur)) return true;

  for ( ; ii!=iiE; ++ii) {
    chCur=*ii;
    if (NeedEscape(chCur, bct, cct)) return true;
  }

  if (bEscapeEdgeWhitespace && bct.IsWsp(chCur)) return true;
  return false;
}


template <typename II,
          typename IO,
          typename CSV_CHAR_TRAITS>
IO CopyEscaped(      II               iiB,
                     II               iiE,
                     IO               io,
               const CSV_CHAR_TRAITS &cct)
{
  *io=cct.chQte;
  ++io;

  for (II i=iiB; i!=iiE; ++i) {
    const CH ch=*i;
    if (cct.IsQte(ch)) {
      *io=cct.chQte;
      ++io;
    }

    *io=ch;
    ++io;
  }

  return io;
}

// Analogous to std::copy()
template <typename II, typename IO>
IO CopyUnescaped(II iiB, II iiE, IO io)
{
  for (II ii=iiB; ii!=iiE; ++ii) {
    *io=*ii;
    ++io;
  }
  return io;
}

////////////////////////////////////////////////////////////////////////////////
struct CopierNoEscape {
 public:
  template <typename II, typename IO>
  IO operator()(II iiB, II iiE, IO io)
  { return CopyUnescaped(iiB, iiE, io); }
};


template <typename CSV_CHAR_TRAITS>
struct CopierAlwaysEscape {
  explicit CopierAlwaysEscape(const CSV_CHAR_TRAITS &cct)
   : m_cct(cct) {}

 public:
  template <typename II, typename IO>
  IO operator()(II iiB, II iiE, IO io)
  {
    return m_cct.CopyEscaped(iiB, iiE, io);
  }

 private:
  const CSV_CHAR_TRAITS &m_cct;
};


template <typename BASE_CHAR_TRAITS,
          typename CSV_CHAR_TRAITS>
struct CopierIfNeedEscape {
 public:
  CopierIfNeedEscape(const BASE_CHAR_TRAITS &bct,
                     const CSV_CHAR_TRAITS  &cct)
   : m_bct(bct),
     m_cct(cct) {}

 public:
  template <typename II, typename IO>
  IO operator()(II iiB, II iiE, IO io)
  {
    if (NeedEscape(iiB, iiE, m_bct, m_cct))
      return CopyEscaped(iiB, iiE, io, m_cct);

    return CopyUnescaped(iiB, iiE, io); 
  }

 private:
  const BASE_CHAR_TRAITS &m_bct;
  const CSV_CHAR_TRAITS  &m_cct;
};

}

#endif
