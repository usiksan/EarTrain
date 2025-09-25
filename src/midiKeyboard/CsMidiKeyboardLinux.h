#ifndef CSMIDIKEYBOARDLINUX_H
#define CSMIDIKEYBOARDLINUX_H

#include "CsMidiKeyboard.h"

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QElapsedTimer>

#ifdef Q_OS_LINUX


class CsMidiKeyboardLinux : public CsMidiKeyboard
  {
    Q_OBJECT

    QThread       *mThread;
    QTimer        *mPeriodic;      //!< Timer for polling midi input and generate soft tick sequence
    QElapsedTimer *mElapsedTimer;  //!< Time elapsed from program start
    qint64         mElapsedMsec;   //!< Time in ms of last periodic call
    int            mTickStep;      //!< Tick step in nanoSec
    int            mTickCount;     //!< Current tick in nanoSec. When count over 10ms sended one or more ticks

    int            mMidiHandle;    //!< Handle to midi keyboard
    int            mQuietCount;    //!< With this we monitor that mMidiHandle actualy open and work
    int            mDataIndex;

    //Midi message
    quint8         mControl;
    quint8         mData0;

    quint8         mSysExBuf[1024];
    int            mSysExIndex;
  public:
    explicit CsMidiKeyboardLinux(QObject *parent = nullptr);
    virtual ~CsMidiKeyboardLinux();

    virtual void init() override;

    virtual bool isLink() const override { return mMidiHandle >= 0; }

    virtual void setTempo( int tempo ) override;

  signals:

  public slots:
    virtual void playTempo( int tempo ) override;

  private:
    void periodic();

    void onStart();

    void tickGenerate( int count );

    void parseSysEx();

    virtual void midiSend(int count, quint8 *array) override;
  };

#endif

#endif // CSMIDIKEYBOARDLINUX_H
