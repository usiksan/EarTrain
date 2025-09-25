#include "CsEarTrainer.h"
#include "CsNoteChord.h"

#define NOTE_KEY_MIN octaveBig
#define NOTE_KEY_MAX octaveFourth
#define NOTE_DURATION (duraEighth + duraSixteenth)
#define NOTE_PAUSE_SHORT duraEighth
#define NOTE_PAUSE_LONG  duraHalf

CsEarTrainer::CsEarTrainer(QObject *parent)
  : CsPlayer{parent}
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

void CsEarTrainer::intervalCompare(bool singleFirst, int minInterval, int maxInterval)
  {
  singleFirstSet( singleFirst );
  minIntervalSet( minInterval );
  maxIntervalSet( maxInterval );

  next();
  }



void CsEarTrainer::repeate()
  {
  play( 0, mTickMax, 0, false );
  }



void CsEarTrainer::next()
  {
  //Select start note for both intervals
  int start1 = mRandom.bounded( NOTE_KEY_MIN, NOTE_KEY_MAX );
  int start2 = mRandom.bounded( NOTE_KEY_MIN, NOTE_KEY_MAX );
  //If we use single first note then union start notes
  if( mSingleFirst ) start2 = start1;

  int direction = mRandom.bounded( NOTE_KEY_MIN, NOTE_KEY_MAX );

  int firstInterval = mRandom.bounded( mMinInterval, mMaxInterval + 1 );
  int secondInterval;
  do {
    secondInterval = mRandom.bounded( mMinInterval, mMaxInterval + 1 );
    }
  while( secondInterval == firstInterval );

  resultSet( firstInterval > secondInterval ? 1 : 2 );

  int startTick = 0;
  if( direction & 1 ) {
    //Down direction
    firstInterval = -firstInterval;
    secondInterval = -secondInterval;
    }

  mEventMap.clear();
  mVoiceMap.clear();

  noteAdd( start1, startTick );
  startTick += NOTE_DURATION + NOTE_PAUSE_SHORT;
  noteAdd( start1 + firstInterval, startTick );
  startTick += NOTE_DURATION + NOTE_PAUSE_LONG;

  noteAdd( start2, startTick );
  startTick += NOTE_DURATION + NOTE_PAUSE_SHORT;
  noteAdd( start2 + secondInterval, startTick );
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

