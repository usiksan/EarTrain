#include "CsCell.h"

#include <QMap>

class CsDuration
  {
    int mDuration;
    int mBaseDuration;
    int mPoints;

    static int points( int base, int duration )
      {
      duration -= base;
      base >>= 1;
      if( duration >= base )
        return 1 + points( base, duration );
      return 0;
      }

    static int subDuration( int base, int points )
      {
      int duration = base;
      for( int i = 0; i < points; i++ ) {
        base >>= 1;
        duration += base;
        }
      return duration;
      }

  public:
    CsDuration( int duration ) :
      mDuration(duration)
      {
      for( mBaseDuration = duraBreve; (mDuration < mBaseDuration) && (mBaseDuration > 1); mBaseDuration /= 2 );
      mPoints = points( mBaseDuration, mDuration );
      }

    int baseDuration() const { return mBaseDuration; }

    int points() const { return mPoints; }

    int durationMul() const { return qBound( 1, mBaseDuration * 2, duraBreve ); }

    int durationDiv() const { return qBound( 1, mBaseDuration / 2, duraBreve ); }

    int durationPointsAdd() const { return subDuration( mBaseDuration, (mBaseDuration >> (mPoints + 1)) > 1 ? mPoints + 1 : mPoints ); }

    int durationPointsSub() const { return subDuration( mBaseDuration, mPoints > 0 ? mPoints - 1 : 0 ); }

  };





CsCell::CsCell(int type, int vx, int vy, int vw, int vh, int tsize, bool clefG, int pitch, int duration) :
  mCellType(type),
  mVisualPosX(vx),
  mVisualPosY(vy),
  mVisualWidth(vw),
  mVisualHeight(vh),
  mParam1(tsize)
  {
  static int gClef[39] = {
    //                                90s,   89,     88    87s     86    85s    84
                                      0x100, 0,      1,    0x102,  0x2,  0x103, 0x3,

    //83    82s    81    80s    79    78s    77      76    75s     74    73s    72
      0x4,  0x105, 0x5,  0x106, 0x6,  0x107, 0x7,    0x8,  0x109,  0x9,  0x10a, 0xa,

    //71    70s    69    68s    67    66s    65      64    63s     62    61s    60
      0xb,  0x10c, 0xc,  0x10d, 0xd,  0x10e, 0xe,    0xf,  0x110,  0x10, 0x111, 0x11,

    //59    58s    57    56s    55    54s    53      52
      0x12, 0x113, 0x13, 0x114, 0x14, 0x115, 0x15,   0x16
  };

  static int fClef[38] = {
    //      58s    57    56s    55    54s    53      52    51s     50    49s    48
            0x100, 0,    0x101, 0x1,  0x102, 0x2,    0x3,  0x104,  0x4,  0x105, 0x5,

    //47    46s    45    44s    43    42s    41      40    39s     38    37s    36
      0x6,  0x107, 0x7,  0x108, 0x8,  0x109, 0x9,    0xa,  0x10b,  0xb,  0x10c, 0xc,

    //35    34s    33    32s    31    30s    29      28    27s     26    25s    24
      0xd,  0x10e, 0xe,  0x10f, 0xf,  0x110, 0x10,   0x11, 0x112,  0x12, 0x113, 0x13,

    //23    22s    21
      0x14, 0x115, 0x15
  };

  mText = classicNote(3, pitch, duration);
  if( pitch == noteMute ) {
    if( clefG ) {
      mParam0 = gClef[25];
      }
    else {
      mParam0 = fClef[18];
      }
    }
  else {
    if( clefG ) {
      pitch = 90 - pitch;
      if( pitch >= 0 && pitch < 39 )
        mParam0 = gClef[pitch];
      else
        mText.clear();
      }
    else {
      pitch = 58 - pitch;
      if( pitch >= 0 && pitch < 38 )
        mParam0 = fClef[pitch];
      else
        mText.clear();
      }
    }

  }




CsCell::CsCell(int type, int vx, int vy, int vw, int vh, int startTime, int duration) :
  mCellType(type),
  mVisualPosX(vx),
  mVisualPosY(vy),
  mVisualWidth(vw),
  mVisualHeight(vh),
  mParam0(startTime),
  mParam1(duration)
  {
  }




