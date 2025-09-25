#include "CsParser.h"
#include "CsNoteChord.h"
#include "CsCell.h"

#include <QTextBlock>


QSet<QString> CsParser::mLineChord( {"c:", "c0:", "c1:", "c2:", "c3:"} );

QSet<QString> CsParser::mLineNote( {"n:","n0:","n1:","n2:","n3:","n4:","n5:","n6:","n7:","n8:","n9:","b:","p:",
                                    "nf:","nf0:","nf1:","nf2:","nf3:","nf4:","nf5:","nf6:","nf7:","nf8:","nf9:"} );

QMap<QString,int> CsParser::mLineDrum;

QSet<QString> CsParser::mLineChordNoteTime( mLineChord + mLineNote + QSet<QString>({fieldTime, fieldGuitar, fieldFinger})  );

QSet<QString> CsParser::mLineChordNote( mLineChord + mLineNote + QSet<QString>({fieldGuitar, fieldFinger})  );

QSet<QString> CsParser::mLineChordNoteLyric;

QSet<QString> CsParser::mKeyWords(mLineChordNoteTime + QSet<QString>({fieldTitle, fieldArtist, fieldComposer,
                                        fieldWords, fieldScoreCreator, fieldTempo,
                                        fieldSynthesizerProgram, fieldSynthesizerStyle,
                                        fieldDrumProgram, fieldBassProgram, fieldGuitarProgram, fieldPadProgram, fieldLyric, fieldAudioFile, fieldAudioVolume } ) );   //!< Key words (line name)

QMap<int,int>     CsParser::mGuitarChords;




CsParser::CsParser(const QString &line, int ptr)
  : mLine(line)
  , mPos(ptr)
  , mNoteDuration(0)
  {

  }



void CsParser::blank()
  {
  while( mPos < mLine.length() && (mLine.at(mPos) == QChar(' ') || mLine.at(mPos) == QChar(' ')) )
    mPos++;
  }

void CsParser::skeepNonBlank()
  {
  while( mPos < mLine.length() && mLine.at(mPos) != QChar(' ') )
    mPos++;
  }

int CsParser::scanInt()
  {
  int digitPos = mPos;
  while( mPos < mLine.length() && mLine.at(mPos).isNumber() ) mPos++;
  if( digitPos != mPos )
    return mLine.mid( digitPos, mPos - digitPos ).toInt();
  return -1;
  }




bool CsParser::scanDuration()
  {
  int div = scanInt();
  if( div >= 0 ) {
    switch( div ) {
      case 1 : mNoteDuration = duraHole; break;
      case 2 : mNoteDuration = duraHalf; break;
      case 4 : mNoteDuration = duraQuarter; break;
      case 8 : mNoteDuration = duraEighth; break;
      case 16 : mNoteDuration = duraSixteenth; break;
      case 32 : mNoteDuration = duraThirtySecond; break;
      case 64 : mNoteDuration = duraSixtyFourth; break;
      case 128 : mNoteDuration = duraOneHundredTwentyEighth; break;
      default :
        mNoteDuration = 0;
        skeepNonBlank();
        return false;
      }
    }
  return true;
  }


//!
//! \brief scanNote Find and scan next note from current position
//! \return         true if note parsed successfull
//!
bool CsParser::scanNote()
  {
  mScanStart = mPos;
  return getNote(mPos);
  }



//!
//! \brief scanChord Find and scan next chord from current position
//! \return          true if chord parsed successfull
//!
bool CsParser::scanChord()
  {
  mScanStart = mPos;
  return getChord(mPos);
  }



bool CsParser::getNote(int position)
  {
  if( position < mLine.length() ) {
    mPos = position;

    QChar note(mLine.at(mPos++));

    bool sharp = mPos < mLine.length() && mLine.at(mPos) == QChar('s');
    if( sharp ) mPos++;

    int octaveOffset = 0;
    while( mPos < mLine.length() && mLine.at(mPos) == QChar('\'') ) {
      octaveOffset++;
      mPos++;
      }
    while( mPos < mLine.length() && mLine.at(mPos) == QChar(',') ) {
      octaveOffset--;
      mPos++;
      }

    if( !scanDuration() )
      return false;

    int shift = mNoteDuration;
    while( mPos < mLine.length() && mLine.at(mPos) == QChar('.') ) {
      shift >>= 1;
      mNoteDuration += shift;
      mPos++;
      }

    if( note == QChar('a') ) mNotePitch = sharp ? noteAd : noteA;
    else if( note == QChar('b') ) mNotePitch = noteB;
    else if( note == QChar('c') ) mNotePitch = sharp ? noteCd : noteC;
    else if( note == QChar('d') ) mNotePitch = sharp ? noteDd : noteD;
    else if( note == QChar('e') ) mNotePitch = noteE;
    else if( note == QChar('f') ) mNotePitch = sharp ? noteFd : noteF;
    else if( note == QChar('g') ) mNotePitch = sharp ? noteGd : noteG;
    else if( note == QChar('r') ) {
      mNotePitch = noteMute;
      return true;
      }
    else {
      skeepNonBlank();
      return false;
      }

    mNotePitch += octaveFirst + octaveOffset * 12;

    return (mPos >= mLine.length() || mLine.at(mPos) == QChar(' '));
    //return mNoteDuration != 0 && (mPos >= mLine.length() || mLine.at(mPos) == QChar(' '));
    }
  return false;
  }




