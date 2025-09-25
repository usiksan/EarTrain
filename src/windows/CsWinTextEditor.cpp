/*
  Проект "IronArt"
    Визуальное программирование роботов
  Автор
    Сибилев А.С.
  Описание
    Текстовый редактор скриптов с подсветкой, автодополнением, автоотступом и отладкой
*/
#include "CsSongEditorConfig.h"
#include "CsWinTextEditor.h"
#include "CsHighlighter.h"
#include "CsWinCommand.h"
#include "score/CsPlayer.h"
#include "score/CsParser.h"
#include "import/text/CsImportText.h"
#include "audioFile/CsMelodyDetector.h"

#include <QKeyEvent>
#include <QPainter>
#include <QDir>
#include <QFileInfo>
#include <algorithm>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QClipboard>
#include <QMimeData>
#include <QGuiApplication>
#include <QMessageBox>

#include <QDebug>


bool            CsWinTextEditor::mAutoCompleteParenthesis = true;


CsWinTextEditor::CsWinTextEditor(QWidget *parent) :
  QPlainTextEdit(parent),
  mAutoComplete(nullptr),
  mMelodyTaktIndex(-1),    //!< Time line index for melody detector
  mIsUndoAvailable(false),
  mIsRedoAvailable(false),
  mIsSelectPresent(false),
  mControlPress(false)
  {

  //Создать окно автозавершения
  mAutoComplete = new QListWidget(this);
  mAutoComplete->hide();
  //Установить размер автозавершателя
  mAutoComplete->resize( 250, 150 );

  connect( this, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()) );

  setLineWrapMode(QPlainTextEdit::NoWrap);

  //Состояние редактора
  connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(onUndoAvailable(bool)) );
  connect(this, SIGNAL(redoAvailable(bool)), this, SLOT(onRedoAvailable(bool)) );
  connect(this, SIGNAL(copyAvailable(bool)), this, SLOT(onSelection(bool)) );

  setMouseTracking(true);
  }




CsWinTextEditor::~CsWinTextEditor()
  {
  }





QString CsWinTextEditor::tabTitle() const
  {
  QFileInfo info(mFilePath);
  return info.completeBaseName();
  }





//Получить строку под курсором
QString cursorLine( QTextCursor c, int *posPtr ) {
  int pos = c.position();
  c.movePosition( QTextCursor::StartOfBlock );
  int linePos = c.position();
  c.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor );
  //Вернуть позицию курсора относительно начала строки
  if( posPtr )
    *posPtr = pos - linePos;
  return c.selectedText();
  }





//Получить директорий файла
QString CsWinTextEditor::getFileDirectory()
  {
  QFileInfo info(mFilePath);
  return info.absolutePath();
  }




//!
//! \brief currentLine Возвращает номер текущей строки
//! \return            Номер текущей строки
//!
int CsWinTextEditor::currentLine() const
  {
  return textCursor().blockNumber();
  }




void CsWinTextEditor::cmEditPasteSong( int halfTaktCount )
  {
  const QMimeData *mime = QGuiApplication::clipboard()->mimeData();
  //Try convert from textual coding
  if( mime != nullptr && mime->hasText() ) {
    QString src = mime->text();
    //Importer
    CsImportText importText;
    QString dst = importText.parseText( src, halfTaktCount );
    if( !dst.isEmpty() )
      insertPlainText( dst );
    }
  }







bool CsWinTextEditor::autoCompleteParenthesis()
  {
  return mAutoCompleteParenthesis;
  }




void CsWinTextEditor::setAutoCompleteParenthesis(bool autoComplete)
  {
  mAutoCompleteParenthesis = autoComplete;
  }








QString CsWinTextEditor::lineType() const
  {
  auto cr = textCursor();
  QString line = cr.block().text();
  if( line.contains(':') ) {
    return line.left( line.indexOf(':') + 1 );
    }
  return QString{};
  }




















