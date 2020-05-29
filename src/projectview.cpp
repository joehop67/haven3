#include <wx/wxprec.h>

#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif // WX_PRECOMP

#include <wx/filename.h>

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
  EVT_MENU_RANGE(ID_MULTIPLE, ID_VERT_RULES, ProjectView::OnStyleChange)
  EVT_MENU(ID_FOREGROUND_COLOR, ProjectView::OnSetForegroundColor)
  EVT_MENU(ID_BACKGROUND_COLOR, ProjectView::OnSetBackgroundColor)
  EVT_MENU(ID_COLLAPSE, ProjectView::OnCollapse)
  EVT_MENU(ID_EXPAND, ProjectView::OnExpand)
  EVT_MENU(ID_SHOW_CURRENT, ProjectView::OnShowCurrent)

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
  wxString* name;
  wxFileName::SplitPath(path, NULL, name, NULL);
  m_projectview = new wxDataViewCtrl(this, ID_PROJECT_ITEM_CTRL);
  m_projectview->Connect(wxEVT_CHAR, wxKeyEventHandler(ProjectView::OnDataViewChar), NULL, this);
  m_project_model = new ProjectModel(path, name);

  m_projectview->AssociateModel(m_project_model.get());

  wxDataViewTextRenderer *textRender = new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);
  wxDataViewColumn *col0 = new wxDataViewColumn("title", tr, 0, 350, wxALIGN_LEFT);

  m_projectview->AppendColumn(col0);

  wxDataViewTextRenderer *langRend = new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);
  wxDataViewColumn *col1 = new wxDataViewColumn("language", langRend, 1, 80, wxALIGN_RIGHT);

  m_projectview->AppendColumn(col1);
}