QString CsCell::classicNote(int var, int pitch, int duration)
  {
  if( var == 3 ) {
    //Return modern note
    QString postfixDuration;
    CsDuration d( duration );
    if( pitch == noteMute ) {
      switch( d.baseDuration() ) {
        case duraBreve :
          postfixDuration += QString("𝄺 ");
          break;
        case duraHole :
          postfixDuration += QString("𝄻 ");
          break;
        case duraHalf :
          postfixDuration += QString("𝄼 ");
          break;
        case duraQuarter :
          postfixDuration += QString("𝄽 ");
          break;
        case duraEighth :
          postfixDuration += QString("𝄾 ");
          break;
        case duraSixteenth :
          postfixDuration += QString("𝄿 ");
          break;
        case duraThirtySecond :
          postfixDuration += QString("𝅀 ");
          break;
        case duraSixtyFourth :
          postfixDuration += QString("𝅁 ");
          break;
        case duraOneHundredTwentyEighth :
          postfixDuration += QString("𝅂 ");
          break;
        }
      }
    else {
      switch( d.baseDuration() ) {
        case duraBreve :
          postfixDuration += QString("𝅜 ");
          break;
        case duraHole :
          postfixDuration += QString("𝅝 ");
          break;
        case duraHalf :
          postfixDuration += QString("𝅗𝅥 ");
          break;
        case duraQuarter :
          postfixDuration += QString("𝅘𝅥 ");
          break;
        case duraEighth :
          postfixDuration += QString("𝅘𝅥𝅮 ");
          break;
        case duraSixteenth :
          postfixDuration += QString("𝅘𝅥𝅯 ");
          break;
        case duraThirtySecond :
          postfixDuration += QString("𝅘𝅥𝅰 ");
          break;
        case duraSixtyFourth :
          postfixDuration += QString("𝅘𝅥𝅱 ");
          break;
        case duraOneHundredTwentyEighth :
          postfixDuration += QString("𝅘𝅥𝅲 ");
          break;
        }
      }

    switch( d.points() ) {
      case 1 : postfixDuration += QString("."); QChar(0x2024/*0x1cdd*/); break;
      case 2 : postfixDuration += QString(".."); QChar(0x2025/*0x1cde*/); break;
      case 3 : postfixDuration += QString("..."); QChar(0x2026/*0x1cdf*/); break;
      case 4 : postfixDuration += QString("...."); QChar(0x205e); break;
      }

    return postfixDuration;
    }
  else {
    QString octaveText;
    switch(pitch / 12) {
      case 0 : octaveText = QString(",,,,,"); break;
      case 1 : octaveText = QString(",,,,"); break;
      case 2 : octaveText = QString(",,,"); break;
      case 3 : octaveText = QString(",,"); break;
      case 4 : octaveText = QString(","); break;
      case 5 : break;
      case 6 : octaveText = QString("'"); break;
      case 7 : octaveText = QString("''"); break;
      case 8 : octaveText = QString("'''"); break;
      case 9 : octaveText = QString("''''"); break;
      case 10 : octaveText = QString("'''''"); break;
      }

    if( var == 0 ) {
      //Simple note with octave
      if( pitch == noteMute )
        return QString("0");
      switch( csNoteInOctave(pitch) ) {
        case noteC : return QString("1") + octaveText;
        case noteCd : return QString("1|2") + octaveText;
        case noteD : return QString("2") + octaveText;
        case noteDd : return QString("2|3") + octaveText;
        case noteE : return QString("3") + octaveText;
        case noteF : return QString("4") + octaveText;
        case noteFd : return QString("4|5") + octaveText;
        case noteG : return QString("5") + octaveText;
        case noteGd : return QString("5|6") + octaveText;
        case noteA : return QString("6") + octaveText;
        case noteAd : return QString("6|7") + octaveText;
        case noteB : return QString("7") + octaveText;
        }
      }
    else if( var == 2 ) {
      //Letter note with octave
      if( pitch == noteMute )
        return QString("M");
      switch( csNoteInOctave(pitch) ) {
        case noteC : return QString("C") + octaveText;
        case noteCd : return QString("C|D") + octaveText;
        case noteD : return QString("D") + octaveText;
        case noteDd : return QString("D|E") + octaveText;
        case noteE : return QString("E") + octaveText;
        case noteF : return QString("F") + octaveText;
        case noteFd : return QString("F|G") + octaveText;
        case noteG : return QString("G") + octaveText;
        case noteGd : return QString("G|A") + octaveText;
        case noteA : return QString("A") + octaveText;
        case noteAd : return QString("A|B") + octaveText;
        case noteB : return QString("B") + octaveText;
        }
      }
    else if( var == 4 ) {
      if( pitch == noteMute )
        return QString("r");
      switch( csNoteInOctave(pitch) ) {
        case noteC : return QString("c") + octaveText;
        case noteCd : return QString("cs") + octaveText;
        case noteD : return QString("d") + octaveText;
        case noteDd : return QString("ds") + octaveText;
        case noteE : return QString("e") + octaveText;
        case noteF : return QString("f") + octaveText;
        case noteFd : return QString("fs") + octaveText;
        case noteG : return QString("g") + octaveText;
        case noteGd : return QString("gs") + octaveText;
        case noteA : return QString("a") + octaveText;
        case noteAd : return QString("as") + octaveText;
        case noteB : return QString("b") + octaveText;
        }
      }
    else {
      //With duration
      QString prefixDuration;
      QString postfixDuration;
      CsDuration d( duration );
      switch( d.baseDuration() ) {
        case duraBreve :
          postfixDuration = QChar(0x21db);
          break;
        case duraHole :
          postfixDuration = QChar(0x21d2);
          break;
        case duraHalf :
          postfixDuration = QChar(0x2192);
          break;
        case duraEighth :
          prefixDuration = QChar(0x2190);
          break;
        case duraSixteenth :
          prefixDuration = QChar(0x21d0);
          break;
        case duraThirtySecond :
          prefixDuration = QChar(0x21da);
          break;
        case duraSixtyFourth :
          prefixDuration = QChar(0x2b45);
          break;
        case duraOneHundredTwentyEighth :
          prefixDuration = QChar(0x2190);
          prefixDuration += QChar(0x2b45);
          break;
        case 1 :
          prefixDuration = QChar(0x21d0);
          prefixDuration += QChar(0x2b45);
        }

      switch( d.points() ) {
        case 1 : postfixDuration += QChar(0x2027); break;
        case 2 : postfixDuration += QChar(0x205a); break;
        case 3 : postfixDuration += QChar(0x205d); break;
        case 4 : postfixDuration += QChar(0x205e); break;
        }

      if( pitch == noteMute )
        return prefixDuration + QString("0") + postfixDuration;
      switch( csNoteInOctave(pitch) ) {
        case noteC : return prefixDuration + QString("1") + octaveText + postfixDuration;
        case noteCd : return prefixDuration + QString("#1") + octaveText + postfixDuration;
        case noteD : return prefixDuration + QString("2") + octaveText + postfixDuration;
        case noteDd : return prefixDuration + QString("#2") + octaveText + postfixDuration;
        case noteE : return prefixDuration + QString("3") + octaveText + postfixDuration;
        case noteF : return prefixDuration + QString("4") + octaveText + postfixDuration;
        case noteFd : return prefixDuration + QString("#4") + octaveText + postfixDuration;
        case noteG : return prefixDuration + QString("5") + octaveText + postfixDuration;
        case noteGd : return prefixDuration + QString("#5") + octaveText + postfixDuration;
        case noteA : return prefixDuration + QString("6") + octaveText + postfixDuration;
        case noteAd : return prefixDuration + QString("#6") + octaveText + postfixDuration;
        case noteB : return prefixDuration + QString("7") + octaveText + postfixDuration;
        }
      }
    }
  return QString{};
  }







