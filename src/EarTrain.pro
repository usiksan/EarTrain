QT += core quick quickcontrols2 multimedia
#android: QT += androidextras

CONFIG += c++20

VERSION = 1.23.0
ANDROID_VERSION_NAME = $$VERSION
ANDROID_VERSION_CODE = 12300

TARGET = EarTrain
TEMPLATE = app


SOURCES += \
        Main.cpp \
        SvLib/SvDir.cpp \
        SvLib/SvSimpleListModel.cpp \
        SvLib/SvTime2x.cpp \
        midiKeyboard/CsMidiKeyboard.cpp \
        midiKeyboard/CsMidiKeyboardLinux.cpp \
        score/CsCell.cpp \
        score/CsComposition.cpp \
        score/CsEarTrainer.cpp \
        score/CsParser.cpp \
        score/CsPlayer.cpp \
        score/CsSongParser.cpp \
        score/CsTimeLineInfo.cpp \
        score/CsViewSettings.cpp



HEADERS += \
  CsConfig.h \
  SvLib/SvDir.h \
  SvLib/SvJsonIO.h \
  SvLib/SvSimpleListModel.h \
  SvLib/SvSingleton.h \
  SvLib/SvTime2x.h \
  midiKeyboard/CsMidiKeyboard.h \
  midiKeyboard/CsMidiKeyboardLinux.h \
  score/CsCell.h \
  score/CsComposition.h \
  score/CsEarTrainer.h \
  score/CsNoteChord.h \
  score/CsParser.h \
  score/CsPlayer.h \
  score/CsSongParser.h \
  score/CsTimeLineInfo.h \
  score/CsViewSettings.h


RESOURCES += EarTrain.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

android: ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

#INCLUDEPATH += /usr/include/x86_64-linux-gnu


ANDROID_MIN_SDK_VERSION = 30
ANDROID_TARGET_SDK_VERSION = 31
ANDROID_API_VERSION = 30


