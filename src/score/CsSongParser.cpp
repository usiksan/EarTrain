#include "CsSongParser.h"
#include "CsParser.h"
#include "CsViewSettings.h"

#include <QTextBlock>

CsSongParser::CsSongParser(QObject *parent)
  : CsViewSettings{parent}
  , mTickMax(0)
  , mTickBegin(0)
  , mTickEnd(0)
  , mLineYPos(0)
  {

  }



//!
//! \brief fragmentSize Calculate count of CsCtSelector cells beatween from and to
//! \param from         Cell index from which count including
//! \param to           Cell index to witch count excluding
//! \return             Count of CsCtSelector cells
//!
int CsSongParser::fragmentSize(int from, int to) const
  {
  int count = 0;
  for( int i = from; i < to && i < mCellList.count(); i++ )
    if( mCellList.at(i).cellType() == CsCtSelector ) count++;
  return count;
  }




//!
//! \brief fragmentPosition Calculates the end position of a fragment separated from the specified position by a specified number of CsCtSelector cells
//! \param from             the initial position of the fragment
//! \param count            Number of CsCtSelector cells
//! \param forwardDirection When true then fragment selected in forward direction
//! \return                 Cell index of the end position of fragment
//!
int CsSongParser::fragmentPosition(int from, int count, bool forwardDirection ) const
  {
  if( forwardDirection ) {
    //Find in forward direction
    for( int i = from; i < mCellList.count(); i++ )
      if( mCellList.at(i).cellType() == CsCtSelector ) {
        count--;
        if( count == 0 ) return i;
        }
    return mCellList.count();
    }
  //Find in backward direction
  for( int i = from; i >= 0; i-- )
    if( mCellList.at(i).cellType() == CsCtSelector ) {
      count--;
      if( count == 0 ) return i;
      }
  return -1;
  }




