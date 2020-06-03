#include "languagemeta.h"

using namespace Haven::LanguageMeta;
using namespace std;

Language::Language(const char *name, const char *filepattern, int lexer, int type, const char *words, int folds) {
  LanguageName = name;
  FilePattern = filepattern;
  int Lexer = lexer;
  LanguageStyle style{
    type,
    words
  };
  vector<LanguageStyle> styles{
    style
  };
  Styles = styles;
  Folds = folds;
}

Language::Language(const char *name, const char *filepattern, int lexer) {
  Lexer = lexer;
  LanguageName = name;
  FilePattern = filepattern;
}

Language::~Language() {}

LanguageStyle Language::GetStyleByType(int type) {
  LanguageStyle found;
  for (LanguageStyle s : Styles) {
    if (s.sType == type) {
      found = s;
      break;
    }
  }
  return found;
}

void Language::SetLexer(int lexer) {
  Lexer = lexer;
}

void Language::AppendStyle(LanguageStyle style) {
  Styles.push_back(style);
}

LanguageDB::LanguageDB() {
  //LanguageMap = map<array<char, 3>, Language&>;
  DBSize = 0;
}

LanguageDB::LanguageDB(Language *language) {
  char *pattern = const_cast<char*>(language->FilePattern);
  array<char, 3> exte;
  size_t len = strlen(pattern);
  for (size_t i = 0; i < len; i++) {
    char c = pattern[i];
    exte[i] = c;
  }
  LanguageMap.insert(make_pair(exte, language));
  DBSize = LanguageMap.size();
}

LanguageDB::LanguageDB(map<array<char, 3>, Language*> languageDb) {
  LanguageMap = languageDb;
  DBSize = languageDb.size();
}

Language* LanguageDB::GetLanguageByExtension(const char *ext) {
  size_t len = strlen(ext);
  if (len <= 3) {
    array<char, 3> extension;
    for (size_t i = 0; i < len; i++) {
      char c = ext[i];
      extension[i] = c;
    }
    Language *lang = LanguageMap.find(extension)->second;
    return lang;
  }
  return NULL;
}

bool LanguageDB::Register(Language *languageDef, const char *ext) {
  size_t len = strlen(ext);
  array<char, 3> extension;
  for (size_t i = 0; i < len; i++) {
    char c = ext[i];
    extension[i] = c;
  }
  LanguageMap.insert(make_pair(extension, languageDef));
  DBSize = LanguageMap.size();
  return true;
}

StyleDB::StyleDB() {
  //StylePrefs = new vector<StyleDef>();
  DBSize = 0;
}

StyleDB::StyleDB(StyleDef def) {
  //StylePrefs = new vector<StyleDef>();
  StylePrefs.push_back(def);
  DBSize = StylePrefs.size();
}

StyleDB::~StyleDB() {}

StyleDef StyleDB::At(int index) {
  return StylePrefs[index];
}

void StyleDB::Add(StyleDef def) {
  StylePrefs.push_back(def);
}
