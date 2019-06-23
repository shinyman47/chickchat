import Felgo 3.0
import QtQuick 2.0
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import client 1.0

Page {

    id: registerPage
    UdpClient{id:client}
    signal registerSucceeded()

    title: qsTr("REGISTER")
    Rectangle{
        border.color: "#000d0b0b"
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.00; color: "gold";}
            //GradientStop { position: 0.57; color: "#d3d3d3";}
            GradientStop { position: 0.92; color: "white";}
        }
    }
    anchors.fill: parent
    Column{
        spacing: 30
        anchors.centerIn: parent
        Rectangle{
            y: 150
            anchors.horizontalCenter: parent.horizontalCenter
            width: 120; height: 120
            radius: 60
            Image {
                id: image
                source: "../assets/littleChick"
                anchors.fill: parent
                visible: false
            }
            Rectangle{
                id: mask
                anchors.fill: parent
                radius: width/2
                visible: false
            }
            OpacityMask{
                anchors.fill: image
                source: image
                maskSource: mask
                visible: true
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("点击切换头像")
                anchors.top: parent.bottom
            }
        }

            Row{
                AppText{

                    text: "用 户 名 ："
                    font.bold: true
                    font.pixelSize: 25
                }
                AppTextField{
                    id:username
                    focus: true;
                    width: 250;
                }
            }
            Row{
                AppText{

                    text: "密       码 ："
                    font.bold: true
                    font.pixelSize: 25
                }
                AppTextField{
                    id:password
                    focus: true;
                    width: 250;
                    echoMode: TextInput.Password
                }
            }
            Row{
                AppText{
                    text: "<b>确认密码：<b>"
                    font.pixelSize: 25
                }
                AppTextField{
                    focus: true;
                    width: 250;
                    echoMode: TextInput.Password
                }
            }

            Row{
                x: 40; y: 500
                spacing: 60
                anchors.left: parent.left
                AppButton{
                    text: "确认"
                    onClicked:{
                        client.setInfo(username.text,password.text);
                        if(username.text==""){
                            console.log("empty input")
                            return
                        }
                        var comp = Qt.createComponent("RegisterSucceed.qml")
                        comp.createObject(registerPage)

                    }
                }
                AppButton{
                    text: "返回"
                    onClicked:{
                        var comp = Qt.createComponent("LoginPage.qml")
                        comp.createObject(registerPage)
                    }
                }
            }
    }
}
