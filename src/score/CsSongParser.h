#ifndef CSSONGPARSER_H
#define CSSONGPARSER_H

#include "CsCell.h"
#include "CsViewSettings.h"
#include "CsTimeLineInfo.h"

#include <QObject>
#include <QMultiMap>
#include <QTextCursor>

struct CsMidiEvent
  {
    quint8  mChannel;
    quint8  mPitch;
    quint8  mLevel;
  };

struct CsMidiVoice
  {
    quint8  mChannel;
    quint8  mVolume;
    quint16 mVoiceId;

    CsMidiVoice() {}
    CsMidiVoice( int channel, int volume, int voiceId ) : mChannel(channel), mVolume(volume), mVoiceId(voiceId) {}
  };

using CsCellMap = QMultiMap<int,int>;

class CsSongParser : public CsViewSettings
  {
    Q_OBJECT
  protected:
    QMultiMap<int,CsMidiEvent> mEventMap;
    QMultiMap<int,CsMidiVoice> mVoiceMap;
    CsCellList                 mCellList;
    CsCellMap                  mCellActivate;
    CsCellMap                  mCellDeactivate;
    int                        mTempo;            //!< Song tempo
    int                        mTickMax;
    int                        mTickBegin;
    int                        mTickEnd;

    int                        mLineYPos;

    int                        mWidth;            //!< Visual width
    int                        mHeight;           //!< Visual height
  public:
    explicit CsSongParser(QObject *parent = nullptr);

    int               visualWidth() const { return mWidth; }
    int               visualHeight() const { return mHeight == 0 ? 1 : mHeight; }

    const CsCellList &cellList() const { return mCellList; }
    const CsCellMap  &cellActivate() const { return mCellActivate; }
    const CsCellMap  &cellDeactivate() const { return mCellDeactivate; }

    bool              cellContains( int tickIndex ) const { return mCellActivate.contains(tickIndex) || mCellDeactivate.contains(tickIndex); }

    //!
    //! \brief fragmentSize Calculate count of CsCtSelector cells beatween from and to
    //! \param from         Cell index from which count including
    //! \param to           Cell index to witch count excluding
    //! \return             Count of CsCtSelector cells
    //!
    int               fragmentSize( int from, int to ) const;

    //!
    //! \brief fragmentPosition Calculates the end position of a fragment separated from the specified position by a specified number of CsCtSelector cells
    //! \param from             the initial position of the fragment
    //! \param count            Number of CsCtSelector cells
    //! \param forwardDirection When true then fragment selected in forward direction
    //! \return                 Cell index of the end position of fragment
    //!
    int               fragmentPosition(int from, int count , bool forwardDirection) const;

    //!
    //! \brief parse    Parse full song with building score cells and play events
    //! \param songPath Path to song to use as base for audioFile
    //! \param doc      Pointer to text document for parsing
    //! \return         true if parsing successfull
    //!
    bool              parse( const QString &songPath, const QTextDocument *doc );
  signals:

  protected:
    //!
    //! \brief tickToSampleIndex Converts tick index to sample index of back audio file
    //! \param tickIndex         Tick index to convert
    //! \return                  Sample index in audio file correspondent to tickIndex
    //!
    int          tickToSampleIndex( int tickIndex );

    //!
    //! \brief parseSingleLine Parse current note line only
    //! \param from            Cursor at note line
    //! \param pStartTick      Start tick index of this line begin
    //! \return                true if parse successfull
    //!
    bool         parseSingleLine( QTextCursor from, int *pStartTick );

    //!
    //! \brief parseMultiLine Parse block of note lines where current line resides
    //! \param from           Cursor at any line of block
    //! \param pStartTick     Start tick index of this line begin
    //! \return               true if parse successfull
    //!
    bool         parseMultiLine( QTextCursor from, int *pStartTick );

    //!
    //! \brief parseLyricIndexesOfPositions Parse positions of lyric
    //! \param notePositions                Decoded note positions from time line
    //! \return                             Indexes for note positions in notePositions where lyric
    //!
    QList<int>    parseLyricIndexesOfPositions( const CsTimeLineInfo &notePositions ) const;

    void          parseLineList(const QStringList &lineList, CsTimeLineInfo &notePositions, QTextCursor from );

    //!
    //! \brief parseNoteLine Parse note line, decode notes and fill mEventMap with them. Also we fill mCellList to visualise notes
    //! \param lineNote      Text line with notes
    //! \param notePositions Time line note positions info
    //! \param channel       Channel for notes
    //!
    void          parseNoteLine(const QString &lineNote, CsTimeLineInfo &notePositions, int channel );

    //!
    //! \brief parseDrumLine Parse drum line, decode drum shots and fill mEventMap with them.
    //! \param line          Text line with shots
    //! \param notePositions Time line note positions info
    //! \param channel       Channel for drum
    //!
    void          parseDrumLine(const QString &line, const CsTimeLineInfo &notePositions, int channel );

    //!
    //! \brief parseGuitar   Parse guitar line, decode guitar shots and fill mEventMap with them.
    //! \param lineNote      Text line with guitar shots
    //! \param notePositions Time line note positions info
    //! \param channel       Channel for guitar
    //!
    void          parseGuitar(const QString &lineNote, const CsTimeLineInfo &notePositions, int channel );

    //!
    //! \brief parseLyric    Parse lyric line and fill mCellList to visualise lyric
    //! \param lyric         Lyric text line
    //! \param notePositions Time line note positions info
    //!
    void          parseLyric( const QString &lyric, const CsTimeLineInfo &notePositions );

    void          parseChord( const QString &chord, CsTimeLineInfo &notePositions );

    void          parseFinger(const QString &lineFinger, const CsTimeLineInfo &notePositions );

  private:
    int           leftPos( const QString &line, int pos ) const;
  };

#endif // CSSONGPARSER_H
