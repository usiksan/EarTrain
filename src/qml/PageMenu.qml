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
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCore
import CsBook 1.0


Page {
  id: idMenuPage
  property real scale : 1.0

  Grid {
    columns: 2
    spacing: 10
    Button {
      width: 600
      height: 150
      text: qsTr("Сравнение интервалов")
      onClicked: rootStack.push( pageIntervalCompare )
    }

  }


}
