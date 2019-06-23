import Felgo 3.0
import QtQuick 2.0
import client 1.0
Page {
    id: errorpage
    Rectangle{
        id: box
        width: 400
        height: 250
        color: "grey"
        anchors.centerIn: parent
        radius: 20
        Rectangle{
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.34; color: "gold";}
                //GradientStop { position: 0.68; color: "#ffc0cb";}
                GradientStop { position: 0.94; color: "#ffffff";}
            }
        }
        Column{

            x: 28
            y: 59
            width: 265
            height: 99
            spacing: 15
            Row{
                spacing: 15

                Image {
                    id: error
                    source: "../assets/littleChick"
                    width: 50; height: 50
                }
                AppText{
                    fontSize: 14
                    text: "你输入的数据不符合要求，\n请重新输入!"
                    font.family: "汉仪乐喵体"
                }
            }
            Row{
                anchors.centerIn: box
                spacing: 80
                AppButton{
                    text: "重试"
                    minimumWidth: 100
                    minimumHeight: 50
                    radius: 20
                    onClicked: { errorpage.visible=false }
                }
                AppButton{
                    text: "取消"
                    minimumWidth: 100
                    minimumHeight: 50
                    radius: 20
                    onClicked: {errorpage.visible=false}
                }
            }
        }
    }
}
