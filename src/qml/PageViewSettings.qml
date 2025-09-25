import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {

  header: ToolBar {
      RowLayout {
          anchors.fill: parent
          ToolButton {
            icon.source: "qrc:/pic/iconBack.png"
            icon.color: "transparent"
            onClicked: {
              csComposition.songBuild()
              rootStack.pop()
            }
          }
          Label {
              text: "Score settings"
              elide: Label.ElideRight
              horizontalAlignment: Qt.AlignHCenter
              verticalAlignment: Qt.AlignVCenter
              Layout.fillWidth: true
          }
      }
  }

  TextMetrics {
    id: fieldWidthMetrics
    text: qsTr("As digit standard")
    font.pixelSize: textSize
  }

  property int textSize : csComposition.isMobile ? 20 : 12
  property int fieldWidth : fieldWidthMetrics.advanceWidth * 1.3 + idBaseTextSize.height + 10 // csComposition.isMobile ? 80 : 40


  Flickable {
    anchors.fill: parent
    contentHeight: idContent.height + 50
    Grid {
      id: idContent
      columns: 2
      spacing: 5

      Text {
        height: idBaseTextSize.height
        verticalAlignment: Text.AlignVCenter
        text: qsTr("Note type:")
        font.pixelSize: textSize
      }
      ComboBox {
        model: [
          qsTr("Modern notes"),
          qsTr("As digit standard"),
          qsTr("As letter")
        ]
        // When an item is selected, update the backend.
        onActivated: csPlayer.viewMode = currentIndex
        // Set the initial currentIndex to the value stored in the backend.
        Component.onCompleted: currentIndex = csPlayer.viewMode
        width: fieldWidth
        font.pixelSize: textSize
      }



      Text {
        height: idBaseTextSize.height
        verticalAlignment: Text.AlignVCenter
        text: qsTr("Line spacing:")
        font.pixelSize: textSize
      }
      SpinBox {
        from: 10
        to: 60
        value: csPlayer.lineSpacing
        onValueModified: csPlayer.lineSpacing = value
        width: fieldWidth
        font.pixelSize: textSize
      }



      Text {
        height: idBaseTextSize.height
        verticalAlignment: Text.AlignVCenter
        text: qsTr("Title text size:")
        font.pixelSize: textSize
      }
      SpinBox {
        from: 10
        to: 60
        value: csPlayer.titleTextSize
        onValueModified: csPlayer.titleTextSize = value
        width: fieldWidth
        font.pixelSize: textSize
      }



      Text {
        height: idBaseTextSize.height
        verticalAlignment: Text.AlignVCenter
        text: qsTr("Tield text size:")
        font.pixelSize: textSize
      }
      SpinBox {
        from: 10
        to: 60
        value: csPlayer.fieldTextSize
        onValueModified: csPlayer.fieldTextSize = value
        width: fieldWidth
        font.pixelSize: textSize
      }



      Text {
        height: idBaseTextSize.height
        verticalAlignment: Text.AlignVCenter
        text: qsTr("Finger text size:")
        font.pixelSize: textSize
      }
      SpinBox {
        from: 6
        to: 60
        value: csPlayer.fingerTextSize
        onValueModified: csPlayer.fingerTextSize = value
        width: fieldWidth
        font.pixelSize: textSize
      }



      Text {
        height: idBaseTextSize.height
        verticalAlignment: Text.AlignVCenter
        text: qsTr("Text size:")
        font.pixelSize: textSize
      }
      SpinBox {
        id: idBaseTextSize
        from: 10
        to: 30
        value: csPlayer.textSize
        onValueModified: csPlayer.textSize = value
        width: fieldWidth
        font.pixelSize: textSize
      }



      Text {
        height: idBaseTextSize.height
        verticalAlignment: Text.AlignVCenter
        text: qsTr("Lyric text size:")
        font.pixelSize: textSize
      }
      SpinBox {
        from: 10
        to: 60
        value: csPlayer.lirycSize
        onValueModified: csPlayer.lirycSize = value
        width: fieldWidth
        font.pixelSize: textSize
      }



      Text {
        height: idBaseTextSize.height
        verticalAlignment: Text.AlignVCenter
        text: qsTr("Chord text size:")
        font.pixelSize: textSize
      }
      SpinBox {
        from: 10
        to: 60
        value: csPlayer.chordSize
        onValueModified: csPlayer.chordSize = value
        width: fieldWidth
        font.pixelSize: textSize
      }



      Text {
        height: idBaseTextSize.height
        verticalAlignment: Text.AlignVCenter
        text: qsTr("Classic note size:")
        font.pixelSize: textSize
      }
      SpinBox {
        from: 10
        to: 70
        value: csPlayer.classicNoteSize
        onValueModified: csPlayer.classicNoteSize = value
        width: fieldWidth
        font.pixelSize: textSize
      }



      Text {
        height: idBaseTextSize.height
        verticalAlignment: Text.AlignVCenter
        text: qsTr("Digit note size:")
        font.pixelSize: textSize
      }
      SpinBox {
        from: 10
        to: 60
        value: csPlayer.digitNoteSize
        onValueModified: csPlayer.digitNoteSize = value
        width: fieldWidth
        font.pixelSize: textSize
      }



      Text {
        height: idBaseTextSize.height
        verticalAlignment: Text.AlignVCenter
        text: qsTr("Hole note width (pixels):")
        font.pixelSize: textSize
      }
      SpinBox {
        from: 200
        to: 600
        value: csPlayer.widthPerHole
        onValueModified: csPlayer.widthPerHole = value
        width: fieldWidth
        font.pixelSize: textSize
      }


    }

  }

}
