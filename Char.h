#ifndef Char_h_already_included__05_03_2015__179AD701
#define Char_h_already_included__05_03_2015__179AD701
//
// SeVlaT, 05.03.2015
//

namespace TinyCsv {

//!!!! Make cEoF = 0
enum TCharTag {
  cEoF,   // End of file 
  cEoL,   // End of line (usually 0x0D, 0x0A or their combination)
  cSep,   // Separator (usually comma, semicolon, space, tab or | )
  cQte,   // Quotation mark (usually double quote ("), or single ('))
  cWsp,   // Whitespace symbol (space or tab)
  cSmb,   // Any other allowed symbol

  cLast
};

////////////////////////////////////////////////////////////////////////////////
template<typename CH, CH ChPad=' ', CH ChEoL='\n'>
struct TBaseCharTraits {
  typedef CH TChar;

  static const CH chPad=ChPad;
  static const CH chEoL=ChEoL;

  static bool IsWsp(int ch) { return ch==0x09 || ch==0x20 || ch==ChPad; }
  static bool IsEol(int ch) { return ch==0x0D || ch==0x0A || ch==ChEoL; }

  static bool IsEoF(int ch) { return ch==std::char_traits<CH>::eof(); }

  static bool IsNotASymb(int ch) { return false; }
};

typedef TBaseCharTraits<char>    TBaseCharTraitsA;
typedef TBaseCharTraits<wchar_t> TBaseCharTraitsW;

////////////////////////////////////////////////////////////////////////////////
template<typename CH>
struct TCsvCharTraits {
  typedef CH TChar;

  CH chSep;
  CH chQte;

  bool IsSep(int ch) const { return ch==chSep; }
  bool IsQte(int ch) const { return ch==chQte; }

  TCsvCharTraits()
   : chSep(';'), chQte('"') {}

  TCsvCharTraits(CH chS, CH chQ)
   : chSep(chS), chQte(chQ) {}
};

typedef TCsvCharTraits<char>    TCsvCharTraitsA;
typedef TCsvCharTraits<wchar_t> TCsvCharTraitsW;

////////////////////////////////////////////////////////////////////////////////
template<typename BASE_CHAR_TRAITS,
         typename CSV_CHAR_TRAITS>
TCharTag CharTag(      int               ch,
                 const BASE_CHAR_TRAITS &bct,
                 const CSV_CHAR_TRAITS  &cct)
{
  if (cct.IsSep(ch)) return cSep;
  if (cct.IsQte(ch)) return cQte;

  if (bct.IsWsp(ch)) return cWsp;
  if (bct.IsEol(ch)) return cEoL;

  if (!bct.IsNotASymb(ch)) return cSmb;

  return cSmb;
}

template<typename BASE_CHAR_TRAITS,
         typename CSV_CHAR_TRAITS>
TCharTag CharTagEof(      int               ch,
                    const BASE_CHAR_TRAITS &bct,
                    const CSV_CHAR_TRAITS  &cct)
{
  if (bct.IsEoF(ch)) return cEoF;
  return CharTag(ch, bct, cct);
}

}

#endif
