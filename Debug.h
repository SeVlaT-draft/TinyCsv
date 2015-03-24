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

}

#endif
