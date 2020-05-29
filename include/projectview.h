#ifndef HAVEN_PROJECTVIEW_H_INCLUDED
#define HAVEN_PROJECTVIEW_H_INCLUDED

#include <wx/dataview.h>

#include "projectmodel.h"

namespace Haven {
  class ProjectView : public wxPanel {
  public:
    ProjectView(wxWindow *parent, const wxString &path, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0);
    ~ProjectView();

    void BuildDataViewCtrl(const wxString &path);

  private:
    void OnStyleChange(wxCommandEvent &event);
    void OnSetBackgroundColor(wxCommandEvent &event);
    void OnSetForegroundColor(wxCommandEvent &event);

    void OnCollapse(wxCommandEvent &event);
    void OnExpand(wxCommandEvent &event);
    void OnShowCurrent(wxCommandEvent &event);

    void OnDeleteTreeItem(wxCommandEvent &event);
    void OnAddTreeItem(wxCommandEvent &event);
    void OnAddTreeContainerItem(wxCommandEvent &event);

    void OnActivated(wxDataViewEvent &event);
    void OnExpanding(wxDataViewEvent &event);
    void OnExpanded(wxDataViewEvent &event);
    void OnCollapsing(wxDataViewEvent &event);
    void OnCollapsed(wxDataViewEvent &event);
    void OnSelectionChanged(wxDataViewEvent &event);

    void OnHeaderClick(wxDataViewEvent &event);
    void OnHeaderRightClick(wxDataViewEvent &event);
    //void OnAttrHeaderClick(wxDataViewEvent &event);
    //void OnHeaderClickList(wxDataViewEvent &event);

    void OnContextMenu(wxDataViewEvent &event);

    void OnDataViewChar(wxKeyEvent &event);

    void DeleteSelectedItems();

    wxDataViewCtrl* m_projectview;
    wxObjectDataPtr<ProjectModel> m_project_model;

    wxDataViewColumn* m_col;
    wxDataViewColumn* m_attrs;

    wxDECLARE_EVENT_TABLE();
  };
}

#endif // HAVEN_PROJECTVIEW_H_INCLUDED