//!
//! \brief cursorForLine Возвращает курсор с выделением строки с заданным номером
//! \param lineNumber    Номер строки, которую необходимо выделить
//! \return              Курсор с выделением строки с заданным номером
//!
QTextCursor CsWinTextEditor::cursorForLine(int lineNumber) const
  {
  QTextCursor cursor = textCursor();
  //Текущее положение курсора
  int curLine = cursor.blockNumber();

  //Относительное смещение курсора
  curLine -= lineNumber;
  if( curLine > 0 )
    cursor.movePosition( QTextCursor::PreviousBlock, QTextCursor::MoveAnchor, curLine );
  if( curLine < 0 )
    cursor.movePosition( QTextCursor::NextBlock, QTextCursor::MoveAnchor, -curLine );

  //Выполнить выделение
  cursor.movePosition( QTextCursor::StartOfLine );
  cursor.movePosition( QTextCursor::EndOfLine, QTextCursor::KeepAnchor );

  return cursor;
  }
















void CsWinTextEditor::autoComplete()
  {
  //Проверить наличие текущего выбора
  if( mAutoComplete->currentItem() ) {
    //Вставить текущий выбор из автозавершения
    QTextCursor c = textCursor();
    // QString word = getWordLeft();
    // c.movePosition( QTextCursor::Left, QTextCursor::KeepAnchor, word.length() );
    c.removeSelectedText();
    c.insertText( mAutoComplete->currentItem()->text() );
    //Текущая строка
    setTextCursor( c );
    mAutoComplete->hide();
    }
  }






void CsWinTextEditor::refillAutoComplete()
  {
  // QStringList srcList;
  // bool checkLen = true;

  // if( word.length() < 3 && checkLen ) mAutoComplete->hide();
  // else {
  //   QString pat("^");
  //   pat.append( word );
  //   //Перезаполняем список автозавершения
  //   mAutoComplete->clear();
  //   srcList = srcList.filter( QRegularExpression(pat, QRegularExpression::CaseInsensitiveOption) );
  //   if( !srcList.isEmpty() ) {
  //     p.rx() += mLeftViewPortMargin;
  //     mAutoComplete->addItems( srcList );
  //     mAutoComplete->setCurrentRow(0);
  //     //Вынести окно автозавершения в позицию начала слова
  //     mAutoComplete->move( p );
  //     mAutoComplete->show();
  //     }
  //   }
  }






//Переключить комментарии в одной строке
void CsWinTextEditor::toggleRemarkLine(QTextCursor tc)
  {
  //Получить содержимое блока, соответствующее данному курсору
  int pos;
  QString line = cursorLine( tc, &pos );
  tc.movePosition( QTextCursor::StartOfBlock );
  if( line.startsWith( QString("//") ) ) {
    //Удаляем два символа из начала строки
    tc.movePosition( QTextCursor::Right, QTextCursor::KeepAnchor, 2 );
    tc.removeSelectedText();
    }
  else if( !line.simplified().isEmpty() ) {
    //Вставляем два символа // в начало строки
    tc.insertText( QString("//") );
    }
  }





//Переключить комментарий
void CsWinTextEditor::toggleRemark()
  {
  //Если есть выделение, то вставка-удаление коментирования для каждой строки
  QTextCursor tc = textCursor();
  if( tc.hasSelection() ) {
    //Получить начало и конец выделения
    int start = tc.selectionStart();
    int end = tc.selectionEnd();
    //Снять выделение
    tc.clearSelection();
    //Обеспечить, чтобы начало было меньше конца
    if( end < start ) {
      int sw = start;
      start = end;
      end = sw;
      }
    //Едем к началу выделения
    tc.setPosition( start );
    //Начало строки
    tc.movePosition( QTextCursor::StartOfBlock );

    QTextCursor tcs(tc);
    tcs.setPosition( start );
    start = tcs.blockNumber();
    tcs.setPosition( end );
    int lastLineEndPosition = end;
    tcs.movePosition( QTextCursor::StartOfBlock );
    end = tcs.blockNumber();
    if( lastLineEndPosition > tcs.position() )
      end++;
    //Переключаем для всех строк, пока не достигнем конца выделения
    for( int i = start; i < end; i++ ) {
      toggleRemarkLine( tc );
      tc.movePosition( QTextCursor::NextBlock );
      }
    }
  else
    //Вставка-удаление коментария только для текущей строки
    toggleRemarkLine( tc );
  }




