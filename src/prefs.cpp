#include <wx/wxprec.h>

#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif // WX_PRECOMP

#include "../include/defs.h"
#include "../include/prefs.h"

using namespace Haven;

const CommonInfo Haven::g_CommonPrefs = {
  true,
  true,
  true,
  false,
  false,
  false,
  false,
  false,
  true,
  false,
  false
};

const ExtMap Haven::g_ExtensionMap = {
  {
    "*.c;*.cc;*.cpp;*.cxx;*.cs;*.h;*.hh;*.hpp;*.hxx;*.sma",
    "C++"
  }
};
const int Haven::g_ExtensionMapSize = WXSIZEOF(g_ExtensionMap);

const char* CppWordlist1 =
    "asm auto bool break case catch char class const const_cast "
    "continue default delete do double dynamic_cast else enum explicit "
    "export extern false float for friend goto if inline int long "
    "mutable namespace new operator private protected public register "
    "reinterpret_cast return short signed sizeof static static_cast "
    "struct switch template this throw true try typedef typeid "
    "typename union unsigned using virtual void volatile wchar_t "
    "while";
const char* CppWordlist2 =
    "file";
const char* CppWordlist3 =
    "a addindex addtogroup anchor arg attention author b brief bug c "
    "class code date def defgroup deprecated dontinclude e em endcode "
    "endhtmlonly endif endlatexonly endlink endverbatim enum example "
    "exception f$ f[ f] file fn hideinitializer htmlinclude "
    "htmlonly if image include ingroup internal invariant interface "
    "latexonly li line link mainpage name namespace nosubgrouping note "
    "overload p page par param post pre ref relates remarks return "
    "retval sa section see showinitializer since skip skipline struct "
    "subsection test throw todo typedef union until var verbatim "
    "verbinclude version warning weakgroup $ @ \"\" & < > # { }";

const Haven::LanguageInfo Haven::g_LanguagePrefs[] = {
  {
    "C++",
    "*.c;*.cc;*.cpp;*.cxx;*.cs;*.h;*.hh;*.hpp;*.hxx;*.sma",
    wxSTC_LEX_CPP,
    {
      {
        havenSTC_TYPE_DEFAULT,
        NULL
      },
      {
        havenSTC_TYPE_COMMENT,
        NULL
      },
      {
        havenSTC_TYPE_COMMENT_LINE,
        NULL
      },
      {
        havenSTC_TYPE_COMMENT_DOC,
        NULL
      },
      {
        havenSTC_TYPE_NUMBER,
        NULL
      },
      {
        havenSTC_TYPE_WORD1,
        CppWordlist1
      },
      {
        havenSTC_TYPE_STRING,
        NULL
      },
      {
        havenSTC_TYPE_CHARACTER,
        NULL
      },
      {
        havenSTC_TYPE_UUID,
        NULL
      },
      {
        havenSTC_TYPE_PREPROCESSOR,
        NULL
      },
      {
        havenSTC_TYPE_OPERATOR,
        NULL
      },
      {
        havenSTC_TYPE_IDENTIFIER,
        NULL
      },
      {
        havenSTC_TYPE_STRING_EOL,
        NULL
      },
      {
        havenSTC_TYPE_DEFAULT,
        NULL
      },
      {
        havenSTC_TYPE_REGEX,
        NULL
      },
      {
        havenSTC_TYPE_COMMENT_SPECIAL,
        NULL
      },
      {
        havenSTC_TYPE_WORD2,
        CppWordlist2
      },
      {
        havenSTC_TYPE_WORD3,
        CppWordlist3
      },
      {
        havenSTC_TYPE_ERROR,
        NULL
      },
      {
        -1,
        NULL
      },
      {
        -1,
        NULL
      },
      {
        -1,
        NULL
      },
      {
        -1,
        NULL
      },
      {
        -1,
        NULL
      },
      {
        -1,
        NULL
      },
      {
        -1,
        NULL
      },
      {
        -1,
        NULL
      },
      {
        -1,
        NULL
      },
      {
        -1,
        NULL
      },
      {
        -1,
        NULL
      },
      {
        -1,
        NULL
      },
      {
        -1,
        NULL
      }
    },
    havenSTC_FOLD_COMMENT | havenSTC_FOLD_COMPACT | havenSTC_FOLD_PREPROC
  },
  {
    "<default>",
    "*.*",
    wxSTC_LEX_PROPERTIES,
    {
      {
        havenSTC_TYPE_DEFAULT,
        NULL
      },
      {
        havenSTC_TYPE_DEFAULT,
        NULL
      },
      {
        havenSTC_TYPE_DEFAULT,
        NULL
      },
      {
        havenSTC_TYPE_DEFAULT,
        NULL
      },
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL}
    },
    0
  }
};