int CsCell::noteLine( int pitch )
  {
  if( pitch == noteMute )
    return 5;
  switch( pitch ) {
    case 95 : return -10;//B
    case 94 :            //A#
    case 93 : return -9; //A
    case 92 :            //G#
    case 91 : return -8; //G
    case 90 :            //#F
    case 89 : return -7; //F
    case 88 : return -6; //E
    case 87 :            //#D
    case 86 : return -5; //D
    case 85 :            //#C
    case 84 : return -4; //C
    case 83 : return -3; //B
    case 82 :            //#A
    case 81 : return -2; //A
    case 80 :            //#G
    case 79 : return -1; //G
    case 78 :            //#F
    case 77 : return 0;  //F
    case 76 : return 1;  //E
    case 75 :            //#D
    case 74 : return 2;  //D
    case 73 :            //#C
    case 72 : return 3;  //C
    case 71 : return 4;  //B
    case 70 :            //#A
    case 69 : return 5;  //A
    case 68 :            //#G
    case 67 : return 6;  //G
    case 66 :            //#F
    case 65 : return 7;  //F
    case 64 : return 8;  //E
    case 63 :            //#D
    case 62 : return 9;  //D
    case 61 :            //#C
    case 60 : return 10; //C
    case 59 : return 11; //B
    case 58 :            //#A
    case 57 : return 12; //A
    case 56 :            //#G
    case 55 : return 13; //G
    case 54 :            //#F
    case 53 : return 14; //F
    case 52 : return 15; //E
    }
  return 100;
  }




struct CsChordMod
  {
    const char *mChord;
    int         mVariant;
  };