//!
//! \brief parse    Parse full song with building score cells and play events
//! \param songPath Path to song to use as base for audioFile
//! \param doc      Pointer to text document for parsing
//! \return         true if parsing successfull
//!
bool CsSongParser::parse(const QString &songPath, const QTextDocument *doc)
  {
  mEventMap.clear();
  mVoiceMap.clear();

  mCellList.clear();
  mCellActivate.clear();
  mCellDeactivate.clear();

  mTempo = 120;
  mWidth = 1000;
  mHeight = mMargin;
  bool prefixBlock = true;

  //Go to top of doc
  QTextCursor cr( doc->firstBlock() );
  cr.movePosition( QTextCursor::Start );
  CsTimeLineInfo notePositions;
  while( true ) {
    if( cr.isNull() ) break;
    QString line(cr.block().text());
    if( CsParser::isLineTime(line) ) {
      prefixBlock = false;
      //For each time line we accum sync lines
      notePositions.parse( line );
      if( !cr.movePosition( QTextCursor::Down ) ) break;
      if( notePositions.count() ) {
        mHeight += mLineSpacing;
        QStringList lineList( CsParser::accumLines( CsParser::isLineChordNoteLyric, cr ) );
        parseLineList( lineList, notePositions, cr );

        notePositions.nextTimeLine();

        //Correction width of score as max width of song line
        int width = widthOfDuration( notePositions.lineDuration() ) + leftGap() + mMargin;
        if( width > mWidth )
          mWidth = width;
        }
      }
    else {
      if( prefixBlock ) {
        if( CsParser::isLineField( line, fieldTitle ) ) {
          QString text( CsParser::fieldValue(line) );
          mCellList.append( CsCell( CsCtText, 0, mHeight, mWidth, mTitleTextSize, mTitleTextSize, 1, text ) );
          mHeight += mTitleTextSize + mFieldSpacing;
          }
        else if( CsParser::isLineField( line, fieldArtist ) ) {
          QString text( CsParser::fieldValue(line) );
          mCellList.append( CsCell( CsCtText, 0, mHeight, mWidth, mFieldTextSize, mFieldTextSize, 0, tr("Артист: ") + text ) );
          mHeight += mFieldTextSize + mFieldSpacing;
          }
        else if( CsParser::isLineField( line, fieldComposer ) ) {
          QString text( CsParser::fieldValue(line) );
          mCellList.append( CsCell( CsCtText, 0, mHeight, mWidth, mFieldTextSize, mFieldTextSize, 0, tr("Композитор: ") + text ) );
          mHeight += mFieldTextSize + mFieldSpacing;
          }
        else if( CsParser::isLineField( line, fieldWords ) ) {
          QString text( CsParser::fieldValue(line) );
          mCellList.append( CsCell( CsCtText, 0, mHeight, mWidth, mFieldTextSize, mFieldTextSize, 0, tr("Слова: ") + text ) );
          mHeight += mFieldTextSize + mFieldSpacing;
          }
        else if( CsParser::isLineField( line, fieldTempo ) ) {
          QString text( CsParser::fieldValue(line) );
          mTempo = CsParser::fieldValueInt( line, 120 );
          mCellList.append( CsCell( CsCtText, 0, mHeight, mWidth, mFieldTextSize, mFieldTextSize, 0, tr("Темп: ") + text ) );
          mHeight += mFieldTextSize + mFieldSpacing;
          }
        else if( CsParser::isLineField( line, fieldSynthesizerProgram ) ) {
          QString text( CsParser::fieldValue(line) );
          mCellList.append( CsCell( CsCtText, 0, mHeight, mWidth, mFieldTextSize, mFieldTextSize, 0, tr("Программа: ") + text ) );
          mHeight += mFieldTextSize + mFieldSpacing;
          }
        else if( CsParser::isLineField( line, fieldSynthesizerStyle ) ) {
          QString text( CsParser::fieldValue(line) );
          mCellList.append( CsCell( CsCtText, 0, mHeight, mWidth, mFieldTextSize, mFieldTextSize, 0, tr("Стиль: ") + text ) );
          mHeight += mFieldTextSize + mFieldSpacing;
          }
        else if( CsParser::isLineField( line, fieldAudioFile ) ) {
          QString text( CsParser::fieldValue(line) );
          mCellList.append( CsCell( CsCtText, 0, mHeight, mWidth, mFieldTextSize, mFieldTextSize, 0, tr("Аудио файл: ") + text ) );
          mHeight += mFieldTextSize + mFieldSpacing;
          }
        }
      if( !cr.movePosition( QTextCursor::Down ) ) break;
      }
    }
  mHeight += mLineSpacing;
  mTickMax = mTickEnd = notePositions.lineStart();
  return notePositions.lineStart() != 0;
  }







//!
//! \brief parseSingleLine Parse current note line only
//! \param from            Cursor at note line
//! \param pStartTick      Start tick index of this line begin
//! \return                true if parse successfull
//!
bool CsSongParser::parseSingleLine( QTextCursor from, int *pStartTick )
  {
  if( from.isNull() ) return false;

  CsTimeLineInfo notePositions;
  notePositions.blockParse( from, pStartTick );

  if( notePositions.count() ) {
    mEventMap.clear();
    mVoiceMap.clear();

    QString line(from.block().text());
    if( CsParser::isLineDrum(line) )
      parseLineList( CsParser::accumLines( CsParser::isLineDrum, from ), notePositions, from );
    else
      parseLineList( {line}, notePositions, from );

    if( !mEventMap.isEmpty() ) {
      mTickMax = mTickEnd = notePositions.lineDuration();
      mTickBegin = 0;
      return true;
      }
    }
  return false;
  }




//!
//! \brief parseMultiLine Parse block of note lines where current line resides
//! \param from           Cursor at any line of block
//! \param pStartTick     Start tick index of this line begin
//! \return               true if parse successfull
//!
bool CsSongParser::parseMultiLine( QTextCursor from, int *pStartTick )
  {
  if( from.isNull() ) return false;

  CsTimeLineInfo notePositions;
  notePositions.blockParse( from, pStartTick );

  if( notePositions.count() ) {

    mEventMap.clear();
    mVoiceMap.clear();

    parseLineList( CsParser::accumLines( CsParser::isLineChordNote, from ), notePositions, from );

    if( !mEventMap.isEmpty() ) {
      mTickMax = mTickEnd = notePositions.lineDuration();
      mTickBegin = 0;
      return true;
      }
    }
  return false;
  }











