import QtQuick 2.15
import CsBook 1.0

Item {
  property int  octaveIndex : 5
  property int  octaveKey: octaveIndex - 3
  property real whiteOpacity : 0.3
  property real blackOpacity : 0.6

  Image {
    source: "qrc:/pic/octaveSmall.png"
    fillMode: Image.Stretch
    anchors.fill: parent
  }
  property real scalex : width / 371
  property real scaley : height / 178

  property real whiteWidth : 48 * scalex
  property real whiteWidthHalf : 34 * scalex
  property real whiteWidthQuad : 20 * scalex
  property real blackWidth : 31 * scalex
  property real whiteHeight: keyBot - keyMid
  property real blackHeight: keyMid - keyTop
  property real keyTop : 3 * scaley
  property real keyMid : 99 * scaley
  property real keyBot : 176 * scaley

  property int  keyC  : (csComposition.pkey0 >> (octaveKey * 2 + 0)) & 0x3
  property int  keyCD : (csComposition.pkey0 >> (octaveKey * 2 + 10)) & 0x3
  property int  keyD  : (csComposition.pkey0 >> (octaveKey * 2 + 20)) & 0x3
  property int  keyDE : (csComposition.pkey1 >> (octaveKey * 2 + 0)) & 0x3
  property int  keyE  : (csComposition.pkey1 >> (octaveKey * 2 + 10)) & 0x3
  property int  keyF  : (csComposition.pkey1 >> (octaveKey * 2 + 20)) & 0x3
  property int  keyFG : (csComposition.pkey2 >> (octaveKey * 2 + 0)) & 0x3
  property int  keyG  : (csComposition.pkey2 >> (octaveKey * 2 + 10)) & 0x3
  property int  keyGA : (csComposition.pkey2 >> (octaveKey * 2 + 20)) & 0x3
  property int  keyA  : (csComposition.pkey3 >> (octaveKey * 2 + 0)) & 0x3
  property int  keyAB : (csComposition.pkey3 >> (octaveKey * 2 + 10)) & 0x3
  property int  keyB  : (csComposition.pkey3 >> (octaveKey * 2 + 20)) & 0x3

  property color keyFree  : "transparent"
  property color keyGreen : "green"
  property color keyBlue  : "blue"
  property color keyRed   : "red"

  //White keys
  //C (1)
  Rectangle {
    id: idC
    x : 3 * scalex
    y : keyMid
    width: whiteWidth
    height: whiteHeight
    color: keyC == 0 ? keyFree : (keyC == 1 ? keyGreen : (keyC == 2 ? keyBlue : keyRed) )
    opacity: whiteOpacity

    MouseArea {
      anchors.fill: parent
      onPressed: csComposition.kbdNote( octaveIndex * 12 + 0, 127 )
      onReleased: csComposition.kbdNote( octaveIndex * 12 + 0, 0 )
    }
  }
  Rectangle {
    x : 3 * scalex
    y : keyTop
    width: whiteWidthHalf
    height: blackHeight
    color: idC.color
    opacity: whiteOpacity
  }

  //D (2)
  Rectangle {
    id: idD
    x : 55 * scalex
    y : keyMid
    width: whiteWidth
    height: whiteHeight
    color: keyD == 0 ? keyFree : (keyD == 1 ? keyGreen : (keyD == 2 ? keyBlue : keyRed) )
    opacity: whiteOpacity

    MouseArea {
      anchors.fill: parent
      onPressed: csComposition.kbdNote( octaveIndex * 12 + 2, 127 )
      onReleased: csComposition.kbdNote( octaveIndex * 12 + 2, 0 )
    }
  }
  Rectangle {
    x : 69 * scalex
    y : keyTop
    width: whiteWidthQuad
    height: blackHeight
    color: idD.color
    opacity: whiteOpacity
  }


  //E (3)
  Rectangle {
    id: idE
    x : 108 * scalex
    y : keyMid
    width: whiteWidth
    height: whiteHeight
    color: keyE == 0 ? keyFree : (keyE == 1 ? keyGreen : (keyE == 2 ? keyBlue : keyRed) )
    opacity: whiteOpacity

    MouseArea {
      anchors.fill: parent
      onPressed: csComposition.kbdNote( octaveIndex * 12 + 4, 127 )
      onReleased: csComposition.kbdNote( octaveIndex * 12 + 4, 0 )
    }
  }
  Rectangle {
    x : 123 * scalex
    y : keyTop
    width: whiteWidthHalf
    height: blackHeight
    color: idE.color
    opacity: whiteOpacity
  }



  //F (4)
  Rectangle {
    id: idF
    x : 161 * scalex
    y : keyMid
    width: whiteWidth
    height: whiteHeight
    color: keyF == 0 ? keyFree : (keyF == 1 ? keyGreen : (keyF == 2 ? keyBlue : keyRed) )
    opacity: whiteOpacity

    MouseArea {
      anchors.fill: parent
      onPressed: csComposition.kbdNote( octaveIndex * 12 + 5, 127 )
      onReleased: csComposition.kbdNote( octaveIndex * 12 + 5, 0 )
    }
  }
  Rectangle {
    x : 161 * scalex
    y : keyTop
    width: whiteWidthHalf
    height: blackHeight
    color: idF.color
    opacity: whiteOpacity
  }

  //G (5)
  Rectangle {
    id: idG
    x : 210 * scalex
    y : keyMid
    width: whiteWidth
    height: whiteHeight
    color: keyG == 0 ? keyFree : (keyG == 1 ? keyGreen : (keyG == 2 ? keyBlue : keyRed) )
    opacity: whiteOpacity

    MouseArea {
      anchors.fill: parent
      onPressed: csComposition.kbdNote( octaveIndex * 12 + 7, 127 )
      onReleased: csComposition.kbdNote( octaveIndex * 12 + 7, 0 )
    }
  }
  Rectangle {
    x : 228 * scalex
    y : keyTop
    width: whiteWidthQuad
    height: blackHeight
    color: idG.color
    opacity: whiteOpacity
  }

  //A (6)
  Rectangle {
    id: idA
    x : 267 * scalex
    y : keyMid
    width: whiteWidth
    height: whiteHeight
    color: keyA == 0 ? keyFree : (keyA == 1 ? keyGreen : (keyA == 2 ? keyBlue : keyRed) )
    opacity: whiteOpacity

    MouseArea {
      anchors.fill: parent
      onPressed: csComposition.kbdNote( octaveIndex * 12 + 9, 127 )
      onReleased: csComposition.kbdNote( octaveIndex * 12 + 9, 0 )
    }
  }
  Rectangle {
    x : 281 * scalex
    y : keyTop
    width: whiteWidthQuad
    height: blackHeight
    color: idA.color
    opacity: whiteOpacity
  }

  //B (7)
  Rectangle {
    id: idB
    x : 320 * scalex
    y : keyMid
    width: whiteWidth
    height: whiteHeight
    color: keyB == 0 ? keyFree : (keyB == 1 ? keyGreen : (keyB == 2 ? keyBlue : keyRed) )
    opacity: whiteOpacity

    MouseArea {
      anchors.fill: parent
      onPressed: csComposition.kbdNote( octaveIndex * 12 + 11, 127 )
      onReleased: csComposition.kbdNote( octaveIndex * 12 + 11, 0 )
    }
  }
  Rectangle {
    x : 334 * scalex
    y : keyTop
    width: whiteWidthHalf
    height: blackHeight
    color: idB.color
    opacity: whiteOpacity
  }




  //Black keys
  //C# (1|2)
  Rectangle {
    x : 38 * scalex
    y : keyTop
    width: blackWidth
    height: blackHeight
    color: keyCD == 0 ? keyFree : (keyCD == 1 ? keyGreen : (keyCD == 2 ? keyBlue : keyRed) )
    opacity: blackOpacity

    MouseArea {
      anchors.fill: parent
      onPressed: csComposition.kbdNote( octaveIndex * 12 + 1, 127 )
      onReleased: csComposition.kbdNote( octaveIndex * 12 + 1, 0 )
    }
  }
  //D# (2|3)
  Rectangle {
    x : 92 * scalex
    y : keyTop
    width: blackWidth
    height: blackHeight
    color: keyDE == 0 ? keyFree : (keyDE == 1 ? keyGreen : (keyDE == 2 ? keyBlue : keyRed) )
    opacity: blackOpacity

    MouseArea {
      anchors.fill: parent
      onPressed: csComposition.kbdNote( octaveIndex * 12 + 3, 127 )
      onReleased: csComposition.kbdNote( octaveIndex * 12 + 3, 0 )
    }
  }
  //F# (4|5)
  Rectangle {
    x : 197 * scalex
    y : keyTop
    width: blackWidth
    height: blackHeight
    color: keyFG == 0 ? keyFree : (keyFG == 1 ? keyGreen : (keyFG == 2 ? keyBlue : keyRed) )
    opacity: blackOpacity

    MouseArea {
      anchors.fill: parent
      onPressed: csComposition.kbdNote( octaveIndex * 12 + 6, 127 )
      onReleased: csComposition.kbdNote( octaveIndex * 12 + 6, 0 )
    }
  }
  //G# (5|6)
  Rectangle {
    x : 250 * scalex
    y : keyTop
    width: blackWidth
    height: blackHeight
    color: keyGA == 0 ? keyFree : (keyGA == 1 ? keyGreen : (keyGA == 2 ? keyBlue : keyRed) )
    opacity: blackOpacity

    MouseArea {
      anchors.fill: parent
      onPressed: csComposition.kbdNote( octaveIndex * 12 + 8, 127 )
      onReleased: csComposition.kbdNote( octaveIndex * 12 + 8, 0 )
    }
  }
  //A# (6|7)
  Rectangle {
    x : 303 * scalex
    y : keyTop
    width: blackWidth
    height: blackHeight
    color: keyAB == 0 ? keyFree : (keyAB == 1 ? keyGreen : (keyAB == 2 ? keyBlue : keyRed) )
    opacity: blackOpacity

    MouseArea {
      anchors.fill: parent
      onPressed: csComposition.kbdNote( octaveIndex * 12 + 10, 127 )
      onReleased: csComposition.kbdNote( octaveIndex * 12 + 10, 0 )
    }
  }


}

