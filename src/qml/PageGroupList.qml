import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
  //Top bar menu
  header: ToolBar {
      RowLayout {
          anchors.fill: parent
          ToolButton {
            text: qsTr("<")
            onClicked: rootStack.pop()
          }
          Label {
              text: "Play lists"
              elide: Label.ElideRight
              horizontalAlignment: Qt.AlignHCenter
              verticalAlignment: Qt.AlignVCenter
              Layout.fillWidth: true
          }

          ToolButton {
            icon.source: "qrc:/pic/plPlus.png"
            icon.color: "transparent"
            text: qsTr("New list")
            onClicked: groupAppendDialog.open()
          }
      }
  }


  //Popup dialog for new group name enter
  Dialog {
    id: groupAppendDialog
    anchors.centerIn: parent
    title: qsTr("Enter name for new group")
    standardButtons: Dialog.Ok | Dialog.Cancel

    TextField {
      id: tf
    }

    onAccepted: {
      csGroupList.groupAppend( tf.text )
      onClicked: rootStack.pop()
    }
    //onRejected: console.log("Cancel clicked")
  }

  //Popup dialog for group name edit
  Dialog {
    id: groupEditDialog
    anchors.centerIn: parent
    title: qsTr("Enter name for group")
    standardButtons: Dialog.Ok | Dialog.Cancel

    TextField {
      id: idGroupName
      text: csSongList.currentGroupName
    }

    onAccepted: {
      csSongList.currentGroupName = idGroupName.text
      csGroupList.groupUpdate()
    }
    //onRejected: console.log("Cancel clicked")
  }


  Menu {
    id: groupItemMenu
    MenuItem {
      icon.source: "qrc:/pic/plMinus.png"
      icon.color: "transparent"
      text: qsTr("Delete list")
      onClicked: csGroupList.groupRemove(currentGroup)
    }
    MenuItem {
      icon.source: "qrc:/pic/editEnable.png"
      icon.color: "transparent"
      text: qsTr("Edit list name")
      onClicked: groupEditDialog.open()
    }

  }


  Flickable {
    id: idGroupList
    anchors.margins: 5
    anchors.fill: parent
    contentHeight: idGroupLines.height

    ScrollBar.vertical: ScrollBar {
      id: vbar
      Behavior on position {
        NumberAnimation { duration : 200 }
      }
    }

    focus: true

    //Group list consists from group name lines
    Column {
      id: idGroupLines
      width: parent.width
      spacing: 2

      Repeater {
        model: csGroupList

        Rectangle {
          width: idGroupLines.width
          height: 30

          Text {
            anchors.fill: parent
            anchors.rightMargin: 30
            text: groupName
            clip: true

            MouseArea {
              anchors.fill: parent
              onClicked: {
                csSongList.currentGroup = index
                onClicked: rootStack.pop()
              }
            }
          }

          ToolButton {
            width: 24
            height: 24
            text: qsTr("⋮")
            onClicked: {
              csSongList.currentGroup = index
              groupItemMenu.popup()
            }
            anchors.right: parent.right
          }

        }

      }
    }
  }



}