static const CsChordMod csChord[36] = {
  { "", chordMaj },       // chordMaj,      Maj,       //0,4,7
  { "maj6", chordMaj6 },   // chordMaj6,     Maj6,    //0,4,7,9
  { "maj7", chordMaj7 },   // chordMaj7,     Maj7,     //0,4,7,11
  { "7#11", chordMaj7sharp11 },   // chordMaj7sharp11,  M7#11, //0,4,6,7,11
  { "add9", chordMaj_9_ },   // chordMaj_9_,   Madd9,    //0,2,4,7
  { "7(9)", chordMaj7_9_ },   // chordMaj7_9_,  M7(9),    //0,2,4,7,11
  { "6(9)", chordMaj6_9_ },   // chordMaj6_9_,  M6(9),    //0,2,4,7,9
  { "aug", chordAug },       // chordAug,   aug,       //0,4,8
  { "m", chordMin },      // chordMin,      m,    //0,3,7
  { "m6", chordMin6 },     // chordMin6,     m6,    //0,3,7,9
  { "m7", chordMin7 },     // chordMin7,     m7,    //0,3,7,10
  { "m7b5", chordMin7b5 },   // chordMin7b5,   m7b5,    //0,3,6,10
  { "m(9)", chordMin_9_ },   // chordMin_9_,   m(9),    //0,2,3,7
  { "m7(9)", chordMin7_9_ },  // chordMin7_9_,  m7(9),    //0,2,3,7,10
  { "m7(11)", chordMin7_11_ }, // chordMin7_11_, m7(11),    //0,3,5,7,10
  { "mM7", chordMinMaj7 },    // chordMinMaj7,  mM7,    //0,3,7,11
  { "mM7(9)", chordMinMaj7_9_ }, // chordMinMaj7_9_, mM7(9),  //0,2,3,7,11
  { "dim", chordDim },    // chordDim,      dim,    //0,3,6
  { "dim7", chordDim7 },   // chordDim7,     dim7,    //0,3,6,9
  { "7", chord7th },      // chord7th,      7,    //0,4,7,10
  { "7sus4", chord7sus4 },  // chord7sus4,    7sus4,    //0,5,7,10
  { "7b5", chord7b5 },    // chord7b5,      7b5,    //0,4,6,10
  { "7(9)", chord7_9_ },   // chord7_9_,     7(9),    //0,2,4,7,10
  { "7(#11)", chord7sharp11 }, // chord7sharp11, 7(#11),    //0,4,6,7,10
  { "7(13)", chord7_13_ },  // chord7_13_,    7(13),    //0,4,7,9,10
  { "7(b9)", chord7_b9_ },  // chord7_b9_,    7(b9),    //0,1,4,7,10
  { "7(b13)", chord7_b13_ }, // chord7_b13_,   7(b13),    //0,4,7,8,10
  { "7(#9)", chord7_sharp9_ },  // chord7_sharp9_,7(#9),    //0,3,4,7,10
  { "M7aug", chordMaj7aug },  // chordMaj7aug,  M7aug,    //0,4,8,11
  { "7aug", chord7aug },   // chord7aug,     7aug,    //0,4,8,10
  { "1+8", chord1plus8 },    // chord1plus8,   1+8,    //0
  { "1+5", chord1plus5 },    // chord1plus5,   1+5,    //0,7
  { "sus4", chordSus4 },   // chordSus4,     sus4    //0,5,7
  { "1+2+5", chord1plus2plus5 },   // chord1plus2plus5, 1+2+5 //0,2,7
  { "sus2", chord1plus2plus5 },
  { nullptr, chordMaj }
};




struct CsChordBase
  {
    const char *mChord;
    int         mLenght;
    int         mPitch;
    CsNotes     mNote;
  };

static const CsChordBase csChordBase[17] = {
  { "C#", 2, 1, noteCd },  // noteCd,
  { "Db", 2, 1, noteCd },  // noteCd,
  { "D#", 2, 3, noteDd },  // noteDd,
  { "Eb", 2, 3, noteDd },  // noteDd,
  { "F#", 2, 6, noteFd },  // noteFd,
  { "Gb", 2, 6, noteFd },  // noteFd,
  { "G#", 2, 8, noteGd },  // noteGd,
  { "Ab", 2, 8, noteGd },  // noteGd,
  { "A#", 2,10, noteAd },  // noteAd,
  { "Bb", 2,10, noteAd },  // noteAd,
  { "C",  1, 0, noteC },  // noteC,
  { "D",  1, 2, noteD },  // noteD,
  { "E",  1, 4, noteE },  // noteE,
  { "F",  1, 5, noteF },  // noteF,
  { "G",  1, 7, noteG },  // noteG,
  { "A",  1, 9, noteA },  // noteA,
  { "B",  1,11, noteB }   // noteB
};



