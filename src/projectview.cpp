#include <wx/wxprec.h>

#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif // WX_PRECOMP

#include <wx/filename.h>
#include <wx/colordlg.h>

#include "../include/projectview.h"

using namespace Haven;

enum {
  ID_BACKGROUND_COLOR,
  ID_FOREGROUND_COLOR,
  ID_STYLE_MENU,

  ID_MULTIPLE,
  ID_ROW_LINES,
  ID_HORTZ_RULES,
  ID_VERT_RULES,

  ID_PROJECT_ITEM_CTRL = 40,

  ID_COLLAPSE = 50,
  ID_EXPAND = 51,
  ID_SHOW_CURRENT,

  ID_DELETE_TREE_ITEM = 100,
  ID_ADD_TREE_ITEM = 101,
  ID_ADD_TREE_CONTAINER_ITEM = 103
};

wxBEGIN_EVENT_TABLE(ProjectView, wxPanel)
  //EVT_MENU_RANGE(ID_MULTIPLE, ID_VERT_RULES, ProjectView::OnStyleChange)
  EVT_MENU(ID_FOREGROUND_COLOR, ProjectView::OnSetForegroundColor)
  EVT_MENU(ID_BACKGROUND_COLOR, ProjectView::OnSetBackgroundColor)
  EVT_MENU(ID_COLLAPSE, ProjectView::OnCollapse)
  EVT_MENU(ID_EXPAND, ProjectView::OnExpand)
  //EVT_MENU(ID_SHOW_CURRENT, ProjectView::OnShowCurrent)

  EVT_MENU(ID_DELETE_TREE_ITEM, ProjectView::OnDeleteTreeItem)
  EVT_MENU(ID_ADD_TREE_ITEM, ProjectView::OnAddTreeItem)
  EVT_MENU(ID_ADD_TREE_CONTAINER_ITEM, ProjectView::OnAddTreeContainerItem)

  EVT_DATAVIEW_ITEM_ACTIVATED(ID_PROJECT_ITEM_CTRL, ProjectView::OnActivated)
  EVT_DATAVIEW_ITEM_EXPANDING(ID_PROJECT_ITEM_CTRL, ProjectView::OnExpanding)
  EVT_DATAVIEW_ITEM_EXPANDED(ID_PROJECT_ITEM_CTRL, ProjectView::OnExpanded)
  EVT_DATAVIEW_ITEM_COLLAPSING(ID_PROJECT_ITEM_CTRL, ProjectView::OnCollapsing)
  EVT_DATAVIEW_ITEM_COLLAPSED(ID_PROJECT_ITEM_CTRL, ProjectView::OnCollapsed)
  EVT_DATAVIEW_SELECTION_CHANGED(ID_PROJECT_ITEM_CTRL, ProjectView::OnSelectionChanged)

  EVT_DATAVIEW_COLUMN_HEADER_CLICK(ID_PROJECT_ITEM_CTRL, ProjectView::OnHeaderClick)
  EVT_DATAVIEW_COLUMN_HEADER_RIGHT_CLICK(ID_PROJECT_ITEM_CTRL, ProjectView::OnHeaderRightClick)

  EVT_DATAVIEW_ITEM_CONTEXT_MENU(ID_PROJECT_ITEM_CTRL, ProjectView::OnContextMenu)
wxEND_EVENT_TABLE()

ProjectView::ProjectView(wxWindow *parent, const wxString &path, wxWindowID id, const wxPoint &pos, const wxSize &size, long style)
  : wxPanel(parent, id, pos, size, style)
{
  m_projectview = NULL;
  m_col = NULL;

  BuildDataViewCtrl(path);
}

ProjectView::~ProjectView() {}

void ProjectView::BuildDataViewCtrl(const wxString &path) {
  wxString* name = new wxString;
  wxFileName::SplitPath(path, NULL, name, NULL);
  wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
  m_projectview = new wxDataViewCtrl(this, ID_PROJECT_ITEM_CTRL, wxDefaultPosition, wxDefaultSize, wxEXPAND);
  //m_projectview->Connect(wxEVT_CHAR, wxKeyEventHandler(ProjectView::OnDataViewChar), NULL, this);
  m_project_model = new ProjectModel(path, *name);

  m_projectview->AssociateModel(m_project_model.get());

  wxDataViewTextRenderer *textRender = new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);
  wxDataViewColumn *col0 = new wxDataViewColumn("title", textRender, 0, 350, wxALIGN_LEFT);

  m_projectview->AppendColumn(col0);

 // wxDataViewTextRenderer *langRend = new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);
  //wxDataViewColumn *col1 = new wxDataViewColumn("language", langRend, 1, 80, wxALIGN_RIGHT);

  //m_projectview->AppendColumn(col1);
  sizer->Add(m_projectview, 1, wxEXPAND);
  SetSizer(sizer);
}

