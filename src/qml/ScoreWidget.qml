import QtQuick
import QtQuick.Controls 2.15
import QtCore
import QtQuick.Dialogs
import CsBook 1.0

Item {

  StackView {
    id: rootStack
    anchors.fill: parent
    initialItem: pageScore
  }

  Component {
    id: pageScore
    PageScore {}
  }

  Component {
    id: pageViewSettings
    PageViewSettings {}
  }

  // onHeightChanged: console.log("height", height )
  // onWidthChanged: console.log("width", width )
  // Score {
  //   id: idSheet
  //   anchors.fill: parent
  //   // anchors.left: parent.left
  //   // anchors.right: muteDurationTable.left
  //   // anchors.bottom: chordBoard.top
  //   // anchors.top: parent.top
  // }


  // PianoKeyboard {
  //   id: pianoKeyboard
  //   visible: pianoCheckBox.checked
  //   anchors.left: parent.left
  //   anchors.right: parent.right
  //   anchors.bottom: parent.bottom
  //   height: parent.height / 8
  // }

}
