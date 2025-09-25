/*
Project "SaliSongBook Simple score music edit, view and tutorial program"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Single modern note display
*/
import QtQuick 2.15

Item {
  property real lineHeight : cellHeight
  property real noteLineHeight: lineHeight / 12
  property real noteWidth : noteSymbol.contentWidth // + 8 * factor
  property color lineColor : "gray"
  property color noteColor : cellActive ? "blue" : "black"

  //Note lines (3 top, 5 main, 3 bot)
  //Here we don't draw 5 main lines because it drawed in Clef visualisation
  Repeater {
    model: 11
    Rectangle {
      x: 4
      y: noteLineHeight * (index + 1)
      width: noteWidth
      height: 1
      color: (cellNoteLine <= (index * 2 + 1) && index <= 2) || (cellNoteLine >= index * 2 && index >= 8 ) ? lineColor : "transparent"
    }
    // Rectangle {
    //   x: (index > 2 && index < 8) ? 0 : 4
    //   y: noteLineHeight * (index + 1)
    //   width: index > 2 && index < 8 ? cellWidth + 4 : noteWidth
    //   height: 1
    //   color: (cellNoteLine <= (index * 2 + 1) && index <= 2) || (index > 2 && index < 8) || (cellNoteLine >= index * 2 && index >= 8 ) ? lineColor : "transparent"
    // }
  }

  //Note visual representation
  Item {
    //visible: false
    x: 4
    y: noteLineHeight / 2 * (cellNoteLine + 1)

    //For notes upper 3 line we mirror note vertically
    transform: Rotation {
      origin.x: 2
      origin.y: 0
      axis.x: 1; axis.y: 0; axis.z: 0     // set axis.y to 1 to rotate around y-axis
      angle: cellNoteLine < 11 ? 180 : 0    // the default angle
      }

    //Sharp symbol
    Text {
      id: sharpSymbol
      visible: cellNoteSharp
      text: "♯"
      font.pixelSize: cellTextSize * 0.4
      font.family: csMusicFont
      anchors.bottom: parent.top
      anchors.bottomMargin: -0.32 * cellTextSize
      color: noteColor
    }

    //Note symbol
    Text {
      id: noteSymbol
      font.pixelSize: cellTextSize
      font.family: csMusicFont
      text: cellText
      anchors.left: sharpSymbol.right
      anchors.bottom: parent.top
      anchors.bottomMargin: -0.55 * cellTextSize
      verticalAlignment: Text.AlignBottom
      color: noteColor
    }
  }


}
