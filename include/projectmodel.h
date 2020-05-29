#ifndef HAVEN_PROJECTMODEL_H_INCLUDED
#define HAVEN_PROJECTMODEL_H_INCLUDED

#include <wx/hashmap.h>
#include <wx/dir.h>

#include "prefs.h"

WX_DECLARE_HASH_MAP(unsigned, wxString, wxIntegerHash, wxIntegerEqual, IntToStringMap);

namespace Haven {
  class ProjectModelNode;
  WX_DEFINE_ARRAY_PTR(ProjectModelNode*, ProjectModelNodePtrArray);

  class ProjectModelNode {
  public:
    ProjectModelNode(ProjectModelNode *parent, const wxString &fullPath, const wxString &fileName, const wxString &language, bool isDir) {
      m_parent = parent;
      fullPath = fullPath;
      fileName = fileName;
      language = language;
      title = fileName;
      isDir = isDir;
      m_container = false;
    }

    ProjectModelNode(ProjectModelNode *parent, const wxString &fullPath, const wxString &fileName, const wxString &language, const wxString &branch = NULL) {
      m_parent = parent;
      fullPath = fullPath;
      fileName = fileName;
      language = language;
      title = branch;
      isDir = true;
      m_container = true;
    }

    ~ProjectModelNode() {
      size_t count = m_children.GetCount();
      for (size_t i = 0; i < count; i++) {
        ProjectModelNode *child = m_children[i];
        delete child;
      }
    }

    bool IsContainer() const { return m_container; }
    bool IsDir() const { return isDir; }

    ProjectModelNode* GetParent() { return m_parent; }
    ProjectModelNodePtrArray& GetChildren { return m_children; }
    ProjectModelNode* GetChildAt(unsigned int n) { return m_children.Item(n); }

    void Insert(ProjectModelNode* child, unsigned int n) { m_children.Insert(child, n); }
    void Append(ProjectModelNode* child) { m_children.Add(child); }
    unsigned int GetChildCount() const { return m_children.GetCount(); }

  public:
    wxString fullPath;
    wxString fileName;
    wxString language;
    wxString title;
    bool isDir;
    bool m_container;

  private:
    ProjectModelNode *m_parent;
    ProjectModelNodePtrArray m_children;
  };

  class ProjectModel : public wxDataViewModel {
  public:
    ProjectModel(const wxString& sPath, const wxString& dirName);
    ~ProjectModel() { delete m_root; }

    wxString GetFilename(const wxDataViewItem &item) const;
    wxString GetFullpath(const wxDataViewItem &item) const;
    wxString GetTitle(const wxDataViewItem &item) const;
    wxString GetLanguage(const wxDataViewItem &item) const;

    void Delete(const wxDataViewItem &item);

    int Compare(const wxDataViewItem &item1, const wxDataViewItem &item2, unsigned int col, bool ascend) const wxOVERRIDE;

    static wxString GetLanguage(const wxString &ext);

    virtual unsigned int GetColumnCount() const wxOVERRIDE { return 2; }
    virtual wxString GetColumnType(unsigned int col) const wxOVERRIDE { return wxT("string"); }
    virtual void GetValue(wxVariant &variant, const wxDataViewItem &item, unsigned int col) const wxOVERRIDE;
    virtual bool SetValue(const wxVariant &variant, const wxDataViewItem &item, unsigned int col) wxOVERRIDE;
    virtual wxDataViewItem GetParent(const wxDataViewItem &item) const wxOVERRIDE;
    virtual bool IsContainer(const wxDataViewItem &item) const wxOVERRIDE;
    virtual unsigned int GetChildren(const wxDataViewItem &parent, wxDataViewItemArray &array) const wxOVERRIDE;

  private:
    ProjectModelNode* m_root;
  };

  class ProjectTraverser : public wxDirTraverser {
  public:
    ProjectTraverser(ProjectModelNode *root) : parentNode(root) {}
    virtual wxDirTraverseResult OnFile(const wxString &filename);
    virtual wxDirTraverseResult OnDir(const wxString &dirname);

  private:
    ProjectModelNode* parentNode;
  };
}

#endif // HAVEN_PROJECTMODEL_H_INCLUDED
