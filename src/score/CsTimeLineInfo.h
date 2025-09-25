#ifndef CSTIMELINEINFO_H
#define CSTIMELINEINFO_H

#include <QList>
#include <QTextCursor>

struct CsTimeTick
  {
    int  mPosition;    //!< Tick position at time line
    int  mDuration;    //!< Tick duration
    bool mIsTakt;      //!< true for takt line
    bool mIsQuartTakt; //!< true for quarter of takt
    bool mTextAnchor;  //!< when true lyric text must anchor to this tick

    CsTimeTick() : mPosition(0), mDuration(0), mIsTakt(false), mIsQuartTakt(false), mTextAnchor(false) {}
  };

class CsTimeLineInfo
  {
    QList<CsTimeTick> mTickList;     //!< List of all ticks
    int               mStartTick;    //!< Moment start of line in tick inside composition
    int               mLineDuration; //!< Full line duration in tick (Summ of all durations of each CsTimeTick)
  public:
    CsTimeLineInfo();

    //!
    //! \brief blockParse Parse time line from block of lines. At first we find timeLine
    //! \param from       Cursor in some where in block
    //! \param pStartTick Corrects start tick of line
    //!
    void blockParse( QTextCursor from, int *pStartTick );

    //!
    //! \brief parse    Parse text line of timeLine and build list of ticks, start line and line duration
    //! \param timeLine Text line of timeLine
    //!
    void parse( const QString &timeLine );

    //!
    //! \brief isValid Return true when tick list is not empty
    //! \return        true when tick list is not empty
    //!
    bool isValid() const { return mTickList.count() > 0; }

    //!
    //! \brief clear Clears list of all ticks and make info invalid
    //!
    void clear() { mTickList.clear(); }

    //!
    //! \brief duration Duration of exact index tick
    //! \param index    Index of tick
    //! \return         Duration of tick with index
    //!
    int  duration( int index ) const { return mTickList.at(index).mDuration; }

    //!
    //! \brief duration  Summarized duration of all ticks from fromIndex to toIndex (exclude last)
    //! \param fromIndex Start index for calculation
    //! \param toIndex   Stop index for calculation. This is excluded from calculation
    //! \return          Summarized duration
    //!
    int  duration( int fromIndex, int toIndex ) const;

    //!
    //! \brief startFromLine Time of begin of tick with index
    //! \param index         Index of tick
    //! \return              Duration from begin line to tick with index
    //!
    int  startFromLine( int index ) const { return duration( 0, index ); }

    //!
    //! \brief start Time of begin of tick with index from begin of composition
    //! \param index Index of tick
    //! \return      Duration from begin of composition to this line and tick with index
    //!
    int  start( int index ) const { return startFromLine(index) + mStartTick; }

    //!
    //! \brief lineDuration Return full line duration
    //! \return             Full line duration
    //!
    int  lineDuration() const { return mLineDuration; }

    //!
    //! \brief lineStart Return line start in tick from begin of composition
    //! \return          Line start in tick from begin of composition
    //!
    int  lineStart() const { return mStartTick; }

    //!
    //! \brief count Return count of ticks in this timeLine
    //! \return      Count of ticks in this timeLine
    //!
    int  count() const { return mTickList.count(); }

    //!
    //! \brief position Return position of tick with index in text line from begin of line
    //! \param index    Index of tick
    //! \return         Position of tick with index in text line from begin of line
    //!
    int  position( int index ) const { return mTickList.at(index).mPosition; }

    //!
    //! \brief setAnchor Set anchor on tick with index for lyric
    //! \param index     Index of tick
    //!
    void setAnchor( int index ) { mTickList[index].mTextAnchor = true; }

    //!
    //! \brief isAnchor Return anchor status of tick with index
    //! \param index    Index of tick
    //! \return         Anchor status of tick with index
    //!
    bool isAnchor( int index ) const { return mTickList.at(index).mTextAnchor; }

    //!
    //! \brief isTakt Return takt status of tick with index
    //! \param index  Index of tick
    //! \return       Takt status of tick with index
    //!
    bool isTakt( int index ) const { return mTickList.at(index).mIsTakt; }

    //!
    //! \brief isQuartTakt Return quartTakt status of tick with index
    //! \param index       Index of tick
    //! \return            QuartTakt status of tick with index
    //!
    bool isQuartTakt( int index ) const { return mTickList.at(index).mIsQuartTakt; }

    //!
    //! \brief nextTimeLine Begins next time line
    //!
    void nextTimeLine() { mStartTick += mLineDuration; }
  };

#endif // CSTIMELINEINFO_H
