#ifndef Debug_h_already_included__05_03_2015__110AE4BB
#define Debug_h_already_included__05_03_2015__110AE4BB
//
// SeVlaT, 05.03.2015
//

// Define TINY_CSV_ASSERT if not defined yet

#ifndef TINY_CSV_ASSERT

# ifdef assert
#  define TINY_CSV_ASSERT assert
# else
#  define TINY_CSV_ASSERT(X) ((void)0)
# endif

#endif

namespace TinyCsv {

template<typename INDEX>
struct TSuccessiveIndex {
  template<int I0, int I1, int I2, int I3, int I4, int I5>
  class IndexShouldBeSuccessive {};

  template<INDEX I0, INDEX I1, INDEX I2, INDEX I3, INDEX I4, INDEX I5>
  static IndexShouldBeSuccessive<I0, I1, I2, I3, I4, I5> Head()
  { return IndexShouldBeSuccessive<I0, I1, I2, I3, I4, I5>(); }

  TSuccessiveIndex(IndexShouldBeSuccessive<0, 1, 2, 3, 4, 5>) {}
};

}

#endif