//При нажатии на F1 показываем помощь по функции, на которой стоит курсор, в противном случае - показываем помощь по редактору
void CsWinTextEditor::helpF1()
  {

  }













void
CsWinTextEditor::keyPressEvent(QKeyEvent *e)
  {
  mControlPress = (e->modifiers() & Qt::ControlModifier) != 0;

  //Return (Enter)
  if( e->key() == Qt::Key_Return ) {
    if( mAutoComplete->isHidden() ) {
      //после стандартной обработки нажатия Enter сделать автоотступ
      QPlainTextEdit::keyPressEvent(e);
      }
    else
      autoComplete();
    }

  else if( e->key() == Qt::Key_Paste ) {
    paste();
    }

  //Insert
  else if( e->key() == Qt::Key_Insert ) {
    if( Qt::ShiftModifier & e->modifiers() )
      paste();
    else if( Qt::ControlModifier & e->modifiers() )
      copy();
    else QPlainTextEdit::keyPressEvent( e );
    }


  //Delete
  else if( e->key() == Qt::Key_Delete ) {
    if( Qt::ShiftModifier & e->modifiers() )
      cut();
    else QPlainTextEdit::keyPressEvent( e );
    }

  //Backspace
  else if( e->key() == Qt::Key_Backspace ) {
    auto lt = lineType();
    if( maEditAutoSyncro->isChecked() && (CsParser::mLineChordNoteTime.contains(lt) || lt == QString("t:")) ) {
      //Sync move n,c,t,b,p,g,d to left
      //accumulate adjacent rows
      QList<QTextCursor> cursorList( CsParser::accumChordNoteTime(true, textCursor()) );
      //For all blocks remove previous space
      for( int i = 0; i < cursorList.size(); i++ ) {
        cursorList[i].movePosition(QTextCursor::Left,QTextCursor::KeepAnchor);
        if( cursorList[i].selectedText() == QString(" ") )
          cursorList[i].removeSelectedText();
        }
      //Fix new position
      setTextCursor( cursorList[0] );
      onCursorPositionChanged();
      }
    else QPlainTextEdit::keyPressEvent( e );
    }




  //Automatic append octave shift
  else if( CsParser::mLineNote.contains(lineType()) && !e->text().isEmpty() && QString("abcdefg").contains(e->text().toLower()) ) {
    //Get previous octave shift
    QTextCursor tc = textCursor();
    QString line = tc.block().text();
    int pos = qMin( tc.positionInBlock(), line.length() - 1 );
    int upperOctave = 0;
    int lowerOctave = 0;
    for( ; pos >= 0; pos-- ) {
      if( line.at(pos).isSpace() ) continue;
      if( line.at(pos) == QChar('\'') ) upperOctave++;
      else if( line.at(pos) == QChar(',') ) lowerOctave++;
      else break;
      }
    //Insert note symbol
    tc.insertText( e->text() );
    for( int i = 0; i < upperOctave; ++i )
      tc.insertText( QString("'") );
    for( int i = 0; i < lowerOctave; ++i )
      tc.insertText( QString(",") );
    setTextCursor( tc );
    }



  //Insert s sign before octave shift
  else if(  CsParser::mLineNote.contains(lineType()) && !e->text().isEmpty() && QString("s").contains(e->text().toLower()) ) {
    QTextCursor tc = textCursor();
    QString line = tc.block().text();
    int pos = qMin( tc.positionInBlock(), line.length() );
    if( pos-1 < line.length() && (line.at(pos-1) == QChar('\'') || line.at(pos-1) == QChar(',')) ) {
      //At left cursor there is octave shift symbols
      int count = 1;
      while( count - 1 >= 0 && (line.at(pos-count-1) == QChar('\'') || line.at(pos-count-1) == QChar(',')))
        count++;
      tc.movePosition( QTextCursor::Left, QTextCursor::MoveAnchor, count );
      tc.insertText( e->text() );
      tc.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor, count );
      setTextCursor( tc );
      }
    else QPlainTextEdit::keyPressEvent( e );
    }



  //Space pressed
  else if( e->text() == QString(" ") ) {
    mAutoComplete->hide();
    //Sync move n,c,t,b,p,g,d to right
    auto lt = lineType();
    if( maEditAutoSyncro->isChecked() && (CsParser::mLineChordNoteTime.contains(lt) || lt == QString("t:")) ) {
      //accumulate adjacent rows
      QList<QTextCursor> cursorList( CsParser::accumChordNoteTime(true, textCursor()) );
      //For all blocks append space
      for( int i = 0; i < cursorList.size(); i++ ) {
        cursorList[i].insertText( QString(" ") );
        }
      //Fix new position
      setTextCursor( cursorList[0] );
      onCursorPositionChanged();
      }

    else if( maEditAutoIndent->isChecked() && (CsParser::mLineChordNoteTime.contains(lt)) ) {
      //Get time line
      QTextCursor timeLineCursor( CsParser::findNearestTime( textCursor() ) );
      if( !timeLineCursor.isNull() ) {
        QString timeLine( timeLineCursor.block().text() );
        QTextCursor tc(textCursor());
        int pos = tc.positionInBlock();
        if( CsParser::mLineChord.contains(lt) ) {
          while(true) {
            tc.insertText( QString(" ") );
            pos++;
            if( pos >= timeLine.length() || timeLine.at(pos) == QChar('|') )
              break;
            }
          }
        else {
          while(true) {
            tc.insertText( QString(" ") );
            pos++;
            if( pos >= timeLine.length() || timeLine.at(pos) == QChar('+') || timeLine.at(pos) == QChar('|') )
              break;
            }
          }
        setTextCursor(tc);
        }
      else QPlainTextEdit::keyPressEvent( e );
      }
    else QPlainTextEdit::keyPressEvent( e );
    }





  //При нажатии ctrl+/ выполнить переключение комментария
  else if( e->text() == QString("/") && mControlPress ) toggleRemark();


  //Клавиши вверх-вниз в списке завершателей
  else if( (e->key() == Qt::Key_Up || e->key() == Qt::Key_Down) && !mAutoComplete->isHidden() ) {
    //Изменить выбор в автозавершателе
    int i = mAutoComplete->currentRow();
    if( e->key() == Qt::Key_Up ) i--;
    else i++;
    if( i < 0 ) i = mAutoComplete->count() - 1;
    if( i >= mAutoComplete->count() ) i = 0;
    mAutoComplete->setCurrentRow( i );
    }

  //Клавиша забой
  else if( e->key() == Qt::Key_Backspace ) {
    QPlainTextEdit::keyPressEvent(e);
    //Если активно автозавершение, то перенакопить
    // if( !mAutoComplete->isHidden() )
    //refillAutoComplete();
    //emit contensChanged();
    }

  else if( e->key() == Qt::Key_F11 && CsParser::mLineNote.contains(lineType()) ) {
    //On-off melody detector
    if( mMelodyDetectorTime.isValid() ) {
      mMelodyDetectorTime.clear();
      CsMelodyDetector::instance()->detectStop();
      if( !mPreviousNoteLine.isEmpty() ) {
        //Remove that melody detector inserted
        QTextCursor tc(textCursor());
        tc.select( QTextCursor::BlockUnderCursor );
        tc.removeSelectedText();
        //Insert previous note line
        tc.insertText( mPreviousNoteLine );
        setTextCursor( tc );
        }
      }
    else {
      QString noteLineType( lineType() );
      QTextCursor tc(textCursor());
      mMelodyDetectorTime.blockParse( tc, nullptr );
      if( mMelodyDetectorTime.isValid() ) {
        mMelodyTaktIndex = -1;
        //Delete current content of note line
        tc.select( QTextCursor::BlockUnderCursor );
        mPreviousNoteLine = tc.selectedText();
        tc.removeSelectedText();
        tc.insertBlock();
        tc.insertText( noteLineType );
        setTextCursor( tc );
        }
      }
    }


  else if( e->key() == Qt::Key_Right && Qt::ControlModifier & e->modifiers() && lineType() == fieldLyric ) {
    //Drag word and all at right to right nearest half takt
    QTextCursor tc = textCursor();
    int startTick;
    CsTimeLineInfo notePositions;
    notePositions.blockParse( tc, &startTick );
    int pos = tc.positionInBlock();
    for( int i = 0; i < notePositions.count(); ++i )
      if( pos < notePositions.position(i) && notePositions.isQuartTakt(i) ) {
        int delta = notePositions.position(i) - pos;
        //Find begin of word
        tc.movePosition( QTextCursor::StartOfWord );
        int offsetFromStartOfWord = pos - tc.positionInBlock();
        for( int k = 0; k < delta; k++ )
          tc.insertText( QString(" ") );
        tc.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor, offsetFromStartOfWord );
        setTextCursor( tc );
        break;
        }
    }

  else {
    //Проверить ввод цифро-буквенного значения
    if( !e->text().isEmpty() ) {
      auto symbol = e->text().at(0);
      if( symbol.isLetterOrNumber() || symbol == QChar('_') || symbol == QChar('<') ) {
        //Ввести знак
        QPlainTextEdit::keyPressEvent(e);
        refillAutoComplete();
        return;
        }
      }
    mAutoComplete->hide();
    if( e->key() == Qt::Key_Enter ) {
      //после стандартной обработки нажатия Enter сделать автоотступ
      }
    QPlainTextEdit::keyPressEvent(e);
    }
  }




