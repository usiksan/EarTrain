#include "CsSongEditorConfig.h"
#include "CsWinMain.h"
#include "CsWinCommand.h"
#include "CsWinTextEditor.h"
#include "CsDlgVoiceCalibr.h"
#include "CsHighlighter.h"
#include "score/CsPlayer.h"
#include "score/CsComposition.h"
#include "audioFile/CsSongBuilder.h"

#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileInfo>
#include <QFileDialog>
#include <QInputDialog>
#include <QCloseEvent>
#include <QSettings>
#include <QToolButton>
#include <QDesktopServices>
#include <QStatusBar>
#include <QPixmap>
#include <QActionGroup>
#include <QtQuickWidgets/QQuickWidget>
#include <QQmlEngine>
#include <QQmlContext>


CsWinMain::CsWinMain(QWidget *parent)
  : QMainWindow(parent)
  {
  //Создать меню
  QMenu *menu;

  menu = new QMenu( tr("Файл") );
  maFile = menuBar()->addMenu( menu );

  maFileNew = menu->addAction( QIcon(QStringLiteral(":/pic/fileNew.png")), tr("Новый..."), QString("Ctrl+N"), this, &CsWinMain::cmFileNew );
  maFileNew->setToolTip( tr("Создать новый файл программы") );
  maFileNewMp3 = menu->addAction( QIcon(QStringLiteral(":/pic/fileNewFromMp3.png")), tr("Новый из mp3..."), this, &CsWinMain::cmFileNewFromMp3 );
  maFileOpen = menu->addAction( QIcon(QStringLiteral(":/pic/fileOpen.png")), tr("Открыть..."), QString("Ctrl+O"), this, &CsWinMain::cmFileOpen );
  maFileOpen->setToolTip( tr("Открыть файл программы") );

  QMenu *recent = new QMenu( tr("Предыдущие") );
  maFileRecent = menu->addMenu( recent );
  maFileRecent->setEnabled(false);
  for( int i = 0; i < CS_MAX_RECENT_FILE; i++ ) {
    maRecentFile[i] = recent->addAction( QString(), this, &CsWinMain::cmFileRecent );
    maRecentFile[i]->setVisible(false);
    }

  menu->addSeparator();
  maFileSave = menu->addAction( QIcon(QStringLiteral(":/pic/fileSave.png")), tr("Сохранить"), QString("Ctrl+S"), this, &CsWinMain::cmFileSave );
  maFileSave->setToolTip( tr("Сохранить текущий файл программы") );
  maFileSaveAs = menu->addAction( QIcon(QStringLiteral(":/pic/fileSaveAs.png")), tr("Сохранить как..."), this, &CsWinMain::cmFileSaveAs );
  maFileClose = menu->addAction( QIcon(QStringLiteral(":/pic/closeFile.png")), tr("Закрыть"), this, &CsWinMain::cmFileClose );
  maFileCloseAll = menu->addAction( QIcon(QStringLiteral(":/pic/save_as.png")), tr("Закрыть все"), this, &CsWinMain::cmFileCloseAll );


  menu = new QMenu( tr("Правка") );
  maEdit = menuBar()->addMenu( menu );
  maEditUndo = menu->addAction( QIcon(QString(":/pic/editUndo.png")), tr("Отменить"), QString("Ctrl+Z"), this, &CsWinMain::cmEditUndo );
  maEditRedo = menu->addAction( QIcon(QString(":/pic/editRedo.png")), tr("Повторить"), QString("Ctrl+X"), this, &CsWinMain::cmEditRedo );
  menu->addSeparator();
  maEditCopy = menu->addAction( QIcon(QString(":/pic/editCopy.png")), tr("Копировать"), QString("Ctrl+Ins"), this, &CsWinMain::cmEditCopy );
  maEditCut  = menu->addAction( QIcon(QString(":/pic/editCut.png")),  tr("Вырезать"),  QString("Shift+Del"),  this, &CsWinMain::cmEditCut );
  maEditPaste = menu->addAction( QIcon(QString(":/pic/editPaste.png")),  tr("Вставить"),  QString("Shift+Ins"), this, &CsWinMain::cmEditPaste );
  maEditSelectAll = menu->addAction( QIcon(QString(":/pic/editSelectAll.png")),  tr("Выделить все"),  this, &CsWinMain::cmEditSelectAll );
  maEditSearch = menu->addAction( QIcon(QString(":/pic/editSearch.png")), tr("Поиск"), QString ("Ctrl+F"), this, &CsWinMain::cmEditSearchText );
  maEditReplace = menu->addAction( QIcon(QString(":/pic/editReplace.png")), tr("Замена"), QString ("Ctrl+R"), this, &CsWinMain::cmEditReplaceText );
  maEditAutoIndent = menu->addAction( QIcon(QString(":/pic/editIndent.png")), tr("Автоотступ"), QString ("F2"), this, &CsWinMain::cmEditRepaint );
  maEditAutoIndent->setCheckable(true);
  maEditAutoIndent->setChecked(true);
  maEditAutoSyncro = menu->addAction( QIcon(QString(":/pic/editSyncro.png")), tr("Синхронно"), QString ("F3"), this, &CsWinMain::cmEditRepaint );
  maEditAutoSyncro->setCheckable(true);
  menu->addSeparator();
  menu->addAction( tr("Вставить текст песни 2такта"), this, &CsWinMain::cmEditPasteSong2T );
  menu->addAction( tr("Вставить текст песни 4такта"), this, &CsWinMain::cmEditPasteSong4T );
  //maEditEditorSettings = menu->addAction( tr("Настройки редактора..."), this, &CsWinMain::cmEditAutoIndentSettings );

  menu = new QMenu( tr("Вид") );
  menuBar()->addMenu( menu );
  maViewComposition = menu->addAction( QIcon(QString(":/pic/playLine.png")), tr("Show composition"), this, &CsWinMain::cmViewComposition );

  menu = new QMenu( tr("Play") );
  menuBar()->addMenu( menu );
  maPlayLine = menu->addAction( QIcon(QString(":/pic/playLine.png")), tr("Play track"), QString("F4"), this, &CsWinMain::cmPlayLine );
  maPlaySync = menu->addAction( QIcon(QString(":/pic/playRun.png")), tr("Play line"), QString("F7"), this, &CsWinMain::cmPlaySyncro );
  maPlayAll  = menu->addAction( QIcon(QString(":/pic/playRun.png")), tr("Play All"), QString("F6"), this, &CsWinMain::cmPlayAll );
  maPlayStop = menu->addAction( QIcon(QString(":/pic/playStop.png")), tr("Play stop"), QString("F5"), this, &CsWinMain::cmPlayStop );

  menu = new QMenu( tr("Мелодия") );
  menuBar()->addMenu( menu );
  menu->addAction( tr("Калибровка..."), this, [this] () {
    CsDlgVoiceCalibr calibrDlg(this);
    calibrDlg.exec();
    } );


  QToolBar *bar = addToolBar( tr("Tools") );
  bar->addAction(maFileNew);
  bar->addAction(maFileOpen);
  bar->addAction(maFileSave);
  bar->addSeparator();
  bar->addAction(maEditUndo);
  bar->addAction(maEditRedo);
  bar->addSeparator();
  bar->addAction(maEditCopy);
  bar->addAction(maEditCut);
  bar->addAction(maEditPaste);
  bar->addSeparator();
  bar->addAction(maEditAutoIndent);
  bar->addAction(maEditAutoSyncro);
  bar->addSeparator();
  bar->addAction(maPlayLine);
  bar->addAction(maPlaySync);
  bar->addAction(maPlayStop);

  mCentralWidget = new QSplitter( Qt::Vertical, this );
  setCentralWidget( mCentralWidget );
  mEditorTab = new QTabWidget(this);
  mCentralWidget->addWidget( mEditorTab );

  QQuickWidget *qw = new QQuickWidget();
  mCentralWidget->addWidget( qw );

  mCentralWidget->setSizes( { 600, 400 } );

  //In Android default font has not musical symbols,
  // so we append in application musical font
  int id = QFontDatabase::addApplicationFont(":/ttf/NotoMusic-Regular.ttf");
  QString fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);
  qw->engine()->rootContext()->setContextProperty( "csComposition", CsComposition::instance() );
  qw->engine()->rootContext()->setContextProperty( "csPlayer", CsPlayer::instance() );
  qw->engine()->rootContext()->setContextProperty( "csMusicFont", fontFamily );

  qw->setResizeMode( QQuickWidget::SizeRootObjectToView );
  qw->setSource( QUrl(QStringLiteral("qrc:/qml/ScoreWidget.qml")) );



  //Fill recent files
  QSettings s;
  QStringList list;
  if( s.contains(CSS_RECENT_FILES) )
    //Получить список
    list = s.value(CSS_RECENT_FILES).toStringList();

  for( int i = 0; i < CS_MAX_RECENT_FILE; ++i )
    if( i < list.count() ) {
      QFileInfo info( list.at(i) );
      maRecentFile[i]->setText( info.completeBaseName() );
      maRecentFile[i]->setData( list.at(i) );
      maRecentFile[i]->setToolTip( list.at(i) );
      maRecentFile[i]->setVisible(true);
      }
    else maRecentFile[i]->setVisible(false);

  maFileRecent->setEnabled( list.count() != 0 );
  }

