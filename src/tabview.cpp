#include <wx/wxprec.h>

#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif // WX_PRECOMP

#include "../include/tabview.h"

using namespace std;
using namespace Haven;

wxBEGIN_EVENT_TABLE(TabView, wxNotebook)
  EVT_BOOKCTRL_PAGE_CHANGING(wxID_ANY, OnTabChange)
wxEND_EVENT_TABLE()

int Haven::g_TabTableSize = 0;

TabView::TabView(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style)
  : wxNotebook(parent, id, pos, size, style)
  {
//    parentFrame = frame;
    Edit *tabEdit = new Edit(this, wxID_ANY);
    TabInfo defaultTab{
      0,
      "default",
      tabEdit,
      true
    };
    vector<TabInfo> t{
      defaultTab
    };
    TabTable = t;
    activeIndex = 0;
    currentTab = TabTable[activeIndex];
    AddPage(currentTab.t_edit, currentTab.title, true);
    projectFileIsOpen = false;
  }

TabView::~TabView() { /*delete &TabTable;*/ };

void TabView::AddTab(const wxString &title, Edit *editor) {
  editor->SetFocus();
  TabInfo tabAdd{
    TabTable.size(),
    //g_TabTableSize++,
    title,
    editor,
    true
  };
  TabTable.push_back(tabAdd);
  //g_TabTableSize = g_TabTable.size();
  //activeIndex = tabAdd.index;
  //currentTab = tabAdd;
  projectFileIsOpen = true;
  AddPage(tabAdd.t_edit, title, true);
}

void TabView::AddDefaultTab(const wxString &title) {
  Edit *edit = new Edit(this, wxID_ANY);
  edit->SetFocus();
  TabInfo defaultTab{
    TabTable.size(),
    //g_TabTableSize++,
    title,
    edit,
    true
  };
  TabTable.push_back(defaultTab);
  activeIndex = defaultTab.index;
  currentTab = defaultTab;
  AddPage(currentTab.t_edit, title, true);
}

//void TabView::RemoveTab(int index) {
//  if (index < g_TabTableSize) {
//
//  }
//}

void TabView::SetTabTitle(int tabIndex, const wxString &newTitle) {
  TabTable[tabIndex].title = newTitle;
  size_t curPage = GetSelection();
  if (curPage != wxNOT_FOUND) {
    SetPageText(curPage, newTitle);
  }

}

void TabView::OnTabChange(wxNotebookEvent &WXUNUSED(event)) {
  TabTable[activeIndex].isActive = false;
  int tabIdx;
  for (TabInfo tab : TabTable) {
    if (tab.isActive) {
      currentTab = tab;
      tabIdx = tab.index;
      break;
    }
  }
  activeIndex = tabIdx;
}

int TabView::GetCurrentTabIndex() {
  return activeIndex;
}

TabInfo TabView::GetCurrentTab() {
  return TabTable[activeIndex];
}
