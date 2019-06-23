import Felgo 3.0
import QtQuick 2.0

Page {
    id: registerSucceed
    Rectangle{
        id: box
        width: 400
        height: 250
        anchors.centerIn: parent
        radius: 60
        border.color: "black"
        Rectangle{
            radius: 70
            anchors.fill: parent
            gradient: Gradient {
                GradientStop {
                    position: 0.00;
                    color: "#da70d6";
                }
                GradientStop {
                    position: 0.23;
                    color: "#ffc0cb";
                }
                GradientStop {
                    position: 0.66;
                    color: "#ffffff";
                }
                GradientStop {
                    position: 1.00;
                    color: "#ffd700";
                }
            }
            AppText{
                text: "恭喜你，注册成功！"
                anchors.centerIn: parent
                font.family: "汉仪乐喵体"
                font.bold: true
                font.pixelSize: 35
            }
        }

    }
    Timer{
        interval: 3000
        running: true
        onTriggered: {
            var comp= Qt.createComponent("LoginPage.qml")
            comp.createObject(registerSucceed);
        }
    }
}
