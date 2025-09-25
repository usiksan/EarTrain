#ifndef CSPLAYER_H
#define CSPLAYER_H

#include "CsSongParser.h"

#include <QObject>
#include <QTextCursor>
#include <QList>

class CsPlayer : public CsSongParser
  {
    Q_OBJECT

  protected:

    int                        mTickCount;
    bool                       mLoop;
    int                        mTrainLevel;

    quint8                     mGuitarPrevCords[6];
    quint8                     mGuitarNextCords[6];

    QSet<int>                  mPlayKbdPitch;
    QSet<int>                  mPlayWaitPitch;

  public:
    explicit CsPlayer(QObject *parent = nullptr);

    //!
    //! \brief play     Play current single note line
    //! \param songPath Path to song to use as base for audioFile
    //! \param from     Cursor at none line
    //! \param loop     when true play after end begins from start line
    //!
    void play( const QString &songPath, QTextCursor from, bool loop );

    //!
    //! \brief playSync Play full block where current line resides
    //! \param songPath Path to song to use as base for audioFile
    //! \param from     Cursor at none line
    //!
    void playSync( const QString &songPath, QTextCursor from );

    //!
    //! \brief playAll  Play full song (parse, build play events and play them)
    //! \param songPath Path to song to use as base for audioFile
    //! \param doc      Pointer to song
    //!
    void playAll( const QString &songPath, const QTextDocument *doc );

    //!
    //! \brief stop Stops current plaing
    //!
    void stop() { mTickCount = -1; }

    //!
    //! \brief play       Play fragment from begin tick to end tick
    //! \param begin      Begin tick of fragment
    //! \param end        End tick of fragment
    //! \param trainLevel Train level: 0 - simple play, 1 - play with highlight right and wrong user key pressing, 2 - wait for right key pressing
    //! \param loop       when true play after end begins from start of fragment
    //!
    void play( int begin, int end, int trainLevel, bool loop );

    //!
    //! \brief playFragment Play fragment from begin score cell index to end score cell index
    //! \param cellBegin    Begin score cell index
    //! \param cellEnd      End score cell index
    //! \param trainLevel   Train level: 0 - simple play, 1 - play with highlight right and wrong user key pressing, 2 - wait for right key pressing
    //! \param loop         when true play after end begins from start of fragment
    //!
    void playFragment( int cellBegin, int cellEnd, int trainLevel, bool loop );

    //!
    //! \brief isLoop Returns state of loop flag
    //! \return       State of loop flag
    //!
    bool isLoop() const { return mLoop; }
  signals:
    void playNote( int channel, int note, int velo );
    void playChord( int chordCode );
    void playTempo( int tempo );
    void playVoice( int channel, int voice );
    void playVolume( int channel, int volume );
    void playStyle( int style );
    void pianoKeySet( int pitch, int code );

    void cellTick( int tick );
  public slots:
    void setTempo(QTextCursor from, bool backward );
    void tick( int count );

    void guitarPlayChord( int chordCode );

    void kbdNote( int channel, int pitch, int velo );
  };

#endif // CSPLAYER_H
