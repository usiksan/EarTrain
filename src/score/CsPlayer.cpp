#include "CsConfig.h"
#include "CsPlayer.h"
#include "CsNoteChord.h"
#include "CsParser.h"
#include "CsComposition.h"

#include <QTextBlock>
#include <QElapsedTimer>

//                   cords         1                     2                    3                    4                    5                    6
static const int guitarOpen[6] = { noteE + octaveSecond, noteB + octaveFirst, noteG + octaveFirst, noteD + octaveFirst, noteA + octaveSmall, noteE + octaveSmall };



CsPlayer::CsPlayer(QObject *parent)
  : CsSongParser{parent}
  , mTickCount(-1)
  , mLoop(false)
  , mTrainLevel(0)
  {
  mGuitarPrevCords[0] = mGuitarPrevCords[1] = mGuitarPrevCords[2] = mGuitarPrevCords[3] = mGuitarPrevCords[4] = mGuitarPrevCords[5] = 0;
  }





//!
//! \brief play     Play current single note line
//! \param songPath Path to song to use as base for audioFile
//! \param from     Cursor at none line
//! \param loop     when true play after end begins from start line
//!
void CsPlayer::play(const QString &songPath, QTextCursor from, bool loop)
  {
  mLoop = loop;

  int startTickIndex = 0;
  if( parseSingleLine( from, &startTickIndex ) ) {
    setTempo( from, true );
    mTickCount = mTickBegin = 0;
    }
  }




//!
//! \brief playSync Play full block where current line resides
//! \param songPath Path to song to use as base for audioFile
//! \param from     Cursor at none line
//!
void CsPlayer::playSync(const QString &songPath, QTextCursor from )
  {
  mLoop = false;

  int startTickIndex = 0;
  if( parseMultiLine( from, &startTickIndex ) ) {
    setTempo( from, true );
    mTickCount = mTickBegin = 0;
    }

  }




//!
//! \brief playAll  Play full song (parse, build play events and play them)
//! \param songPath Path to song to use as base for audioFile
//! \param doc      Pointer to song
//!
void CsPlayer::playAll(const QString &songPath, const QTextDocument *doc)
  {
  if( parse( songPath, doc ) ) {
    setTempo( QTextCursor(doc->firstBlock()), false );
    mTickCount = mTickBegin = 0;
    }
  }




//!
//! \brief play       Play fragment from begin tick to end tick
//! \param begin      Begin tick of fragment
//! \param end        End tick of fragment
//! \param trainLevel Train level: 0 - simple play, 1 - play with highlight right and wrong user key pressing, 2 - wait for right key pressing
//! \param loop       when true play after end begins from start of fragment
//!
void CsPlayer::play(int begin, int end, int trainLevel, bool loop)
  {
  mTrainLevel = trainLevel;
  mLoop = loop;
  mTickEnd = end;
  mTickCount = mTickBegin = begin;
  }




//!
//! \brief playFragment Play fragment from begin score cell index to end score cell index
//! \param cellBegin    Begin score cell index
//! \param cellEnd      End score cell index
//! \param trainLevel   Train level: 0 - simple play, 1 - play with highlight right and wrong user key pressing, 2 - wait for right key pressing
//! \param loop         when true play after end begins from start of fragment
//!
void CsPlayer::playFragment(int cellBegin, int cellEnd, int trainLevel, bool loop)
  {
  //Setup song tempo
  emit playTempo( mTempo );
  //Start play fragment
  if( cellBegin < 0 )
    play( 0, mTickMax, trainLevel, loop );
  else {
    play( mCellList.at(cellBegin).tickPosition(), mCellList.at(cellEnd).tickEnd(), trainLevel, loop );
    }
  }




void CsPlayer::setTempo(QTextCursor from, bool backward)
  {
  mTempo = CsParser::findValueInt( QString("Tempo:"), 120, nullptr, from, backward ? QTextCursor::Up : QTextCursor::Down );
  emit playTempo( mTempo );
  }