//!
//! \brief getChord Extract chord from position and parse it
//! \param position Position where find chord
//! \return         true if chord parsed successfull
//!
bool CsParser::getChord(int position)
  {
  if( position < mLine.length() ) {
    mPos = position;
    QString chord;
    while( mPos < mLine.length() && mLine.at(mPos) != QChar(' ') )
      chord.append( mLine.at(mPos++) );
    if( !chord.isEmpty() ) {
      mNotePitch = CsCell::chordParse( chord );
      return mNotePitch != noteMute;
      }
    }
  return false;
  }




//!
//! \brief getGuitar Extract guitar flash type from position and parse it
//! \param position  Position where find flash type
//! \return          true if flash parsed successfull
//!
bool CsParser::getGuitar(int position)
  {
  if( position < mLine.length() ) {
    mPos = position;
    QChar flashType = mLine.at(mPos++);
    if( flashType == QChar('/') ) mNotePitch = cdGuitarUp;
    else if( flashType == QChar('\\') ) mNotePitch = cdGuitarDown;
    else if( flashType == QChar('|') ) mNotePitch = cdGuitarMute;
    else {
      skeepNonBlank();
      return false;
      }

    if( !scanDuration() )
      return false;

    return true;
    }
  return false;
  }




QString CsParser::lineType(const QString &line)
  {
  if( line.contains(':') ) {
    return line.left( line.indexOf(':') + 1 );
    }
  return QString{};
  }




