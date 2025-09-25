#ifndef CSVIEWSETTINGS_H
#define CSVIEWSETTINGS_H

#include <QObject>

class CsViewSettings : public QObject
  {
    Q_OBJECT
  protected:
    int mLineSpacing;
    int mFieldSpacing;
    int mInLineSpacing;
    int mTitleTextSize;
    int mFieldTextSize;
    int mFingerTextSize;
    int mTextSize;
    int mLyricSize;
    int mChordSize;
    int mClassicNoteSize;
    int mClassicNoteHeight;
    int mDigitNoteSize;
    int mWidthPerHole;
    int mViewMode;
    int mLeftGapClassicNote;
    int mLeftGapDigitNote;
    int mMargin;
  public:
    explicit CsViewSettings(QObject *parent = nullptr);

    enum CsViewMode
      {
      CsVmClassicNote,
      CsVmDigitNote,
      CsVmLetterNote
      };
    Q_ENUM(CsViewMode)

    enum CsCellType {
      CsCtText,
      CsCtClassicNote,
      CsCtDigitalNote,
      CsCtTaktLine,
      CsCtSelector,
      CsCtClef,
      CsCtMarker,
    };
    Q_ENUM(CsCellType)

    //int noteCellType() const {  }

    int lineSpacing() const { return mLineSpacing; }
    void lineSpacingSet(int newLineSpacing);

    int titleTextSize() const { return mTitleTextSize; }
    void titleTextSizeSet(int newTitleTextSize);

    int fieldTextSize() const { return mFieldTextSize; }
    void fieldTextSizeSet(int newFieldTextSize);

    int fingerTextSize() const { return mFingerTextSize; }
    void fingerTextSizeSet(int newFingerTextSize);

    int textSize() const { return mTextSize; }
    void textSizeSet(int newTextSize);

    int lirycSize() const { return mLyricSize; }
    void lirycSizeSet(int newLirycSize);

    int chordSize() const { return mChordSize; }
    void chordSizeSet(int newChordSize);

    int classicNoteSize() const { return mClassicNoteSize; }
    void classicNoteSizeSet(int newClassicNoteSize);

    int digitNoteSize() const { return mDigitNoteSize; }
    void digitNoteSizeSet(int newDigitNoteSize);

    int widthPerHole() const { return mWidthPerHole; }
    void widthPerHoleSet(int newWidthPerHole);

    int viewMode() const { return mViewMode; }
    void viewModeSet(int newViewMode);


    void loadConfig();

  signals:

    void lineSpacingChanged();
    void titleTextSizeChanged();
    void fieldTextSizeChanged();
    void fingerTextSizeChanged();
    void textSizeChanged();
    void lirycSizeChanged();
    void chordSizeChanged();
    void classicNoteSizeChanged();
    void digitNoteSizeChanged();
    void widthPerHoleChanged();

    void viewModeChanged();


  private:
    Q_PROPERTY(int lineSpacing READ lineSpacing WRITE lineSpacingSet NOTIFY lineSpacingChanged FINAL)
    Q_PROPERTY(int titleTextSize READ titleTextSize WRITE titleTextSizeSet NOTIFY titleTextSizeChanged FINAL)
    Q_PROPERTY(int fieldTextSize READ fieldTextSize WRITE fieldTextSizeSet NOTIFY fieldTextSizeChanged FINAL)
    Q_PROPERTY(int fingerTextSize READ fingerTextSize WRITE fingerTextSizeSet NOTIFY fingerTextSizeChanged FINAL)
    Q_PROPERTY(int textSize READ textSize WRITE textSizeSet NOTIFY textSizeChanged FINAL)
    Q_PROPERTY(int lirycSize READ lirycSize WRITE lirycSizeSet NOTIFY lirycSizeChanged FINAL)
    Q_PROPERTY(int chordSize READ chordSize WRITE chordSizeSet NOTIFY chordSizeChanged FINAL)
    Q_PROPERTY(int classicNoteSize READ classicNoteSize WRITE classicNoteSizeSet NOTIFY classicNoteSizeChanged FINAL)
    Q_PROPERTY(int digitNoteSize READ digitNoteSize WRITE digitNoteSizeSet NOTIFY digitNoteSizeChanged FINAL)
    Q_PROPERTY(int widthPerHole READ widthPerHole WRITE widthPerHoleSet NOTIFY widthPerHoleChanged FINAL)

    Q_PROPERTY(int viewMode READ viewMode WRITE viewModeSet NOTIFY viewModeChanged FINAL)

  protected:
    int widthOfDuration( int duration ) const { return duration * mWidthPerHole >> 8; }

    int leftGap() const { return (mViewMode == CsVmClassicNote ? mLeftGapClassicNote : mLeftGapDigitNote) + mMargin; }
  };

#endif // CSVIEWSETTINGS_H
