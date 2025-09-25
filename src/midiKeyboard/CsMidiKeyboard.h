#ifndef CSMIDIKEYBOARD_H
#define CSMIDIKEYBOARD_H

#include <QObject>
#include <QMap>

class CsMidiKeyboard : public QObject
  {
    Q_OBJECT

    int           mBank;       //!< Full voice bank MSB bank:LSB bank
    QMap<int,int> mVoiceMap;   //!< Current selected voice map
  protected:
    bool    mPlayEna;    //!< Enable play over midi keyboard

    Q_PROPERTY(bool playEna READ playEna WRITE playEnaSet NOTIFY playEnaChanged FINAL)
  public:
    explicit CsMidiKeyboard(QObject *parent = nullptr);

    virtual void init() = 0;

    virtual bool isLink() const = 0;

    virtual void setTempo( int tempo ) = 0;

    bool playEna() const { return mPlayEna; }
    void playEnaSet(bool newPlayEna);

  signals:
    void tick( int count );

    void midiNote( int channel, int pitch, int velo );

    void midiRun( bool run );

    void midiLink( bool on );

    void midiVoice( int voice );

    void playEnaChanged();

  public slots:
    virtual void playRun( bool run );
    virtual void playNote( int channel, int note, int velo );
    virtual void playTempo( int tempo );
    virtual void playVoice( int channel, int voice );
    void         playClear() { mVoiceMap.clear(); }

  protected:
    void         midiSignal( quint8 control, quint8 data0, quint8 data1 );

    virtual void midiSend(int count, quint8 *array) = 0;
  };

#endif // CSMIDIKEYBOARD_H