void CsWinTextEditor::keyReleaseEvent(QKeyEvent *e)
  {
  if( mControlPress && (e->modifiers() & Qt::ControlModifier) == 0 ) {
    mControlPress = false;
    }

  if( e->key() == Qt::Key_F12 && CsParser::mLineNote.contains(lineType()) && mMelodyDetectorTime.isValid() ) {
    QTextCursor tc(textCursor());
    if( tc.positionInBlock() < mMelodyDetectorTime.position(0) ) {
      //First pressing start detection process
      //Track cursor to first time position
      while( tc.positionInBlock() < mMelodyDetectorTime.position(0) )
        tc.insertText( QString(" ") );
      CsMelodyDetector::instance()->detectStart();
      }
    else {
      int startIndex = mMelodyTaktIndex + 1;
      if( startIndex >= mMelodyDetectorTime.count() ) {
        //We stop melody detector
        mMelodyDetectorTime.clear();
        CsMelodyDetector::instance()->detectStop();
        }
      else {
        int nextQuarterIndex = startIndex;
        while( nextQuarterIndex < mMelodyDetectorTime.count() && !mMelodyDetectorTime.isQuartTakt(nextQuarterIndex) )
          nextQuarterIndex++;

        auto notes = CsMelodyDetector::instance()->detectGet();
        //We place note at startIndex
        addNoteAt( mMelodyDetectorTime.position(startIndex), notes.mEighth0 );
        //We place note at nextQuarterIndex
        if( nextQuarterIndex < mMelodyDetectorTime.count() ) {
          addNoteAt( mMelodyDetectorTime.position(nextQuarterIndex), notes.mEighth1 );
          mMelodyTaktIndex = nextQuarterIndex;
          }
        else {
          //We stop melody detector
          mMelodyDetectorTime.clear();
          CsMelodyDetector::instance()->detectStop();
          }
        }
      }
    }

  QPlainTextEdit::keyReleaseEvent( e );
  if( !e->isAutoRepeat() && !e->text().isEmpty() ) {
    //if( !mAutoComplete->isHidden() )
    //   refillAutoComplete();
    emit contensChanged();
    }
  }





