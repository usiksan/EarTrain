#include "CsMidiKeyboard.h"

#include <QDebug>
#include <QSettings>

#define KEY_MIDI_PLAY_ENA "MidiKeyboardPlayEna"


CsMidiKeyboard::CsMidiKeyboard(QObject *parent)
  : QObject{parent}
  , mBank(0)
  , mPlayEna(true)
  {
  QSettings s;
  mPlayEna = s.value( KEY_MIDI_PLAY_ENA, true ).toBool();
  }


void CsMidiKeyboard::playEnaSet(bool newPlayEna)
  {
  if( mPlayEna == newPlayEna )
    return;
  mPlayEna = newPlayEna;
  emit playEnaChanged();
  QSettings s;
  s.setValue( KEY_MIDI_PLAY_ENA, mPlayEna );
  }




void CsMidiKeyboard::playRun(bool run)
  {
  quint8 buf[2];
  buf[0] = run ? 0xfa : 0xfc;
  midiSend( 1, buf );
  }




void CsMidiKeyboard::playNote(int channel, int note, int velo)
  {
  qDebug() << "Note" << note << "velo" << velo;
  if( !mPlayEna || note == 0 ) return;
  quint8 buf[3];
  buf[0] = 0x90 | (channel & 0xf);
  buf[1] = note & 0x7f;
  buf[2] = velo & 0x7f;
  midiSend( 3, buf );
  }




void CsMidiKeyboard::playTempo(int tempo)
  {
  if( !mPlayEna ) return;

  }



void CsMidiKeyboard::playVoice(int channel, int voice)
  {
  if( !mPlayEna ) return;
  if( mVoiceMap.value(channel, -1) == voice ) return;
  mVoiceMap.insert( channel, voice );
  //Bank high part
  quint8 buf[3];
  buf[0] = 0xb0 | (channel & 0xf);
  buf[1] = 0;
  buf[2] = (voice >> 14) & 0x7f;
  midiSend( 3, buf );

  //Bank low part
  buf[1] = 0x20;
  buf[2] = (voice >> 7) & 0x7f;
  midiSend( 3, buf );

  //Program
  buf[0] = 0xc0 | (channel & 0xf);
  buf[1] = (voice) & 0x7f;
  midiSend( 2, buf );
  }




void CsMidiKeyboard::midiSignal( quint8 control, quint8 data0, quint8 data1 )
  {
  switch( control & 0xf0 ) {
    case 0 :
      //Note off
      emit midiNote( control & 0xf, data0, 0 );
      break;

    case 0x10 :
      //Note on
      emit midiNote( control & 0xf, data0, data1 );
      break;

    case 0x30 :
      //Controller
      if( data0 == 0 )
        //MSB bank
        mBank = (data1 << 7) | (mBank & 0x7f);
      else if( data0 == 0x20 )
        mBank = data1 | (mBank & 0x3f80);
      else
        qDebug() << "midi ctrl" << data0 << "d=" << data1;
      break;

    case 0x40 :
      //Voice changed
//      emit midiVoice( csVoiceId( mBank, data0 ) );
      break;
    }
  }
