#ifndef HAVEN_PROJECT_EVENT_H_INCLUDED
#define HAVEN_PROJECT_EVENT_H_INCLUDED

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif // WX_PRECOMP

namespace Haven {
  class ProjectEvent : public wxEvent {
  public:
    ProjectEvent(wxWindow *win = (wxWindow*)NULL);
    ProjectEvent(const ProjectEvent& event);

    wxEvent* Clone() const { return new ProjectEvent(*this); }
    wxString GetPath() const { return m_filepath; }
    void SetPath(const wxString& path);

  private:
    wxString m_filepath;

    DECLARE_DYNAMIC_CLASS(ProjectEvent)
  };
}

typedef void (wxEvtHandler::*ProjectEventFunction)(Haven::ProjectEvent &);

BEGIN_DECLARE_EVENT_TYPES()
  DECLARE_EVENT_TYPE(PROJECT_EVENT_OPEN, 1)
END_DECLARE_EVENT_TYPES()


#define ProjectEventHandler(func) wxEVENT_HANDLER_CAST(ProjectEventFunction, func)

#define EVT_PROJECT_OPEN(id, func) \
  DECLARE_EVENT_TABLE_ENTRY(PROJECT_EVENT_OPEN, id, -1, (wxObjectEventFunction)(ProjectEventFunction) & func, (wxObject *)NULL),

#endif // HAVEN_PROJECT_EVENT_H_INCLUDED
