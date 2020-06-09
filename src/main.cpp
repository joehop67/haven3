#include <wx/wxprec.h>

#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif // WX_PRECOMP

#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/dirdlg.h>
#include <wx/notebook.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/image.h>
#include <wx/splitter.h>
#include <wx/event.h>
#include <wx/stdpaths.h>

#include "../include/defs.h"
#include "../include/projectevent.h"
#include "../include/tabview.h"
#include "../include/edit.h"
#include "../include/prefs.h"
#include "../include/projectview.h"
#include "../include/languagemeta.h"

using namespace Haven;
using namespace Haven::LanguageMeta;

class HavenBook;
class HavenFrame;

wxString *g_appname = NULL;

class HavenApp : public wxApp {
  friend class HavenFrame;
public:
  virtual bool OnInit();

  virtual int OnExit();

private:
  HavenFrame* m_frame;
  wxFrame* MinimalEditor();

protected:
  void CreateHavenConfig(wxFileConfig *config);
  void OnMinimalEditor(wxCommandEvent&);
  wxDECLARE_EVENT_TABLE();
};

DECLARE_APP(HavenApp);

class HavenFrame : public wxFrame {
  friend class HavenApp;
  friend class HavenBook;
  friend class HavenAbout;
public:
  HavenFrame(const wxString& title);
  ~HavenFrame();

  void OnClose(wxCloseEvent &event);
  void OnAbout(wxCommandEvent &event);
  void OnExit(wxCommandEvent &event);
  void OnTimerEvent(wxTimerEvent &event);
  // File Events
  void OnFileNew(wxCommandEvent &event);
  void OnFileNewFrame(wxCommandEvent &event);
  void OnFileOpen(wxCommandEvent &event);
  void OnFileOpenFrame(wxCommandEvent &event);
  void OnFileSave(wxCommandEvent &event);
  void OnFileSaveAs(wxCommandEvent &event);
  void OnFileClose(wxCommandEvent &event);
  void OnOpenProject(wxCommandEvent &event);
  // Properties
  void OnProperties(wxCommandEvent &event);
  void OnEdit(wxCommandEvent &event);
//  void OnTabChange(wxNotebookEvent &event);
  void OnFileOpenNT(wxCommandEvent &event);
  void ChangeTab(TabInfo tab);
  void OnProjectOpen(ProjectEvent &event);
  void OnTabChange(wxMouseEvent &event);
private:
  bool IsProject();
  ProjectInfo project;

  wxPanel *m_pane;
  TabView *m_tabman;
  Edit *m_edit;
  ProjectView *m_project_view;
  wxSplitterWindow *winSplit;
  wxBoxSizer *splitSizer;
  LanguageDB *languageDb;
  StyleDB *styleDb;

  void FileOpen(wxString fname);
  void ProjectOpen(wxString folder);
  void InitLangAndStyleDBs(LanguageDB *lang, StyleDB *style);

  wxMenuBar *m_menuBar;
  void CreateMenu();

  wxDECLARE_EVENT_TABLE();
};

class HavenAbout : public wxDialog {
public:
  HavenAbout(wxWindow *parent, int ms = 0, long style = 0);
  ~HavenAbout();

  void OnTimerEvent(wxTimerEvent &event);

private:
  wxTimer *m_timer;
  wxDECLARE_EVENT_TABLE();
};

IMPLEMENT_APP(HavenApp);

wxBEGIN_EVENT_TABLE(HavenApp, wxApp)
  EVT_MENU(havenID_WINDOW_MINIMAL, HavenApp::OnMinimalEditor)
wxEND_EVENT_TABLE()

bool HavenApp::OnInit() {
  wxInitAllImageHandlers();

  SetAppName("Haven");
  SetVendorName("Joey Hop");
  g_appname = new wxString();
  g_appname->Append("Haven");

  wxString confDir = wxStandardPaths::Get().GetUserConfigDir() + wxFileName::GetPathSeparator() + "joeyhop";
  wxString confPath = confDir + wxFileName::GetPathSeparator() + "haven.conf";

  if(!wxDir::Exists(confDir)) {
    wxDir::Make(confDir);
  }
  wxFileConfig *havenConfig = new wxFileConfig("", "", confPath);
  if (!wxFile::Exists(confPath)) {
    CreateHavenConfig(havenConfig);
  }

  wxConfigBase::Set(havenConfig);
  m_frame = new HavenFrame(*g_appname);
  //wxBoxSizer *winSize = new wxBoxSizer(wxHORIZONTAL);
  //winSize->SetMinSize(750, 550);
  //winSize->Add(m_frame->m_pane, 1, wxEXPAND);
  //m_frame->SetSizerAndFit(winSize);
  m_frame->Layout();
  m_frame->Show(true);

  return true;
}

void HavenApp::CreateHavenConfig(wxFileConfig *config) {
  config->SetPath("/Editor");
  config->Write("ShowLineNumbers", true);
  config->Write("EnableCodeFolding", true);

  config->SetPath("/Layout");
  config->Write("StartFullScreen", false);
  config->Write("ShowProjectPane", true);

  config->SetPath("/Startup");
  config->Write("ShowWelcome", true);
  config->Write("DefaultProject", false);

  config->SetPath("/Paths");
  config->Write("PluginPath", false);
  config->Write("ThemePath", false);

  config->Flush();
}

