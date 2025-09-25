import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
  ToolBar {
      RowLayout {
          anchors.fill: parent
          ToolButton {
              text: qsTr("‹")
              onClicked: stack.pop()
          }
          Label {
              text: "Title"
              elide: Label.ElideRight
              horizontalAlignment: Qt.AlignHCenter
              verticalAlignment: Qt.AlignVCenter
              Layout.fillWidth: true
          }
          ToolButton {
              text: qsTr("⋮")
              onClicked: menu.open()
          }
      }
  }

}