//!
//! \brief parseLyricIndexesOfPositions Parse positions of lyric
//! \param notePositions                Decoded note positions from time line
//! \return                             Indexes for note positions in notePositions where lyric
//!
QList<int> CsSongParser::parseLyricIndexesOfPositions(const CsTimeLineInfo &notePositions) const
  {
  QList<int> lyricPositions;
  for( int i = 0; i < notePositions.count(); i++ )
    if( notePositions.isAnchor(i) ) lyricPositions.append(i);
  return lyricPositions;
  }








void CsSongParser::parseLineList(const QStringList &lineList, CsTimeLineInfo &notePositions, QTextCursor from)
  {
  //Prepare calculation takt lines
  int heightStart = mHeight;
  int heightStop = mHeight;

  //Channel init before song line
  QSet<int> channelInit;

  //Parse each line of song line
  for( const QString &line : std::as_const(lineList) ) {
    if( CsParser::isLineBass(line) ) {
      if( !channelInit.contains(CS_BASS_CHANNEL) ) {
        CsMidiVoice voice( CS_BASS_CHANNEL, CsParser::findValueInt( QString("Bass volume:"), 127, nullptr, from ), CsParser::findValueInt( QString("Bass program:"), CS_DEFAULT_BASS, nullptr, from ) );
        mVoiceMap.insert( notePositions.lineStart(), voice );
        channelInit.insert(CS_BASS_CHANNEL);
        }
      parseNoteLine( line, notePositions, CS_BASS_CHANNEL );
      }


    else if( CsParser::isLinePad(line) ) {
      if( !channelInit.contains(CS_PAD_CHANNEL) ) {
        CsMidiVoice voice( CS_PAD_CHANNEL, CsParser::findValueInt( QString("Pad volume:"), 127, nullptr, from ), CsParser::findValueInt( QString("Pad program:"), CS_DEFAULT_PAD, nullptr, from ) );
        mVoiceMap.insert( notePositions.lineStart(), voice );
        channelInit.insert(CS_PAD_CHANNEL);
        }
      parseNoteLine( line, notePositions, CS_PAD_CHANNEL );
      }


    else if( CsParser::isLineNote(line) ) {
      int channel( line.at(1).isDigit() ? line.at(1).digitValue() : 0 );
      if( !channelInit.contains(channel) ) {
        CsMidiVoice voice;
        if( channel )
          voice = CsMidiVoice( channel, CsParser::findValueInt( QString("Synthesizer volume ch%1:").arg(channel), 127, nullptr, from), CsParser::findValueInt( QString("Synthesizer program ch%1:").arg(channel), 0, nullptr, from ) );
        else
          voice = CsMidiVoice( channel, CsParser::findValueInt( QString("Synthesizer volume:"), 127, nullptr, from), CsParser::findValueInt( QString("Synthesizer program:"), 0, nullptr, from ) );
        mVoiceMap.insert( notePositions.lineStart(), voice );
        channelInit.insert( channel );
        }
      parseNoteLine( line, notePositions, channel );
      }


    else if( CsParser::isLineDrum(line) ) {
      if( !channelInit.contains(CS_DRUM_CHANNEL) ) {
        CsMidiVoice voice( CS_DRUM_CHANNEL, CsParser::findValueInt( QString("Drum volume:"), 127, nullptr, from ), CsParser::findValueInt( QString("Drum program:"), CS_DEFAULT_DRUM, nullptr, from ) );
        mVoiceMap.insert( notePositions.lineStart(), voice );
        channelInit.insert( CS_DRUM_CHANNEL );
        }
      parseDrumLine( line, notePositions, CS_DRUM_CHANNEL );
      }

    else if( CsParser::isLineGuitar(line) ) {
      if( !channelInit.contains(CS_GUITAR_CHANNEL) ) {
        CsMidiVoice voice( CS_GUITAR_CHANNEL, CsParser::findValueInt( QString("Guitar volume:"), 127, nullptr, from ), CsParser::findValueInt( QString("Guitar program:"), CS_DEFAULT_GUITAR, nullptr, from ) );
        mVoiceMap.insert( notePositions.lineStart(), voice );
        channelInit.insert(CS_GUITAR_CHANNEL);
        }
      parseGuitar( line, notePositions, CS_GUITAR_CHANNEL );
      }

    else if( CsParser::isLineChord(line) ) {
      parseChord( line, notePositions );
      }

    else if( CsParser::isLineLyric(line) ) {
      heightStop = mHeight;
      parseLyric( line, notePositions );
      }

    else if( CsParser::isLineFinger(line) ) {
      parseFinger( line, notePositions );
      }

    else continue;
    mHeight += mInLineSpacing;
    }

  if( heightStop == heightStart )
    heightStop = mHeight;

  //Append takt lines
  for( int tick = 0; tick < notePositions.count(); tick++ )
    if( notePositions.isTakt(tick) )
      mCellList.append( CsCell(CsCtTaktLine, leftGap() + widthOfDuration(notePositions.startFromLine(tick)) - 1, heightStart, 1, heightStop - heightStart, 0, 0, QString{} ) );

  //Append selector
  int tickDuration = notePositions.lineDuration();
  mCellList.append( CsCell(CsCtSelector, mMargin, heightStart, leftGap() + widthOfDuration(tickDuration), mHeight - heightStart, notePositions.lineStart(), tickDuration ) );

  //Append marker
  mCellActivate.insert( notePositions.lineStart(), mCellList.count() );
  mCellDeactivate.insert( notePositions.lineStart() + tickDuration - 1, mCellList.count() );
  mCellList.append( CsCell(CsCtMarker, leftGap(), heightStart, widthOfDuration(tickDuration), mHeight - heightStart, notePositions.lineStart(), tickDuration ) );
  }




