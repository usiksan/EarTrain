#ifndef CSNOTECHORD_H
#define CSNOTECHORD_H

#include <QString>

enum CsNotes {
  noteC,
  noteCd,
  noteD,
  noteDd,
  noteE,
  noteF,
  noteFd,
  noteG,
  noteGd,
  noteA,
  noteAd,
  noteB,
  noteMute = 255
  };



enum CsOctave {
  octaveNoName   = 0,
  octaveSubContr = 12,
  octaveContr    = 24,
  octaveBig      = 36,
  octaveSmall    = 48,
  octaveFirst    = 60,
  octaveSecond   = 72,
  octaveThird    = 84,
  octaveFourth   = 96,
  octaveFifth    = 108,
  octaveSixth    = 120
  };

inline CsNotes csNoteInOctave( int note ) { return (CsNotes)(note % 12); }


constexpr inline bool csKeyNoBlackFromLeft( int note ) { note %= 12; return (note == noteC || note == noteF); }
constexpr inline bool csKeyNoBlackFromRight( int note ) { note %= 12; return (note == noteE || note == noteB); }
constexpr inline bool csKeyIsBlack( int note ) { note %= 12; return note == noteCd || note == noteDd || note == noteFd || note == noteGd || note == noteAd; }
constexpr inline bool csKeyIsWhite( int note ) { return !csKeyIsBlack(note); }
constexpr inline int  csChordModCode( int off1, int off2, int off3, int off4 ) { return ((off1 & 0xf) << 8) | ((off2 & 0xf) << 12) | ((off3 & 0xf) << 16) | ((off4 & 0xf) << 20); }
constexpr inline int  csChordCode( int pitch, int mod ) { return (pitch & 0x7f) | (mod & 0xffff00); }
constexpr inline int  csChordCode( int pitch, int off1, int off2, int off3, int off4 ) { return csChordCode( pitch, csChordModCode( off1, off2, off3, off4 ) ); }
constexpr inline int  csChordPitch( int chordCode ) { return chordCode & 0x7f; }
constexpr inline int  csChordMod( int chordCode ) { return chordCode & 0xffff00; }
constexpr inline int  csChordOff1( int chordCode ) { return (chordCode >> 8) & 0xf; }
constexpr inline int  csChordOff2( int chordCode ) { return (chordCode >> 12) & 0xf; }
constexpr inline int  csChordOff3( int chordCode ) { return (chordCode >> 16) & 0xf; }
constexpr inline int  csChordOff4( int chordCode ) { return (chordCode >> 20) & 0xf; }
constexpr inline int  csGuitarChordCode( int cord6, int cord5, int cord4, int cord3, int cord2, int cord1 ) { return (cord1 << 0) | (cord2 << 4) | (cord3 << 8) | (cord4 << 12) | (cord5 << 16) << (cord6 << 20); }

enum CsWhiteKey {
  whiteC,
  whiteD,
  whiteE,
  whiteF,
  whiteG,
  whiteA,
  whiteB
  };


enum CsWhiteOctave {
  whiteOctaveNoName   = 0,
  whiteOctaveSubContr = 7,
  whiteOctaveContr    = 14,
  whiteOctaveBig      = 21,
  whiteOctaveSmall    = 28,
  whiteOctaveFirst    = 35,
  whiteOctaveSecond   = 42,
  whiteOctaveThird    = 49,
  whiteOctaveFourth   = 56,
  whiteOctaveFifth    = 63,
  whiteOctaveSixth    = 70
  };



enum CsChordVar {
  chordMaj         = csChordModCode(4,7,0,0),   //0,4,7
  chordMaj6        = csChordModCode(4,7,9,0),   //0,4,7,9
  chordMaj7        = csChordModCode(4,7,11,0),  //0,4,7,11
  chordMaj7sharp11 = csChordModCode(4,6,7,11),  //0,4,6,7,11
  chordMaj_9_      = csChordModCode(2,4,7,0),   //0,2,4,7
  chordMaj7_9_     = csChordModCode(2,4,7,11),  //0,2,4,7,11
  chordMaj6_9_     = csChordModCode(2,4,7,9),   //0,2,4,7,9
  chordAug         = csChordModCode(4,8,0,0),   //0,4,8
  chordMin         = csChordModCode(3,7,0,0),   //0,3,7
  chordMin6        = csChordModCode(3,7,9,0),   //0,3,7,9
  chordMin7        = csChordModCode(3,7,10,0),  //0,3,7,10
  chordMin7b5      = csChordModCode(3,6,10,0),  //0,3,6,10
  chordMin_9_      = csChordModCode(2,3,7,0),   //0,2,3,7
  chordMin7_9_     = csChordModCode(2,3,7,10),  //0,2,3,7,10
  chordMin7_11_    = csChordModCode(3,5,7,10),  //0,3,5,7,10
  chordMinMaj7     = csChordModCode(3,7,11,0),  //0,3,7,11
  chordMinMaj7_9_  = csChordModCode(2,3,7,11),  //0,2,3,7,11
  chordDim         = csChordModCode(3,6,0,0),   //0,3,6
  chordDim7        = csChordModCode(3,6,9,0),   //0,3,6,9
  chord7th         = csChordModCode(4,7,10,0),  //0,4,7,10
  chord7sus4       = csChordModCode(5,7,10,0),  //0,5,7,10
  chord7b5         = csChordModCode(4,6,10,0),  //0,4,6,10
  chord7_9_        = csChordModCode(2,4,7,10),  //0,2,4,7,10
  chord7sharp11    = csChordModCode(4,6,7,10),  //0,4,6,7,10
  chord7_13_       = csChordModCode(4,7,9,10),  //0,4,7,9,10
  chord7_b9_       = csChordModCode(1,4,7,10),  //0,1,4,7,10
  chord7_b13_      = csChordModCode(4,7,8,10),  //0,4,7,8,10
  chord7_sharp9_   = csChordModCode(3,4,7,10),  //0,3,4,7,10
  chordMaj7aug     = csChordModCode(4,8,11,0),  //0,4,8,11
  chord7aug        = csChordModCode(4,8,10,0),  //0,4,8,10
  chord1plus8      = csChordModCode(0,0,0,0),   //0
  chord1plus5      = csChordModCode(7,0,0,0),   //0,7
  chordSus4        = csChordModCode(5,7,0,0),   //0,5,7
  chord1plus2plus5 = csChordModCode(2,7,0,0),  //0,2,7
  chordCancel      = csChordModCode(15,15,15,15),
  chordContinue    = 254
  };




#define duraBreve                  512 // 2
#define duraHole                   256 // 1
#define duraHalf                   128 // 1/2
#define duraQuarter                 64 // 1/4
#define duraEighth                  32 // 1/8
#define duraSixteenth               16 // 1/16
#define duraThirtySecond             8 // 1/32
#define duraSixtyFourth              4 // 1/64
#define duraOneHundredTwentyEighth   2 // 1/128

#define cdGuitarUp                   128
#define cdGuitarDown                 129
#define cdGuitarMute                 130
#define cdGuitarCord1                131
#define cdGuitarCord2                132
#define cdGuitarCord3                133
#define cdGuitarCord4                134
#define cdGuitarCord5                135
#define cdGuitarCord6                136

#define cdVoiceSelect                137
#define cdTempoSelect                139


#endif // CSNOTECHORD_H
