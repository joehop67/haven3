#include <wx/wxprec.h>

#ifndef WX_PRECOMP
  #include <wx/wx.h>
  #include <wx/textdlg.h>
#endif // WX_PRECOMP

#include <wx/config.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/filedlg.h>

#include "../include/defs.h"
#include "../include/edit.h"

using namespace Haven;
using namespace Haven::LanguageMeta;

const int ANNOTATION_STYLE = wxSTC_STYLE_LASTPREDEFINED + 1;

wxBEGIN_EVENT_TABLE(Edit, wxStyledTextCtrl)
  EVT_SIZE(Haven::Edit::OnSize)
  EVT_MENU(wxID_CLEAR, Haven::Edit::onEditClear)
  EVT_MENU(wxID_CUT, Haven::Edit::onEditCut)
  EVT_MENU(wxID_COPY, Haven::Edit::onEditCopy)
  EVT_MENU(wxID_PASTE, Haven::Edit::onEditPaste)
  EVT_MENU(havenID_INDENTINC, Haven::Edit::onEditIndentInc)
  EVT_MENU(havenID_INDENTRED, Haven::Edit::onEditIndentRed)
  EVT_MENU(wxID_SELECTALL, Haven::Edit::onEditSelectAll)
  EVT_MENU(havenID_SELECTLINE, Haven::Edit::onEditSelectLine)
  EVT_MENU(wxID_REDO, Haven::Edit::onEditRedo)
  EVT_MENU(wxID_UNDO, Haven::Edit::onEditUndo)
  //Find Events
  EVT_MENU(wxID_FIND, Haven::Edit::onFind)
  EVT_MENU(havenID_FINDNEXT, Haven::Edit::onFindNext)
  EVT_MENU(havenID_REPLACE, Haven::Edit::onReplace)
  EVT_MENU(havenID_REPLACENEXT, Haven::Edit::onReplaceNext)
  EVT_MENU(havenID_BRACEMATCH, Haven::Edit::onBraceMatch)
  EVT_MENU(havenID_GOTO, Haven::Edit::onGoto)
  // View Events
  EVT_MENU_RANGE(havenID_HILIGHTFIRST, havenID_HILIGHTLAST, Haven::Edit::onHighlightLang)
  EVT_MENU(havenID_DISPLAYEOL, Haven::Edit::onDisplayEOL)
  EVT_MENU(havenID_INDENTGUIDE, Haven::Edit::onIndentGuide)
  EVT_MENU(havenID_LINENUMBER, Haven::Edit::onLineNumber)
  EVT_MENU(havenID_LONGLINEON, Haven::Edit::onLongLineOn)
  EVT_MENU(havenID_WHITESPACE, Haven::Edit::onWhiteSpace)
  EVT_MENU(havenID_FOLDTOGGLE, Haven::Edit::onFoldToggle)
  EVT_MENU(havenID_OVERTYPE, Haven::Edit::onSetOverType)
  EVT_MENU(havenID_READONLY, Haven::Edit::onSetReadOnly)
  EVT_MENU(havenID_WRAPMODEON, Haven::Edit::onWrapModeOn)
  EVT_MENU(havenID_CHARSETANSI, Haven::Edit::onUseCharset)
  EVT_MENU(havenID_CHARSETMAC, Haven::Edit::onUseCharset)
  // Annotation Events
  EVT_MENU(havenID_ANNOTATION_ADD, Haven::Edit::onAnnotationAdd)
  EVT_MENU(havenID_ANNOTATION_REMOVE, Haven::Edit::onAnnotationRemove)
  EVT_MENU(havenID_ANNOTATION_CLEAR, Haven::Edit::onAnnotationClear)
  EVT_MENU(havenID_ANNOTATION_STYLE_HIDDEN, Haven::Edit::onAnnotationStyle)
  EVT_MENU(havenID_ANNOTATION_STYLE_STANDARD, Haven::Edit::onAnnotationStyle)
  EVT_MENU(havenID_ANNOTATION_STYLE_BOXED, Haven::Edit::onAnnotationStyle)
  // Extra Events
  EVT_MENU(havenID_CHANGELOWER, Haven::Edit::onChangeCase)
  EVT_MENU(havenID_CHANGEUPPER, Haven::Edit::onChangeCase)
  EVT_MENU(havenID_CONVERTCR, Haven::Edit::onConvertEOL)
  EVT_MENU(havenID_CONVERTCRLF, Haven::Edit::onConvertEOL)
  EVT_MENU(havenID_CONVERTLF, Haven::Edit::onConvertEOL)
  // STC EVENTS
  EVT_STC_MARGINCLICK(wxID_ANY, Haven::Edit::onMarginClick)
  EVT_STC_CHARADDED(wxID_ANY, Haven::Edit::onCharAdded)
  EVT_STC_KEY(wxID_ANY, Haven::Edit::onKey)

  EVT_KEY_DOWN(Haven::Edit::OnKeyDown)
