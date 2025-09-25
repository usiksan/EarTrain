#ifndef CSPARSER_H
#define CSPARSER_H

#include <QString>
#include <QSet>
#include <QMap>
#include <QTextCursor>

#define fieldTime               QString("t:")
#define fieldBass               QString("b:")
#define fieldLyric              QString("l:")
#define fieldFinger             QString("f:")
#define fieldGuitar             QString("g:")
#define fieldPad                QString("p:")
#define fieldTitle              QString("Title:")
#define fieldArtist             QString("Artist:")
#define fieldComposer           QString("Composer:")
#define fieldWords              QString("Words:")
#define fieldScoreCreator       QString("Score creator:")
#define fieldTempo              QString("Tempo:")
#define fieldSynthesizerProgram QString("Synthesizer program:")
#define fieldSynthesizerStyle   QString("Synthesizer style:")
#define fieldDrumProgram        QString("Drum program:")
#define fieldBassProgram        QString("Bass program:")
#define fieldGuitarProgram      QString("Guitar program:")
#define fieldPadProgram         QString("Pad program:")
#define fieldAudioFile          QString("Audio file:")
#define fieldAudioVolume        QString("Audio volume:")


class CsParser
  {
    QString mLine;
    int     mPos;
    int     mScanStart;
    int     mNotePitch;
    int     mNoteDuration;

  public:
    static QSet<QString>     mKeyWords;   //!< Key words (line name)
    static QSet<QString>     mLineChord;
    static QSet<QString>     mLineNote;
    static QSet<QString>     mLineChordNoteTime;
    static QSet<QString>     mLineChordNote;
    static QSet<QString>     mLineChordNoteLyric;
    static QMap<QString,int> mLineDrum;
    static QMap<int,int>     mGuitarChords;

    CsParser( const QString &line, int ptr );

    int     scanStart() const { return mScanStart; }
    int     scanPos() const { return mPos; }
    int     blankAndScanPos() { blank(); return mPos; }

    int     notePitch() const { return mNotePitch; }
    int     noteDuration() const { return mNoteDuration; }

    int     flashType() const { return mNotePitch; }
    int     flashInterval() const { return mNoteDuration; }

    void    blank();

    void    skeepNonBlank();

    int     scanInt();

    bool    scanDuration();

    QString scan();

    //!
    //! \brief scanNote Find and scan next note from current position
    //! \return         true if note parsed successfull
    //!
    bool    scanNote();

    //!
    //! \brief scanChord Find and scan next chord from current position
    //! \return          true if chord parsed successfull
    //!
    bool    scanChord();

    //!
    //! \brief getNote  Extract note from position and parse it
    //! \param position Position where find note
    //! \return         true if note parsed successfull
    //!
    bool    getNote(int position );

    //!
    //! \brief getChord Extract chord from position and parse it
    //! \param position Position where find chord
    //! \return         true if chord parsed successfull
    //!
    bool    getChord( int position );

    //!
    //! \brief getGuitar Extract guitar flash type from position and parse it
    //! \param position  Position where find flash type
    //! \return          true if flash parsed successfull
    //!
    bool    getGuitar( int position );

    static bool               isLineField( const QString &line, const QString &field ) { return line.startsWith(field); }
    static bool               isLineTime( const QString &line ) { return line.startsWith(fieldTime); }
    static bool               isLineBass( const QString &line ) { return line.startsWith(fieldBass); }
    static bool               isLineGuitar( const QString &line ) { return line.startsWith("g:"); }
    static bool               isLinePad( const QString &line ) { return line.startsWith("p:"); }
    static bool               isLineLyric( const QString &line ) { return line.startsWith(fieldLyric); }
    static bool               isLineFinger( const QString &line ) { return line.startsWith(fieldFinger); }
    static bool               isLineChord( const QString &line ) { return mLineChord.contains( lineType(line) ); }
    static bool               isLineNote( const QString &line ) { return mLineNote.contains( lineType(line) ); }
    static bool               isLineDrum( const QString &line ) { return mLineDrum.contains( lineType(line) ); }
    static bool               isLineTimeChordNote( const QString &line ) { return mLineChordNoteTime.contains( lineType(line) ); }
    static bool               isLineChordNote( const QString &line ) { return mLineChordNote.contains( lineType(line) ); }
    static bool               isLineChordNoteLyric( const QString &line ) { return mLineChordNoteLyric.contains( lineType(line) ); }
    static QString            lineType( const QString &line );
    static int                drumIndex( const QString &line ) { return mLineDrum.value( lineType(line) ); }
    static void               init();
    static QString            fieldValue( const QString &line );
    static int                fieldValueInt( const QString &line, int defValue, bool *ok = nullptr );
    static QString            find(const QString &lineName, QTextCursor from, QTextCursor::MoveOperation op = QTextCursor::Up);
    static QString            findValue( const QString &lineName, QTextCursor from, QTextCursor::MoveOperation op = QTextCursor::Up);
    static int                findValueInt( const QString &lineName, int defValue, bool *ok, QTextCursor from, QTextCursor::MoveOperation op = QTextCursor::Up );
    static QTextCursor        findNearestLine(std::function<bool(const QString &line)> testFunc, bool breakOnEmpty, QTextCursor from, QTextCursor::MoveOperation op = QTextCursor::Up );
    static QTextCursor        findNearestTime(QTextCursor from);
    static QList<QTextCursor> accumCursors(bool (*testFunc)( const QString &line ), bool checkPos, QTextCursor from);
    static QStringList        accumLines(  bool (*testFunc)( const QString &line ), QTextCursor from );
    static QList<QTextCursor> accumChordNoteTime(bool checkPos, QTextCursor from);
    static QList<QTextCursor> collectCursors(bool (*testFunc)( const QString &line ), QTextCursor from, QTextCursor::MoveOperation op = QTextCursor::Up );
    static QStringList        collectLines(  bool (*testFunc)( const QString &line ), QTextCursor from, QTextCursor::MoveOperation op = QTextCursor::Up );
  };

#endif // CSPARSER_H