//!
//! \brief parseNoteLine Parse note line, decode notes and fill mEventMap with them. Also we fill mCellList to visualise notes
//! \param lineNote      Text line with notes
//! \param notePositions Time line note positions info
//! \param channel       Channel for notes
//!
void CsSongParser::parseNoteLine(const QString &lineNote, CsTimeLineInfo &notePositions, int channel)
  {
  //Append clef and score lines
  if( mViewMode == CsVmClassicNote ) {
    if( lineNote.at(1) == QChar('f') )
      //F-clef
      mCellList.append( CsCell(CsCtClef, mMargin, mHeight,
                               mLeftGapClassicNote + widthOfDuration( notePositions.lineDuration() ) + 10, mClassicNoteHeight, mClassicNoteSize, 13, "𝄢" ) );
    else
      //G-clef
      mCellList.append( CsCell(CsCtClef, mMargin, mHeight,
                               mLeftGapClassicNote + widthOfDuration( notePositions.lineDuration() ) + 10, mClassicNoteHeight, mClassicNoteSize, 13, "𝄞" ) );

    }
  CsParser parser( lineNote, 0 );
  CsMidiEvent ev;
  ev.mChannel = channel;
  for( int index = 0; index < notePositions.count(); index++ ) {
    int pos = notePositions.position(index);
    if( pos >= lineNote.length() )
      break;

    if( parser.getNote(pos) ) {
      //Note present, set lyric anchor to this position
      notePositions.setAnchor(index);
      //Calculate note duration as count of step with empty note position
      int duration = 1;
      for( int idx = index + 1; idx < notePositions.count(); idx++ ) {
        //Get note position
        int pos = notePositions.position(idx);
        //Check if position inside note line
        if( pos < lineNote.length() ) {
          //Check if position is empty
          if( lineNote.at(pos) != QChar(' ') ) break;
          }
        duration++;
        }
      //Note duration
      duration = notePositions.duration( index, index + duration );

      ev.mPitch = parser.notePitch();
      if( ev.mPitch != noteMute ) {
        ev.mLevel = 120;
        mEventMap.insert( notePositions.start(index), ev );
        ev.mLevel = 0;
        mEventMap.insert( notePositions.start(index) + duration - 1, ev );
        }

      mCellActivate.insert( notePositions.start(index), mCellList.count() );
      mCellDeactivate.insert( notePositions.start(index) + duration - 1, mCellList.count() );

      if( mViewMode == CsVmClassicNote ) {
        mCellList.append( CsCell(CsCtClassicNote, leftGap() + widthOfDuration( notePositions.startFromLine(index) ), mHeight,
                                 widthOfDuration(duration) - 2, mClassicNoteHeight,
                                 mClassicNoteSize, true, ev.mPitch, duration ) );
        }
      else {
        //Digital notes
        mCellList.append( CsCell(CsCtDigitalNote, leftGap() + widthOfDuration( notePositions.startFromLine(index) ), mHeight,
                                 widthOfDuration(duration) - 2, mDigitNoteSize,
                                 mDigitNoteSize - 2, 1,
                                 CsCell::classicNote( mViewMode == CsVmDigitNote ? 0 : 2, ev.mPitch, duration ) ));
        }
      }
    }
  if( mViewMode == CsVmClassicNote )
    mHeight += mClassicNoteHeight;
  else
    mHeight += mDigitNoteSize;
  }