void CsPlayer::tick(int count)
  {
  static QElapsedTimer tt;
  static int tcc = 0;
  if( tcc == 0 ) tt.start();
  if( tcc >= 256 ) {
    qDebug() << "---- period" << tt.elapsed();
    tt.restart();
    tcc = 0;
    }
  tcc += count;

  if( count > 0 && mTickCount >= mTickBegin && mTickCount < mTickEnd ) {
    while( count-- ) {
      //At first we check midi voice
      QList<CsMidiVoice> voiceList = mVoiceMap.values(mTickCount);
      for( const CsMidiVoice &voice : std::as_const(voiceList) ) {
        emit playVoice( voice.mChannel, voice.mVoiceId );
        emit playVolume( voice.mChannel, voice.mVolume );
        //emit playTempo();
        }

      //At second we play notes
      QList<CsMidiEvent> eventList = mEventMap.values(mTickCount);
      for( CsMidiEvent ev : std::as_const(eventList) ) {
        if( ev.mPitch >= cdGuitarCord1 && ev.mPitch <= cdGuitarCord6 ) {
          //Guitar codes
          int cordIndex = ev.mPitch - cdGuitarCord1;
          //Mute previous cord note
          emit playNote( ev.mChannel, mGuitarPrevCords[cordIndex], 0 );
          if( ev.mLevel ) {
            //Next cord note
            emit playNote( ev.mChannel, mGuitarPrevCords[cordIndex] = mGuitarNextCords[cordIndex], ev.mLevel );
            }
          }
        else {
          if( ev.mChannel == 0 ) {
            if( mTrainLevel == 0 ) {
              //Play note and highlight on keyboard
              emit playNote( ev.mChannel, ev.mPitch, ev.mLevel );
              emit pianoKeySet( ev.mPitch, ev.mLevel == 0 ? CsComposition::CsPcInactive : CsComposition::CsPcBlue );
              }
            else {
              if( ev.mLevel ) {
                if( mPlayKbdPitch.contains( ev.mPitch ) ) {
                  mPlayKbdPitch.remove( ev.mPitch );
                  emit pianoKeySet( ev.mPitch, CsComposition::CsPcGreen );
                  }
                else {
                  mPlayWaitPitch.insert( ev.mPitch );
                  emit pianoKeySet( ev.mPitch, CsComposition::CsPcBlue );
                  }
                }
              else {
                if( mTrainLevel == 2 ) {
                  //Wait for user press right key
                  if( mPlayWaitPitch.contains( ev.mPitch ) ) return;
                  }
                mPlayWaitPitch.remove( ev.mPitch );
                emit pianoKeySet( ev.mPitch, CsComposition::CsPcInactive );
                }
              }
            }
          else
            emit playNote( ev.mChannel, ev.mPitch, ev.mLevel );
          }
        }

      //At on end we check cell
      emit cellTick( mTickCount );

      mTickCount++;
      }


    if( mTickCount >= mTickEnd ) {
      //Reached end of play
      if( mLoop ) {
        mTickCount = mTickBegin;
        }
      else {
        //Stop all guitar
        for( int i = 0; i < 6; i++ )
          emit playNote( CS_GUITAR_CHANNEL, mGuitarPrevCords[i], 0 );
        //Clear all training notes
        mPlayKbdPitch.clear();
        mPlayWaitPitch.clear();
        }

      //Hide active cells
      emit cellTick( -1 );
      }
    }
  }




void CsPlayer::guitarPlayChord(int chordCode)
  {
  int guitarCode = CsParser::mGuitarChords.value( chordCode, 0 );
  // if( guitarCode == 0 )
  //   return;
  for( int i = 0; i < 6; i++ ) {
    //qDebug() << guitarOpen[i] + (guitarCode & 0xf);
    mGuitarNextCords[i] = guitarOpen[i] + (guitarCode & 0xf);
    guitarCode >>= 4;
    }
  }



void CsPlayer::kbdNote(int channel, int pitch, int velo)
  {
  Q_UNUSED(channel)
  if( mTrainLevel ) {
    if( velo ) {
      if( mPlayWaitPitch.contains(pitch) ) {
        mPlayWaitPitch.remove(pitch);
        emit pianoKeySet( pitch, CsComposition::CsPcGreen );
        }
      else {
        mPlayKbdPitch.insert(pitch);
        //Wrong key
        emit pianoKeySet( pitch, CsComposition::CsPcRed );
        }
      }
    else {
      emit pianoKeySet( pitch, CsComposition::CsPcInactive );
      if( mPlayKbdPitch.contains(pitch) )
        mPlayKbdPitch.remove(pitch);
      }
    }

  }


