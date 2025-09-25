#include "CsMidiKeyboardAndroid.h"
#include "score/CsComposition.h"

#include <QDebug>

#ifdef Q_OS_ANDROID

#include <jni.h>
#include <amidi/AMidi.h>

static AMidiDevice* sNativeReceiveDevice = NULL;
static AMidiOutputPort* sMidiOutputPort = NULL;

static AMidiDevice* sNativeSendDevice = NULL;
static AMidiInputPort* sMidiInputPort = NULL;

//
// JNI Functions
//
extern "C" {

/**
 * Native implementation of TBMidiManager.startReadingMidi() method.
 * Opens the first "output" port from specified MIDI device for sReading.
 * @param   env  JNI Env pointer.
 * @param   (unnamed)   TBMidiManager (Java) object.
 * @param   midiDeviceObj   (Java) MidiDevice object.
 * @param   portNumber      The index of the "output" port to open.
 */
JNIEXPORT void JNICALL Java_com_salilab_salisongbook_AppMidiManager_startReadingMidi( JNIEnv* env, jobject, jobject midiDeviceObj, jint portNumber) {
  AMidiDevice_fromJava(env, midiDeviceObj, &sNativeReceiveDevice);

  AMidiOutputPort* outputPort;
  AMidiOutputPort_open(sNativeReceiveDevice, portNumber, &outputPort);

  sMidiOutputPort = outputPort;

  CsComposition::instance()->debugSet("startReadingMidi");
  }

/**
 * Native implementation of the (Java) TBMidiManager.stopReadingMidi() method.
 * @param   (unnamed)   JNI Env pointer.
 * @param   (unnamed)   TBMidiManager (Java) object.
 */
JNIEXPORT void JNICALL Java_com_salilab_salisongbook_AppMidiManager_stopReadingMidi(JNIEnv*, jobject) {
  AMidiDevice_release(sNativeReceiveDevice);
  sNativeReceiveDevice = NULL;
  }

/*
 * Sending API
 */
/**
 * Native implementation of TBMidiManager.startWritingMidi() method.
 * Opens the first "input" port from specified MIDI device for writing.
 * @param   env  JNI Env pointer.
 * @param   (unnamed)   TBMidiManager (Java) object.
 * @param   midiDeviceObj   (Java) MidiDevice object.
 * @param   portNumber      The index of the "input" port to open.
 */
JNIEXPORT void JNICALL Java_com_salilab_salisongbook_AppMidiManager_startWritingMidi( JNIEnv* env, jobject, jobject midiDeviceObj, jint portNumber ) {
  AMidiDevice_fromJava(env, midiDeviceObj, &sNativeSendDevice);

  AMidiInputPort* inputPort;
  AMidiInputPort_open(sNativeSendDevice, portNumber, &inputPort);
  sMidiInputPort = inputPort;

  CsComposition::instance()->debugSet("startWritingMidi");
  }

/**
 * Native implementation of the (Java) TBMidiManager.stopWritingMidi
 * @param   (unnamed)   JNI Env pointer.
 * @param   (unnamed)   TBMidiManager (Java) object.
 */
JNIEXPORT void JNICALL Java_com_salilab_salisongbook_AppMidiManager_stopWritingMidi(JNIEnv*, jobject) {
  AMidiDevice_release(sNativeSendDevice);
  sNativeSendDevice = NULL;
  }

/**
 * Native implementation of the (Java) TBMidiManager.writeMidi() method.
 * Writes a byte buffer to the (already open) "input" port.
 * @param   env  JNI Env pointer.
 * @param   (unnamed)   TBMidiManager (Java) object.
 * @param   data    The data buffer.
 * @param   numBytes    The number of bytes to send.
 */
void Java_com_example_nativemidi_AppMidiManager_writeMidi(JNIEnv* env, jobject,
                                                          jbyteArray data,
                                                          jint numBytes) {
  jbyte* bufferPtr = env->GetByteArrayElements(data, NULL);
  /*ssize_t numSent =*/AMidiInputPort_send(sMidiInputPort, (uint8_t*)bufferPtr,
                                           numBytes);
  env->ReleaseByteArrayElements(data, bufferPtr, JNI_ABORT);
}

}  // extern "C"



