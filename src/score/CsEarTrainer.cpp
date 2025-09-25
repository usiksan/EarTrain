#include "CsEarTrainer.h"
#include "CsNoteChord.h"

#define NOTE_KEY_MIN octaveBig
#define NOTE_KEY_MAX octaveFourth
#define NOTE_DURATION (duraEighth + duraSixteenth)
#define NOTE_PAUSE_SHORT duraEighth
#define NOTE_PAUSE_LONG  duraHalf


CsEarTrainer::CsEarTrainer(QObject *parent)
  : CsPlayer{parent}
  , mSingleFirst(true)
  , mMinInterval(1)
  , mMaxInterval(2)
  , mMinNote(NOTE_KEY_MIN)
  , mMaxNote(NOTE_KEY_MAX)
  , mErrorRepeate(0)
  , mErrorCount(0)
  , mTestCount(0)
  {

  }


void CsEarTrainer::singleFirstSet(bool newSingleFirst)
  {
  if (mSingleFirst == newSingleFirst)
    return;
  mSingleFirst = newSingleFirst;
  emit singleFirstChanged();
  }

void CsEarTrainer::minIntervalSet(int newMinInterval)
  {
  if (mMinInterval == newMinInterval)
    return;
  mMinInterval = newMinInterval;
  emit minIntervalChanged();
  }

void CsEarTrainer::maxIntervalSet(int newMaxInterval)
  {
  if (mMaxInterval == newMaxInterval)
    return;
  mMaxInterval = newMaxInterval;
  emit maxIntervalChanged();
  }

void CsEarTrainer::resultSet(int newResult)
  {
  if (mResult == newResult)
    return;
  mResult = newResult;
  emit resultChanged();
  }




QString CsEarTrainer::statistic() const
  {
  return tr("Всего ошибок: %1 из %2 попыток    Качество: %3").arg(mErrorCount).arg(mTestCount).arg( 100.0 - (double)mErrorCount * 100.0 / ((double)mTestCount + 0.0001), 0, 'f', 1 );
  }


void CsEarTrainer::minNoteSet(int newMinNote)
  {
  if (mMinNote == newMinNote)
    return;
  mMinNote = newMinNote;
  emit minNoteChanged();
  }

void CsEarTrainer::maxNoteSet(int newMaxNote)
  {
  if (mMaxNote == newMaxNote)
    return;
  mMaxNote = newMaxNote;
  emit maxNoteChanged();
  }




void CsEarTrainer::intervalCompare(bool singleFirst, int minInterval, int maxInterval)
  {
  singleFirstSet( singleFirst );
  minIntervalSet( minInterval );
  maxIntervalSet( maxInterval );

  next(true);
  }



void CsEarTrainer::repeate()
  {
  play( 0, mTickMax, 0, false );
  }



void CsEarTrainer::next(bool isCorrect)
  {
  mTestCount++;
  if( isCorrect && mErrors.count() && mErrorRepeate > 7 ) {
    //Repeat interval with error
    mCurrent = mErrors.takeFirst();
    mErrorRepeate = 0;
    emit errorsChanged();
    }
  else {
    if( !isCorrect ) {
      //Common error count
      mErrorCount++;
      //Store as error previous pair
      mErrors.append( mCurrent );
      emit errorsChanged();
      }

    if( mErrors.count() )
      mErrorRepeate++;
    else
      mErrorRepeate = 0;

    //Select start note for both intervals
    mCurrent.mFirst1  = mRandom.bounded( mMinNote, mMaxNote );
    mCurrent.mSecond1 = mRandom.bounded( mMinNote, mMaxNote );
    //If we use single first note then union start notes
    if( mSingleFirst )
      mCurrent.mSecond1 = mCurrent.mFirst1;

    int direction = mRandom.bounded( NOTE_KEY_MIN, NOTE_KEY_MAX );

    int firstInterval = mRandom.bounded( mMinInterval, mMaxInterval + 1 );
    int secondInterval;
    do {
      secondInterval = mRandom.bounded( mMinInterval, mMaxInterval + 1 );
      }
    while( secondInterval == firstInterval );

    mCurrent.mResult = firstInterval > secondInterval ? 1 : 2;

    if( direction & 1 ) {
      //Down direction
      firstInterval = -firstInterval;
      secondInterval = -secondInterval;
      }

    mCurrent.mFirst2 = mCurrent.mFirst1 + firstInterval;
    mCurrent.mSecond2 = mCurrent.mSecond1 + secondInterval;
    }

  emit statisticChanged();
  resultSet( mCurrent.mResult );

  mEventMap.clear();
  mVoiceMap.clear();
  int startTick = 0;

  noteAdd( mCurrent.mFirst1, startTick );
  startTick += NOTE_DURATION + NOTE_PAUSE_SHORT;
  noteAdd( mCurrent.mFirst2, startTick );
  startTick += NOTE_DURATION + NOTE_PAUSE_LONG;

  noteAdd( mCurrent.mSecond1, startTick );
  startTick += NOTE_DURATION + NOTE_PAUSE_SHORT;
  noteAdd( mCurrent.mSecond2, startTick );
  startTick += NOTE_DURATION;

  mTickMax = mTickEnd = startTick;
  mTickBegin = 0;
  repeate();
  }




void CsEarTrainer::noteAdd(int pitch, int startTick)
  {
  CsMidiEvent ev;
  ev.mChannel = 0;

  ev.mPitch = pitch;
  ev.mLevel = 120;
  mEventMap.insert( startTick, ev );
  ev.mLevel = 0;
  mEventMap.insert( startTick + NOTE_DURATION - 1, ev );
  }

