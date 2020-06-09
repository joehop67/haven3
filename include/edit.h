#ifndef HAVEN_EDIT_H_INCLUDED
#define HAVEN_EDIT_H_INCLUDED

#include <wx/stc/stc.h>

#include "languagemeta.h"
#include "prefs.h"

namespace Haven {
  class EditProperties;

  class Edit : public wxStyledTextCtrl {
    friend class EditProperties;

  public:
    Edit(wxWindow *parent, Haven::LanguageMeta::LanguageDB *langDb, Haven::LanguageMeta::StyleDB *styleDb, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0);
    ~Edit();

    void OnSize(wxSizeEvent &event);
    // Edit Events
    void onEditRedo(wxCommandEvent &event);
    void onEditUndo(wxCommandEvent &event);
    void onEditClear(wxCommandEvent &event);
    void onEditCut(wxCommandEvent &event);
    void onEditCopy(wxCommandEvent &event);
    void onEditPaste(wxCommandEvent &event);
    // Find Events
    void onFind(wxCommandEvent &event);
    void onFindNext(wxCommandEvent &event);
    void onReplace(wxCommandEvent &event);
    void onReplaceNext(wxCommandEvent &event);
    void onBraceMatch(wxCommandEvent &event);
    void onGoto(wxCommandEvent &event);
    void onEditIndentInc(wxCommandEvent &event);
    void onEditIndentRed(wxCommandEvent &event);
    void onEditSelectAll(wxCommandEvent &event);
    void onEditSelectLine(wxCommandEvent &event);
    // View Events
    void onHighlightLang(wxCommandEvent &event);
    void onDisplayEOL(wxCommandEvent &event);
    void onIndentGuide(wxCommandEvent &event);
    void onLineNumber(wxCommandEvent &event);
    void onLongLineOn(wxCommandEvent &event);
    void onWhiteSpace(wxCommandEvent &event);
    void onFoldToggle(wxCommandEvent &event);
    void onSetOverType(wxCommandEvent &event);
    void onSetReadOnly(wxCommandEvent &event);
    void onWrapModeOn(wxCommandEvent &event);
    void onUseCharset(wxCommandEvent &event);
    // Annotation Events
    void onAnnotationAdd(wxCommandEvent &event);
    void onAnnotationRemove(wxCommandEvent &event);
    void onAnnotationClear(wxCommandEvent &event);
    void onAnnotationStyle(wxCommandEvent &event);
    // Extra
    void onChangeCase(wxCommandEvent &event);
    void onConvertEOL(wxCommandEvent &event);
    // STC Events
    void onMarginClick(wxStyledTextEvent &event);
    void onCharAdded(wxStyledTextEvent &event);
    void onKey(wxStyledTextEvent &event);

    void OnKeyDown(wxKeyEvent &event);

    // Language Settings
    wxString DeterminePrefs(const wxString &filename);
    bool InitializePrefs(const wxString &filename, const wxString &ext);
    bool UserSettings(const wxString &filename);
    Haven::LanguageMeta::Language* GetLanguageInfo() { return curLanguage; };

    // File methods
    bool LoadFile();
    bool LoadFile(const wxString &filename);
    bool SaveFile();
    bool SaveFile(const wxString &filename);
    bool Modified();
    wxString GetFilename() { return m_filename; };
    void SetFilename(const wxString &filename) { m_filename = filename; };

  private:
    bool confLL;
    bool confCF;
    wxString m_filename;

    Haven::LanguageMeta::LanguageDB *languageDB;
    Haven::LanguageMeta::StyleDB *styleDB;
    Haven::LanguageMeta::Language *curLanguage;

    int m_LineNrID;
    int m_LineNrMargin;
    int m_FoldingID;
    int m_FoldingMargin;
    int m_DividerID;

    wxDECLARE_EVENT_TABLE();
  };

  class EditProperties : public wxDialog {
  public:
    EditProperties(Edit *edit, long style = 0);
  };
}

#endif // HAVEN_EDIT_H_INCLUDED
