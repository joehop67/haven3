#include <iostream>
//#include <filesystem>
#include <string>
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif // WX_PRECOMP

#include <wx/dataview.h>
#include <wx/filename.h>

#include "../include/projectmodel.h"

//using namespace std;
using namespace boost;
using namespace Haven;

ProjectModel::ProjectModel(const wxString &sPath, const wxString &dirName) {
  if (wxFileName::DirExists(sPath)) {
    m_root = new ProjectModelNode(NULL, sPath, dirName, "null", true);
    filesystem::path rootPath(sPath);
    if (filesystem::is_directory(rootPath)) {
      filesystem::directory_iterator projectIterator(rootPath);
      filesystem::directory_iterator end_iter;
      ProjectModelNode *parent = m_root;
      ProjectModelNode *subDirs = NULL;
      for (; projectIterator != end_iter; projectIterator++) {
        wxString *name = new wxString(projectIterator->path().filename().string());
        wxString *path = new wxString(projectIterator->path().string());
        if (filesystem::is_directory(projectIterator->path())) {
          subDirs = new ProjectModelNode(parent, *path, *name, "null", true);
          BuildDirectoryChain(subDirs, projectIterator->path());
          parent->Append(subDirs);
          subDirs = NULL;
        }
        if (filesystem::is_regular_file(projectIterator->path())) {
          wxString *extensionPart = new wxString(projectIterator->path().extension().string());
          wxString ext = GetLanguage(*extensionPart);
          ProjectModelNode* fNode = new ProjectModelNode(parent, *path, *name, ext, false);
        }
      }
      m_root = parent;
    }
  }
}

void ProjectModel::BuildDirectoryChain(ProjectModelNode* node, const filesystem::path& nodePath) {
  if (filesystem::is_directory(nodePath)) {
    ProjectModelNode *parent = node;
    filesystem::path curPath = nodePath;
    filesystem::directory_iterator nodeItr(nodePath), endItr;
    while (nodeItr != endItr) {
      curPath = nodeItr->path();
      if (filesystem::is_directory(nodeItr->path())) {
        AppendDirNode(parent, curPath);
      }
      if (filesystem::is_regular_file(curPath)) {
        AppendFileNode(parent, curPath);
      }
      nodeItr++;
    }
  }
}

void ProjectModel::AppendFileNode(ProjectModelNode* parent, const filesystem::path& path) {
  wxString *pathName = new wxString;
  wxString *name = new wxString;
  wxString *ext = new wxString;
  pathName->Append(path.c_str());
  name->Append(path.filename().c_str());
  ext->Append(path.extension().c_str());
  wxString lang = GetLanguage(*ext);
  ProjectModelNode *fileNode = new ProjectModelNode(parent, *pathName, *name, lang, false);
  parent->Append(fileNode);
}

void ProjectModel::AppendDirNode(ProjectModelNode* parent, const filesystem::path& path) {
  wxString *pathName = new wxString;
  wxString *name = new wxString;
  pathName->Append(path.c_str());
  name->Append(path.filename().c_str());
  ProjectModelNode *dirNode = new ProjectModelNode(parent, *pathName, *name, "null", true);
  BuildDirectoryChain(dirNode, path);
  parent->Append(dirNode);
}


wxString ProjectModel::GetLanguage(const wxString &ext) {
  int checked;
  bool found;
  wxString *lang = new wxString;
  ExtMap currentExt;
  for (checked = 0; checked < g_ExtensionMapSize; checked++) {
    currentExt = g_ExtensionMap[checked];
    wxString curStr(currentExt.extensions);
    if (curStr.find(ext) != wxNOT_FOUND) {
      lang->Append(currentExt.language);
      found = true;
      break;
    }
  }
  if (found) return *lang;
  lang->Append("#!NO");
  return *lang;
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

bool ProjectModel::IsContainer(const wxDataViewItem &item) const {
  if (!item.IsOk())
    return true;

  ProjectModelNode *node = (ProjectModelNode*) item.GetID();
  return node->IsContainer();
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
