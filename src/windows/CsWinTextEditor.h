/*
  Project "Sali song editor for SaliSongBook"
  Author
    Sibilev Alexander S.

  Web
    www.saliLab.com
    www.saliLab.ru

  Description
    Text editor with highlight, autocompleting
*/
#ifndef CSWINTEXTEDITOR_H
#define CSWINTEXTEDITOR_H

#include "score/CsTimeLineInfo.h"

#include <QPlainTextEdit>
#include <QTextBlock>
#include <QDateTime>
#include <QListWidget>
#include <QTimer>
#include <QScrollBar>
#include <functional>


//Текстовый редактор
class CsWinTextEditor : public QPlainTextEdit {
    Q_OBJECT

    static bool     mAutoCompleteParenthesis; //!< Автоматическая вставка завершающих скобок

    CsTimeLineInfo        mMelodyDetectorTime; //!< Time line for current actived melody detector
    QDateTime             mFileTime;           //!< Возраст файла
    QListWidget          *mAutoComplete;       //!< Окно списка автозавершения
    QString               mPreviousNoteLine;   //!< Note line before melody detector work
    int                   mMelodyTaktIndex;    //!< Time line index for melody detector
    int                   mLeftViewPortMargin; //!< Отступ слева для номеров строк
    bool                  mIsUndoAvailable;
    bool                  mIsRedoAvailable;
    bool                  mIsSelectPresent;
    bool                  mControlPress;       //!< Флаг нажатия кнопки Control
  public:
    QString               mFilePath;             //!< Полный путь к файлу

    explicit CsWinTextEditor(QWidget *parent = 0);
    virtual ~CsWinTextEditor();

    QString tabTitle() const;

    void setFileTime( const QDateTime &t ) { mFileTime = t; }
    bool isOlder( const QDateTime &t ) { return mFileTime < t; }


    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int  lineNumberAreaWidth();

    //Получить директорий файла
    QString getFileDirectory();

    //!
    //! \brief currentLine Возвращает номер текущей строки
    //! \return            Номер текущей строки
    //!
    int     currentLine() const;

    //Получить состояние редактора
    bool    isUndoAvailable() const { return mIsUndoAvailable; }
    bool    isRedoAvailable() const { return mIsRedoAvailable; }
    bool    isSelectPresent() const { return mIsSelectPresent; }

    void    cmEditPasteSong( int halfTaktCount );

    static bool autoCompleteParenthesis();
    static void setAutoCompleteParenthesis(bool autoComplete);

    QString            lineType() const;
  private:
    //!
    //! \brief cursorForLine Возвращает курсор с выделением строки с заданным номером
    //! \param lineNumber    Номер строки, которую необходимо выделить
    //! \return              Курсор с выделением строки с заданным номером
    //!
    QTextCursor     cursorForLine( int lineNumber ) const;

    //Автоматическое завершение }
    bool      completeParenthesis();

    //Автозавершение ввода
    void      autoComplete();

    //Перезаполнить список автозавершения
    void      refillAutoComplete();

    //Переключить комментарии
    void      toggleRemarkLine( QTextCursor tc );
    void      toggleRemark();

    //При нажатии на F1 показываем помощь по функции, на которой стоит курсор, в противном случае - показываем помощь по редактору
    void      helpF1();

    //определить начало и конец блока для выделения скобок. Возвращает пару индексов символов документа <начало, конец>
    QPair<int, int> getParenthesisBlock(const QString & text, int pos)const;

    //закомментирован ли символ
    bool      isCommented(int index)const;

    void      addNoteAt( int position, int note );

  private slots:

    void onUndoAvailable( bool ena ) { mIsUndoAvailable = ena; }
    void onRedoAvailable( bool ena ) { mIsRedoAvailable = ena; }
    void onSelection( bool ena ) { mIsSelectPresent = ena; }

  protected:
    virtual void keyPressEvent(QKeyEvent *e) override;
    virtual void keyReleaseEvent(QKeyEvent *e) override;
    virtual void mouseMoveEvent(QMouseEvent *ev) override;
    virtual void mousePressEvent(QMouseEvent *e) override;

  signals:
    void rehighlightBlock( const QTextBlock block );
    void contensChanged();
    void fileOpen( const QString &fname );
    void setLink( const QString link );
    void jump( const QString fname, int line );

 public slots:
    void onCursorPositionChanged();
    void locateLine( int line );
  };

#endif // CSWINTEXTEDITOR_H