wxEND_EVENT_TABLE()

Edit::Edit (wxWindow *parent, LanguageDB *langDb, StyleDB *styleDb, wxWindowID id, const wxPoint &pos, const wxSize &size, long style)
  : wxStyledTextCtrl(parent, id, pos, size, style)
  {
    wxConfigBase *havenConf = wxConfigBase::Get(false);
    havenConf->SetPath("/Editor");
    confLL = havenConf->ReadBool("ShowLineNumbers", true);
    confCF = havenConf->ReadBool("EnabledCodeFolding", true);
    m_filename = wxEmptyString;

    languageDB = langDb;
    styleDB = styleDb;

    m_LineNrID = 0;
    m_DividerID = 1;
    m_FoldingID = 2;

    curLanguage = NULL;

    SetStyleBits(8);

    SetViewEOL(g_CommonPrefs.displayEOLEnabled);
    SetIndentationGuides(g_CommonPrefs.indentGuideEnabled);
    SetEdgeMode(g_CommonPrefs.longLineOnEnabled ? wxSTC_EDGE_LINE : wxSTC_EDGE_NONE);
    SetViewWhiteSpace(g_CommonPrefs.whiteSpaceEnabled ? wxSTC_WS_VISIBLEALWAYS : wxSTC_WS_INVISIBLE);
    SetOvertype(g_CommonPrefs.overTypeInitial);
    SetReadOnly(g_CommonPrefs.readOnlyInitial);
    SetWrapMode(g_CommonPrefs.wrapModeInitial ? wxSTC_WRAP_WORD : wxSTC_WRAP_NONE);
    wxFont font(10, wxMODERN, wxNORMAL, wxNORMAL);
    StyleSetFont(wxSTC_STYLE_DEFAULT, font);
    StyleSetForeground(wxSTC_STYLE_DEFAULT, *wxBLACK);
    StyleSetBackground(wxSTC_STYLE_DEFAULT, *wxWHITE);
    StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColor(wxT("DARK GREY")));
    StyleSetBackground(wxSTC_STYLE_LINENUMBER, *wxWHITE);
    StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColor(wxT("DARK GREY")));
    InitializePrefs(DEFAULT_LANGUAGE, "any");

    SetVisiblePolicy(wxSTC_VISIBLE_STRICT|wxSTC_VISIBLE_SLOP, 1);
    SetXCaretPolicy(wxSTC_CARET_EVEN|wxSTC_VISIBLE_STRICT|wxSTC_VISIBLE_SLOP, 1);
    SetYCaretPolicy(wxSTC_CARET_EVEN|wxSTC_VISIBLE_STRICT|wxSTC_VISIBLE_SLOP, 1);

    MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_DOTDOTDOT, wxT("BLACK"), wxT("BLACK"));
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN, wxT("BLACK"), wxT("BLACK"));
    MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY, wxT("BLACK"), wxT("BLACK"));
    MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_DOTDOTDOT, wxT("BLACK"), wxT("WHITE"));
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN, wxT("BLACK"), wxT("WHITE"));
    MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY, wxT("BLACK"), wxT("BLACK"));
    MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY, wxT("BLACK"), wxT("BLACK"));

    AnnotationSetVisible(wxSTC_ANNOTATION_BOXED);

    m_LineNrMargin = TextWidth(wxSTC_STYLE_LINENUMBER, "_999999");
    m_FoldingMargin = 16;
    CmdKeyClear(wxSTC_KEY_TAB, 0);
    SetLayoutCache(wxSTC_CACHE_PAGE);
  }

  Edit::~Edit() {}

  void Edit::OnSize(wxSizeEvent& event) {
    int x = GetClientSize().x + (g_CommonPrefs.lineNumberEnabled ? m_LineNrMargin : 0) + (g_CommonPrefs.foldEnable ? m_FoldingMargin : 0);
    if (x > 0) SetScrollWidth(x);
    event.Skip();
  }

  void Edit::onEditRedo(wxCommandEvent &WXUNUSED(event)) {
    if (!CanRedo()) return;
    Redo();
  }

  void Edit::onEditUndo(wxCommandEvent &WXUNUSED(event)) {
    if (!CanUndo()) return;
    Undo();
  }

  void Edit::onEditClear(wxCommandEvent &WXUNUSED(event)) {
    if (GetReadOnly()) return;
    Clear();
  }

  void Edit::onKey(wxStyledTextEvent &WXUNUSED(event)) {
    wxMessageBox("onKey");
  }

  void Edit::OnKeyDown(wxKeyEvent &event) {
    if (CallTipActive())
      CallTipCancel();
    if (event.GetKeyCode() == WXK_SPACE && event.ControlDown() && event.ShiftDown()) {
      int pos = GetCurrentPos();
      CallTipSetBackground(*wxYELLOW);
      CallTipShow(pos, "This is a CallTip with multiple lines\n" "It is meant to be blah blah blah.");
      return;
    }
    event.Skip();
  }

  void Edit::onEditCut(wxCommandEvent &WXUNUSED(event)) {
    if (GetReadOnly() || (GetSelectionEnd() - GetSelectionStart() <= 0)) return;
    Cut();
  }

  void Edit::onEditCopy(wxCommandEvent &WXUNUSED(event)) {
    if (GetSelectionEnd() - GetSelectionStart() <= 0) return;
    Copy();
  }

  void Edit::onEditPaste(wxCommandEvent &WXUNUSED(event)) {
    if(!CanPaste()) return;
    Paste();
  }

  void Edit::onFind(wxCommandEvent &WXUNUSED(event)) {}
  void Edit::onFindNext(wxCommandEvent &WXUNUSED(event)) {}
  void Edit::onReplace(wxCommandEvent &WXUNUSED(event)) {}
  void Edit::onReplaceNext(wxCommandEvent &WXUNUSED(event)) {}

  void Edit::onBraceMatch(wxCommandEvent &WXUNUSED(event)) {
    int min = GetCurrentPos();
    int max = BraceMatch(min);
    if (max > (min + 1)) {
      BraceHighlight(min + 1, max);
      SetSelection(min + 1, max);
    } else {
      BraceBadLight(min);
    }
  }

  void Edit::onGoto(wxCommandEvent &WXUNUSED(event)) {}

  void Edit::onEditIndentInc(wxCommandEvent &WXUNUSED(event)) {
    CmdKeyExecute(wxSTC_CMD_TAB);
  }

  void Edit::onEditIndentRed(wxCommandEvent &WXUNUSED(event)) {
    CmdKeyExecute(wxSTC_CMD_DELETEBACK);
  }

  void Edit::onEditSelectAll(wxCommandEvent &WXUNUSED(event)) {
    SetSelection(0, GetTextLength());
  }

  void Edit::onEditSelectLine(wxCommandEvent &WXUNUSED(event)) {
    int lineStart = PositionFromLine(GetCurrentLine());
    int lineEnd = PositionFromLine(GetCurrentLine() + 1);
    SetSelection(lineStart, lineEnd);
  }

  void Edit::onHighlightLang(wxCommandEvent &event) {
    InitializePrefs(languageDB->GetLanguageByExtension("cpp")->GetName(), "cpp");
  }

  void Edit::onDisplayEOL(wxCommandEvent &WXUNUSED(event)) {
    SetViewEOL(!GetViewEOL());
  }

  void Edit::onIndentGuide(wxCommandEvent &WXUNUSED(event)) {
    SetIndentationGuides(!GetIndentationGuides());
  }

  void Edit::onLineNumber(wxCommandEvent &WXUNUSED(event)) {
    SetMarginWidth(m_LineNrID, GetMarginWidth(m_LineNrID) == 0 ? m_LineNrMargin : 0);
  }

  void Edit::onLongLineOn(wxCommandEvent &WXUNUSED(event)) {
    SetEdgeMode(GetEdgeMode() == 0 ? wxSTC_EDGE_LINE : wxSTC_EDGE_NONE);
  }

  void Edit::onWhiteSpace(wxCommandEvent &WXUNUSED(event)) {
    SetViewWhiteSpace(GetViewWhiteSpace() == 0 ? wxSTC_WS_VISIBLEALWAYS : wxSTC_WS_INVISIBLE);
  }

  void Edit::onFoldToggle(wxCommandEvent &WXUNUSED(event)) {
    ToggleFold(GetFoldParent(GetCurrentLine()));
  }

  void Edit::onSetOverType(wxCommandEvent &WXUNUSED(event)) {
    SetOvertype(!GetOvertype());
  }

  void Edit::onSetReadOnly(wxCommandEvent &WXUNUSED(event)) {
    SetReadOnly(!GetReadOnly());
  }

  void Edit::onWrapModeOn(wxCommandEvent &WXUNUSED(event)) {
    SetWrapMode(GetWrapMode() == 0 ? wxSTC_WRAP_WORD : wxSTC_WRAP_NONE);
  }

  void Edit::onUseCharset(wxCommandEvent &event) {
    int Nr;
    int charset = GetCodePage();
    switch (event.GetId()) {
      case havenID_CHARSETANSI: {
        charset = wxSTC_CHARSET_ANSI;
        break;
      }
      case havenID_CHARSETMAC: {
        charset = wxSTC_CHARSET_ANSI;
        break;
      }
    }
    for (Nr = 0; Nr < wxSTC_STYLE_LASTPREDEFINED; Nr++) {
      StyleSetCharacterSet(Nr, charset);
    }
    SetCodePage(charset);
  }

  void Edit::onAnnotationAdd(wxCommandEvent &WXUNUSED(event)) {
    const int line = GetCurrentLine();

    wxString annotation = AnnotationGetText(line);
    annotation = wxGetTextFromUser(wxString::Format("Enter annotation for line &d", line), "Edit Annotation", annotation, this);

    if (annotation.empty())
      return;

    AnnotationSetText(line, annotation);
    AnnotationSetStyle(line, ANNOTATION_STYLE);

    const int width = GetScrollWidth();
    int indent = GetLineIndentation(line);
    indent += 3;

    const int widthAnn = TextWidth(ANNOTATION_STYLE, annotation + wxString(indent, ' '));

    if (widthAnn > width)
      SetScrollWidth(widthAnn);
  }

  void Edit::onAnnotationRemove(wxCommandEvent &WXUNUSED(event)) {
    AnnotationSetText(GetCurrentLine(), wxString());
  }

  void Edit::onAnnotationClear(wxCommandEvent &WXUNUSED(event)) {
    AnnotationClearAll();
  }

  void Edit::onAnnotationStyle(wxCommandEvent &event) {
    int style = 0;
    switch (event.GetId()) {
    case havenID_ANNOTATION_STYLE_HIDDEN:
      style = wxSTC_ANNOTATION_HIDDEN;
      break;
    case havenID_ANNOTATION_STYLE_STANDARD:
      style = wxSTC_ANNOTATION_STANDARD;
      break;
    case havenID_ANNOTATION_STYLE_BOXED:
      style = wxSTC_ANNOTATION_BOXED;
      break;
    }
    AnnotationSetVisible(style);
  }

  void Edit::onChangeCase(wxCommandEvent &event) {
    switch(event.GetId()) {
      case havenID_CHANGELOWER: {
        CmdKeyExecute(wxSTC_CMD_LOWERCASE);
        break;
      }
      case havenID_CHANGEUPPER: {
        CmdKeyExecute(wxSTC_CMD_UPPERCASE);
        break;
      }
    }
  }

  void Edit::onConvertEOL(wxCommandEvent &event) {
    int eolMode = GetEOLMode();
    switch(event.GetId()) {
      case havenID_CONVERTCR: {
        eolMode = wxSTC_EOL_CR;
        break;
      }
      case havenID_CONVERTCRLF: {
        eolMode = wxSTC_EOL_CRLF;
        break;
      }
      case havenID_CONVERTLF: {
        eolMode = wxSTC_EOL_LF;
        break;
      }
    }
    ConvertEOLs(eolMode);
    SetEOLMode(eolMode);
  }

  void Edit::onMarginClick(wxStyledTextEvent &event) {
    if (event.GetMargin() == 2) {
      int lineClick = LineFromPosition(event.GetPosition());
      int levelClick = GetFoldLevel(lineClick);
      if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0) {
        ToggleFold(lineClick);
      }
    }
  }

  void Edit::onCharAdded(wxStyledTextEvent &event) {
    char chr = (char)event.GetKey();
    int currentLine = GetCurrentLine();
    if (chr == '\n') {
      int lineInd = 0;
      if (currentLine > 0) {
        lineInd = GetLineIndentation(currentLine - 1);
      }
      if (lineInd == 0) return;
      SetLineIndentation(currentLine, lineInd);
      GotoPos(PositionFromLine(currentLine) + lineInd);
    }
  }

  wxString Edit::DeterminePrefs(const wxString &filename) {
    Haven::LanguageInfo const* curInfo;

    int languageNum;
    for (languageNum = 0; languageNum < Haven::g_LanguagePrefsSize; languageNum++) {
      curInfo = &Haven::g_LanguagePrefs[languageNum];
      wxString filepattern = curInfo->filepattern;
      filepattern.Lower();
      while(!filepattern.empty()) {
        wxString cur = filepattern.BeforeFirst(';');
        if ((cur == filename) || (cur == (filename.BeforeLast('.') + wxT(".*"))) || (cur == (wxT("*.") + filename.AfterLast('.')))) {
          return curInfo->name;
        }
        filepattern = filepattern.AfterFirst(';');
      }
    }
    return wxEmptyString;
  }

  bool Edit::InitializePrefs(const wxString &name, const wxString &ext) {
    StyleClearAll();

   // Haven::LanguageInfo const* curInfo = NULL;

    Language *curLang = languageDB->GetLanguageByExtension(ext);
    curLanguage = curLang;

    bool found = false;
    int languageNum;

    //if (!found) return false;

    SetLexer(curLang->GetLexer());
    //curLanguage = curLang;

    SetMarginType(m_LineNrID, wxSTC_MARGIN_NUMBER);
    StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColor(wxT("DARK GREY")));
    StyleSetBackground(wxSTC_STYLE_LINENUMBER, *wxWHITE);
    SetMarginWidth(m_LineNrID, 0);

    StyleSetBackground(ANNOTATION_STYLE, wxColor(244, 220, 220));
    StyleSetForeground(ANNOTATION_STYLE, *wxBLACK);
    StyleSetSizeFractional(ANNOTATION_STYLE, (StyleGetSizeFractional(wxSTC_STYLE_DEFAULT) * 4) / 5);

    int num;
    for (num = 0; num < wxSTC_STYLE_LASTPREDEFINED; num++) {
      wxFont font(10, wxMODERN, wxNORMAL, wxNORMAL);
      StyleSetFont(num, font);
    }

    StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor(wxT("DARK GREY")));
    StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColor(wxT("DARK GREY")));

    // TODO Switch to config system
    if (Haven::g_CommonPrefs.syntaxEnable && strcmp(curLang->GetName(), "<default>") != 0) {
      int keywordNum = 0;
      for (num = 0; num < curLanguage->GetStyleSize(); num++) {
        if (curLanguage->GetStyles()[num].sType <= -1 || curLanguage->GetStyles()[num].sType > STYLE_TYPES_COUNT) continue;
        //const Haven::StyleInfo &curType = Haven::g_StylePrefs[curInfo->styles[num].type];
        StyleDef curType = styleDB->At(curLanguage->GetStyles()[num].sType);
        wxFont font(curType.fontSize, wxMODERN, wxNORMAL, wxNORMAL, false, curType.fontname);
        StyleSetFont(num, font);
        if (curType.foreground) {
          StyleSetForeground(num, wxColor(curType.foreground));
        }
        if (curType.background) {
          StyleSetBackground(num, wxColor(curType.background));
        }
        StyleSetBold(num, (curType.fontStyle & havenSTC_STYLE_BOLD) > 0);
        StyleSetItalic(num, (curType.fontStyle & havenSTC_STYLE_ITALIC) > 0);
        StyleSetUnderline(num, (curType.fontStyle & havenSTC_STYLE_UNDERL) > 0);
        StyleSetVisible(num, (curType.fontStyle & havenSTC_STYLE_HIDDEN) == 0);
        StyleSetCase(num, curType.letterCase);
        const char *pwords = curLanguage->GetStyles()[num].keyWords;
        if (pwords) {
          SetKeyWords(keywordNum, pwords);
          keywordNum += 1;
        }
      }
    }

    SetMarginType(m_DividerID, wxSTC_MARGIN_SYMBOL);
    SetMarginWidth(m_DividerID, 0);
    SetMarginSensitive(m_DividerID, false);

    SetMarginType(m_FoldingID, wxSTC_MARGIN_SYMBOL);
    SetMarginMask(m_FoldingID, wxSTC_MASK_FOLDERS);
    StyleSetBackground(m_FoldingID, *wxWHITE);
    SetMarginWidth(m_FoldingID, 0);
    SetMarginSensitive(m_FoldingID, false);
    if (confCF) {
      SetMarginWidth(m_FoldingID, curLanguage->GetFolds() != 0 ? m_FoldingMargin : 0);
      SetMarginSensitive(m_FoldingID, curLanguage->GetFolds() != 0);
      SetProperty(wxT("fold"), curLanguage->GetFolds() != 0 ? wxT("1") : wxT("0"));
      SetProperty(wxT("fold.comment"), (curLanguage->GetFolds() & havenSTC_FOLD_COMMENT) > 0 ? wxT("1") : wxT("0"));
      SetProperty(wxT("fold.compact"), (curLanguage->GetFolds() & havenSTC_FOLD_COMPACT) > 0 ? wxT("1") : wxT("0"));
      SetProperty(wxT("fold.preprocessor"), (curLanguage->GetFolds() & havenSTC_FOLD_PREPROC) > 0 ? wxT("1") : wxT("0"));
      SetProperty(wxT("fold.html"), (curLanguage->GetFolds() & havenSTC_FOLD_HTML) > 0 ? wxT("1") : wxT("0"));
      SetProperty(wxT("fold.html.preprocessor"), (curLanguage->GetFolds() & havenSTC_FOLD_HTMLPREP) > 0 ? wxT("1") : wxT("0"));
    }
    SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);

    SetTabWidth(2);
    SetUseTabs(false);
    SetTabIndents(true);
    SetBackSpaceUnIndents(true);
    SetIndent(Haven::g_CommonPrefs.indentEnable ? 2 : 0);

    SetViewEOL(Haven::g_CommonPrefs.displayEOLEnabled);
    SetIndentationGuides(Haven::g_CommonPrefs.indentGuideEnabled);
    SetEdgeColumn(80);
    SetEdgeMode(Haven::g_CommonPrefs.longLineOnEnabled ? wxSTC_EDGE_LINE : wxSTC_EDGE_NONE);
    SetViewWhiteSpace(Haven::g_CommonPrefs.whiteSpaceEnabled ? wxSTC_WS_VISIBLEALWAYS : wxSTC_WS_INVISIBLE);
    SetOvertype(Haven::g_CommonPrefs.overTypeInitial);
    SetReadOnly(Haven::g_CommonPrefs.readOnlyInitial);
    SetWrapMode(Haven::g_CommonPrefs.wrapModeInitial ? wxSTC_WRAP_WORD : wxSTC_WRAP_NONE);

    if (confLL) {
      SetMarginWidth(m_LineNrID, GetMarginWidth(m_LineNrID) == 0 ? m_LineNrMargin : 0);
    }

    return true;
  }

  bool Edit::LoadFile() {
    if (!m_filename) {
      wxFileDialog dialog(this, wxT("Open File"), wxEmptyString, wxEmptyString, wxT("Any File (*)|*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);
      if (dialog.ShowModal() != wxID_OK) return false;
      m_filename = dialog.GetPath();
    }
    return LoadFile(m_filename);
  }

  bool Edit::LoadFile(const wxString &filename) {
    if (!filename.empty()) m_filename = filename;
    wxString *ext = new wxString();
    wxFileName::SplitPath(filename, NULL, NULL, ext);
    wxFileName fname (m_filename);

    curLanguage = languageDB->GetLanguageByExtension(ext->ToAscii());
    InitializePrefs(DeterminePrefs(fname.GetFullName()), ext->ToAscii());

    wxStyledTextCtrl::LoadFile(m_filename);

    EmptyUndoBuffer();


    return true;
  }

  bool Edit::SaveFile() {
    if (!Modified()) return true;

    if (!m_filename) {
      wxFileDialog dialog(this, wxT("Save File"), wxEmptyString, wxEmptyString, wxT("Any File (*)|*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
      if (dialog.ShowModal() != wxID_OK) return false;
      m_filename = dialog.GetPath();
    }

    return SaveFile(m_filename);
  }

  bool Edit::SaveFile(const wxString &filename) {
    if (!Modified()) return true;

    return wxStyledTextCtrl::SaveFile(filename);
  }

  bool Edit::Modified() {
    return (GetModify() && !GetReadOnly());
  }

  EditProperties::EditProperties(Haven::Edit *edit, long style)
    : wxDialog(edit, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, style | wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
    {
      SetTitle(_("Properties"));
      wxString text;

      wxBoxSizer *fullname = new wxBoxSizer(wxHORIZONTAL);
      fullname->Add(10, 0);
      fullname->Add(new wxStaticText(this, wxID_ANY, _("Full filename"), wxDefaultPosition, wxSize(80, wxDefaultCoord)), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
      fullname->Add(new wxStaticText(this, wxID_ANY, edit->GetFilename()), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
      wxGridSizer *textInfo = new wxGridSizer(4, 0, 2);
      textInfo->Add(new wxStaticText(this, wxID_ANY, _("Language"), wxDefaultPosition, wxSize(80, wxDefaultCoord)), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxLeft, 4);
      textInfo->Add(new wxStaticText(this, wxID_ANY, edit->curLanguage->GetName()), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxRight, 4);
      textInfo->Add(new wxStaticText(this, wxID_ANY, _("Lexer-ID: "), wxDefaultPosition, wxSize(80, wxDefaultCoord)), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxLeft, 4);
      text = wxString::Format(wxT("%d"), edit->GetLexer());
      textInfo->Add(new wxStaticText(this, wxID_ANY, text), 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxRight, 4);

      wxString EOLType = wxEmptyString;
      switch(edit->GetEOLMode()) {
        case wxSTC_EOL_CR: {
          EOLType = wxT("CR (Unix)");
          break;
        }
        case wxSTC_EOL_CRLF: {
          EOLType = wxT("CRLF (Windows)");
          break;
        }
        case wxSTC_EOL_LF: {
          EOLType = wxT("LF (Mac)");
          break;
        }
      }
      textInfo->Add(new wxStaticText(this, wxID_ANY, _("Line Endings"), wxDefaultPosition, wxSize(80, wxDefaultCoord)), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxLeft, 4);
      textInfo->Add(new wxStaticText(this, wxID_ANY, EOLType), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxRight, 4);
      wxStaticBoxSizer *textInfos = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Information")), wxVERTICAL);
      textInfos->Add(textInfo, 0, wxEXPAND);
      textInfos->Add(0, 6);

      wxGridSizer *statistics = new wxGridSizer(4, 0, 2);
      statistics->Add(new wxStaticText(this, wxID_ANY, _("Total Lines"), wxDefaultPosition, wxSize(80, wxDefaultCoord)), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxLeft, 4);
      text = wxString::Format(wxT("%d"), edit->GetLineCount());
      statistics->Add(new wxStaticText(this, wxID_ANY, text), 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxRight, 4);
      statistics->Add(new wxStaticText(this, wxID_ANY, _("Total Chars"), wxDefaultPosition, wxSize(80, wxDefaultCoord)), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxLeft, 4);
      text = wxString::Format(wxT("%d"), edit->GetTextLength());
      statistics->Add(new wxStaticText(this, wxID_ANY, text), 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxRight, 4);
      statistics->Add(new wxStaticText(this, wxID_ANY, _("Current Line"), wxDefaultPosition, wxSize(80, wxDefaultCoord)), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxLeft, 4);
      text = wxString::Format(wxT("%d"), edit->GetCurrentLine());
      statistics->Add(new wxStaticText(this, wxID_ANY, text), 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxRight, 4);
      statistics->Add(new wxStaticText(this, wxID_ANY, _("Current POS"), wxDefaultPosition, wxSize(80, wxDefaultCoord)), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxLeft, 4);
      text = wxString::Format(wxT("%d"), edit->GetCurrentPos());
      statistics->Add(new wxStaticText(this, wxID_ANY, text), 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxRight, 4);

      wxStaticBoxSizer *statistic = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Statistics")), wxVERTICAL);
      statistic->Add(statistics, 0, wxEXPAND);
      statistic->Add(0, 6);

      wxBoxSizer *totalPane = new wxBoxSizer(wxVERTICAL);
      totalPane->Add(fullname, 0, wxEXPAND | wxLeft | wxRight | wxTop, 10);
      totalPane->Add(0, 6);
      totalPane->Add(textInfos, 0, wxEXPAND | wxLeft | wxRight, 10);
      totalPane->Add(0, 10);
      totalPane->Add(statistic, 0, wxEXPAND | wxLeft | wxRight, 10);
      totalPane->Add(0, 6);
      wxButton *okButton = new wxButton(this, wxID_OK, _("OK"));
      okButton->SetDefault();
      totalPane->Add(okButton, 0, wxALIGN_CENTER | wxALL, 10);

      SetSizerAndFit(totalPane);

      ShowModal();
    }
