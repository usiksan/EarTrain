/*
Project "SaliSongBook Simple score music edit, view and tutorial program"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Single score visual representation
*/
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCore
import QtQuick.Dialogs
import CsBook 1.0


Page {
  id: idScorePage
  property real scale : 1.0

  header: ToolBar {
      RowLayout {
          anchors.fill: parent
          ToolButton {
            visible: rootStack.depth > 1
            icon.source: "qrc:/pic/iconBack.png"
            icon.color: "transparent"
            onClicked: rootStack.pop()
          }
          ToolButton {
            icon.source: "qrc:/pic/viewTrain.png"
            icon.color: "transparent"
            onClicked: csComposition.playStart(2)
          }
          ToolButton {
            icon.source: "qrc:/pic/playStart.png"
            icon.color: "transparent"
            onClicked: csComposition.playStart(0)
          }
          ToolButton {
            icon.source: "qrc:/pic/playPause.png"
            icon.color: "transparent"
            onClicked: csComposition.playPause()
          }
          ToolButton {
            icon.source: "qrc:/pic/playStop.png"
            icon.color: "transparent"
            onClicked: csComposition.playStop()
          }
          ToolButton {
            icon.source: "qrc:/pic/metronom.png"
            icon.color: "transparent"
            onClicked: csComposition.playRithm()
          }

          Label {
              text: csComposition.fileName == "" ? "Score" : csComposition.fileName
              elide: Label.ElideRight
              horizontalAlignment: Qt.AlignHCenter
              verticalAlignment: Qt.AlignVCenter
              Layout.fillWidth: true
          }
          ToolButton {
            id: pianoCheckBox
            icon.source: "qrc:/pic/plPiano.png"
            icon.color: "transparent"
            checkable: true
            checked: true
          }
          ToolButton {
            icon.source: "qrc:/pic/editSettings.png"
            icon.color: "transparent"
            text: qsTr("View settings")
            onClicked: rootStack.push(pageViewSettings)
          }
      }
  }



  Score {
    id: idSheet
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: pianoKeyboard.top
    anchors.top: parent.top
  }

  //At right edge column with buttons to change notes duration
  PianoKeyboard {
    id: pianoKeyboard
    visible: pianoCheckBox.checked
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    height: parent.height / 8
  }
}