CsWinMain::~CsWinMain()
  {
  }






void CsWinMain::cmFileNew()
  {
  QString inc(
R"VVV(
Title:
Artist:
Composer:
Words:
Score creator:
Tempo: 120
Synthesizer program: 0
Synthesizer style: 0

//t: time line
//c: c0-c3: chord line A or '-' for lyric sync (A A# Am A#m A7 B Bm B7 C C# Cm C#m C7 and so on)
//n: n0-n9: b: p: nf: nf0-nf9: note line
//f: finger index
//g: guitar / \
//p: pad
//l: lyric line sync with chord or note line


//time: <start of line in 1/256 of takt>/<step count>
t:  /8   |    +    +    +    +    +    +    +    |    +    +    +    +    +    +    +
c:       Am
n:       d    e    f         e         e
l:

)VVV" );
  buildEditor( CS_NO_NAME, inc );
  }




void CsWinMain::cmFileNewFromMp3()
  {
  //Открыть файл mp3
  QString mp3name = QFileDialog::getOpenFileName( this, tr("Select mp3 file for song creating"), QString(),
                                                tr("MP3 files (*.mp3)") );
  if( !mp3name.isEmpty() ) {
    CsSongBuilder builder( mp3name );
    if( builder.build() ) {
      fileOpenInternal( builder.songPath() );
      }
    }
  }



