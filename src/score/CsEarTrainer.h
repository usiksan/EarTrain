#ifndef CSEARTRAINER_H
#define CSEARTRAINER_H

#include "CsPlayer.h"
#include "SvLib/SvSingleton.h"

#include <QRandomGenerator>


class CsEarTrainer : public CsPlayer
  {
    Q_OBJECT

    QRandomGenerator mRandom;

    bool             mSingleFirst;
    int              mMinInterval;
    int              mMaxInterval;
    int              mResult;


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

  public slots:
    void intervalCompare( bool singleFirst, int minInterval, int maxInterval );

    void repeate();

    void next();

  private:
    void noteAdd( int pitch, int startTick );
  signals:
    void singleFirstChanged();
    void minIntervalChanged();
    void maxIntervalChanged();
    void resultChanged();

  private:
    Q_PROPERTY(bool singleFirst READ singleFirst WRITE singleFirstSet NOTIFY singleFirstChanged FINAL)
    Q_PROPERTY(int minInterval READ minInterval WRITE minIntervalSet NOTIFY minIntervalChanged FINAL)
    Q_PROPERTY(int maxInterval READ maxInterval WRITE maxIntervalSet NOTIFY maxIntervalChanged FINAL)
    Q_PROPERTY(int result READ result WRITE resultSet NOTIFY resultChanged FINAL)
  };

#endif // CSEARTRAINER_H
