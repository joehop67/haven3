#include "../include/projectevent.h"

using namespace Haven;

IMPLEMENT_DYNAMIC_CLASS(ProjectEvent, wxCommandEvent)
DEFINE_EVENT_TYPE(PROJECT_EVENT_OPEN)

ProjectEvent::ProjectEvent(wxWindow *win) {
  SetEventType(PROJECT_EVENT_OPEN);
  SetEventObject(win);
}

ProjectEvent::ProjectEvent(const ProjectEvent& event) {
  SetEventType(PROJECT_EVENT_OPEN);
  SetEventObject(event.GetEventObject());
  wxString pathName(event.GetPath());
  this->SetPath(pathName);
}

void ProjectEvent::SetPath(const wxString &path) {
  wxString nfilepath(path);
  this->m_filepath = nfilepath;
}