void CsWinMain::cmFileStyleNew()
  {

  }





void CsWinMain::cmFileOpen()
  {
  QString fname = QFileDialog::getOpenFileName( this, tr("Select file for open"), QString(),
                                                tr("Song files SaliSong (*%1)").arg(CS_SONG_EXTENSION) );
  fileOpenInternal( fname );
  }




void CsWinMain::cmFileSave()
  {
  fileSaveIndex( mEditorTab->currentIndex() );
  }




void CsWinMain::cmFileSaveAs()
  {
  fileSaveIndexAs( mEditorTab->currentIndex() );
  }




void CsWinMain::cmFileRecent()
  {
  QAction *action = qobject_cast<QAction *>(sender());
  if(action) {
    fileOpenInternal( action->data().toString() );
    }
  }




void CsWinMain::cmFileClose()
  {
  fileCloseIndex( mEditorTab->currentIndex() );
  }




void CsWinMain::cmFileCloseAll()
  {
  for( int i = mEditorTab->count() - 1; i >= 0; i-- )
    if( !fileCloseIndex( i ) ) break;
  }




void CsWinMain::cmEditUndo()
  {
  CsWinTextEditor *editor = getCurrentEditor();
  if( editor ) {
    editor->undo();
    onEditorIndex( mEditorTab->currentIndex() );
    }
  }

void CsWinMain::cmEditRedo()
  {
  CsWinTextEditor *editor = getCurrentEditor();
  if( editor ) {
    editor->redo();
    //Обновить разрешение команд undo и redo
    maEditUndo->setEnabled( editor->document()->availableUndoSteps() );
    maEditRedo->setEnabled( editor->document()->availableRedoSteps() );
    }
  }



void CsWinMain::cmEditCopy()
  {
  CsWinTextEditor *editor = getCurrentEditor();
  if( editor )
    editor->copy();
  }




void CsWinMain::cmEditCut()
  {
  CsWinTextEditor *editor = getCurrentEditor();
  if( editor )
    editor->cut();
  }