int HavenApp::OnExit() {
  delete wxConfigBase::Set((wxConfigBase*) NULL);
  delete g_appname;
  return 0;
}

wxBEGIN_EVENT_TABLE(HavenFrame, wxFrame)
  EVT_CLOSE(HavenFrame::OnClose)
  EVT_MENU(wxID_OPEN, HavenFrame::OnFileOpen)
  EVT_MENU(wxID_OPEN + 3000, HavenFrame::OnFileOpenNT)
  EVT_MENU(havenID_OPEN_PROJECT, HavenFrame::OnOpenProject)
  EVT_MENU(wxID_SAVE, HavenFrame::OnFileSave)
  EVT_MENU(wxID_SAVEAS, HavenFrame::OnFileSaveAs)
  EVT_MENU(wxID_CLOSE, HavenFrame::OnFileClose)
  EVT_MENU(havenID_PROPERTIES, HavenFrame::OnProperties)
  EVT_MENU(wxID_EXIT, HavenFrame::OnExit)
  EVT_MENU(wxID_CLEAR, HavenFrame::OnEdit)
  EVT_MENU(wxID_CUT, HavenFrame::OnEdit)
  EVT_MENU(wxID_COPY, HavenFrame::OnEdit)
  EVT_MENU(wxID_PASTE, HavenFrame::OnEdit)
  EVT_MENU(wxID_SELECTALL, HavenFrame::OnEdit)
  EVT_MENU(wxID_REDO, HavenFrame::OnEdit)
  EVT_MENU(wxID_UNDO, HavenFrame::OnEdit)
  EVT_MENU(wxID_FIND, HavenFrame::OnEdit)
  EVT_MENU_RANGE(havenID_EDIT_FIRST, havenID_EDIT_LAST, HavenFrame::OnEdit)
  EVT_MENU(wxID_ABOUT, HavenFrame::OnAbout)
  EVT_PROJECT_OPEN(PROJECT_OPEN_FILE_ID, HavenFrame::OnProjectOpen)
wxEND_EVENT_TABLE()

HavenFrame::HavenFrame(const wxString& title)
: wxFrame((wxFrame *)NULL, wxID_ANY, title, wxDefaultPosition, wxSize(750, 550), wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE)
{
  SetIcon(wxICON(sample));

  m_pane = NULL;
  m_tabman = NULL;
  m_edit = NULL;
  m_project_view = NULL;
  languageDb = new LanguageDB();
  styleDb = new StyleDB();

  InitLangAndStyleDBs(languageDb, styleDb);

  wxString tmp = wxFileName::GetCwd();
  const char* cwdDir = tmp.mb_str();

  project = {
      "default",
      cwdDir,
      cwdDir,
      "n/a"
  };

  SetTitle(*g_appname);
  SetBackgroundColour(wxT("WHITE"));

  m_menuBar = new wxMenuBar;
  CreateStatusBar(4);
  CreateMenu();

  SetStatusText(wxT("New File"), 0);
  SetStatusText(wxT("<default>"), 1);
  SetStatusText(wxT("placeholder"), 2);
  SetStatusText(wxT("Placeholder"), 3);

  splitSizer = new wxBoxSizer(wxVERTICAL);
  splitSizer->SetMinSize(750, 550);
  winSplit = new wxSplitterWindow(this, wxID_ANY);
  winSplit->SetSashGravity(0.5);
  winSplit->SetMinimumPaneSize(20);
  splitSizer->Add(winSplit, 1, wxEXPAND, 0);

  wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
  m_pane = new wxPanel(winSplit, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, "");
  m_tabman = new TabView(m_pane, languageDb, styleDb, wxID_ANY);
  //m_tabman->Bind(wxEVT_LEFT_UP, &HavenFrame::OnTabChange, this);
  sizer->Add(m_tabman, 1, wxEXPAND);
  m_pane->SetSizer(sizer);

  m_project_view = new ProjectView(winSplit, this, project.rootPath, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxEXPAND);

  winSplit->SplitVertically(m_project_view, m_pane, 0);
  this->SetSizerAndFit(splitSizer);
  winSplit->SetSashPosition(200, true);

  m_edit = m_tabman->GetCurrentTab().t_edit;
  m_edit->SetFocus();

  // FileOpen(wxT("main.cpp"));
}

HavenFrame::~HavenFrame() {
  //wxConfigBase *conf = wxConfigBase::Get(false);
  //if (conf == NULL)
    //return;

  //conf->Write("/test/test123", "This is a test val");
  //conf->Write("/test/oanother", "another");
  //conf->Flush();
}

