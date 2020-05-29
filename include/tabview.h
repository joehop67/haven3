#ifndef HAVEN_TABVIEW_H_INCLUDED
#define HAVEN_TABVIEW_H_INCLUDED

#include <vector>
#include <wx/notebook.h>

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

  class TabView : public wxNotebook {
  public:
    //TabView(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxVSCROLL, wxNotebookPage *page);
    TabView(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0);
    ~TabView();

    void AddTab(const wxString &title, Edit *editor);
    void AddDefaultTab(const wxString &title);
    //void RemoveTab(int index);

    int GetCurrentTabIndex();
    TabInfo GetCurrentTab();

    void SetTabTitle(int tabIndex, const wxString &newTitle);

    void OnTabChange(wxNotebookEvent &event);
  protected:
    std::vector<TabInfo> TabTable;
    int activeIndex;
    TabInfo currentTab;

    wxDECLARE_EVENT_TABLE();
  };
}

#endif // HAVEN_TABVIEW_H_INCLUDED