void CsWinMain::cmEditPaste()
  {
  CsWinTextEditor *editor = getCurrentEditor();
  if( editor )
    editor->paste();
  }




void CsWinMain::cmEditPasteSong2T()
  {
  CsWinTextEditor *editor = getCurrentEditor();
  if( editor )
    editor->cmEditPasteSong( 4 );
  }




void CsWinMain::cmEditPasteSong4T()
  {
  CsWinTextEditor *editor = getCurrentEditor();
  if( editor )
    editor->cmEditPasteSong( 8 );
  }




void CsWinMain::cmEditSelectAll()
  {
  CsWinTextEditor *editor = getCurrentEditor();
  if( editor )
    editor->selectAll();
  }

void CsWinMain::cmEditSearchText()
  {

  }

void CsWinMain::cmEditReplaceText()
  {

  }

void CsWinMain::cmEditRepaint()
  {
  CsWinTextEditor *editor = getCurrentEditor();
  if( editor )
    editor->onCursorPositionChanged();
  }



void CsWinMain::cmViewComposition()
  {
  CsWinTextEditor *editor = getCurrentEditor();
  if( editor ) {
    CsComposition::instance()->songSet( editor->toPlainText() );
    }
 // mCentralWidget
  }




void CsWinMain::cmPlayLine()
  {
  CsWinTextEditor *editor = getCurrentEditor();
  if( editor != nullptr )
    CsPlayer::instance()->play( editor->getFileDirectory(), editor->textCursor(), false );
  }



void CsWinMain::cmPlaySyncro()
  {
  CsWinTextEditor *editor = getCurrentEditor();
  if( editor != nullptr )
    CsPlayer::instance()->playSync( editor->getFileDirectory(), editor->textCursor() );
  }



void CsWinMain::cmPlayStop()
  {
  CsPlayer::instance()->stop();
  }



void CsWinMain::cmPlayAll()
  {
  CsWinTextEditor *editor = getCurrentEditor();
  if( editor != nullptr )
    CsPlayer::instance()->playAll( editor->getFileDirectory(), editor->document() );
  }





void CsWinMain::cmHelpContext()
  {

  }




void CsWinMain::cmHelpContens()
  {

  }




void CsWinMain::cmHelpWeb()
  {
  //Go on home page
  QDesktopServices::openUrl( QUrl(QString(CS_APPLICATION_WEB)) );
  }




void CsWinMain::cmHelpAbout()
  {
  //Вывести диалог с версией
  QMessageBox::about( this, tr("About SaliSongEditor"), tr("SaliSongEditor %1.%2").arg( CS_VERSION_MAJOR ).arg( CS_VERSION_MINOR ) );
  }




void CsWinMain::fileOpenInternal(const QString &fname)
  {
  if( !fname.isEmpty() ) {
    //Check if file always opened
    for( int i = 0; i < mEditorTab->count(); i++ ) {
      auto editor = getEditor(i);
      if( editor != nullptr ) {
        if( editor->mFilePath == fname ) {
          mEditorTab->setCurrentIndex(i);
          return;
          }
        }
      }

    QFile file(fname);
    if( file.open(QIODevice::ReadOnly) ) {
      buildEditor( fname, QString::fromUtf8(file.readAll()) );

      QSettings s;
      QStringList list;
      if( s.contains(CSS_RECENT_FILES) )
        //Получить список
        list = s.value(CSS_RECENT_FILES).toStringList();

      int idx = list.indexOf( fname );
      if( idx > 0 ) {
        list.removeAt( idx );
        list.insert( 0, fname );
        }
      else if( idx < 0 ) {
        if( list.count() >= CS_MAX_RECENT_FILE )
          list.removeLast();
        list.insert( 0, fname );
        }
      s.setValue( CSS_RECENT_FILES, list );


      //Изменить меню прежних проектов
      for( int i = 0; i < CS_MAX_RECENT_FILE; ++i )
        if( i < list.count() ) {
          QFileInfo info( list.at(i) );
          maRecentFile[i]->setText( info.completeBaseName() );
          maRecentFile[i]->setData( list.at(i) );
          maRecentFile[i]->setToolTip( list.at(i) );
          maRecentFile[i]->setVisible(true);
          }
        else maRecentFile[i]->setVisible(false);

      maFileRecent->setEnabled( list.count() != 0 );
      }
    }
  }




