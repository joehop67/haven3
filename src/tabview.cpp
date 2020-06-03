#include <wx/wxprec.h>

#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif // WX_PRECOMP

#include "../include/tabview.h"

using namespace std;
using namespace Haven;

wxBEGIN_EVENT_TABLE(TabView, wxAuiNotebook)
  EVT_AUINOTEBOOK_PAGE_CHANGING(wxID_ANY, OnTabChange)
wxEND_EVENT_TABLE()

int Haven::g_TabTableSize = 0;

TabView::TabView(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style)
  : wxAuiNotebook(parent, id, pos, size, style | wxAUI_NB_CLOSE_BUTTON | wxAUI_NB_CLOSE_ON_ALL_TABS)
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
    this->AddPage(currentTab.t_edit, currentTab.title, true);
    projectFileIsOpen = false;
    wxAuiTabArt *art = new wxAuiSimpleTabArt;
    SetArtProvider(art);
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
  AddPage(tabAdd.t_edit, title, true, wxNullBitmap);
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
  AddPage(currentTab.t_edit, title, true, wxNullBitmap);
}

void TabView::SetTabTitle(int tabIndex, const wxString &newTitle) {
  TabTable[tabIndex].title = newTitle;
  size_t curPage = GetSelection();
  if (curPage != wxNOT_FOUND) {
    SetPageText(curPage, newTitle);
  }

}

void TabView::SetCurrentActiveTab(int index) {
  TabInfo selected = TabTable[index];
  if (selected.t_edit != NULL) {
    TabTable[activeIndex].isActive = false;
    selected.isActive = true;
    currentTab = selected;
    activeIndex = index;
    SetSelection(index);
  }
}

void TabView::OnTabChange(wxAuiNotebookEvent &event) {
  wxEvtHandler *type1 = NULL;
  TabTable[activeIndex].isActive = false;
  int curIdx = GetSelection();
  int tabIdx;
  for (TabInfo tab : TabTable) {
    if (tab.isActive) {
      currentTab = tab;
      tabIdx = tab.index;
      break;
    }
  }
  activeIndex = tabIdx;
  type1 = event.GetPropagatedFrom();
  if (type1 != NULL) {
    int cur = event.GetSelection();
    SetCurrentActiveTab(cur);
  }
}

int TabView::GetCurrentTabIndex() {
  return activeIndex;
}

TabInfo TabView::GetCurrentTab() {
  return TabTable[activeIndex];
}
