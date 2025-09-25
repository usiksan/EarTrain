/*
Project "SaliSongBook Simple score music edit, view and tutorial program"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Root of visual representation
*/
import QtQuick
import QtQuick.Controls 2.15
import QtCore
import CsBook 1.0

Window {

  id: rootWindow
  width: 640
  height: 480
  visible: true
  title: qsTr("EarTrainer") + csComposition.version
  //onActiveFocusItemChanged: console.log("Current focus", activeFocusItem )

  property int tapeCurrentIndex : -1


  StackView {
    id: rootStack
    anchors.fill: parent
    initialItem: pageMenu
  }

  Component {
    id: pageMenu
    PageMenu {}
  }

  Component {
    id: pageIntervalCompare
    PageIntervalCompare {}
  }

  Component {
    id: pageScore
    PageScore {}
  }

  Component {
    id: pageViewSettings
    PageViewSettings {}
  }


}
