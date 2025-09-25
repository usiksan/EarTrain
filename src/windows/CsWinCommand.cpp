/*
  Проект "IronArt"
    Визуальное программирование роботов
  Автор
    Сибилев А.С.
  Описание
    Список команд
*/

#include "CsWinCommand.h"


QAction
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


QActionPtr maRecentFile[CS_MAX_RECENT_FILE];