void CsWinTextEditor::mouseMoveEvent(QMouseEvent *ev)
  {
  //Выполнять всякую херь только если есть фоновый компилятор
  QPlainTextEdit::mouseMoveEvent( ev );
  }






void CsWinTextEditor::mousePressEvent(QMouseEvent *e)
  {
  //Если автозавершатель не скрыть - скрыть его
  if( !mAutoComplete->isHidden() )
    mAutoComplete->hide();
  //Обработать нажатие мыши по умолчанию
  QPlainTextEdit::mousePressEvent(e);
  }









void
CsWinTextEditor::onCursorPositionChanged() {
  QTextEdit::ExtraSelection selection;

  QColor lineColor = QColor(Qt::yellow).lighter(160);
  QTextCursor cr = textCursor();

  selection.format.setBackground(lineColor);
  selection.format.setProperty(QTextFormat::FullWidthSelection, true);
  selection.cursor = cr;
  selection.cursor.clearSelection();

  QList<QTextEdit::ExtraSelection> extraSelections;

  if( maEditAutoSyncro->isChecked() ) {
    //If current line note or chord then we highlight also time
    QString line( cr.block().text() );
    if( CsParser::isLineTimeChordNote(line) ) {
      auto cursorList( CsParser::accumChordNoteTime(false,textCursor()) );
      for( auto crr : std::as_const(cursorList) ) {
        crr.clearSelection();
        selection.cursor = crr;
        extraSelections.append(selection);
        }
      }
    else
      extraSelections.append(selection);
    }
  else {
    if( maEditAutoIndent->isChecked() ) {
      extraSelections.append(selection);
      //If current line note or chord then we highlight also time
      QString line( cr.block().text() );
      if( CsParser::isLineChordNote(line) ) {
        //Scan upper to nearest time line
        selection.cursor = CsParser::findNearestTime( textCursor() );
        if( !selection.cursor.isNull() ) {
          selection.cursor.clearSelection();
          extraSelections.append(selection);
          }
        }
      }
    else
      extraSelections.append(selection);
    }


  setExtraSelections(extraSelections);
  }


