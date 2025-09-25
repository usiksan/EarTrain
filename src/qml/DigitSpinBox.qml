import QtQuick
import QtQuick.Controls

Item {
  id: control
  property int spinBoxHeight: 30
  property int value: 1
  property int minValue: 1
  property int maxValue: 12

  height: spinBoxHeight
  width: spinBoxHeight * 4

  signal editFinish( string txt )
  signal buttonMinus()
  signal buttonPlus()

  Button {
    x: 0
    height: spinBoxHeight
    width: spinBoxHeight
    text: "-"
    font.pixelSize: spinBoxHeight * 0.6
    onClicked: {
      if( control.value > control.minValue )
        control.value = control.value - 1
      else
        control.value = control.minValue
    }
  }

  Rectangle {
    x: spinBoxHeight
    height: spinBoxHeight
    width: spinBoxHeight * 2
    border.width: 1
    border.color: "#bdbebf"
    Text {
      anchors.fill: parent
      text: value
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
    }
  }

  Button {
    x: spinBoxHeight * 3
    height: spinBoxHeight
    width: spinBoxHeight
    text: "+"
    font.pixelSize: spinBoxHeight * 0.6
    onClicked: {
      if( control.value < control.maxValue )
        control.value = control.value + 1
      else
        control.value = control.maxValue
    }
  }
}