void HavenFrame::InitLangAndStyleDBs(LanguageDB *lang, StyleDB *style) {

  const char* CppWordlist1 =
    "asm auto bool break case catch char class const const_cast "
    "continue default delete do double dynamic_cast else enum explicit "
    "export extern false float for friend goto if inline int long "
    "mutable namespace new operator private protected public register "
    "reinterpret_cast return short signed sizeof static static_cast "
    "struct switch template this throw true try typedef typeid "
    "typename union unsigned using virtual void volatile wchar_t "
    "while";
  const char* CppWordlist2 =
    "file";
  const char* CppWordlist3 =
    "a addindex addtogroup anchor arg attention author b brief bug c "
    "class code date def defgroup deprecated dontinclude e em endcode "
    "endhtmlonly endif endlatexonly endlink endverbatim enum example "
    "exception f$ f[ f] file fn hideinitializer htmlinclude "
    "htmlonly if image include ingroup internal invariant interface "
    "latexonly li line link mainpage name namespace nosubgrouping note "
    "overload p page par param post pre ref relates remarks return "
    "retval sa section see showinitializer since skip skipline struct "
    "subsection test throw todo typedef union until var verbatim "
    "verbinclude version warning weakgroup $ @ \"\" & < > # { }";

  const char* ext = "cpp";
  const char* defExt = "any";

  Language *cpp = new Language("C++", "*.c;*.cc;*.cpp;*.cxx;*.cs;*.h;*.hh;*.hpp;*.hxx;*.sma", wxSTC_LEX_CPP);
  Language *defaultLang = new Language("<default>", "*.*", wxSTC_LEX_PROPERTIES);


      LanguageStyle lFirst{
        havenSTC_TYPE_DEFAULT,
        NULL
      };

      LanguageStyle lSec{
        havenSTC_TYPE_COMMENT,
        NULL
      };

      LanguageStyle lThird{
        havenSTC_TYPE_COMMENT_LINE,
        NULL
      };

      LanguageStyle lFourth{
        havenSTC_TYPE_COMMENT_DOC,
        NULL
      };

      LanguageStyle lFifth{
        havenSTC_TYPE_NUMBER,
        NULL
      };

      LanguageStyle lSixth{
        havenSTC_TYPE_WORD1,
        CppWordlist1
      };

      LanguageStyle lSeventh{
        havenSTC_TYPE_STRING,
        NULL
      };

      LanguageStyle lEighth{
        havenSTC_TYPE_CHARACTER,
        NULL
      };

      LanguageStyle lNinth{
        havenSTC_TYPE_UUID,
        NULL
      };

      LanguageStyle lTenth{
        havenSTC_TYPE_PREPROCESSOR,
        NULL
      };

      LanguageStyle lEleventh{
        havenSTC_TYPE_OPERATOR,
        NULL
      };

      LanguageStyle lTwelveth{
        havenSTC_TYPE_IDENTIFIER,
        NULL
      };

      LanguageStyle lThirteenth{
        havenSTC_TYPE_STRING_EOL,
        NULL
      };

      LanguageStyle lFourteenth{
        havenSTC_TYPE_DEFAULT,
        NULL
      };

      LanguageStyle lFifteenth{
        havenSTC_TYPE_REGEX,
        NULL
      };

      LanguageStyle lSixteenth{
        havenSTC_TYPE_COMMENT_SPECIAL,
        NULL
      };

      LanguageStyle lSeventeenth{
        havenSTC_TYPE_WORD2,
        CppWordlist2
      };

      LanguageStyle lEighteenth{
        havenSTC_TYPE_WORD3,
        CppWordlist3
      };

      LanguageStyle lNineteenth{
        havenSTC_TYPE_ERROR,
        NULL
      };


    cpp->AppendStyle(lFirst);
    cpp->AppendStyle(lSec);
    cpp->AppendStyle(lThird);
    cpp->AppendStyle(lFourth);
    cpp->AppendStyle(lFifth);
    cpp->AppendStyle(lSixth);
    cpp->AppendStyle(lSeventh);
    cpp->AppendStyle(lEighth);
    cpp->AppendStyle(lNinth);
    cpp->AppendStyle(lTenth);
    cpp->AppendStyle(lEleventh);
    cpp->AppendStyle(lTwelveth);
    cpp->AppendStyle(lThirteenth);
    cpp->AppendStyle(lFourteenth);
    cpp->AppendStyle(lFifteenth);
    cpp->AppendStyle(lSixteenth);
    cpp->AppendStyle(lSeventeenth);
    cpp->AppendStyle(lEighteenth);
    cpp->AppendStyle(lNineteenth);

    defaultLang->AppendStyle(lFirst);
    defaultLang->AppendStyle(lFirst);
    defaultLang->AppendStyle(lFirst);

    languageDb->Register(cpp, ext);
    languageDb->Register(defaultLang, defExt);


  StyleDef defaultStyle{
    wxT("Default"),
    wxT("BLACK"),
    wxT("WHITE"),
    wxT(""),
    10,
    0,
    0
  };

  StyleDef Key1{
    wxT("Keyword1"),
    wxT("BLUE"),
    wxT("WHITE"),
    wxT(""),
    10,
    havenSTC_STYLE_BOLD,
    0
  };

    // mySTC_TYPE_WORD2
    StyleDef Key2{
      wxT("Keyword2"),
      wxT("MIDNIGHT BLUE"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    // mySTC_TYPE_WORD3
    StyleDef key3{
      wxT("Keyword3"),
      wxT("CORNFLOWER BLUE"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    // mySTC_TYPE_WORD4
    StyleDef Key4{
      wxT("Keyword4"),
      wxT("CYAN"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    // mySTC_TYPE_WORD5
    StyleDef Key5{
      wxT("Keyword5"),
      wxT("DARK GREY"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    // mySTC_TYPE_WORD6
    StyleDef Key6{
      wxT("Keyword6"),
      wxT("GREY"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef Com{
      wxT("Comment"),
      wxT("FOREST GREEN"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef ComLine{
      wxT("Comment line"),
      wxT("FOREST GREEN"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef ComSpec{
      wxT("Special comment"),
      wxT("FOREST GREEN"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef CharStyle{
      wxT("Character"),
      wxT("KHAKI"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef CharEOLStyle{
      wxT("Character (EOL)"),
      wxT("KHAKI"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef StrStyle{
      wxT("String"),
      wxT("BROWN"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef StrEOLStyle{
      wxT("String (EOL)"),
      wxT("BROWN"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef DelimStyle{
      wxT("Delimiter"),
      wxT("ORANGE"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef PuncStyle{
      wxT("Punctuation"),
      wxT("ORANGE"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef OpStyle{
      wxT("Operator"),
      wxT("BLACK"),
      wxT("WHITE"),
      wxT(""),
      10,
      havenSTC_STYLE_BOLD,
      0
    };

    StyleDef BraceStyle{
      wxT("Brace"),
      wxT("VIOLET"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef CommandStyle{
      wxT("Command"),
      wxT("BLUE"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef LabelStyle{
      wxT("Label"),
      wxT("VIOLET"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef NumStyle{
      wxT("Number"),
      wxT("SIENNA"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef ParamStyle{
      wxT("Parameter"),
      wxT("VIOLET"),
      wxT("WHITE"),
      wxT(""),
      10,
      havenSTC_STYLE_ITALIC,
      0
    };

    StyleDef RegexStyle{
      wxT("Regex"),
      wxT("ORCHID"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef UUIDStyle{
      wxT("UUID"),
      wxT("ORCHID"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef ValStyle{
      wxT("Value"),
      wxT("ORCHID"),
      wxT("WHITE"),
      wxT(""),
      10,
      havenSTC_STYLE_ITALIC,
      0
    };

    StyleDef PreProcStyle{
      wxT("Preprocessor"),
      wxT("GREY"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef ScriptStyle{
      wxT("Script"),
      wxT("DARK GREY"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef ErrStyle{
      wxT("Error"),
      wxT("Red"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    StyleDef UndefStyle{
      wxT("Undefined"),
      wxT("ORANGE"),
      wxT("WHITE"),
      wxT(""),
      10,
      0,
      0
    };

    style->Add(defaultStyle);
    style->Add(Key1);
    style->Add(Key2);
    style->Add(key3);
    style->Add(Key4);
    style->Add(Key5);
    style->Add(Key6);
    style->Add(Com);
    style->Add(ComLine);
    style->Add(ComSpec);
    style->Add(CharStyle);
    style->Add(CharEOLStyle);
    style->Add(StrStyle);
    style->Add(StrEOLStyle);
    style->Add(DelimStyle);
    style->Add(PuncStyle);
    style->Add(OpStyle);
    style->Add(BraceStyle);
    style->Add(CommandStyle);
    style->Add(LabelStyle);
    style->Add(NumStyle);
    style->Add(ParamStyle);
    style->Add(RegexStyle);
    style->Add(UUIDStyle);
    style->Add(ValStyle);
    style->Add(PreProcStyle);
    style->Add(ScriptStyle);
    style->Add(ErrStyle);
    style->Add(UndefStyle);
}

void HavenFrame::OnClose(wxCloseEvent &event) {
  wxCommandEvent ev;
  OnFileClose(ev);
  if (m_edit && m_edit->Modified()) {
    if (event.CanVeto()) event.Veto(true);
    return;
  }
  Destroy();
}

//void HavenFrame::OnTabChange(wxBookCtrlEvent &WXUNUSED(event)) {
//  if (this->wxWindow::IsShown()) {
//    m_edit = m_tabman->GetCurrentTab().t_edit;
//  }
//}

void HavenFrame::ChangeTab(TabInfo tab) {
  m_edit = tab.t_edit;
}

void HavenFrame::OnAbout(wxCommandEvent &WXUNUSED(event)) {
  HavenAbout dialog(this);
}

void HavenFrame::OnExit(wxCommandEvent &WXUNUSED(event)) {
  Close(true);
}

void HavenFrame::OnOpenProject(wxCommandEvent &WXUNUSED(event)) {
  if (!m_edit || !m_project_view) return;
  wxString folder;
  wxDirDialog dialog(this, wxT("Open Project"), wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST | wxDD_CHANGE_DIR);
  if (dialog.ShowModal() != wxID_OK) return;
  folder = dialog.GetPath();
  ProjectOpen(folder);
}

void HavenFrame::OnTabChange(wxMouseEvent &event) {
  long flags;
  int ht = m_tabman->HitTest(wxPoint(event.GetX(), event.GetY()), &flags);
  if ((flags & wxBK_HITTEST_NOWHERE) == 0) {
    m_tabman->SetCurrentActiveTab(ht);
    m_edit = m_tabman->GetCurrentTab().t_edit;
  }
  //event.Skip();
}

void HavenFrame::OnProjectOpen(ProjectEvent &event) {
  wxString tPath = event.GetPath();
  FileOpen(tPath);
}

void HavenFrame::OnFileOpen(wxCommandEvent &WXUNUSED(event)) {
  if (!m_edit) return;
  wxString fname;
  wxFileDialog dialog(this, wxT("Open File"), wxEmptyString, wxEmptyString, wxT("Any file (*)|*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);
  if (dialog.ShowModal() != wxID_OK) return;
  fname = dialog.GetPath();
  //m_tabman->SetTabTitle(m_tabman->GetCurrentTabIndex(), dialog.GetFilename());
  FileOpen(fname);
}

void HavenFrame::OnFileOpenNT(wxCommandEvent &WXUNUSED(event)) {
  if (!m_tabman) return;
  Edit *ntab = new Edit(m_tabman, languageDb, styleDb, wxID_ANY);
  wxString fname;
  wxFileDialog dialog(this, wxT("Open File"), wxEmptyString, wxEmptyString, wxT("Any file (*)|*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);
  if (dialog.ShowModal() != wxID_OK) return;
  fname = dialog.GetPath();
  m_tabman->AddTab(dialog.GetFilename(), ntab);
  FileOpen(fname);
}

void HavenFrame::OnFileSave(wxCommandEvent &WXUNUSED(event)) {
  if (!m_edit) return;
  if (!m_edit->Modified()) {
    wxMessageBox(_("Nothing to save"), _("Save File"), wxOK | wxICON_EXCLAMATION);
    return;
  }
  m_edit->SaveFile();
}

void HavenFrame::OnFileSaveAs(wxCommandEvent &WXUNUSED(event)) {
  if (!m_edit) return;
  wxString filename = wxEmptyString;
  wxFileDialog dialog(this, wxT("Save File"), wxEmptyString, wxEmptyString, wxT("Any File (*)|*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (dialog.ShowModal() != wxID_OK) return;
  filename = dialog.GetPath();
  m_edit->SaveFile(filename);
}

void HavenFrame::OnFileClose(wxCommandEvent &WXUNUSED(event)) {
  if (!m_edit) return;
  if (m_edit->Modified()) {
    if (wxMessageBox(_("Changes not saved, save before closing?"), _("Close"), wxYES_NO | wxICON_QUESTION) == wxYES) {
      m_edit->SaveFile();
      if (m_edit->Modified()) {
        wxMessageBox(_("Text could not be saved"), _("Close abort"), wxOK | wxICON_EXCLAMATION);
        return;
      }
    }
  }
  m_edit->SetFilename(wxEmptyString);
  m_edit->ClearAll();
  m_edit->SetSavePoint();
}

void HavenFrame::OnProperties(wxCommandEvent &WXUNUSED(event)) {
  if (!m_edit) return;
  EditProperties dialog(m_edit, 0);
}

void HavenFrame::OnEdit(wxCommandEvent &event) {
  if (m_edit) m_edit->GetEventHandler()->ProcessEvent(event);
}

void HavenFrame::CreateMenu() {
  wxMenu *menuFile = new wxMenu;
  menuFile->Append(wxID_OPEN, _("&Open...\tCtrl-O"));
  menuFile->Append(wxID_OPEN + 3000, _("&Open (new tab)...\tCtrl-O"));
  menuFile->Append(havenID_OPEN_PROJECT, _("&Open Project..."));
  menuFile->Append(wxID_SAVE, _("&Save...\tCtrl-S"));
  menuFile->Append(wxID_SAVEAS, _("&Save as...\tCtrl-Shift-S"));
  menuFile->Append(wxID_CLOSE, _("&Close...\tCtrl-W"));
  menuFile->AppendSeparator();
  menuFile->Append(havenID_PROPERTIES, _("&Properties\tCtrl-I"));
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT, _("&Quit\tCtrl-Q"));

  wxMenu *menuEdit = new wxMenu;
  menuEdit->Append(wxID_UNDO, _("&Undo\tCtrl-Z"));
  menuEdit->Append(wxID_REDO, _("&Redo\tCtrl-Shift-Z"));
  menuEdit->AppendSeparator();
  menuEdit->Append(wxID_CUT, _("&Cut\tCtrl-X"));
  menuEdit->Append(wxID_COPY, _("&Copy\tCtrl-C"));
  menuEdit->Append(wxID_PASTE, _("&Paste\tCtrl-V"));
  menuEdit->Append(wxID_CLEAR, _("&Clear\tDel"));
  menuEdit->AppendSeparator();
  menuEdit->Append(wxID_FIND, _("&Find\tCtrl-F"));
  menuEdit->Enable(wxID_FIND, false);
  menuEdit->Append(havenID_FINDNEXT, _("&Find Next\tF3"));
  menuEdit->Enable(havenID_FINDNEXT, false);
  menuEdit->Append(havenID_REPLACE, _("&Replace\tCtrl-H"));
  menuEdit->Enable(havenID_REPLACE, false);
  menuEdit->Append(havenID_REPLACENEXT, _("&Replace Next\tShift-F4"));
  menuEdit->Enable(havenID_REPLACENEXT, false);
  menuEdit->AppendSeparator();
  menuEdit->Append(havenID_BRACEMATCH, _("&Match Brace\tCtrl-M"));
  menuEdit->Append(havenID_GOTO, _("&Goto\tCtrl-G"));
  menuEdit->Enable(havenID_GOTO, false);
  menuEdit->AppendSeparator();
  menuEdit->Append(havenID_INDENTINC, _("&Increase Indent\tTab"));
  menuEdit->Append(havenID_INDENTRED, _("&Reduce Indent\tShift-Tab"));
  menuEdit->AppendSeparator();
  menuEdit->Append(wxID_SELECTALL, _("&Select All\tCtrl-A"));
  menuEdit->Append(havenID_SELECTLINE, _("&Select Line\tCtrl-L"));

  wxMenu *menuHighlight = new wxMenu;
  int num;
  for (num = 0; num < g_LanguagePrefsSize; num++) {
    menuHighlight->Append(havenID_HILIGHTFIRST + num, g_LanguagePrefs[num].name);
  }

  wxMenu *menuCharset = new wxMenu;
  menuCharset->Append(havenID_CHARSETANSI, _("&ANSI (Windows)"));
  menuCharset->Append(havenID_CHARSETMAC, _("MAC (Mac)"));

  wxMenu *menuView = new wxMenu;
  menuView->Append(havenID_HILIGHTLANG, _("&Highlight Language..."), menuHighlight);
  menuView->AppendSeparator();
  menuView->AppendCheckItem(havenID_FOLDTOGGLE, _("&Toggle Current Fold\tCtrl-T"));
  menuView->AppendCheckItem(havenID_OVERTYPE, _("&Overwrite Mode\tIns"));
  menuView->AppendCheckItem(havenID_WRAPMODEON, _("&Wrap Mode\tCtrl-U"));
  menuView->AppendSeparator();
  menuView->AppendCheckItem(havenID_DISPLAYEOL, _("&Show Line Endings"));
  menuView->AppendCheckItem(havenID_INDENTGUIDE, _("&Show Indent Guides"));
  menuView->AppendCheckItem(havenID_LINENUMBER, _("&Show Line Numbers"));
  menuView->AppendCheckItem(havenID_LONGLINEON, _("&Show Long Line Marker"));
  menuView->AppendCheckItem(havenID_WHITESPACE, _("&Show Whitespace"));
  menuView->AppendSeparator();
  menuView->Append(havenID_USECHARSET, _("Use Charset..."), menuCharset);

  wxMenu *menuAnnotations = new wxMenu;
  menuAnnotations->Append(havenID_ANNOTATION_ADD, _("&Add Or Edit Annotation..."), _("Add annotation to current line"));
  menuAnnotations->Append(havenID_ANNOTATION_REMOVE, _("&Remove Annotation"), _("Remove annotation from current line"));
  menuAnnotations->Append(havenID_ANNOTATION_CLEAR, _("&Clear All Annotations"));

  wxMenu *menuAnnoStyle = new wxMenu;
  menuAnnoStyle->AppendRadioItem(havenID_ANNOTATION_STYLE_HIDDEN, _("&Hidden"));
  menuAnnoStyle->AppendRadioItem(havenID_ANNOTATION_STYLE_STANDARD, _("&Standard"));
  menuAnnoStyle->AppendRadioItem(havenID_ANNOTATION_STYLE_BOXED, _("&BOXED"));
  menuAnnotations->AppendSubMenu(menuAnnoStyle, "&Style");

  wxMenu *menuChangeCase = new wxMenu;
  menuChangeCase->Append(havenID_CHANGEUPPER, _("&Upper Case"));
  menuChangeCase->Append(havenID_CHANGELOWER, _("&Lower Case"));

  wxMenu *menuConvertEOL = new wxMenu;
  menuConvertEOL->Append(havenID_CONVERTCR, _("CR (&Linux)"));
  menuConvertEOL->Append(havenID_CONVERTCRLF, _("CRLF (&Windows)"));
  menuConvertEOL->Append(havenID_CONVERTLF, _("LF (&Mac)"));

  wxMenu *menuExtra = new wxMenu;
  menuExtra->AppendCheckItem(havenID_READONLY, _("&Readonly Mode"));
  menuExtra->AppendSeparator();
  menuExtra->Append(havenID_CHANGECASE, _("Change Case..."), menuChangeCase);
  menuExtra->AppendSeparator();
  menuExtra->Append(havenID_CONVERTEOL, _("Change Line Endings..."), menuConvertEOL);

  wxMenu *menuWindow = new wxMenu;
  menuWindow->Append(havenID_PAGEPREV, _("&Previous\tCtrl-Shift-Tab"));
  menuWindow->Append(havenID_PAGENEXT, _("&Next\tCtrl-Tab"));
  menuWindow->Append(havenID_WINDOW_MINIMAL, _("&Minimal Editor"));

  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT, _("&About Haven..."));

  m_menuBar->Append(menuFile, _("&File"));
  m_menuBar->Append(menuEdit, _("&Edit"));
  m_menuBar->Append(menuView, _("&View"));
  m_menuBar->Append(menuAnnotations, _("&Annotations"));
  m_menuBar->Append(menuExtra, _("&Extra"));
  m_menuBar->Append(menuWindow, _("&Window"));
  m_menuBar->Append(menuHelp, _("&Help"));
  SetMenuBar(m_menuBar);

  m_menuBar->Check(havenID_ANNOTATION_STYLE_BOXED, true);
}

void HavenFrame::FileOpen(wxString fname) {
  int curTab = m_tabman->GetCurrentTabIndex();
  wxString fnamePart;
  wxFileName w(fname); w.Normalize(); fname = w.GetFullPath(); fnamePart = w.GetFullName();
  if (!m_tabman->IsProjectFileOpen()) {
    m_tabman->SetTabTitle(curTab, fnamePart);
    m_tabman->SetProjectFileOpen(true);
  } else {
    Edit *nTab = new Edit(m_tabman, languageDb, styleDb, wxID_ANY);
    m_tabman->AddTab(fnamePart, nTab);
    m_tabman->SetProjectFileOpen(true);
  }
  m_edit = m_tabman->GetCurrentTab().t_edit;
  m_edit->LoadFile(fname);
  m_edit->SelectNone();

  SetStatusText(m_edit->GetFilename(), 0);
  SetStatusText(m_edit->GetLanguageInfo()->GetName(), 1);
}

void HavenFrame::ProjectOpen(wxString folder) {
  wxString folderPart;
  wxFileName w(folder); w.Normalize(); folder = w.GetFullPath(); folderPart = w.GetFullName();
  const char* name = folderPart.mb_str();
  const char* path = folder.mb_str();
  project = {
    name,
    name,
    path,
    "lang"
  };

  //this->Show(false);
  this->Freeze();
  splitSizer->Clear();
  SetStatusText(name, 0);
  //splitSizer->Destroy();
  //winSplit->RemoveChild(m_project_view);
  winSplit->Destroy();
  //m_pane->Destroy();
  //while(m_project_view->IsBeingDeleted()) { continue; };
  wxBoxSizer *nSizer = new wxBoxSizer(wxVERTICAL);
  nSizer->SetMinSize(750, 550);
  winSplit = new wxSplitterWindow(this, wxID_ANY);
  winSplit->SetSashGravity(0.5);
  winSplit->SetMinimumPaneSize(20);
  nSizer->Add(winSplit, 1, wxEXPAND, 0);

  wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
  m_pane = new wxPanel(winSplit, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, "");
  m_tabman = new TabView(m_pane, languageDb, styleDb, wxID_ANY);
  sizer->Add(m_tabman, 1, wxEXPAND);
  m_pane->SetSizer(sizer);

  m_project_view = new ProjectView(winSplit, this, project.rootPath);

  winSplit->SplitVertically(m_project_view, m_pane, 0);
  //this->Fit();
  this->SetSizerAndFit(nSizer);

  splitSizer = nSizer;

  m_edit = m_tabman->GetCurrentTab().t_edit;
  m_edit->SetFocus();
  SetStatusText("default", 2);
  this->Layout();
  this->Refresh();
  this->Update();
  this->Thaw();
  //this->Show(true);
  winSplit->SetSashPosition(200, true);
}

wxBEGIN_EVENT_TABLE(HavenAbout, wxDialog)
  EVT_TIMER(havenID_ABOUTTIMER, HavenAbout::OnTimerEvent)
wxEND_EVENT_TABLE()

HavenAbout::HavenAbout(wxWindow *parent, int ms, long style)
  : wxDialog(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, style | wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m_timer = NULL;
  if (ms > 0) {
    m_timer = new wxTimer(this, havenID_ABOUTTIMER);
    m_timer->Start(ms, wxTIMER_ONE_SHOT);
  }

  SetTitle(_("About Haven"));

  wxGridSizer *aboutInfo = new wxGridSizer(2, 0, 2);
  aboutInfo->Add(new wxStaticText(this, wxID_ANY, _("Written By: ")), 0, wxALIGN_LEFT);
  aboutInfo->Add(new wxStaticText(this, wxID_ANY, wxT("Joey Hops")), 1, wxEXPAND | wxALIGN_LEFT);
  aboutInfo->Add(new wxStaticText(this, wxID_ANY, _("Version ")), 0, wxALIGN_LEFT);
  aboutInfo->Add(new wxStaticText(this, wxID_ANY, wxT("0.0.1")), 1, wxEXPAND | wxALIGN_LEFT);
  aboutInfo->Add(new wxStaticText(this, wxID_ANY, _("License: ")), 0, wxALIGN_LEFT);
  aboutInfo->Add(new wxStaticText(this, wxID_ANY, wxT("MIT")), 1, wxEXPAND | wxALIGN_LEFT);
  aboutInfo->Add(new wxStaticText(this, wxID_ANY, _("Copyright: ")), 0, wxALIGN_LEFT);
  aboutInfo->Add(new wxStaticText(this, wxID_ANY, wxT("Joey Hop Corps Inc. 2020")), 1, wxEXPAND | wxALIGN_LEFT);

  wxBoxSizer *aboutPane = new wxBoxSizer(wxHORIZONTAL);
  wxBitmap bmp = wxBitmap(wxICON(sample));
  //aboutPane->Add(new wxStaticBitmap(this, wxID_ANY, bmp), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxLeft | wxRight, 20);
  aboutPane->Add(aboutInfo, 1, wxEXPAND);
  aboutPane->Add(60, 0);

  wxBoxSizer *overallPane = new wxBoxSizer(wxVERTICAL);
  overallPane->Add(0, 20);
  wxStaticText *appName = new wxStaticText(this, wxID_ANY, *g_appname);
  overallPane->Add(appName, 0, wxALIGN_CENTER | wxLeft | wxRight, 40);
  overallPane->Add(0, 10);
  overallPane->Add(aboutPane, 0, wxEXPAND | wxALL, 4);
  overallPane->Add(new wxStaticText(this, wxID_ANY, wxT("Hopefully this editor works like I want it to")), 0, wxALIGN_CENTER | wxALL, 10);
  wxButton *okayBtn = new wxButton(this, wxID_OK, _("OK"));
  okayBtn->SetDefault();
  overallPane->Add(okayBtn, 0, wxALIGN_CENTER | wxLeft | wxRight | wxBottom, 10);

  SetSizerAndFit(overallPane);

  CenterOnScreen();
  ShowModal();
}

HavenAbout::~HavenAbout() { wxDELETE(m_timer); }

void HavenAbout::OnTimerEvent(wxTimerEvent &WXUNUSED(event)) {
  wxDELETE(m_timer);
  EndModal(wxID_OK);
}

class MinimalEditor : public wxStyledTextCtrl {
  enum {
    margin_id_lineno,
    margin_id_fold
  };

public:
  MinimalEditor(wxWindow* parent, wxWindowID id = wxID_ANY)
    : wxStyledTextCtrl(parent, id)
  {
    SetLexerXml();

    SetProperty(wxT("fold"), wxT("1"));
    SetProperty(wxT("fold.comment"), wxT("1"));
    SetProperty(wxT("fold.compact"), wxT("1"));
    SetProperty(wxT("fold.preprocessor"), wxT("1"));
    SetProperty(wxT("fold.html"), wxT("1"));
    SetProperty(wxT("fold.html.preprocessor"), wxT("1"));

    SetMarginType(margin_id_lineno, wxSTC_MARGIN_NUMBER);
    SetMarginWidth(margin_id_lineno, 32);

    MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS, wxT("WHITE"), wxT("BLACK"));
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS, wxT("WHITE"), wxT("BLACK"));
    MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE, wxT("WHITE"), wxT("BLACK"));
    MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUSCONNECTED, wxT("WHITE"), wxT("BLACK"));
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUSCONNECTED, wxT("WHITE"), wxT("BLACK"));
    MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER, wxT("WHITE"), wxT("BLACK"));
    MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER, wxT("WHITE"), wxT("BLACK"));

    SetMarginMask(margin_id_fold, wxSTC_MASK_FOLDERS);
    SetMarginWidth(margin_id_fold, 32);
    SetMarginSensitive(margin_id_fold, true);

    SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);

    SetTabWidth(2);
    SetUseTabs(false);
    SetWrapMode(wxSTC_WRAP_WORD);
    SetWrapVisualFlags(wxSTC_WRAPVISUALFLAG_END);
  }

  virtual bool SetFont(const wxFont& font) {
    StyleSetFont(wxSTC_STYLE_DEFAULT, (wxFont&)font);
    return wxStyledTextCtrl::SetFont(font);
  }

  void SetLexerXml() {
    SetLexer(wxSTC_LEX_XML);
    StyleSetForeground(wxSTC_H_DEFAULT, *wxBLACK);
    StyleSetForeground(wxSTC_H_TAG, *wxBLUE);
    StyleSetForeground(wxSTC_H_TAGUNKNOWN, *wxBLUE);
    StyleSetForeground(wxSTC_H_ATTRIBUTE, *wxRED);
    StyleSetForeground(wxSTC_H_ATTRIBUTEUNKNOWN, *wxRED);
    StyleSetBold(wxSTC_H_ATTRIBUTEUNKNOWN, true);
    StyleSetForeground(wxSTC_H_NUMBER, *wxBLACK);
    StyleSetForeground(wxSTC_H_DOUBLESTRING, *wxBLACK);
    StyleSetForeground(wxSTC_H_SINGLESTRING, *wxBLACK);
    StyleSetForeground(wxSTC_H_OTHER, *wxBLUE);
    StyleSetForeground(wxSTC_H_COMMENT, wxTheColourDatabase->Find(wxT("GREY")));
    StyleSetForeground(wxSTC_H_ENTITY, *wxRED);
    StyleSetBold(wxSTC_H_ENTITY, true);
    StyleSetForeground(wxSTC_H_TAGEND, *wxBLUE);
    StyleSetForeground(wxSTC_H_XMLSTART, *wxBLUE);
    StyleSetForeground(wxSTC_H_XMLEND, *wxBLUE);
    StyleSetForeground(wxSTC_H_CDATA, *wxRED);
  }

protected:
  void OnMarginClick(wxStyledTextEvent&);
  void OnText(wxStyledTextEvent&);;
  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MinimalEditor, wxStyledTextCtrl)
  EVT_STC_MARGINCLICK(wxID_ANY, MinimalEditor::OnMarginClick)
  EVT_STC_CHANGE(wxID_ANY, MinimalEditor::OnText)
wxEND_EVENT_TABLE()

void MinimalEditor::OnMarginClick(wxStyledTextEvent &event) {
  if (event.GetMargin() == margin_id_fold) {
    int lineClick = LineFromPosition(event.GetPosition());
    int levelClick = GetFoldLevel(lineClick);
    if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0) {
      ToggleFold(lineClick);
    }
  }
}

void MinimalEditor::OnText(wxStyledTextEvent& event) {
  wxLogDebug(wxT("Modified"));
  event.Skip();
}

class HavenFrameMin : public wxFrame {
public:
  HavenFrameMin()
    : wxFrame(NULL, wxID_ANY, _("Minimal Haven"))
  {
    MinimalEditor* editor = new MinimalEditor(this);
    editor->SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT));
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(editor, 1, wxEXPAND);
    SetSizer(sizer);
    editor->SetText(
                    "<xml>\n"
                    "  <text>\n"
                    "    This is an XML Example\n"
                    "  </text>\n"
                    "</xml>"
                    );
  }
};

wxFrame* HavenApp::MinimalEditor() {
  HavenFrameMin* frame = new HavenFrameMin;
  frame->Show();
  return frame;
}

void HavenApp::OnMinimalEditor(wxCommandEvent& WXUNUSED(event)) {
  MinimalEditor();
}
