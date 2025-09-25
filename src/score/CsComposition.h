/*
Project "SaliSongBook Simple score music edit, view and tutorial program"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Composition model as set of lines
*/
#ifndef CSCOMPOSITION_H
#define CSCOMPOSITION_H

#include "CsConfig.h"
#include "CsCell.h"
#include "SvLib/SvSimpleListModel.h"
#include "SvLib/SvSingleton.h"

#include <QObject>
#include <QAbstractListModel>
#include <QUrl>

#define CS_CA 4 //!< Count of simultaneously actived cells

class CsComposition : public SvSimpleListModel
  {
    Q_OBJECT

    QString       mFilePath;          //!< Full file path
    QString       mSong;              //!< Song in text format

    int           mFragmentBegin;     //!< Index of cell where play is begin
    int           mFragmentEnd;       //!< Index of cell where play is end

    bool          mPlayLoop;          //!< Loop from begin to end

    CsCell        mPlayPos;           //!< Current visual play position
    int           mPlayPosX;          //!< Current visual cursor position

    int           mPKey[4];           //!< Piano key color

    QSet<int>     mActiveCells;

    Q_PROPERTY( QString version READ version CONSTANT )
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)

    Q_PROPERTY(int visualWidth READ width NOTIFY whChanged FINAL)
    Q_PROPERTY(int visualHeight READ height NOTIFY whChanged FINAL)

    Q_PROPERTY(int fragmentBegin READ fragmentBegin WRITE fragmentBeginSet NOTIFY fragmentChanged FINAL)
    Q_PROPERTY(int fragmentEnd READ fragmentEnd WRITE fragmentEndSet NOTIFY fragmentChanged FINAL)

    Q_PROPERTY(bool playLoop READ playLoop WRITE playLoopSet NOTIFY playLoopChanged FINAL)

    Q_PROPERTY(int playPosX  READ playPosX NOTIFY playPosChanged )
    Q_PROPERTY(int playPosY  READ playPosY NOTIFY playPosYChanged )
    Q_PROPERTY(int playPosHeight READ playPosHeight NOTIFY playPosYChanged )
    Q_PROPERTY(int playBarY  READ playBarY NOTIFY playPosYChanged )

    Q_PROPERTY(int pkey0 READ pkey0 NOTIFY pkey0Changed FINAL)
    Q_PROPERTY(int pkey1 READ pkey1 NOTIFY pkey1Changed FINAL)
    Q_PROPERTY(int pkey2 READ pkey2 NOTIFY pkey2Changed FINAL)
    Q_PROPERTY(int pkey3 READ pkey3 NOTIFY pkey3Changed FINAL)

    Q_PROPERTY(bool isMobile READ isMobile CONSTANT)

  public:
    enum CsColorKeys {
      CsCkSongListBackground,
      CsCkSongListText
    };
    Q_ENUM(CsColorKeys)

    enum CsPianoCodes {
      CsPcInactive,
      CsPcGreen,
      CsPcBlue,
      CsPcRed
    };
    Q_ENUM(CsPianoCodes)

    explicit CsComposition(QObject *parent = nullptr);

    SV_SINGLETON(CsComposition)

    //!
    //! \brief version Return textual representation of programm version
    //! \return        Textual representation of programm version
    //!
    QString     version() const { return tr(" v%1.%2").arg(CS_VERSION_MAJOR).arg(CS_VERSION_MINOR); }

    QString     fileName() const;

    QString     fileBasePath() const;

    int         width() const;

    int         height() const;

    int         fragmentBegin() const { return mFragmentBegin; }
    void        fragmentBeginSet(int newFragmentBegin);

    int         fragmentEnd() const { return mFragmentEnd; }
    void        fragmentEndSet(int newFragmentEnd);

    bool        playLoop() const { return mPlayLoop; }
    void        playLoopSet( bool newPlayLoop );

    int         playPosX() const { return mPlayPosX; }
    int         playPosY() const { return mPlayPos.visualPosY(); }
    int         playPosHeight() const { return mPlayPos.visualHeight(); }
    double      playBarY() const;

    int         pkey0() const { return mPKey[0]; }
    int         pkey1() const { return mPKey[1]; }
    int         pkey2() const { return mPKey[2]; }
    int         pkey3() const { return mPKey[3]; }

    bool        isMobile() const;

    //!
    //! \brief clear Очистить композицию
    //!
    void        clear();



    //!
    //! \brief count Возвращает количество элементов в списке
    //! \return      Количество элементов в списке
    //!
    virtual int count() const override;

    virtual int      rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

  signals:
    void fileNameChanged();

    void whChanged();

    void fragmentChanged();

    void playLoopChanged();
    void playPosChanged();
    void playPosYChanged();

    void midiNote( int channel, int pitch, int level );

    //Screen keyboard colored
    void pkey0Changed();
    void pkey1Changed();
    void pkey2Changed();
    void pkey3Changed();

//    void cellActiveChanged();

  public slots:

    void      songSet( const QString &song );

    //!
    //! \brief playStart  Start play of song
    //! \param trainLevel 0 - simple play, 1 - play without sound on track 0, 2 - play without sound on track 0 and wait right user key pressed
    //!
    void      playStart( int trainLevel );

    //!
    //! \brief playPause Pause of play
    //!
    //void      playPause();

    //!
    //! \brief playStop Play stop
    //!
    void      playStop();

    //!
    //! \brief midiNote User play note with screen keyboard.
    //! \param pitch    Note pitch
    //! \param velo     Note velo
    //!
    void      kbdNote( int pitch, int velo );

    //!
    //! \brief pianoKeySet Show code color on piano key keyboard
    //! \param pitch       Key pitch
    //! \param code        Code: one of 0 - not active, 1 - green, 2 - blue, 3 - red
    //!
    void      pianoKeySet( int pitch, int code );

    //!
    //! \brief cellTick  Active and/or passive cell changed
    //! \param tickIndex Tick index from begin song when cell changed
    //!
    void      cellTick( int tickIndex );

    //!
    //! \brief songBuild Rebuild song with new settings
    //!
    void      songBuild();
  };


#endif // CSCOMPOSITION_H