void CsParser::init()
  {
  //Build drum lines
  for( int i = 0; i < 100; i++ ) {
    QString drum( QString("d%1:").arg(i) );
    mLineDrum.insert( drum, i );
    mLineChordNoteTime.insert( drum );
    mLineChordNote.insert( drum );
    mKeyWords.insert( drum );
    }
  mLineChordNoteLyric = mLineChordNote;
  mLineChordNoteLyric.insert( fieldLyric );

  mGuitarChords.insert( csChordCode( noteA, chordMaj ), csGuitarChordCode( 0, 0, 2, 2, 2, 0 ) );
  mGuitarChords.insert( csChordCode( noteA, chordMin ), csGuitarChordCode( 0, 0, 2, 2, 1, 0 ) );
  mGuitarChords.insert( csChordCode( noteA, chordMaj7 ), csGuitarChordCode( 0, 0, 2, 2, 2, 3 ) );
  mGuitarChords.insert( csChordCode( noteA, chordMin7 ), csGuitarChordCode( 0, 0, 2, 0, 1, 0 ) );

  mGuitarChords.insert( csChordCode( noteAd, chordMaj ), csGuitarChordCode( 0, 0, 3, 3, 3, 1 ) );
  mGuitarChords.insert( csChordCode( noteAd, chordMin ), csGuitarChordCode( 0, 0, 3, 3, 2, 1 ) );
  mGuitarChords.insert( csChordCode( noteAd, chordMaj7 ), csGuitarChordCode( 0, 1, 3, 3, 3, 4 ) );
  mGuitarChords.insert( csChordCode( noteAd, chordMin7 ), csGuitarChordCode( 0, 0, 3, 3, 2, 4 ) );

  mGuitarChords.insert( csChordCode( noteB, chordMaj ), csGuitarChordCode( 2, 2, 3, 3, 3, 2 ) );
  mGuitarChords.insert( csChordCode( noteB, chordMin ), csGuitarChordCode( 0, 0, 4, 4, 3, 2 ) );
  mGuitarChords.insert( csChordCode( noteB, chordMaj7 ), csGuitarChordCode( 0, 2, 1, 2, 2, 2 ) );
  mGuitarChords.insert( csChordCode( noteB, chordMin7 ), csGuitarChordCode( 2, 2, 4, 2, 3, 2 ) );

  mGuitarChords.insert( csChordCode( noteC, chordMaj ), csGuitarChordCode( 0, 3, 2, 0, 1, 0 ) );
  mGuitarChords.insert( csChordCode( noteC, chordMin ), csGuitarChordCode( 3, 3, 5, 5, 4, 3 ) );
  mGuitarChords.insert( csChordCode( noteC, chordMaj7 ), csGuitarChordCode( 0, 3, 2, 3, 1, 0 ) );
  mGuitarChords.insert( csChordCode( noteC, chordMin7 ), csGuitarChordCode( 0, 0, 1, 3, 1, 3 ) );

  mGuitarChords.insert( csChordCode( noteCd, chordMaj ), csGuitarChordCode( 0, 0, 3, 1, 2, 1 ) );
  mGuitarChords.insert( csChordCode( noteCd, chordMin ), csGuitarChordCode( 0, 0, 2, 1, 2, 0 ) );
  mGuitarChords.insert( csChordCode( noteCd, chordMaj7 ), csGuitarChordCode( 0, 0, 3, 4, 2, 4 ) );
  mGuitarChords.insert( csChordCode( noteCd, chordMin7 ), csGuitarChordCode( 0, 0, 2, 4, 2, 4 ) );

  mGuitarChords.insert( csChordCode( noteD, chordMaj ), csGuitarChordCode( 0, 0, 0, 2, 3, 2 ) );
  mGuitarChords.insert( csChordCode( noteD, chordMin ), csGuitarChordCode( 0, 0, 0, 2, 3, 1 ) );
  mGuitarChords.insert( csChordCode( noteD, chordMaj7 ), csGuitarChordCode( 0, 0, 0, 2, 1, 2 ) );
  mGuitarChords.insert( csChordCode( noteD, chordMin7 ), csGuitarChordCode( 0, 0, 0, 2, 1, 1 ) );

  mGuitarChords.insert( csChordCode( noteDd, chordMaj ), csGuitarChordCode( 0, 0, 5, 3, 4, 3 ) );
  mGuitarChords.insert( csChordCode( noteDd, chordMin ), csGuitarChordCode( 0, 0, 4, 3, 4, 2 ) );
  mGuitarChords.insert( csChordCode( noteDd, chordMaj7 ), csGuitarChordCode( 0, 0, 2, 4, 3, 4 ) );
  mGuitarChords.insert( csChordCode( noteDd, chordMin7 ), csGuitarChordCode( 0, 0, 1, 3, 2, 2 ) );

  mGuitarChords.insert( csChordCode( noteE, chordMaj ), csGuitarChordCode( 0, 2, 2, 1, 0, 0 ) );
  mGuitarChords.insert( csChordCode( noteE, chordMin ), csGuitarChordCode( 0, 2, 2, 0, 0, 0 ) );
  mGuitarChords.insert( csChordCode( noteE, chordMaj7 ), csGuitarChordCode( 0, 2, 0, 1, 0, 0 ) );
  mGuitarChords.insert( csChordCode( noteE, chordMin7 ), csGuitarChordCode( 0, 2, 0, 0, 0, 0 ) );

  mGuitarChords.insert( csChordCode( noteF, chordMaj ), csGuitarChordCode( 1, 3, 3, 2, 1, 1 ) );
  mGuitarChords.insert( csChordCode( noteF, chordMin ), csGuitarChordCode( 1, 3, 3, 1, 1, 1 ) );
  mGuitarChords.insert( csChordCode( noteF, chordMaj7 ), csGuitarChordCode( 1, 3, 1, 2, 1, 1 ) );
  mGuitarChords.insert( csChordCode( noteF, chordMin7 ), csGuitarChordCode( 1, 3, 1, 1, 1, 1 ) );

  mGuitarChords.insert( csChordCode( noteFd, chordMaj ), csGuitarChordCode( 2, 4, 4, 3, 2, 2 ) );
  mGuitarChords.insert( csChordCode( noteFd, chordMin ), csGuitarChordCode( 2, 4, 4, 2, 2, 2 ) );
  mGuitarChords.insert( csChordCode( noteFd, chordMaj7 ), csGuitarChordCode( 0, 0, 4, 3, 2, 0 ) );
  mGuitarChords.insert( csChordCode( noteFd, chordMin7 ), csGuitarChordCode( 2, 4, 2, 2, 2, 2 ) );

  mGuitarChords.insert( csChordCode( noteG, chordMaj ), csGuitarChordCode( 3, 2, 0, 0, 0, 3 ) );
  mGuitarChords.insert( csChordCode( noteG, chordMin ), csGuitarChordCode( 3, 5, 5, 3, 3, 3 ) );
  mGuitarChords.insert( csChordCode( noteG, chordMaj7 ), csGuitarChordCode( 3, 2, 0, 0, 0, 1 ) );
  mGuitarChords.insert( csChordCode( noteG, chordMin7 ), csGuitarChordCode( 3, 5, 3, 3, 3, 3 ) );

  mGuitarChords.insert( csChordCode( noteGd, chordMaj ), csGuitarChordCode( 4, 6, 6, 5, 4, 4 ) );
  mGuitarChords.insert( csChordCode( noteGd, chordMin ), csGuitarChordCode( 4, 6, 6, 4, 4, 4 ) );
  mGuitarChords.insert( csChordCode( noteGd, chordMaj7 ), csGuitarChordCode( 0, 0, 1, 1, 1, 2 ) );
  mGuitarChords.insert( csChordCode( noteGd, chordMin7 ), csGuitarChordCode( 4, 6, 4, 4, 4, 4 ) );

  }



