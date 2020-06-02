#ifndef HAVEN_PREFS_H_INCLUDED
#define HAVEN_PREFS_H_INCLUDED

#include <wx/intl.h>
#include <wx/stc/stc.h>

#define STYLE_TYPES_COUNT 32

#define havenSTC_TYPE_DEFAULT 0

#define havenSTC_TYPE_WORD1 1
#define havenSTC_TYPE_WORD2 2
#define havenSTC_TYPE_WORD3 3
#define havenSTC_TYPE_WORD4 4
#define havenSTC_TYPE_WORD5 5
#define havenSTC_TYPE_WORD6 6

#define havenSTC_TYPE_COMMENT 7
#define havenSTC_TYPE_COMMENT_DOC 8
#define havenSTC_TYPE_COMMENT_LINE 9
#define havenSTC_TYPE_COMMENT_SPECIAL 10

#define havenSTC_TYPE_CHARACTER 11
#define havenSTC_TYPE_CHARACTER_EOL 12
#define havenSTC_TYPE_STRING 13
#define havenSTC_TYPE_STRING_EOL 14

#define havenSTC_TYPE_DELIMITER 15

#define havenSTC_TYPE_PUNCTUATION 16

#define havenSTC_TYPE_OPERATOR 17

#define havenSTC_TYPE_BRACE 18

#define havenSTC_TYPE_COMMAND 19
#define havenSTC_TYPE_IDENTIFIER 20
#define havenSTC_TYPE_LABEL 21
#define havenSTC_TYPE_NUMBER 22
#define havenSTC_TYPE_PARAMETER 23
#define havenSTC_TYPE_REGEX 24
#define havenSTC_TYPE_UUID 25
#define havenSTC_TYPE_VALUE 26

#define havenSTC_TYPE_PREPROCESSOR 27
#define havenSTC_TYPE_SCRIPT 28

#define havenSTC_TYPE_ERROR 29

#define havenSTC_STYLE_BOLD 1
#define havenSTC_STYLE_ITALIC 2
#define havenSTC_STYLE_UNDERL 4
#define havenSTC_STYLE_HIDDEN 8

#define havenSTC_FOLD_COMMENT 1
#define havenSTC_FOLD_COMPACT 2
#define havenSTC_FOLD_PREPROC 4

#define havenSTC_FOLD_HTML 16
#define havenSTC_FOLD_HTMLPREP 32

#define havenSTC_FOLD_COMMENTPY 64
#define havenSTC_FOLD_QUOTESPY 128

#define havenSTC_FLAG_WRAPMODE 16

namespace Haven {
  struct CommonInfo {
    bool syntaxEnable;
    bool foldEnable;
    bool indentEnable;
    bool readOnlyInitial;
    bool overTypeInitial;
    bool wrapModeInitial;
    bool displayEOLEnabled;
    bool indentGuideEnabled;
    bool lineNumberEnabled;
    bool longLineOnEnabled;
    bool whiteSpaceEnabled;
  };
  extern const CommonInfo g_CommonPrefs;

  struct LanguageInfo {
    const char *name;
    const char *filepattern;
    int lexer;
    struct {
      int type;
      const char *words;
    } styles [STYLE_TYPES_COUNT];
    int folds;
  };
  extern const LanguageInfo g_LanguagePrefs[];
  extern const int g_LanguagePrefsSize;

  struct ExtMap {
    const char *extensions;
    const char *language;
  };
  extern const ExtMap g_ExtensionMap[];
  extern const int g_ExtensionMapSize;

  struct ProjectInfo {
    const char *name;
    const char *rootFolder;
    const char *rootPath;
    const char *language;
  };

  struct StyleInfo {
    const wxChar *name;
    const wxChar *foreground;
    const wxChar *background;
    const wxChar *fontname;
    int fontSize;
    int fontStyle;
    int letterCase;
  };
  extern const StyleInfo g_StylePrefs[];
  extern const int g_StylePrefsSize;

  enum {
    PROJECT_OPEN_FILE_ID
  };

  enum {
    // menu IDs
    havenID_PROPERTIES = wxID_HIGHEST,
    havenID_EDIT_FIRST,
    havenID_INDENTINC = havenID_EDIT_FIRST,
    havenID_INDENTRED,
    havenID_FINDNEXT,
    havenID_REPLACE,
    havenID_REPLACENEXT,
    havenID_BRACEMATCH,
    havenID_GOTO,
    havenID_PAGEACTIVE,
    havenID_DISPLAYEOL,
    havenID_INDENTGUIDE,
    havenID_LINENUMBER,
    havenID_LONGLINEON,
    havenID_WHITESPACE,
    havenID_FOLDTOGGLE,
    havenID_OVERTYPE,
    havenID_READONLY,
    havenID_WRAPMODEON,
    havenID_ANNOTATION_ADD,
    havenID_ANNOTATION_REMOVE,
    havenID_ANNOTATION_CLEAR,
    havenID_ANNOTATION_STYLE_HIDDEN,
    havenID_ANNOTATION_STYLE_STANDARD,
    havenID_ANNOTATION_STYLE_BOXED,
    havenID_CHANGECASE,
    havenID_CHANGELOWER,
    havenID_CHANGEUPPER,
    havenID_HILIGHTLANG,
    havenID_HILIGHTFIRST,
    havenID_HILIGHTLAST = havenID_HILIGHTFIRST + 99,
    havenID_CONVERTEOL,
    havenID_CONVERTCR,
    havenID_CONVERTCRLF,
    havenID_CONVERTLF,
    havenID_USECHARSET,
    havenID_CHARSETANSI,
    havenID_CHARSETMAC,
    havenID_PAGEPREV,
    havenID_PAGENEXT,
    havenID_SELECTLINE,
    havenID_EDIT_LAST = havenID_SELECTLINE,
    havenID_WINDOW_MINIMAL,

    // other IDs
    havenID_STATUSBAR,
    havenID_TITLEBAR,
    havenID_ABOUTTIMER,
    havenID_UPDATETIMER,

    // dialog find IDs
    havenID_DLG_FIND_TEXT,

    // preferences IDs
    havenID_PREFS_LANGUAGE,
    havenID_PREFS_STYLETYPE,
    havenID_PREFS_KEYWORDS,
    havenID_OPEN_PROJECT
  };

}

#endif // HAVEN_PREFS_H_INCLUDED
