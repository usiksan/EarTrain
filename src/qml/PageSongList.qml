import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CsBook 1.0


Page {

  function fileOpenFunction() {
    if( csComposition.isDirty ) {
      programDirty.doFunction = function () {
        rootStack.push(pageScore)
        idFileOpen.open();
      }
      programDirty.open()
    }
    else {
      rootStack.push(pageScore)
      idFileOpen.open()
    }
  }


  header: ToolBar {
      RowLayout {
          anchors.fill: parent
          ToolButton {
            text: csSongList.name
            onClicked: rootStack.push(pageGroupList)
            Layout.fillWidth: true
          }

          ToolButton {
            icon.source: "qrc:/pic/fileOpen.png"
            icon.color: "transparent"
            text: qsTr("Open score")
            onClicked: fileOpenFunction()
          }

          ToolButton {
            icon.source: "qrc:/pic/fileImport.png"
            icon.color: "transparent"
            text: qsTr("Clipboard import")
            onClicked: fileImportClipboardFunction()
          }
          ToolButton {
            icon.source: "qrc:/pic/editSettings.png"
            icon.color: "transparent"
            text: qsTr("Settings")
            onClicked: rootStack.push(pageViewSettings)
          }

      }
  }



  Flickable {
    id: idGroupList
    anchors.margins: 5
    anchors.fill: parent
    contentHeight: idSongLines.height

    ScrollBar.vertical: ScrollBar {
      id: vbar
      Behavior on position {
        NumberAnimation { duration : 200 }
      }
    }

    focus: true

    //Group list consists from group name lines
    Column {
      id: idSongLines
      width: parent.width
      spacing: 2

      Repeater {
        model: csSongList

        Rectangle {
          width: idSongLines.width
          height: 30

          Text {
            anchors.fill: parent
            anchors.rightMargin: 30
            text: scoreName
            clip: true
            font.pixelSize: 16

            MouseArea {
              anchors.fill: parent
              onClicked: {
                rootStack.push(pageScore)
                csComposition.fileOpen( csSongList.songPath(index) );
              }
            }
          }

          ToolButton {
            visible: csSongList.currentGroup != 0
            width: 24
            height: 24
            icon.source: "qrc:/pic/delete_red.png"
            icon.color: "transparent"
            ToolTip.visible: hovered
            ToolTip.text: qsTr("Remove song from list")
            ToolTip.delay: 300
            ToolTip.timeout: 5000
            onClicked: csSongList.songRemove(index)
            anchors.right: parent.right
          }

        }



      }
    }
  }
}

/*
Bm
Build a fire a thousand miles away
   A       F#m      Bm
to light my long way home
 Bm
I ride a comet my trail is long to stay,
 A                  Bm
silence is a heavy stone

D              E        Bm
I fight the world and take all they can give
          D         A          Bm
There are times my heart hangs low
D              E              Bm
Born to walk against the wind, born to hear my name
   D               A         Bm  C#m
No matter where I stand I'm alone

F#m           D     E           A
Stand and fight live by your heart
Bm                 D       A           E (ag#)
Always one more try, I'm not afraid to die
F#m           D     E          A
Stand and fight say what you feel
Bm           D         A  E
Born with a heart of steel

Bm
Burn the bridge behind you, leave no retreat
   A       F#m      Bm
There's only one way home
 Bm
And those who laugh and crowd the path and cut each others throat
 A                  Bm
Will fall like melting snow

D              E        Bm
They'll watch us rise with fire in our eyes
          D        A  Bm
They'll bow their heads their hearts will hang low
D              E        Bm
And we'll laugh and they will kneel and know this heart of steel
   D               A    Bm  C#m
Was too hard to break too hard to hold

F#m           D     E           A
Stand and fight live by your heart
Bm                 D       A           E (ag#)
Always one more try, I'm not afraid to die
F#m           D     E          A
Stand and fight say what you feel
Bm           D         A  E
Born with a heart of steel

Bm            D        C#m
Born with a heart of steel
D            E        F#m
Born with a heart of steel
*/
