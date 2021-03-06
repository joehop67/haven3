#ifndef HAVEN_TABVIEW_H_INCLUDED
#define HAVEN_TABVIEW_H_INCLUDED

#include <vector>
//#include <wx/notebook.h>
#include <wx/aui/auibook.h>

#include "languagemeta.h"
#include "defs.h"
#include "edit.h"

namespace Haven {
  struct TabInfo {
    int index;
    const char *title;
    Edit *t_edit;
    bool isActive;
  };
 // extern std::vector<TabInfo> g_TabTable;
  extern int g_TabTableSize;

  class TabView : public wxAuiNotebook {
  public:
    //TabView(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxVSCROLL, wxNotebookPage *page);
    TabView(wxWindow *parent, Haven::LanguageMeta::LanguageDB *langDb, Haven::LanguageMeta::StyleDB *styleDb, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0);
    ~TabView();

    void AddTab(const wxString &title, Edit *editor);
    void AddDefaultTab(const wxString &title);
    //void RemoveTab(int index);
    bool IsProjectFileOpen() { return projectFileIsOpen; }
    void SetProjectFileOpen(bool isOpen = false) { projectFileIsOpen = isOpen; }

    int GetCurrentTabIndex();
    TabInfo GetCurrentTab();
    void SetCurrentActiveTab(int index);

    void SetTabTitle(int tabIndex, const wxString &newTitle);

    void OnTabChange(wxAuiNotebookEvent &event);
  protected:
    std::vector<TabInfo> TabTable;
    int activeIndex;
    TabInfo currentTab;
    bool projectFileIsOpen;
    Haven::LanguageMeta::LanguageDB *languageDB;
    Haven::LanguageMeta::StyleDB *styleDB;

    wxDECLARE_EVENT_TABLE();
  };
}

#endif // HAVEN_TABVIEW_H_INCLUDED