//!
//! \brief parseDrumLine Parse drum line, decode drum shots and fill mEventMap with them. Also we fill mCellList to visualise shots
//! \param line          Text line with shots
//! \param notePositions Time line note positions info
//! \param channel       Channel for drum
//!
void CsSongParser::parseDrumLine(const QString &line, const CsTimeLineInfo &notePositions, int channel)
  {
  CsMidiEvent ev;
  ev.mChannel = channel;
  ev.mPitch = CsParser::drumIndex( line );
  for( int index = 0; index < notePositions.count(); index++ ) {
    int pos = notePositions.position(index);
    if( pos >= line.length() )
      break;

    if( line.at(pos) != QChar(' ') ) {
      ev.mLevel = 120;
      mEventMap.insert( notePositions.start(index), ev );
      ev.mLevel = 0;
      mEventMap.insert( notePositions.start(index) + CS_DRUM_DURATION - 1, ev );
      }
    }
  }




//!
//! \brief parseGuitar   Parse guitar line, decode guitar shots and fill mEventMap with them.
//! \param lineNote      Text line with guitar shots
//! \param notePositions Time line note positions info
//! \param channel       Channel for guitar
//!
void CsSongParser::parseGuitar(const QString &lineNote, const CsTimeLineInfo &notePositions, int channel)
  {
  CsParser parser( lineNote, 0 );
  CsMidiEvent ev;
  ev.mChannel = channel;
  for( int index = 0; index < notePositions.count(); index++ ) {
    int pos = notePositions.position(index);
    if( pos >= lineNote.length() )
      break;

    if( parser.getGuitar(pos) ) {
      switch( parser.flashType() ) {
        case cdGuitarUp :
          ev.mLevel = 120;
          for( int i = 0; i < 6; i++ ) {
            ev.mPitch = cdGuitarCord1 + i;
            mEventMap.insert( notePositions.start(index) + i * parser.flashInterval() / 6, ev );
            }
          break;

        case cdGuitarDown :
          ev.mLevel = 120;
          for( int i = 0; i < 6; i++ ) {
            ev.mPitch = cdGuitarCord6 - i;
            mEventMap.insert( notePositions.start(index) + i * parser.flashInterval() / 6, ev );
            }
          break;

        case cdGuitarMute :
          ev.mLevel = 0;
          for( int i = 0; i < 6; i++ ) {
            ev.mPitch = cdGuitarCord1 + i;
            mEventMap.insert( notePositions.start(index), ev );
            }
          break;
        }
      }
    }
  }