const int Haven::g_LanguagePrefsSize = WXSIZEOF(g_LanguagePrefs);

const Haven::StyleInfo Haven::g_StylePrefs[] = {
  {
    wxT("Default"),
    wxT("BLACK"),
    wxT("WHITE"),
    wxT(""),
    10,
    0,
    0
  },
  {wxT("Keyword1"),
     wxT("BLUE"), wxT("WHITE"),
     wxT(""), 10, havenSTC_STYLE_BOLD, 0},

    // mySTC_TYPE_WORD2
    {wxT("Keyword2"),
     wxT("MIDNIGHT BLUE"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_WORD3
    {wxT("Keyword3"),
     wxT("CORNFLOWER BLUE"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_WORD4
    {wxT("Keyword4"),
     wxT("CYAN"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_WORD5
    {wxT("Keyword5"),
     wxT("DARK GREY"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_WORD6
    {wxT("Keyword6"),
     wxT("GREY"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_COMMENT
    {wxT("Comment"),
     wxT("FOREST GREEN"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_COMMENT_DOC
    {wxT("Comment (Doc)"),
     wxT("FOREST GREEN"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_COMMENT_LINE
    {wxT("Comment line"),
     wxT("FOREST GREEN"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_COMMENT_SPECIAL
    {wxT("Special comment"),
     wxT("FOREST GREEN"), wxT("WHITE"),
     wxT(""), 10, havenSTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_CHARACTER
    {wxT("Character"),
     wxT("KHAKI"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_CHARACTER_EOL
    {wxT("Character (EOL)"),
     wxT("KHAKI"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_STRING
    {wxT("String"),
     wxT("BROWN"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_STRING_EOL
    {wxT("String (EOL)"),
     wxT("BROWN"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_DELIMITER
    {wxT("Delimiter"),
     wxT("ORANGE"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_PUNCTUATION
    {wxT("Punctuation"),
     wxT("ORANGE"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_OPERATOR
    {wxT("Operator"),
     wxT("BLACK"), wxT("WHITE"),
     wxT(""), 10, havenSTC_STYLE_BOLD, 0},

    // mySTC_TYPE_BRACE
    {wxT("Label"),
     wxT("VIOLET"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_COMMAND
    {wxT("Command"),
     wxT("BLUE"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_IDENTIFIER
    {wxT("Identifier"),
     wxT("BLACK"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_LABEL
    {wxT("Label"),
     wxT("VIOLET"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_NUMBER
    {wxT("Number"),
     wxT("SIENNA"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_PARAMETER
    {wxT("Parameter"),
     wxT("VIOLET"), wxT("WHITE"),
     wxT(""), 10, havenSTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_REGEX
    {wxT("Regular expression"),
     wxT("ORCHID"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_UUID
    {wxT("UUID"),
     wxT("ORCHID"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_VALUE
    {wxT("Value"),
     wxT("ORCHID"), wxT("WHITE"),
     wxT(""), 10, havenSTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_PREPROCESSOR
    {wxT("Preprocessor"),
     wxT("GREY"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_SCRIPT
    {wxT("Script"),
     wxT("DARK GREY"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_ERROR
    {wxT("Error"),
     wxT("RED"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // mySTC_TYPE_UNDEFINED
    {wxT("Undefined"),
     wxT("ORANGE"), wxT("WHITE"),
     wxT(""), 10, 0, 0}
};

const int g_StylePrefsSize = WXSIZEOF(g_StylePrefs);
