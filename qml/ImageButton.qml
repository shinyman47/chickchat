import QtQuick 2.0
import QtGraphicalEffects 1.12

Rectangle{
    id: btn
    y: 150
    anchors.horizontalCenter: parent.horizontalCenter
    width: 120; height: 120
    radius: 60

    property var iconNormal //最开始的头像
    property var iconPressed

    signal clicked()

    Image {
        id: icon
        source: "/root/tou1.jpg"
        anchors.fill: parent
//        anchors.margins: 1
//        fillMode: Image.PreserveAspectFit
//        visible: false
    }

    onIconNormalChanged: icon.source = iconNormal


    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("点击切换头像")
        font.pixelSize: 15
        anchors.top: parent.bottom
    }

    MouseArea{
        anchors.fill: parent
        onPressed: {
            if(btn.iconPressed != undefined){
                icon.source = iconPressed
            }else{
            border.width = 1
            border.color = "grey"
            }
        }
        onReleased: {
            if(btn.iconPressed == undefined){
                border.width = 0
                border.color = "grey"
            }

            icon.source = iconNormal
        }

        onClicked: btn.clicked()

    }
    Component.onCompleted: {
        icon.source = iconNormal
    }
}
