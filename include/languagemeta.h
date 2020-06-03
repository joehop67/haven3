#ifndef HAVEN_LANGUAGEMETA_H_INCLUDED
#define HAVEN_LANGUAGEMETA_H_INCLUDED

#include <vector>
#include <map>
#include <array>

#include <wx/intl.h>

namespace Haven {
  namespace LanguageMeta {
    struct StyleDef {
      const wxChar *name;
      const wxChar *foreground;
      const wxChar *background;
      const wxChar *fontname;
      int fontSize;
      int fontStyle;
      int letterCase;
      //int sType;
    };
    struct LanguageStyle {
      int sType;
      const char *keyWords;
    };
    class Language {
    public:
      Language(const char *name, const char *filepattern, int lexer, int type, const char *words, int folds);
      Language(const char *name, const char *filepattern, int lexer);
      ~Language();

      char* GetName() const { return const_cast<char*>(LanguageName); }
      int GetLexer() const { return Lexer; }
      int GetFolds() const { return Folds; }

      std::vector<LanguageMeta::LanguageStyle> GetStyles() { return Styles; }
      LanguageMeta::LanguageStyle GetStyleByType(int type);

      void SetLexer(int lexer);
      void AppendStyle(LanguageMeta::LanguageStyle style);
      const char *FilePattern;

    private:
      const char *LanguageName;
      int Lexer;
      std::vector<LanguageMeta::LanguageStyle> Styles;
      int Folds;

      int FindLexerFromPattern(const char *pattern);
    };
    class LanguageDB {
    public:
      LanguageDB();
      LanguageDB(LanguageMeta::Language *language);
      LanguageDB(std::map<std::array<char, 3>, LanguageMeta::Language*> languageDb);

      //std::vector<LanguageMeta::Language*> GetAll() { return LanguageDefinitions; }
      LanguageMeta::Language* GetLanguageByExtension(const char *ext);
      //LanguageMeta::Language* GetLanguageByExtension(std::array<char, 3> ext);

      bool Register(LanguageMeta::Language *languageDef, const char *ext);
      //bool ChangeExt(const char *old, const char *nStr);

    private:
      std::map<std::array<char, 3>, LanguageMeta::Language*> LanguageMap;
      int DBSize;
    };
    class StyleDB {
    public:
      StyleDB();
      StyleDB(LanguageMeta::StyleDef def);
      ~StyleDB();

      LanguageMeta::StyleDef At(int index);
      void Add(LanguageMeta::StyleDef def);
    private:
      std::vector<LanguageMeta::StyleDef> StylePrefs;
      int DBSize;
    };
  }
}

#endif // HAVEN_LANGUAGEMETA_H_INCLUDED
