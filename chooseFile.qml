import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.2
import QtQuick.Dialogs 1.2
import client 1.0
import Felgo 3.0
Item {
    id:chooseFile
    width: parent.width*2/3;
    height: parent.width*2/3-20;
    anchors.centerIn: parent;
    property string ipAddr: ""
    property string usrName: ""
    UdpClient{id:client}
    Rectangle{
        anchors.fill: parent;
        color: "#D3D3D4"
        ProgressBar{
            id:bar
            width: parent.width;
            value: 0
        }

        Component.onCompleted: {
            onUpdateProgressBar.connect(onUpdate);
            function onUpdate(value){
                bar.value=value;
            }
        }


        Column{
            anchors.centerIn: parent
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                id: txt_status
                text:"CHOOSE"
                font.pointSize: 16
                height: 40
            }
            Row{
                TextField{
                    focus: true;
                    id:inputIdAddr;
                    width: 200;
                    placeholderText: "IpAddress"
                }
            }
            Row{
                //filename & choose btn
                Label{
                    id:fileFullName;
                    text: "choose your file..."
                    width: 150
                    height: 40
                    verticalAlignment:Text.AlignVCenter
                    horizontalAlignment:Text.AlignHCenter
                    clip: true;
                    elide: Text.ElideMiddle
                }

                Button{
                    text: "choose";

                    onClicked: {
                        ipAddr = inputIdAddr.text;
                        fileDialog.open();
                    }
                }

            }
            Row{
                spacing: 4
                anchors.horizontalCenter: parent.horizontalCenter
                // send & close btn
                Button{
                    id:btn_send;
                    text:"send";
                    onClicked: {

                        txt_status.text="WAITING...";
                        client.sndMsg(3,fileFullName.text,usrName,ipAddr)

//                    online:    sendMsg(3,frindIpv4,frindName,fileFullName.text)
                    }
                    Material.background: Material.LightBlue
                }
                Button{
                    text:"close";
                    onClicked: chooseFile.destroy();
                    Material.background: Material.Red
                }
            }
        }
        FileDialog{
            //does not visible
            id:fileDialog
           title: "choose a file to send";
            folder: shortcuts.home
            onAccepted: {
                bar.value=0;
                btn_send.enabled=true;

                var path=fileDialog.fileUrl;
                fileFullName.text=path.toString().slice(7);
                setFileName(fileFullName.text);
            }
        }
    }

    function onSignal(status){
        txt_status.text=status;
        if(status==="Success"){
            btn_send.enabled=false ;
        }
    }

    Component.onCompleted: {
        onFileStatus.connect(onSignal);
        //当弹出对话框时，初始化TCP
        initalizeTcp();
    }

}
