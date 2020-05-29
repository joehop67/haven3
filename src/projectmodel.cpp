#include <iostream>
#include <filesystem>

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif // WX_PRECOMP

#include <wx/dataview.h>
#include <wx/filename.h>

#include "../include/projectmodel.h"

using namespace std;
using namespace Haven;

wxDirTraverseResult ProjectTraverser::OnFile(const wxString &filename) {
  if (parentNode != NULL) {
    wxString* fileNamePart = new wxString();
    wxString* extPart = new wxString();
    wxFileName::SplitPath(filename, NULL, fileNamePart, extPart);
    wxString extension = ProjectModel::GetLanguage(extPart);
    ProjectModelNode fileNode = new ProjectModelNode(parentNode, filename, fileNamePart, *extension, false);
    parentNode->Append(fileNode);
    return wxDIR_CONTINUE;
  }
  return wxDIR_STOP;
}

wxDirTraverseResult ProjectTraverser::OnDir(const wxString &dirname) {
  if (parentNode != NULL) {
    wxString dirNamePart = new wxString();
    wxFileName::SplitPath(dirname, NULL, dirNamePart, NULL);
    ProjectModelNode dirNode = new ProjectModelNode(parentNode, dirname, dirNamePart, NULL);
    parentNode->Append(dirNode);
    parentNode = dirNode;
    return wxDIR_CONTINUE;
  }
  return wxDIR_STOP;
}

ProjectModel::ProjectModel(const wxString &sPath, const wxString &dirName) {
  if (wxFileName::DirExists(sPath)) {
    m_root = new ProjectModelNode(NULL, sPath, dirName, NULL, true);
    wxDir dir(sPath);
    if (!dir.IsOpened()) {
      ProjectTraverser traverser(m_root);
      dir.Traverse(traverser);
    }
  }
}

static wxString ProjectModel::GetLanguage(const wxString &ext) {
  int checked;
  bool found;
  wxString lang = new wxString();
  ExtMap currentExt;
  for (checked = 0; checked < g_ExtensionMapSize; checked++) {
    currentExt = g_ExtensionMap[checked];
    if (currentExt->extensions.find(ext) > npos) {
      lang.Append(currentExt->language);
      found = true;
      break;
    }
  }
  if (found) return lang;
  lang.Append("#!NO");
}

wxString ProjectModel::GetTitle(const wxDataViewItem &item) const {
  ProjectModelNode *node = (ProjectModelNode*) item.GetID();
  if (!node)
    return wxEmptyString;
  return node->title;
}

wxString ProjectModel::GetFilename(const wxDataViewItem &item) const {
  ProjectModelNode *node = (ProjectModelNode*) item.GetID();
  if (!node)
    return wxEmptyString;
  return node->fileName;
}

wxString ProjectModel::GetFullpath(const wxDataViewItem &item) const {
  ProjectModelNode *node = (ProjectModelNode*) item.GetID();
  if (!node)
    return wxEmptyString;
  return node->fullPath;
}

wxString ProjectModel::GetLanguage(const wxDataViewItem &item) const {
  ProjectModelNode *node = (ProjectModelNode*) item.GetID();
  if (!node)
    return wxEmptyString;
  return node->language;
}

void ProjectModel::Delete(const wxDataViewItem &item) {
  ProjectModelNode *node = (ProjectModelNode*) item.GetID();
  if (!node)
    return;

  wxDataViewItem parent(node->GetParent());
  if (!parent.IsOk()) {
    wxASSERT(node == m_root);
    wxLogError("Cannot remove root node");
    return;
  }

  node->GetParent()->GetChildren().Remove(node);

  delete node;

  ItemDeleted(parent, item);
}

int ProjectModel::Compare(const wxDataViewItem &item1, const wxDataViewItem &item2, unsigned int col, bool ascend) const {
  wxASSERT(item1.IsOk() && item2.IsOk());

  if (IsContainer(item1) && IsContainer(item2)) {
    wxVariant val1, val2;
    GetValue(val1, item1, 0);
    GetValue(val2, item2, 0);

    wxString str1 = val1.GetString();
    wxString str2 = val2.GetString();
    int res = str1.Cmp(str2);
    if (res) return res;

    wxUIntPtr litem1 = (wxUIntPtr) item1.GetID();
    wxUIntPtr litem2 = (wxUIntPtr) item2.GetID();

    return litem1 - litem2;
  }

  return wxDataViewModel::Compare(item1, item2, col, ascend);
}

void ProjectModel::GetValue(wxVariant &variant, const wxDataViewItem &item, unsigned int col) const {
  wxASSERT(item.IsOk());

  ProjectModelNode *node = (ProjectModelNode*) item.GetID();
  switch (col) {
    case 0:
      variant = node->fileName;
      break;
    case 1:
      variant = node->language;
      break;
    default:
      wxLogError("Wrong column id");
  }
}

bool ProjectModel::SetValue(const wxVariant &variant, const wxDataViewItem &item, unsigned int col) {
  wxASSERT(item.IsOk());

  ProjectModelNode *node = (ProjectModelNode*) item.GetID();
  switch (col) {
    case 0:
      node->fileName = variant.GetString();
      return true;
    case 1:
      node->language = variant.GetString();
      return true;
    default:
      wxLogError("Wrong column id");
  }
  return false;
}

wxDataViewItem ProjectModel::GetParent(const wxDataViewItem &item) const {
  if (!item.IsOk())
    return wxDataViewItem(0);

  ProjectModelNode *node = (ProjectModelNode*) item.GetID();

  if (node == m_root)
    return wxDataViewItem(0);

  return wxDataViewItem((void*) node->GetParent());
}

unsigned int ProjectModel::GetChildren(const wxDataViewItem &parent, wxDataViewItemArray &array) const {
  ProjectModelNode *node = (ProjectModelNode*) parent.GetID();

  if (!node) {
    array.Add(wxDataViewItem((void*) m_root));
    return 1;
  }

  if (node->GetChildCount() == 0) {
    return 0;
  }

  unsigned int count = node->GetChildren().GetCount();
  for (unsigned int pos = 0; pos < count; pos++) {
    ProjectModelNode *child = node->GetChildren().Item(pos);
    array.Add(wxDataViewItem((void*) child));
  }
  return count;
}