QString CsParser::fieldValue(const QString &line)
  {
  if( line.contains(':') ) {
    QString res( line.split(':').at(1) );
    //Remove all start and end spaces
    while( res.length() && res.at(0) == QChar(' ') )
      res.removeFirst();
    while( res.length() && res.at(res.length() - 1) == QChar(' ') )
      res.removeLast();
    return res;
    }
  return QString{};
  }



int CsParser::fieldValueInt(const QString &line, int defValue, bool *ok)
  {
  QString value( fieldValue(line) );
  if( !value.isEmpty() )
    return value.trimmed().toInt( ok );
  if( ok != nullptr )
    *ok = false;
  return defValue;
  }



QString CsParser::find(const QString &lineName, QTextCursor from, QTextCursor::MoveOperation op)
  {
  QTextCursor crField = findNearestLine( [lineName] (const QString &str) -> bool { return str.startsWith(lineName); }, false, from, op );
  if( !crField.isNull() )
    return crField.block().text();
  return QString{};
  }




QString CsParser::findValue(const QString &lineName, QTextCursor from, QTextCursor::MoveOperation op)
  {
  return fieldValue( find( lineName, from, op ) );
  }




int CsParser::findValueInt(const QString &lineName, int defValue, bool *ok, QTextCursor from, QTextCursor::MoveOperation op)
  {
  return fieldValueInt( find( lineName, from, op ), defValue, ok );
  }




QTextCursor CsParser::findNearestLine(std::function<bool (const QString &)> testFunc, bool breakOnEmpty, QTextCursor from, QTextCursor::MoveOperation op )
  {
  QTextCursor cr(from);
  while( cr.movePosition( op ) ) {
    QString line(cr.block().text());
    if( !breakOnEmpty || !line.trimmed().isEmpty() ) {
      if( testFunc(line) )
        return cr;
      }
    else break;
    }
  return QTextCursor{};
  }




QTextCursor CsParser::findNearestTime(QTextCursor from)
  {
  return findNearestLine( isLineTime, true, from );
  }




QList<QTextCursor> CsParser::accumCursors(bool (*testFunc)(const QString &), bool checkPos, QTextCursor from)
  {
  QList<QTextCursor> cursorList;
  QTextCursor cr(from);
  if( testFunc(cr.block().text()) )
    cursorList.append( cr );
  int pos = cr.positionInBlock();

  //Accum in downward direction
  while( cr.movePosition( QTextCursor::Down ) ) {
    if( testFunc(cr.block().text()) ) {
      if( !checkPos || cr.positionInBlock() == pos )
        cursorList.append(cr);
      }
    else break;
    }

  //Accum in upward direction
  cr = from;
  while( cr.movePosition( QTextCursor::Up ) ) {
    if( testFunc(cr.block().text()) ) {
      if( !checkPos || cr.positionInBlock() == pos )
        cursorList.append(cr);
      }
    else break;
    }

  return cursorList;
  }




QStringList CsParser::accumLines(bool (*testFunc)(const QString &), QTextCursor from)
  {
  QList<QTextCursor> cursorList( accumCursors( testFunc, false, from ) );
  QStringList stringList;
  for( const QTextCursor &cr : std::as_const(cursorList) )
    stringList.append( cr.block().text() );
  return stringList;
  }




QList<QTextCursor> CsParser::accumChordNoteTime(bool checkPos, QTextCursor from)
  {
  return accumCursors( isLineTimeChordNote, checkPos, from );
  }



QList<QTextCursor> CsParser::collectCursors(bool (*testFunc)(const QString &), QTextCursor from, QTextCursor::MoveOperation op)
  {
  QList<QTextCursor> cursorList;
  QTextCursor cr(from);
  if( testFunc(cr.block().text()) )
    cursorList.append( cr );

  //Accum in needed direction
  while( cr.movePosition( op ) ) {
    if( testFunc(cr.block().text()) ) {
      cursorList.append(cr);
      }
    }

  return cursorList;
  }





QStringList CsParser::collectLines(bool (*testFunc)(const QString &), QTextCursor from, QTextCursor::MoveOperation op)
  {
  QList<QTextCursor> cursorList( collectCursors( testFunc, from, op ) );
  QStringList stringList;
  for( const QTextCursor &cr : std::as_const(cursorList) )
    stringList.append( cr.block().text() );
  return stringList;
  }