CsMidiKeyboardAndroid::CsMidiKeyboardAndroid(QObject *parent)
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
  connect( mThread, &QThread::started, this, &CsMidiKeyboardAndroid::onStart );
  }


void CsMidiKeyboardAndroid::init()
  {
  mThread->start();
  }

bool CsMidiKeyboardAndroid::isLink() const
  {
  return false;
  }

void CsMidiKeyboardAndroid::setTempo(int tempo)
  {
  mTickStep = tempo * 640 / 6;
  }

void CsMidiKeyboardAndroid::playRun(bool run)
  {
  quint8 buf[2];
  buf[0] = run ? 0xfa : 0xfc;
  midiSend( 1, buf );
  }

void CsMidiKeyboardAndroid::playNote(int note, int velo)
  {
  quint8 buf[3];
  buf[0] = 0x90;
  buf[1] = note & 0x7f;
  buf[2] = velo & 0x7f;
  midiSend( 3, buf );
  }

void CsMidiKeyboardAndroid::playTempo(int tempo)
  {
  Q_UNUSED(tempo)
  }

void CsMidiKeyboardAndroid::playVoice(int voice)
  {
  //Bank high part
  quint8 buf[3];
  buf[0] = 0xb0;
  buf[1] = 0;
  buf[2] = (voice >> 16) & 0x7f;
  midiSend( 3, buf );

  //Bank low part
  buf[1] = 0x20;
  buf[2] = (voice >> 8) & 0x7f;
  midiSend( 3, buf );

  //Program
  buf[0] = 0xc0;
  buf[1] = (voice) & 0x7f;
  midiSend( 2, buf );
  }

void CsMidiKeyboardAndroid::periodic()
  {
  if( sMidiOutputPort != nullptr ) {
    //There is opened midi device. Parse incomming messages
    uint8_t  buf[33];
    int32_t opCode;
    int64_t timestamp;
    size_t   numByteReceived;
    while( AMidiOutputPort_receive( sMidiOutputPort, &opCode, buf, sizeof(buf), &numByteReceived, &timestamp ) > 0 ) {
      if( opCode == AMIDI_OPCODE_DATA ) {
        // Dispatch the MIDI data….
        //Parse bytes
        for( size_t i = 0; i < numByteReceived; ++i ) {
          if( buf[i] & 0x80 ) {
            //Check system command
            int system = buf[i] & 0x7f;
            if( system == 0x78 ) {
              //Timing clock
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
              //qDebug() << "before control midi" << mControl << mData0;
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
              //qDebug() << "normal midi" << mControl << mData0 << static_cast<int>(buf[i] & 0x7f);
              mDataIndex = -1;
              }
            }
          }

        if( mDataIndex >= 0 ) {
          midiSignal( mControl, mData0, 0 );
          //qDebug() << "post midi" << mControl << mData0;
          }
        }
      }
    }
  else {
    tickGenerate( mTickStep );
    }
  }




void CsMidiKeyboardAndroid::onStart()
  {
  //Create periodic timer if it not created yet
  if( mPeriodic == nullptr ) {
    mPeriodic = new QTimer();
    mPeriodic->setInterval(10);
    connect( mPeriodic, &QTimer::timeout, this, &CsMidiKeyboardAndroid::periodic );
    mPeriodic->start();
    }

  //TODO amidi
  //mMidiHandle = open( "/dev/snd/midiC1D0", O_RDWR | O_NONBLOCK );
  if( mMidiHandle >= 0 ) {
    mQuietCount = 0;
    emit midiLink( true );
    //qDebug() << "midi open" << mMidiHandle;
    }
  else {
    //Try connect after one second
    QTimer::singleShot( 1000, this, &CsMidiKeyboardAndroid::onStart );
    }
  }





void CsMidiKeyboardAndroid::tickGenerate(int count)
  {
  mTickCount += count;
  int tc = mTickCount / 10000;
  mTickCount %= 10000;
  if( tc > 0 )
    emit tick( tc );
  }




void CsMidiKeyboardAndroid::parseSysEx()
  {

  }




void CsMidiKeyboardAndroid::midiSend(int count, quint8 *array)
  {
  if( sMidiInputPort != nullptr ) {
    //amidi write
    AMidiInputPort_send( sMidiInputPort, array, count );
    }
  }

#endif

