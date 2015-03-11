#ifndef Char_h_already_included__05_03_2015__179AD701
#define Char_h_already_included__05_03_2015__179AD701
//
// SeVlaT, 05.03.2015
//

namespace TinyCsv {

enum TCharTag {
  cSep,   // Separator (usually comma, semicolon, space, tab or | )
  cQte,   // Quotation mark (usually double quote ("), or single ('))
  cWsp,   // Whitespace symbol (space or tab)
  cSmb,   // Any other allowed symbol
  cEol,   // End of line (usually 0x0D, 0x0A or their combination)
  cEof,   // End of file 

  cLast
};

////////////////////////////////////////////////////////////////////////////////
template<typename CH>
struct TBaseCharTraits {
  typedef CH TChar;

  static bool IsWsp(CH ch) { return ch==0x09 || ch==0x20; }
  static bool IsEol(CH ch) { return ch==0x0D || ch==0x0A; }

  static bool IsNotASymb(CH ch) { return false; }
};

typedef TBaseCharTraits<char>    TBaseCharTraitsA;
typedef TBaseCharTraits<wchar_t> TBaseCharTraitsW;

////////////////////////////////////////////////////////////////////////////////
template<typename CH>
struct TCsvCharTraits {
  typedef CH TChar;

  CH chSep;
  CH chQte;

  bool IsSep(CH ch) const { return ch==chSep; }
  bool IsQte(CH ch) const { return ch==chQte; }

  TCsvCharTraits()
   : chSep(';'), chQte('"') {}

  TCsvCharTraits(CH chS, CH chQ)
   : chSep(chS), chQte(chQ) {}
};

typedef TCsvCharTraits<char>    TCsvCharTraitsA;
typedef TCsvCharTraits<wchar_t> TCsvCharTraitsW;

////////////////////////////////////////////////////////////////////////////////
template<typename CH,
         typename BASE_CHAR_TRAITS,
         typename CSV_CHAR_TRAITS>
TCharTag CharTag(      CH                ch,
                 const BASE_CHAR_TRAITS &bct,
                 const CSV_CHAR_TRAITS  &cct)
{
  if (cct.IsSep(ch)) return cSep;
  if (cct.IsQte(ch)) return cQte;

  if (bct.IsWsp(ch)) return cWsp;
  if (bct.IsEol(ch)) return cEol;

  if (!bct.IsNotASymb(ch)) return cSmb;

  return cSmb;
}

}

#endif
