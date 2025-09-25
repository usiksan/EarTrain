/*
  Project "Ear train"
  Author
    Sibilev Alexander S.

  Web
    www.saliLab.com
    www.saliLab.ru

  Description
    Program configuration
  History
    25.09.2025 v0.1  Begin creating
*/
#ifndef CSSONGEDITORCONFIG_H
#define CSSONGEDITORCONFIG_H

#define CS_AUTHOR                  QString("Sibilev A.S.")
#define CS_ORGANIZATION_NAME       QString("Sali")
#define CS_ORGANIZATION_DOMAIN     QString("http://salilab.com/")
#define CS_APPLICATION_NAME        QString("EarTrain")
#define CS_APPLICATION_WEB         QString("http://salilab.com/eartrain")

//Programm version
#define CS_VERSION_MAJOR           0 //Global changes (releases)
#define CS_VERSION_MINOR           1 //Local changes


#define CS_STEP_CHORD                64
#define CS_STEP_NOTE                 16
#define CS_STEP_LYRIC                16

#define CS_DRUM_CHANNEL              9
#define CS_BASS_CHANNEL              10
#define CS_GUITAR_CHANNEL            11
#define CS_PAD_CHANNEL               12

#define CS_DEFAULT_DRUM              16384
#define CS_DEFAULT_BASS              33
#define CS_DEFAULT_GUITAR            25
#define CS_DEFAULT_PAD               49

#define CS_DRUM_DURATION             8


#define CSS_LANGUAGE                 "Language"
#define CSS_WMAIN_MAX                "WMainMax"
#define CSS_STRUCTURE_SPLITTER       "StructureSplitter"
#define CSS_VSPLITTER                "VSplitter"
#define CSS_HSPLITTER                "HSplitter"
#define CSS_TREE_COL0                "TreeCol0"
#define CSS_TREE_COL1                "TreeCol1"
#define CSS_RECENT_PROJECTS          "RecentProjects"
#define CSS_RECENT_FILES             "RecentFiles"

#define KEY_NOTE_VIEW_TYPE           "NoteViewType"
#define KEY_DURATION_FACTOR          "DurationFactor"
#define KEY_USE_DURATION_WIDTH       "UseDurationWidth"
#define KEY_USE_NOTE_CIRCLE          "UseNoteCircle"
#define KEY_USE_CIRCLE_DURATION      "UseCircleDuration"
#define KEY_TEXT_SIZE                "TextSize"

#endif // CSSONGEDITORCONFIG_H