// QString CsCell::chordString() const
//   {
//   if( mChord < 0 )
//     return QString();
//   if( !mChordCache.isEmpty() )
//     return mChordCache;
//   switch( csChordPitch( mChord ) ) {
//     case noteCd : mChordCache = QString("C#"); break;
//     case noteDd : mChordCache = QString("D#"); break;
//     case noteFd : mChordCache = QString("F#"); break;
//     case noteGd : mChordCache = QString("G#"); break;
//     case noteAd : mChordCache = QString("A#"); break;
//     default :
//     case noteC : mChordCache = QString("C"); break;
//     case noteD : mChordCache = QString("D"); break;
//     case noteE : mChordCache = QString("E"); break;
//     case noteF : mChordCache = QString("F"); break;
//     case noteG : mChordCache = QString("G"); break;
//     case noteA : mChordCache = QString("A"); break;
//     case noteB : mChordCache = QString("B"); break;
//     }

//   static QMap<int,QString> chordMap;
//   if( chordMap.isEmpty() ) {
//     //Fill map with chords
//     for( int i = 0; csChord[i].mChord != nullptr; i++ )
//       chordMap.insert( csChord[i].mVariant, QString::fromLatin1(csChord[i].mChord) );
//     }

//   return mChordCache += chordMap.value( csChordMod( mChord ) );
//   }









int CsCell::chordMod(const QString mod)
  {
  static QMap<QString,int> chordMap;
  if( chordMap.isEmpty() ) {
    //Fill map with chords
    for( int i = 0; csChord[i].mChord != nullptr; i++ )
      chordMap.insert( QString::fromLatin1(csChord[i].mChord), csChord[i].mVariant );
    }

  if( chordMap.contains(mod) )
    return chordMap.value( mod );

  return mod.isEmpty() ? chordMaj : chordCancel;
  }



int CsCell::chordParse(const QString chordName)
  {
  if( chordName == "-" )
    return chordContinue;
  for( int i = 0; i < 17; i++ )
    if( chordName.startsWith(QString(csChordBase[i].mChord)) ) {
      QString mod = chordName.mid( csChordBase[i].mLenght );
      int chord = chordMod( mod );
      if( chord == chordCancel )
        return noteMute;
      return csChordCode( csChordBase[i].mNote, chord );
      }
  return noteMute;
  }



QString CsCell::chordModBuild(int chordCode)
  {
  static QMap<quint32,QString> chordMap;
  if( chordMap.isEmpty() ) {
    //Fill map with chords
    for( int i = 0; csChord[i].mChord != nullptr; i++ )
      chordMap.insert( csChord[i].mVariant, QString::fromLatin1(csChord[i].mChord) );
    }

  chordCode = csChordMod( chordCode );
  if( chordMap.contains(chordCode) )
    return chordMap.value(chordCode);
  return QString("---");
  }


QString CsCell::chordFullBuild(int chordCode)
  {
  static QMap<int,QString> chordNameMap;
  if( chordNameMap.isEmpty() ) {
    //Fill map
    for( int i = 16; i >= 0; i-- )
      chordNameMap.insert( csChordBase[i].mNote, QString(csChordBase[i].mChord) );
    }
  if( chordCode == 0 )
    return QString{};
  return chordNameMap.value( csNoteInOctave(csChordPitch(chordCode)) ) + chordModBuild( chordCode );
  }




bool CsCell::isChord(const QString &chordName)
  {
  return chordParse( chordName ) != noteMute;
  }



bool CsCell::isDuration(QChar ch)
  {
  return CS_JOINT_DURATION <= ch.unicode() && ch.unicode() < CS_SPACE_DURATION + 64;
  }



QChar CsCell::charDuration(bool space, int duration)
  {
  return space ? QChar( CS_SPACE_DURATION + duration / CS_STEP_LYRIC ) : QChar( CS_JOINT_DURATION + duration / CS_STEP_LYRIC );
  }



int CsCell::durationFromChar(QChar ch, bool &space)
  {
  if( CS_JOINT_DURATION <= ch.unicode() && ch.unicode() < CS_JOINT_DURATION + 64 ) {
    space = false;
    return (ch.unicode() - CS_JOINT_DURATION) * CS_STEP_LYRIC;
    }
  if( CS_SPACE_DURATION <= ch.unicode() && ch.unicode() < CS_SPACE_DURATION + 64 ) {
    space = true;
    return (ch.unicode() - CS_SPACE_DURATION) * CS_STEP_LYRIC;
    }
  space = true;
  return duraHalf;
  }


