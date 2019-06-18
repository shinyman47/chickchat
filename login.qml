import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import client 1.0

Item {
    id:loginCom;
    anchors.fill: parent
    signal login(var name);
    UdpClient{
        id:client
    }
    Column{
        anchors.centerIn: parent

        Row{
            spacing: 8
            Label{
                text: "USERNAME"
                topPadding: 10
            }

            TextField{
                focus: true;
                id:username;
                width: 200;
            }
        }

        Button{
            id:btnLogin
            anchors.right: parent.right
            text:"login"
            onClicked:{
                if(username.text==""){
                    console.log("empty input")
                    return
                }
                loginCom.visible=false;
                // when login success
                client.test();
                client.setName(username.text);
                //client.socketSetting();
                //UsrEnter();
                //sendMsg(UsrEnter);
                //@ emit login signal,to delete this Component
                loginCom.login(username.text);
            }
            Material.background: Material.Teal
        }
    }
}