void
CsWinTextEditor::locateLine(int line)
  {
  if ( line > 0 ) line--;
  else return;

  //Текущее положение курсора
  int curLine = textCursor().blockNumber();

  //Относительное смещение курсора
  curLine -= line;
  while ( curLine > 0 ) {
    moveCursor( QTextCursor::PreviousBlock );
    curLine--;
    }
  while ( curLine < 0 ) {
    moveCursor( QTextCursor::NextBlock );
    curLine++;
    }

  //Сделать курсор видимым
  ensureCursorVisible();

  //Обеспечить фокус
  setFocus();
  }



bool CsWinTextEditor:: isCommented(int index)const{
  auto cursort = textCursor();
  cursort.setPosition(index);
  return cursort.charFormat().foreground().color() == Qt::darkGreen;
  }




void CsWinTextEditor::addNoteAt(int position, int note)
  {
  QTextCursor tc( textCursor() );
  while( tc.positionInBlock() < position )
    tc.insertText( QString(" ") );
  if( note )
    tc.insertText( CsCell::classicNote( 4, note, duraQuarter ) );
  setTextCursor( tc );
  }






QString getRemark( const QStringList line ) {
  //Просканировать строку и обнаружить комментарии
  int i;
  for( i = 0; i < line.count(); i++ )
    if( line.at(i).startsWith(QString("//")) ) {
      //Комментарии обнаружены, накопить
      QString str;
      while( i < line.count() ) {
        str.append( line.at(i) );
        str.append( QString(" ") );
        i++;
        }
      return str;
      }
  return QString();
  }



