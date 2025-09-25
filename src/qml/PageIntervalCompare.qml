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
      text: qsTr("минимальная нота")
    }
    DigitSpinBox {
      id: minNote
      value: 36
      maxValue: maxNote.value - 1
      onValueChanged: csPlayer.minNote = value
    }

    Text {
      text: qsTr("максимальная нота")
    }
    DigitSpinBox {
      id: maxNote
      value: 96
      minValue: minNote.value + 1
      onValueChanged: csPlayer.maxNote = value
    }

    Button {
      text: qsTr("36-96")
      onClicked: {
        minNote.value = 36
        maxNote.value = 96
      }
    }

    Button {
      text: qsTr("60-72")
      onClicked: {
        minNote.value = 60
        maxNote.value = 72
      }
    }


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
      onClicked: csPlayer.next(true)
    }

    Text {
      text: " "
    }
    Text {
      text: qsTr("Неповторенных ошибок:") + csPlayer.errors
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
      text: csPlayer.statistic
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
        csPlayer.next( idIntervalComparePage.correct )
      }
    }

    Button {
      width: 600
      height: 150
      text: qsTr("Второй")
      onClicked: {
        idIntervalComparePage.correct = csPlayer.result === 2
        csPlayer.next( idIntervalComparePage.correct )
      }
    }
  }
}
