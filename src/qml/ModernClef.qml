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

  //Note lines (3 top, 5 main, 3 bot)
  Repeater {
    model: 11
    Rectangle {
      x: 0
      y: noteLineHeight * (index + 1)
      width: cellWidth + 4
      height: 1
      color: (index > 2 && index < 8) ? lineColor : "transparent"
    }
  }

  //Note visual representation
  Item {
    //visible: false
    x: 4
    y: noteLineHeight / 2 * (cellNoteLine + 1)

    //Clef symbol
    Text {
      id: noteSymbol
      font.pixelSize: cellTextSize
      font.family: csMusicFont
      text: cellText
      anchors.bottom: parent.top
      anchors.bottomMargin: -0.55 * cellTextSize
      verticalAlignment: Text.AlignBottom
    }
  }


}