void ProjectView::OnSetBackgroundColor(wxCommandEvent &WXUNUSED(event)) {
  wxDataViewCtrl * const dataView = m_projectview;
  wxColor color = wxGetColourFromUser(this, dataView->GetBackgroundColour());
  if (color.IsOk()) {
    dataView->SetBackgroundColour(color);
    Refresh();
  }
}

void ProjectView::OnSetForegroundColor(wxCommandEvent &WXUNUSED(event)) {
  wxDataViewCtrl * const dataView = m_projectview;
  wxColor color = wxGetColourFromUser(this, dataView->GetForegroundColour());
  if (color.IsOk()) {
    dataView->SetForegroundColour(color);
    Refresh();
  }
}

void ProjectView::OnCollapse(wxCommandEvent &WXUNUSED(event)) {
  wxDataViewItem item = m_projectview->GetSelection();
  if (item.IsOk())
    m_projectview->Collapse(item);
}

void ProjectView::OnExpand(wxCommandEvent &WXUNUSED(event)) {
  wxDataViewItem item = m_projectview->GetSelection();
  if (item.IsOk())
    m_projectview->Expand(item);
}

void ProjectView::OnActivated(wxDataViewEvent &event) {
  wxString title = m_project_model->GetTitle(event.GetItem());

  //TODO Open File

  if (m_projectview->IsExpanded(event.GetItem())) {
    return;
  }

}

//TODO Open File
void ProjectView::OnSelectionChanged(wxDataViewEvent &event) {}

void ProjectView::OnExpanding(wxDataViewEvent &event) {}

void ProjectView::OnExpanded(wxDataViewEvent &event) {}

void ProjectView::OnCollapsing(wxDataViewEvent &event) {}

void ProjectView::OnCollapsed(wxDataViewEvent &event) {}

//TODO Change Context menu based on item type
void ProjectView::OnContextMenu(wxDataViewEvent &event) {
  wxMenu menu;
  menu.Append(1, "New File");
  menu.Append(2, "Open File");
  menu.Append(3, "Copy File");

  m_projectview->PopupMenu(&menu);
}

//TODO Open File current window
void ProjectView::OnHeaderClick(wxDataViewEvent &event) {
  event.Skip();
}

//TODO Open Context Menu
void ProjectView::OnHeaderRightClick(wxDataViewEvent &event) {}

void ProjectView::OnDeleteTreeItem(wxCommandEvent &WXUNUSED(event)) {
  wxDataViewTreeCtrl *ctrl = (wxDataViewTreeCtrl*) m_projectview;
  wxDataViewItem selected = ctrl->GetSelection();
  if (!selected.IsOk())
    return;

  ctrl->DeleteItem(selected);
}

void ProjectView::OnAddTreeItem(wxCommandEvent &WXUNUSED(event)) {
  wxDataViewTreeCtrl *ctrl = (wxDataViewTreeCtrl*) m_projectview;
  wxDataViewItem selected = ctrl->GetSelection();
  if (ctrl->IsContainer(selected)) {
    wxDataViewItem nItem = ctrl->AppendItem(selected, "Item", 0);
    ctrl->Select(nItem);
  }
}

void ProjectView::OnAddTreeContainerItem(wxCommandEvent &WXUNUSED(event)) {
  wxDataViewTreeCtrl* ctrl = (wxDataViewTreeCtrl*) m_projectview;
  wxDataViewItem selected = ctrl->GetSelection();
  if (ctrl->IsContainer(selected))
    ctrl->AppendContainer(selected, "Container", 0);
}

void ProjectView::DeleteSelectedItems() {
  wxDataViewItemArray items;
  int len = m_projectview->GetSelections(items);
  for (int i = 0; i < len; i++) {
    if (items[i].IsOk())
      m_project_model->Delete(items[i]);
  }
}
