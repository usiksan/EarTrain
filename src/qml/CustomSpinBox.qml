/*
   Проект "Программа регистратора"
   Автор
     Сибилев А.С.

     www.salilab.ru
     www.salilab.com
   Описание
     Спин-бокс - альтернатива стандартному
*/
import QtQuick
import QtQuick.Controls

Item {
  id: control
  property int spinBoxHeight: 30
  property string minusText : "<"
  property string plusText : ">"
  property int value: 50
  property string tipButtonMinus : "-"
  property string tipButtonPlus : "+"
  property string tipTextInput : ""
  height: spinBoxHeight
  width: spinBoxHeight * 4

  signal editFinish( string txt )
  signal buttonMinus()
  signal buttonPlus()

  Button {
    x: 0
    height: spinBoxHeight
    width: spinBoxHeight
    text: minusText
    font.pixelSize: spinBoxHeight * 0.6
    onClicked: control.buttonMinus()
  }

  Rectangle {
    x: spinBoxHeight
    height: spinBoxHeight
    width: spinBoxHeight * 2
    border.width: 1
    border.color: "#bdbebf"
    TextInput {
      anchors.fill: parent
      text: value
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
      inputMethodHints: Qt.ImhFormattedNumbersOnly
      onEditingFinished: control.editFinish(text)
    }
  }

  Button {
    x: spinBoxHeight * 3
    height: spinBoxHeight
    width: spinBoxHeight
    text: plusText
    font.pixelSize: spinBoxHeight * 0.6
    onClicked: control.buttonPlus()
  }
}
