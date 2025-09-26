#ifndef CSEARTRAINER_H
#define CSEARTRAINER_H

#include "CsPlayer.h"
#include "SvLib/SvSingleton.h"

#include <QRandomGenerator>
#include <QList>

struct IntervalPair
  {
    int mFirst1;
    int mFirst2;
    int mSecond1;
    int mSecond2;
    int mResult;
  };

class CsEarTrainer : public CsPlayer
  {
    Q_OBJECT

    QRandomGenerator    mRandom;

    bool                mSingleFirst;
    int                 mMinInterval;
    int                 mMaxInterval;
    int                 mResult;

    int                 mMinNote;
    int                 mMaxNote;

    QList<IntervalPair> mErrors;
    IntervalPair        mCurrent;
    int                 mErrorRepeate;
    int                 mErrorCount;
    int                 mTestCount;


  public:
    explicit CsEarTrainer(QObject *parent = nullptr);
    SV_SINGLETON(CsEarTrainer)

    bool singleFirst() const { return mSingleFirst; }
    void singleFirstSet(bool newSingleFirst);

    int  minInterval() const { return mMinInterval; }
    void minIntervalSet(int newMinInterval);

    int  maxInterval() const { return mMaxInterval; }
    void maxIntervalSet(int newMaxInterval);

    int  result() const { return mResult; }
    void resultSet(int newResult);

    int  errors() const { return mErrors.count(); }

    QString statistic() const;

    int     minNote() const { return mMinNote; }
    void    minNoteSet(int newMinNote);

    int     maxNote() const { return mMaxNote; }
    void    maxNoteSet(int newMaxNote);

  public slots:
    void repeate();

    void nextIntervalCompare( bool isCorrect );

    //void nextIntervalMeasure();

  private:
    void noteAdd( int pitch, int startTick );
  signals:
    void singleFirstChanged();
    void minIntervalChanged();
    void maxIntervalChanged();
    void resultChanged();
    void errorsChanged();
    void statisticChanged();
    void minNoteChanged();
    void maxNoteChanged();

  private:
    Q_PROPERTY(bool singleFirst READ singleFirst WRITE singleFirstSet NOTIFY singleFirstChanged FINAL)
    Q_PROPERTY(int minInterval READ minInterval WRITE minIntervalSet NOTIFY minIntervalChanged FINAL)
    Q_PROPERTY(int maxInterval READ maxInterval WRITE maxIntervalSet NOTIFY maxIntervalChanged FINAL)
    Q_PROPERTY(int result READ result WRITE resultSet NOTIFY resultChanged FINAL)
    Q_PROPERTY(int errors READ errors NOTIFY errorsChanged FINAL)
    Q_PROPERTY(QString statistic READ statistic NOTIFY statisticChanged FINAL)
    Q_PROPERTY(int minNote READ minNote WRITE minNoteSet NOTIFY minNoteChanged FINAL)
    Q_PROPERTY(int maxNote READ maxNote WRITE maxNoteSet NOTIFY maxNoteChanged FINAL)
  };

#endif // CSEARTRAINER_H
