/*
Project "SaliScore Score music edit, view and tutorial program"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base for position in score (notes, chords, visual cursors, lyric)
*/
#ifndef CSCELL_H
#define CSCELL_H

#include "CsConfig.h"
#include "CsNoteChord.h"

#include <QList>

#define CS_JOINT_DURATION 688
#define CS_SPACE_DURATION 752

class CsCell
  {
    int             mCellType;
    int             mVisualPosX;   //!< Visual x position
    int             mVisualPosY;   //!< Visual y position
    int             mVisualWidth;  //!< Visual width
    int             mVisualHeight; //!< Visual height
    int             mParam0;       //!< Multifunctional param
                                   //!  Text align code 0-left, 1-center, 2-right
                                   //!  Tick position for marker cell
                                   //!  Note line position for modern note (0-22) and sharp flag at 0x100
    //int             mParam1;       //!< Multifunctional param 1
    int             mParam1;     //!< Size of text font in pixel
    QString         mText;         //!< Visual representation of note or lyric or chord
  public:
    CsCell( int type, int vx, int vy, int vw, int vh, int tsize, int align, const QString &txt ) :
      mCellType(type),
      mVisualPosX(vx),
      mVisualPosY(vy),
      mVisualWidth(vw),
      mVisualHeight(vh),
      mParam0(align),
      mParam1(tsize),
      mText(txt)
      {
      mText.replace( '_', " " );
      }

    CsCell(int type, int vx, int vy, int vw, int vh, int tsize, bool clefG, int pitch, int duration );

    CsCell(int type, int vx, int vy, int vw, int vh, int startTime, int duration );

    int            cellType() const { return mCellType; }

    int            visualPosX() const { return mVisualPosX; }
    int            visualPosY() const { return mVisualPosY; }
    int            visualWidth() const { return mVisualWidth; }
    int            visualHeight() const { return mVisualHeight; }

    int            tickPosition() const { return mParam0; }
    int            tickDuration() const { return mParam1; }
    int            tickEnd() const { return mParam0 + mParam1; }

    int            noteLineIndex() const { return mParam0 & 0xff; }
    bool           noteSharp() const { return mParam0 & 0x100; }

    int            textSize() const { return mParam1; }
    int            textAlign() const { return mParam0; }
    QString        text() const { return mText; }

    int            visualTickPosX( int tickPos ) const { return mVisualPosX + (tickPos - mParam0) * mVisualWidth / mParam1; }

    static QString classicNote( int var, int pitch, int duration );

    static int     noteLine( int pitch );

    static int     chordMod(const QString mod);

    static int     chordParse( const QString chordName );

    static QString chordModBuild( int chordCode );

    static QString chordFullBuild( int chordCode );

    static bool    isChord( const QString &chordName );

    static bool    isDuration( QChar ch );

    static QChar   charDuration( bool space, int duration );

    static int     durationFromChar(QChar ch , bool &space);

  private:
    void setChordNote( int note );
    void setChordMod( int mod );
  };

using CsCellList = QList<CsCell>;

#endif // CSCELL_H