//!
//! \brief fileSaveIndex Save the file in the tab with the specified index
//! \param tabIndex      index of tab
//! \return              true when save successfull
//!
bool CsWinMain::fileSaveIndex(int tabIndex)
  {
  if( tabIndex >= 0 ) {
    CsWinTextEditor *editor = getEditor( tabIndex );
    if( editor == nullptr ) return true;
    QString title = editor->mFilePath; // mTab->tabText(index);
    if( title.isEmpty() || title.startsWith( CS_NO_NAME ) )
      return fileSaveIndexAs( tabIndex );
    //Сохранить файл
    QFile file(title);
    if( editor && file.open( QIODevice::WriteOnly | QFile::Text ) ) {
      //При записи преобразуем в utf8
      file.write( editor->toPlainText().toUtf8() );
      file.close();
      //Обновить время модификации
      QFileInfo info( file );
      editor->setFileTime( info.lastModified() );
      //Сбросить флаг изменения
      editor->document()->setModified( false );
      return true;
      }
    return false;
    }
  return true;
  }





//!
//! \brief fileSaveIndexAs Save the file in the tab with the specified index with different name
//! \param tabIndex        index of tab
//! \return                true when save successfull
//!
bool CsWinMain::fileSaveIndexAs(int tabIndex)
  {
  if( tabIndex >= 0 ) {
    CsWinTextEditor *editor = getEditor( tabIndex );
    if( editor == nullptr ) return true;
    QString title = editor->mFilePath; // mTab->tabText(index);
    title = QFileDialog::getSaveFileName( this, tr("Enter file name for song"), title );
    if( title.isEmpty() ) return false;
    if( !title.endsWith(CS_SONG_EXTENSION) )
      title.append( CS_SONG_EXTENSION );
    editor->mFilePath = title;
    tabNameUpdate( tabIndex );
    return fileSaveIndex( tabIndex );
    }
  return true;
  }




//!
//! \brief tabNameUpdate Updates tab name with specified index of tab
//! \param tabIndex      Index of tab
//!
void CsWinMain::tabNameUpdate(int tabIndex)
  {
  if( tabIndex >= 0 ) {
    CsWinTextEditor *editor = getEditor( tabIndex );
    if( editor == nullptr ) return;
    mEditorTab->setTabText( tabIndex, editor->tabTitle() );
    mEditorTab->setTabToolTip( tabIndex, editor->mFilePath );
    }
  }




