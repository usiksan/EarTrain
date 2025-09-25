#ifndef CSMIDIKEYBOARDANDROID_H
#define CSMIDIKEYBOARDANDROID_H

#include "CsMidiKeyboard.h"

#include <QObject>
#include <QTimer>
#include <QThread>


#ifdef Q_OS_ANDROID

class CsMidiKeyboardAndroid : public CsMidiKeyboard
  {
    Q_OBJECT

    QThread *mThread;
    QTimer  *mPeriodic;   //!< Timer for polling midi input and generate soft tick sequence
    int      mTickStep;   //!< Tick step in nanoSec
    int      mTickCount;  //!< Current tick in nanoSec. When count over 10ms sended one or more ticks

    int      mMidiHandle; //!< Handle to midi keyboard
    int      mQuietCount; //!< With this we monitor that mMidiHandle actualy open and work
    int      mDataIndex;

    //Midi message
    quint8   mControl;
    quint8   mData0;

    quint8   mSysExBuf[1024];
    int      mSysExIndex;
  public:
    explicit CsMidiKeyboardAndroid(QObject *parent = nullptr);

    virtual void init();

    virtual bool isLink() const;

    virtual void setTempo( int tempo );

  public slots:
    virtual void playRun( bool run );
    virtual void playNote( int note, int velo );
    virtual void playTempo( int tempo );
    virtual void playVoice( int voice );

  private:
    void periodic();

    void onStart();

    void tickGenerate( int count );

    void parseSysEx();

    void midiSend(int count, quint8 *array);
  };

#endif

#endif // CSMIDIKEYBOARDANDROID_H
