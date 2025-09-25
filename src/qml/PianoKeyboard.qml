import QtQuick 2.15
import CsBook 1.0

Item {
  id: keyboard
  property real octaveWidth : width / 5
  Repeater {
    model: 5
    PianoOctave {
      octaveIndex: index + 3
      width: octaveWidth
      height: keyboard.height
      x: index * octaveWidth
      y: 0
    }
  }
}
