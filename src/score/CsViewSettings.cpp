#include "CsViewSettings.h"

#include <QSettings>

void CsViewSettings::lineSpacingSet(int newLineSpacing)
  {
  if (mLineSpacing == newLineSpacing)
    return;
  mLineSpacing = newLineSpacing;
  QSettings s;
  s.setValue( QString("LineSpacing"), mLineSpacing );
  emit lineSpacingChanged();
  }



void CsViewSettings::titleTextSizeSet(int newTitleTextSize)
  {
  if (mTitleTextSize == newTitleTextSize)
    return;
  mTitleTextSize = newTitleTextSize;
  QSettings s;
  s.setValue( QString("TitleTextSize"), mTitleTextSize );
  emit titleTextSizeChanged();
  }




void CsViewSettings::fieldTextSizeSet(int newFieldTextSize)
  {
  if (mFieldTextSize == newFieldTextSize)
    return;
  mFieldTextSize = newFieldTextSize;
  QSettings s;
  s.setValue( QString("FieldTextSize"), mFieldTextSize );
  emit fieldTextSizeChanged();
  }




void CsViewSettings::fingerTextSizeSet(int newFingerTextSize)
  {
  if (mFingerTextSize == newFingerTextSize)
    return;
  mFingerTextSize = newFingerTextSize;
  QSettings s;
  s.setValue( QString("FingerTextSize"), mFingerTextSize );
  emit fingerTextSizeChanged();
  }



void CsViewSettings::textSizeSet(int newTextSize)
  {
  if (mTextSize == newTextSize)
    return;
  mTextSize = newTextSize;
  QSettings s;
  s.setValue( QString("TextSize"), mTextSize );
  emit textSizeChanged();
  }



void CsViewSettings::lirycSizeSet(int newLirycSize)
  {
  if (mLyricSize == newLirycSize)
    return;
  mLyricSize = newLirycSize;
  QSettings s;
  s.setValue( QString("LirycSize"), mLyricSize );
  emit lirycSizeChanged();
  }



void CsViewSettings::chordSizeSet(int newChordSize)
  {
  if (mChordSize == newChordSize)
    return;
  mChordSize = newChordSize;
  QSettings s;
  s.setValue( QString("ChordSize"), mChordSize );
  emit chordSizeChanged();
  }

void CsViewSettings::classicNoteSizeSet(int newClassicNoteSize)
  {
  if (mClassicNoteSize == newClassicNoteSize)
    return;
  mClassicNoteSize = newClassicNoteSize;
  mClassicNoteHeight = (mClassicNoteSize << 10) * 12 / 3982;
  QSettings s;
  s.setValue( QString("ClassicNoteSize"), mClassicNoteSize );
  emit classicNoteSizeChanged();
  }




void CsViewSettings::digitNoteSizeSet(int newDigitNoteSize)
  {
  if (mDigitNoteSize == newDigitNoteSize)
    return;
  mDigitNoteSize = newDigitNoteSize;
  QSettings s;
  s.setValue( QString("DigitNoteSize"), mDigitNoteSize );
  emit digitNoteSizeChanged();
  }



void CsViewSettings::widthPerHoleSet(int newWidthPerHole)
  {
  if (mWidthPerHole == newWidthPerHole)
    return;
  mWidthPerHole = newWidthPerHole;
  QSettings s;
  s.setValue( QString("WidthPerHole"), mWidthPerHole );
  emit widthPerHoleChanged();
  }




void CsViewSettings::viewModeSet(int newViewMode)
  {
  if (mViewMode == newViewMode)
    return;
  mViewMode = newViewMode;
  QSettings s;
  s.setValue( QString("ViewMode"), mViewMode );
  emit viewModeChanged();
  }


CsViewSettings::CsViewSettings(QObject *parent)
  : QObject{parent}
  {
  loadConfig();
  }


void CsViewSettings::loadConfig()
  {
  QSettings s;
  mLineSpacing = s.value( QString("LineSpacing"), 30 ).toInt();
  mInLineSpacing = s.value( QString("InLineSpacing"), 3 ).toInt();
  mFieldSpacing = s.value( QString("LineSpacing"), 3 ).toInt();
  mTitleTextSize = s.value( QString("TitleTextSize"), 24 ).toInt();
  mFieldTextSize = s.value( QString("FieldTextSize"), 14 ).toInt();
  mFingerTextSize = s.value( QString("FingerTextSize"), 9 ).toInt();
  mTextSize = s.value( QString("TextSize"), 12 ).toInt();
  mLyricSize = s.value( QString("LyricSize"), 18 ).toInt();
  mChordSize = s.value( QString("ChordSize"), 16 ).toInt();
  mClassicNoteSize = s.value( QString("ClassicNoteSize"), 35 ).toInt();
  mClassicNoteHeight = (mClassicNoteSize << 10) * 12 / 3982;
  mDigitNoteSize = s.value( QString("DigitNoteSize"), 16 ).toInt();
  mWidthPerHole = s.value( QString("WidthPerHole"), 350 ).toInt();
  mViewMode = s.value( QString("ViewMode"), CsVmClassicNote ).toInt();
  mLeftGapClassicNote = s.value( QString("LeftGapClassicNote"), 90 ).toInt();
  mLeftGapDigitNote = s.value( QString("LeftGapDigitNote"), 40 ).toInt();
  mMargin = s.value( QString("Margin"), 4 ).toInt();
  }