//!
//! \brief fileCloseIndex Close the tab with the specified index
//! \param tabIndex       Index of tab
//!
bool CsWinMain::fileCloseIndex(int tabIndex)
  {
  CsWinTextEditor *editor = getEditor(tabIndex);
  if( editor && editor->document()->isModified() ) {
    int r = QMessageBox::question( this, tr("Warning!"), tr("File \"%1\" is modified! Do you want to save changes?").arg(editor->mFilePath), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
    if( r == QMessageBox::Cancel ) return false;
    if( r == QMessageBox::Yes ) {
      if( !fileSaveIndex(tabIndex) )
        return false;
      }
    }
  mEditorTab->removeTab(tabIndex);
  if( editor )
    editor->deleteLater();
  return true;
  }




//!
//! \brief onEditorIndex When changing the tab with the editor
//! \param tabIndex      Index of tab
//!
void CsWinMain::onEditorIndex(int tabIndex)
  {
  CsWinTextEditor *editor = getEditor(tabIndex);
  if( editor ) {
    //Обновить разрешение команд undo и redo
    maEditUndo->setEnabled( editor->isUndoAvailable() );
    maEditRedo->setEnabled( editor->isRedoAvailable() );
    //Команды копирования и вырезания
    maEditCopy->setEnabled( editor->isSelectPresent() );
    maEditCut->setEnabled( editor->isSelectPresent() );
    }
  else {
    //Обновить разрешение команд undo и redo
    maEditUndo->setEnabled( false );
    maEditRedo->setEnabled( false );
    //Команды копирования и вырезания
    maEditCopy->setEnabled( false );
    maEditCut->setEnabled( false );
    }
  }




//!
//! \brief onUndoRedoChanged When changing undo, redo
//!
void CsWinMain::onUndoRedoChanged(bool)
  {
  CsWinTextEditor *editor = getCurrentEditor();
  if( editor ) {
    //Обновить разрешение команд undo и redo
    maEditUndo->setEnabled( editor->isUndoAvailable() );
    maEditRedo->setEnabled( editor->isRedoAvailable() );
    }
  }




//!
//! \brief onSelectChanged When the selection changes
//!
void CsWinMain::onSelectChanged(bool)
  {
  CsWinTextEditor *editor = getCurrentEditor();
  if( editor ) {
    maEditCopy->setEnabled( editor->isSelectPresent() );
    maEditCut->setEnabled( editor->isSelectPresent() );
    }
  }




//!
//! \brief textChanged When text changed
//!
void CsWinMain::textChanged()
  {

  }




void CsWinMain::updateCommand()
  {
  bool editorPresent = mEditorTab->count() != 0;
  maFileClose->setEnabled( editorPresent );
  maFileCloseAll->setEnabled( editorPresent );
  maFileSaveAs->setEnabled( editorPresent );
  }




//!
//! \brief getEditor Returns editor with specified index
//! \param tabIndex  Index of editor
//! \return          Editor with specified index
//!
CsWinTextEditor *CsWinMain::getEditor(int tabIndex)
  {
  return dynamic_cast<CsWinTextEditor*>( mEditorTab->widget(tabIndex) );
  }





//!
//! \brief getCurrentEditor Returns current text editor
//! \return                 Current text editor
//!
CsWinTextEditor *CsWinMain::getCurrentEditor()
  {
  return getEditor( mEditorTab->currentIndex() );
  }

CsWinTextEditor *CsWinMain::buildEditor(const QString &fname, const QString &text)
  {
  QFont font;
  font.setFamily("Courier");
  font.setFixedPitch(true);
  font.setPointSize(12);

  CsWinTextEditor *editor = new CsWinTextEditor();
  editor->setFont( font );
  editor->mFilePath = fname;
  editor->setPlainText( text );
  //Append tab
  mEditorTab->addTab( editor, editor->tabTitle() );
  tabNameUpdate( mEditorTab->count() - 1 );
  QFileInfo info(fname);
  editor->setFileTime( info.lastModified() );

  CsHighlighter *hl = new CsHighlighter( editor->document() );
  connect( editor, &CsWinTextEditor::rehighlightBlock, hl, &CsHighlighter::rehighlightBlock );

  return editor;
  }








bool CsWinMain::canClose()
  {
  //Пройти по всем открытым окнам и проверить наличие файлов, требующих сохранение
  //На каждый такой файл выдаем запрос сохранения
  for( int i = 0; i < mEditorTab->count(); i++ ) {
    CsWinTextEditor *editor = getEditor(i);
    if( editor != nullptr && editor->document()->isModified() ) {
      //Текст в данной вкладке изменен, выдаем запрос на сохранение
      auto but = QMessageBox::warning( this, tr("Внимание!"), tr("Файл \"%1\" изменен. Сохранить?").arg(editor->mFilePath), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
      //При нажатии Cancel мы прерываем процесс закрытия
      if( but == QMessageBox::Cancel )
        return false;
      //При нажатии кнопки yes - сохраняем файл
      if( but == QMessageBox::Yes ) {
        if( !fileSaveIndex( i ) )
          return false;
        }
      }
    }
  return true;
  }




//!
//! \brief setTitle Установить заголовок в соотвествии с проектом
//!
void CsWinMain::setTitle()
  {
  // QString name( "noname" );
  // if( !CsRobot::instance()->filePath().isEmpty() ) {
  //   QFileInfo info(CsRobot::instance()->filePath());
  //   name = info.fileName();
  //   }
  setWindowTitle( tr("SaliSongEditor v%1.%2").arg(CS_VERSION_MAJOR).arg(CS_VERSION_MINOR) );
  }







void CsWinMain::closeEvent(QCloseEvent *event)
  {
  //If can close editor
  if( canClose() ) {
    //Сохранить размер окна и положения сплиттеров
    QSettings s;
    s.setValue( QStringLiteral(CSS_WMAIN_MAX), isMaximized() );
    event->accept();
    }
  else event->ignore();
  }
