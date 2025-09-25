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
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCore
import CsBook 1.0


Page {
  id: idIntervalComparePage

  header: ToolBar {
    RowLayout {
      anchors.fill: parent
      ToolButton {
        visible: rootStack.depth > 1
        text: qsTr("< Назад")
        onClicked: rootStack.pop()
      }

      Label {
        text: qsTr("Сравнение интервалов")
        elide: Label.ElideRight
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        Layout.fillWidth: true
      }
    }
  }

  property bool correct : true

  Grid {
    columns: 2
    spacing: 20

    Text {
      text: qsTr("минимальный интервал")
    }
    DigitSpinBox {
      id: lowLimit
      maxValue: upLimit.value - 1
      onValueChanged: csPlayer.minInterval = value
    }

    Text {
      text: qsTr("минимальный интервал")
    }
    DigitSpinBox {
      id: upLimit
      value: 2
      minValue: lowLimit.value + 1
      onValueChanged: csPlayer.maxInterval = value
    }

    Text {
      text: qsTr("одинаковое начало")
    }
    CheckBox {
      checkable: true
      checked: true
      onCheckedChanged: csPlayer.singleFirst = checked
    }

    Text {
      text: " "
    }
    Text {
      text: " "
    }

    Button {
      width: 600
      height: 150
      text: qsTr("Повторить")
      onClicked: csPlayer.repeate()
    }

    Button {
      width: 600
      height: 150
      text: qsTr("Далее")
      onClicked: csPlayer.next()
    }

    Text {
      text: " "
    }
    Text {
      text: " "
    }

    Text {
      text: idIntervalComparePage.correct ? qsTr("Верно") : qsTr("Ошибка")
      color: idIntervalComparePage.correct ? "green" : "red"
      font.pixelSize: 48
    }
    Text {
      text: " "
    }

    Text {
      text: " "
    }
    Text {
      text: " "
    }

    Text {
      text: qsTr("Какой больше:")
    }
    Text {
      text: " "
    }

    Button {
      width: 600
      height: 150
      text: qsTr("Первый")
      onClicked: {
        idIntervalComparePage.correct = csPlayer.result === 1
        csPlayer.next()
      }
    }

    Button {
      width: 600
      height: 150
      text: qsTr("Второй")
      onClicked: {
        idIntervalComparePage.correct = csPlayer.result === 2
        csPlayer.next()
      }
    }
  }
}