//!
//! \brief parseLyric    Parse lyric line and fill mCellList to visualise lyric
//! \param lyric         Lyric text line
//! \param notePositions Time line note positions info
//!
void CsSongParser::parseLyric(const QString &lyric, const CsTimeLineInfo &notePositions)
  {
  QList<int> lyricIndexes( parseLyricIndexesOfPositions( notePositions ) );

  if( !lyric.isEmpty() ) {
    if( lyricIndexes.count() ) {
      for( int i = 0; i < lyricIndexes.count(); i++ ) {
        //Index of note position in notePositions
        int posIndex = lyricIndexes.at(i);
        //Position of note in text line
        int pos = notePositions.position( posIndex );
        //If position outside lyric line we stop loop
        if( pos >= lyric.length() ) break;

        //This is next note position in text line
        int nextPos = i + 1 < lyricIndexes.count() ? notePositions.position( lyricIndexes.at(i+1) ) : -1;

        //Next note index
        int nextNotePosIndex = i + 1 < lyricIndexes.count() ? lyricIndexes.at(i+1) : notePositions.count();

        //Find prev word
        int leftPart = leftPos( lyric, pos );
        if( leftPart < pos ) {
          //Append lyric highlight
          mCellActivate.insert( notePositions.start(posIndex), mCellList.count() );
          mCellDeactivate.insert( notePositions.start(nextNotePosIndex) - 1, mCellList.count() );

          //Append lyric itself
          mCellList.append( CsCell( CsCtText, widthOfDuration( notePositions.startFromLine(posIndex) ) + leftGap(), mHeight, 1, mLyricSize, mLyricSize, 2, lyric.mid( leftPart, pos - leftPart ) ) );
          }


        //Find next word
        if( nextPos >= 0 )
          nextPos = leftPos( lyric, nextPos );
        QString part = lyric.mid( pos, nextPos < 0 ? -1 : nextPos - pos ).simplified();
        if( !part.isEmpty() ) {
          //Append lyric highlight
          mCellActivate.insert( notePositions.start(posIndex), mCellList.count() );
          mCellDeactivate.insert( notePositions.start(nextNotePosIndex) - 1, mCellList.count() );

          //Append lyric itself
          mCellList.append( CsCell( CsCtText, widthOfDuration( notePositions.startFromLine(posIndex) ) + leftGap(), mHeight, 1, mLyricSize, mLyricSize, 0, part ) );
          }
        }
      }
    else {
      //Append lyric as single line
      mCellList.append( CsCell( CsCtText, leftGap(), mHeight, mWidth, mLyricSize, mLyricSize, 0, lyric ) );
      }
    mHeight += mLyricSize;
    }
  }





void CsSongParser::parseChord(const QString &chord, CsTimeLineInfo &notePositions)
  {
  for( int i = 0; i < notePositions.count(); i++ ) {
    int pos = notePositions.position(i);
    if( pos >= chord.length() ) break;

    if( chord.at(pos).isSpace() ) continue;
    notePositions.setAnchor(i);
    if( chord.at(pos) == '-' ) continue;

    //Accum nonspace symbols
    int last = pos + 1;
    while( last < chord.length() && !chord.at(last).isSpace() )
      last++;

    //Append chord highlight
    mCellActivate.insert( notePositions.start(i), mCellList.count() );
    mCellDeactivate.insert( notePositions.start(i + 1) - 1, mCellList.count() );

    //Append chord itself
    QString c( chord.mid( pos, last - pos).simplified() );
    mCellList.append( CsCell(CsCtText, leftGap() + widthOfDuration( notePositions.startFromLine(i) ), mHeight, 1, mChordSize, mChordSize, 0, c ) );
    }
  mHeight += mChordSize;
  }




void CsSongParser::parseFinger(const QString &lineFinger, const CsTimeLineInfo &notePositions)
  {
  for( int index = 0; index < notePositions.count(); index++ ) {
    int pos = notePositions.position(index);
    if( pos >= lineFinger.length() )
      break;

    QChar ch = lineFinger.at(pos);
    if( ch.isDigit() && ch.toLatin1() > '0' && ch.toLatin1() < '6' ) {
      //Right finger digit. Append
      mCellList.append( CsCell(CsCtText, leftGap() + widthOfDuration( notePositions.startFromLine(index) ), mHeight,
                               widthOfDuration( notePositions.duration(index) ) - 2, mFingerTextSize,
                               mFingerTextSize - 2, 1, QString(ch) ));
      }
    }
  mHeight += mFingerTextSize;
  }



int CsSongParser::leftPos(const QString &line, int pos) const
  {
  if( pos >= line.length() ) return pos;
  int leftPart;
  for( leftPart = pos - 1; leftPart >= 0 && !line.at(leftPart).isSpace() && line.at(leftPart) != QChar('-'); leftPart-- );
  leftPart++;
  return leftPart;
  }




