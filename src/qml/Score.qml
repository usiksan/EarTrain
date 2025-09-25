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
import CsBook 1.0

//Score itself
Flickable {
  id: idSheet
  //anchors.margins: 5
  // anchors.fill: parent
  contentHeight: cnt.height
  contentWidth: cnt.width
  // property int  textSize : csComposition.textSize
  // property real factor : textSize / 15.0
  //clip: true


//   //Current line cursor tracking
//   Connections {
//     target: csComposition
//     function onLineYChanged() {
//       if( csComposition.lineY * factor < contentY )
//         vbar.position = csComposition.lineY * factor / contentHeight
//       else if( csComposition.lineY * factor > (contentY + height - 80) )
//         vbar.position = (csComposition.lineY * factor - height + 80) / contentHeight
//       //console.log("CurrentLineY", currentLineY, height, contentHeight )
//     }
//   }

  //Play position tracking
  Connections {
    target: csComposition
    function onPlayPosYChanged() {
      if( csComposition.playPosY < contentY )
        vbar.position = csComposition.playBarY
      else if( csComposition.playPosY > (contentY + height / 3) )
        vbar.position = (csComposition.playPosY - height / 3) / contentHeight
//      console.log("PlayLineY", csComposition.playLineY )
    }
  }


  ScrollBar.vertical: ScrollBar {
    id: vbar
    Behavior on position {
      NumberAnimation { duration : 200 }
    }
  }

  Rectangle {
    id: cnt
    width: csComposition.visualWidth
    height: csComposition.visualHeight
    // border.width: 2
    // border.color: "black"

    //Score consist from elements (cell)
    Repeater {
      model: csComposition

      Rectangle {
        property bool darker : cellType === CsPlayer.CsCtSelector && (index < csComposition.fragmentBegin || index > csComposition.fragmentEnd)
        property color textColor : cellActive ? "blue" : "black"
        x: cellPosX
        y: cellPosY
        width: cellWidth
        height: cellHeight
        radius: darker ? 0 : height / 3
        border.width: cellType === CsPlayer.CsCtDigitalNote ? 1 : 0
        color: cellType === CsPlayer.CsCtTaktLine ? "lightGray" : (darker ? "white" : "transparent")
        opacity: darker ? 0.8 : 1.0

        ModernNote {
          visible: cellType === CsPlayer.CsCtClassicNote
        }

        ModernClef {
          visible: cellType === CsPlayer.CsCtClef
        }

        Text {
          visible: cellType === CsPlayer.CsCtDigitalNote || cellType === CsPlayer.CsCtText
          anchors.fill: parent
          verticalAlignment: Text.AlignVCenter
          text: cellText
          font.pixelSize: cellTextSize
          font.bold: cellActive
          horizontalAlignment: cellAlign === 0 ? Text.AlignLeft : (cellAlign === 1 ? Text.AlignHCenter : Text.AlignRight )
          color: parent.textColor
        }

        MouseArea {
          visible: cellType === CsPlayer.CsCtSelector
          anchors.top: parent.top
          anchors.bottom: parent.bottom
          anchors.left: parent.left
          width: parent.width / 3

          onClicked: csComposition.fragmentBegin = index

        }
        MouseArea {
          visible: cellType === CsPlayer.CsCtSelector
          anchors.top: parent.top
          anchors.bottom: parent.bottom
          anchors.right: parent.right
          width: parent.width / 3

          onClicked: csComposition.fragmentEnd = index
        }

      }

    }

    //Cursor (current play position)
    Rectangle {
      x: csComposition.playPosX
      y: csComposition.playPosY
      width: 2
      height: csComposition.playPosHeight
      color: "black"
      opacity: 0.5
    }
  }
}

