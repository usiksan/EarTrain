#include "CsHighlighter.h"
#include "score/CsParser.h"





CsHighlighter::CsHighlighter(QTextDocument *parent)
  : QSyntaxHighlighter(parent)
  {
  //Образовать подсветку
  mKeywordFormat.setForeground(Qt::darkBlue);           //!< Ключевые слова
  mKeywordFormat.setFontWeight(QFont::Bold);

  mSingleLineCommentFormat.setForeground(Qt::darkGreen); //!< Однострочные коментарии

  mChordFormat.setForeground(Qt::darkMagenta);             //!< Для идентификаторов глобальных
  mNoteFormat.setForeground(Qt::darkCyan);              //!< Десятичные числа
  mLyricFormat.setForeground(Qt::darkGreen);             //!< Строки и символы
  mErrorFormat.setForeground(Qt::red);             //!< Шестнадцатиричные числа

  // mPreprocessFormat.setForeground(Qt::darkRed);        //!< Препроцессор

  // mDebugFormat.setBackground( QColor(Qt::green).lighter(160) );

  // mLinkFormat.setForeground( Qt::magenta );
  // mLinkFormat.setFontUnderline(true);

  // mStructFormat.setForeground( Qt::red );

  }




void
CsHighlighter::highlightBlock(const QString &text) {
  int index = 0;
  int count = text.length();

  if( text.startsWith("//") ) {
    //Однострочный коментарий
    setFormat( index, count - index, mSingleLineCommentFormat );
    return;
    }

  QString ltp( CsParser::lineType(text) );

  if( !ltp.isEmpty() ) {
    //Line of song
    if( CsParser::mKeyWords.contains(ltp) ) {
      index += ltp.length();
      setFormat( 0, index, mKeywordFormat );
      if( CsParser::mLineChord.contains(ltp) ) {
        //Color chords
        CsParser parser( text, index );
        while( parser.blankAndScanPos() < text.length() ) {
          if( parser.scanChord() )
            setFormat( parser.scanStart(), parser.scanPos(), mChordFormat );
          else
            setFormat( parser.scanStart(), parser.scanPos(), mErrorFormat );
          }
        }
      else if( CsParser::mLineNote.contains(ltp) ) {
        //Color notes
        CsParser parser( text, index );
        while( parser.blankAndScanPos() < text.length() ) {
          if( parser.scanNote() )
            setFormat( parser.scanStart(), parser.scanPos(), mNoteFormat );
          else
            setFormat( parser.scanStart(), parser.scanPos(), mErrorFormat );
          }
        }

      }
    else {

      }
    }
  else {
    //Different line

    }

  }


