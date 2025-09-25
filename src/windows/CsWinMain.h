/*
  Project "Sali song editor for SaliSongBook"
  Author
    Sibilev Alexander S.

  Web
    www.saliLab.com
    www.saliLab.ru

  Description
    Main window of application
    Contains editor tabbed widget
*/
#ifndef CSWMAIN_H
#define CSWMAIN_H

#include <QMainWindow>
#include <QSplitter>
#include <QTabWidget>

class CsWinTextEditor;

class CsWinMain : public QMainWindow
  {
    Q_OBJECT

    QSplitter  *mCentralWidget;
    QTabWidget *mEditorTab;

  public:
    CsWinMain(QWidget *parent = nullptr);
    ~CsWinMain();

  public slots:
    void cmFileNew();
    void cmFileNewFromMp3();
    void cmFileStyleNew();
    void cmFileOpen();
    void cmFileSave();
    void cmFileSaveAs();
    void cmFileRecent();
    void cmFileClose();
    void cmFileCloseAll();

    void cmEditUndo();
    void cmEditRedo();
    void cmEditCopy();
    void cmEditCut();
    void cmEditPaste();
    void cmEditPasteSong2T();
    void cmEditPasteSong4T();
    void cmEditSelectAll();
    void cmEditSearchText();
    void cmEditReplaceText();
    void cmEditRepaint();

    void cmViewComposition();

    void cmPlayLine();
    void cmPlaySyncro();
    void cmPlayStop();
    void cmPlayAll();

    void cmHelpContext();
    void cmHelpContens();
    void cmHelpWeb();
    void cmHelpAbout();

    void fileOpenInternal( const QString &fname );

    //!
    //! \brief fileSaveIndex Save the file in the tab with the specified index
    //! \param tabIndex      index of tab
    //! \return              true when save successfull
    //!
    bool fileSaveIndex( int tabIndex );

    //!
    //! \brief fileSaveIndexAs Save the file in the tab with the specified index with different name
    //! \param tabIndex        index of tab
    //! \return                true when save successfull
    //!
    bool fileSaveIndexAs( int tabIndex );

    //!
    //! \brief tabNameUpdate Updates tab name with specified index of tab
    //! \param tabIndex      Index of tab
    //!
    void tabNameUpdate( int tabIndex );

    //!
    //! \brief fileCloseIndex Close the tab with the specified index
    //! \param tabIndex       Index of tab
    //!
    bool fileCloseIndex( int tabIndex );

    //!
    //! \brief onEditorIndex When changing the tab with the editor
    //! \param tabIndex      Index of tab
    //!
    void onEditorIndex( int tabIndex );

    //!
    //! \brief onUndoRedoChanged When changing undo, redo
    //!
    void onUndoRedoChanged( bool );

    //!
    //! \brief onSelectChanged When the selection changes
    //!
    void onSelectChanged(bool);

    //!
    //! \brief textChanged When text changed
    //!
    void textChanged();

    void updateCommand();


  private:

    //!
    //! \brief getEditor Returns editor with specified index
    //! \param tabIndex  Index of editor
    //! \return          Editor with specified index
    //!
    CsWinTextEditor *getEditor( int tabIndex );

    //!
    //! \brief getCurrentEditor Returns current text editor
    //! \return                 Current text editor
    //!
    CsWinTextEditor *getCurrentEditor();

    CsWinTextEditor *buildEditor( const QString &fname, const QString &text );

    void compile(bool link, bool flash, bool runOrPause );

    bool canClose();

    //!
    //! \brief setTitle Установить заголовок в соотвествии с проектом
    //!
    void setTitle();


    // QWidget interface
  protected:
    virtual void closeEvent(QCloseEvent *ev) override;

  };
#endif // CSWMAIN_H
