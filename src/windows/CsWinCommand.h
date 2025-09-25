/*
  Project "Sali song editor for SaliSongBook"
  Author
    Sibilev Alexander S.

  Web
    www.saliLab.com
    www.saliLab.ru

  Description
    Editor command list
*/

#ifndef MTCOMMAND
#define MTCOMMAND

#include "CsSongEditorConfig.h"
#include <QAction>
#include <QMenuBar>

extern QAction

//Main menu
//Menu file
*maFile,
*maFileNew,
*maFileNewMp3,
*maFileOpen,
*maFileRecent,
*maFileSave,
*maFileSaveAll,
*maFileSaveAs,
*maFileClose,
*maFileCloseAll,
*maFilePrint,

//Menu edit
*maEdit,
*maEditUndo,
*maEditRedo,
*maEditCut,
*maEditPaste,
*maEditCopy,
*maEditSelectAll,
*maEditSearch,
*maEditReplace,
*maEditAutoIndent,
*maEditAutoSyncro,
*maEditEditorSettings,

//Menu view
*maViewComposition,

//Play menu
*maPlayLine,
*maPlayStop,
*maPlaySync,
*maPlayAll,


//Menu help
*maHelp,
*maHelpContext,
*maHelpContens,
*maHelpWeb,
*maHelpAbout,

*maHelpCxtHome,
*maHelpCxtBackward,
*maHelpCxtForward,



*maLast;

typedef QAction *QActionPtr;

extern QActionPtr maRecentFile[CS_MAX_RECENT_FILE];

#endif // MTCOMMAND

