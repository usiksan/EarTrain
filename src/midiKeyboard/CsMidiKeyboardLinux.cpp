#include "CsMidiKeyboardLinux.h"

#include <QDebug>

#ifdef Q_OS_LINUX
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

CsMidiKeyboardLinux::CsMidiKeyboardLinux(QObject *parent)
  : CsMidiKeyboard{parent}
  , mThread(nullptr)
  , mPeriodic(nullptr)
  , mTickStep(12800)    //!< Tick step in nanoSec, default 120bpm
  , mTickCount(0)      //!< Current tick in nanoSec. When count over 10ms sended one or more ticks
  , mMidiHandle(-1)    //!< Handle to midi keyboard
  , mQuietCount(0)
  , mDataIndex(-1)
  , mSysExIndex(-1)
  {
  mThread = new QThread();
  moveToThread( mThread );
  connect( mThread, &QThread::started, this, &CsMidiKeyboardLinux::onStart );
  }



CsMidiKeyboardLinux::~CsMidiKeyboardLinux()
  {
  if( mMidiHandle >= 0 )
    close( mMidiHandle );
  }



void CsMidiKeyboardLinux::init()
  {
  mThread->start();
  }



void CsMidiKeyboardLinux::setTempo(int tempo)
  {
  mTickStep = tempo * 640 / 6;
  }






void CsMidiKeyboardLinux::playTempo(int tempo)
  {
  CsMidiKeyboard::playTempo( tempo );
  setTempo(tempo);
  }




void CsMidiKeyboardLinux::periodic()
  {
  qint64 ms = mElapsedTimer->elapsed();
  ms -= mElapsedMsec;
  mElapsedMsec += ms;
  if( mMidiHandle >= 0 ) {
    //There is opened midi device. Parse incomming messages
    char buf[33];
    ssize_t r;
    do {
      //Read buffer
      r = read( mMidiHandle, buf, 30 );

      //This code for detecting disconnect midi keyboard
      if( r < 0 ) {
        mQuietCount++;
        if( mQuietCount > 100 ) {
          //Perhaps that midi keyboard was disconnected
          //We close port and try open it later
          close( mMidiHandle );
          mMidiHandle = -1;
          emit midiLink( false );
          //Try connect after two seconds
          QTimer::singleShot( 2000, this, &CsMidiKeyboardLinux::onStart );
          return;
          }
        }
      else mQuietCount = 0;

      //Parse bytes
      for( int i = 0; i < r; ++i ) {
        if( buf[i] & 0x80 ) {
          //Check system command
          int system = buf[i] & 0x7f;
          if( system == 0x78 ) {
            //Timing clock
            //static QElapsedTimer st;
            //qDebug() << "tick" << st.restart();
            tickGenerate(26666);
            continue;
            }
          if( system == 0x7e )
            //Active sensing
            continue;
          if( system == 0x7a ) {
            emit midiRun( true );
            continue;
            }
          if( system == 0x7c ) {
            emit midiRun( false );
            continue;
            }
          if( system == 0x70 ) {
            //Begin of sysEx message
            qDebug() << "Begin sysEx";
            mSysExIndex = 0;
            continue;
            }
          if( system == 0x77 ) {
            //End of sysEx message
            qDebug() << "End sysEx";
            parseSysEx();
            mSysExIndex = -1;
            continue;
            }
          //Control byte i
          if( mDataIndex >= 0 ) {
            midiSignal( mControl, mData0, 0 );
            qDebug() << "before control midi" << mControl << mData0;
            }
          mControl = buf[i] & 0x7f;
          mDataIndex = 0;
          mData0 = 0;
          }
        else {
          //Data byte
          if( mSysExIndex >= 0 ) {
            //This byte for sysEx
            mSysExBuf[mSysExIndex++] = buf[i] & 0x7f;
            if( mSysExIndex >= 1024 ) mSysExIndex = 1023;
            continue;
            }
          if( mDataIndex == 0 ) {
            mData0 = buf[i] & 0x7f;
            mDataIndex = 1;
            }
          else if( mDataIndex == 1 ) {
            midiSignal( mControl, mData0, buf[i] & 0x7f );
            qDebug() << "normal midi" << mControl << mData0 << static_cast<int>(buf[i] & 0x7f);
            mDataIndex = -1;
            }
          }
        }
      }
    while( r == 30 );
    if( mDataIndex >= 0 ) {
      midiSignal( mControl, mData0, 0 );
      mDataIndex = -1;
      qDebug() << "post midi" << mControl << mData0;
      }
    }
  else {
    //qDebug() << "tick" << mTickStep;
    //Convert time
    // mTickStep / 10 == x / ms
    // x == mTickStep * ms / 10
    tickGenerate( mTickStep * ms / 10 );
    }
  }



void CsMidiKeyboardLinux::onStart()
  {
  //Create periodic timer if it not created yet
  if( mPeriodic == nullptr ) {
    mPeriodic = new QTimer();
    mPeriodic->setInterval(10);
    connect( mPeriodic, &QTimer::timeout, this, &CsMidiKeyboardLinux::periodic );
    mPeriodic->start();

    mElapsedTimer = new QElapsedTimer();
    mElapsedTimer->restart();
    }

  mMidiHandle = open( "/dev/snd/midiC2D0", O_RDWR | O_NONBLOCK );
  if( mMidiHandle >= 0 ) {
    mQuietCount = 0;
    emit midiLink( true );
    //qDebug() << "midi open" << mMidiHandle;
    }
  else {
    //Try connect after one second
    QTimer::singleShot( 1000, this, &CsMidiKeyboardLinux::onStart );
    }
  }



void CsMidiKeyboardLinux::tickGenerate(int count)
  {
  mTickCount += count;
  int tc = mTickCount / 10000;
  mTickCount %= 10000;
  if( tc > 0 )
    emit tick( tc );
  static QElapsedTimer tt;
  static int tcc = 0;
  if( tcc == 0 ) tt.start();
  if( tcc >= 256 ) {
    qDebug() << "Hole period" << tt.elapsed();
    tt.restart();
    tcc = 0;
    }
  tcc += tc;
  }



void CsMidiKeyboardLinux::parseSysEx()
  {
//  qDebug() << "====SysEx:" << mSysExIndex;
//  for( int i = 0; i < mSysExIndex; i++ )
//    qDebug() << mSysExBuf[i];
  //  qDebug() << "----";
  }




void CsMidiKeyboardLinux::midiSend(int count, quint8 *array)
  {
  if( mMidiHandle >= 0 ) {
    write( mMidiHandle, array, count );
    }
  }





#endif
