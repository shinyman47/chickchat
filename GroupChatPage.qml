import QtQuick 2.12
import Felgo 3.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import client 1.0
//import "../qml"

ListPage{
    id:chatCom

    property string username
    property string userIpAddr
    property string usrMsg
    property string psword
    UdpClient{
        id:client
    }
    anchors.fill: parent

    Connections{
        target: client
              onFileCome: {
                      console.debug("signal success");
                      var com = Qt.createComponent("receiveFile.qml");
                      com.createObject(chatCom,{
                                           "usrName":usrName,
                                           "ipAddr":ipAddr,
                                           "msg":msg
                                       });
    }
    }
    //property int numRepeats: 1
    //readonly property int numLoadedItems: blindTextMsgs.length



    /*function onSignal(friendName,friendIpv4,fileName){
        console.log(fileName+" "+friendName+" "+friendIpv4)
        var com=Qt.createComponent("receiveFile.qml");
        com.createObject(homeCom
                         ,{
                             "frindName":usrName,
                             "frindIpv4":ipAddr,
                             "fileName":msg
                         }
                         );
    }
    Component.onCompleted: {
        onFileCome.connect(onSignal)
    }*/

    listView.backgroundColor: "white"
    listView.anchors.bottomMargin: send_content.height
    listView.header: VisibilityRefreshHandler {
      onRefresh: loadTimer.start()
    }
    Timer {
      id: loadTimer
      interval: 2000
      onTriggered: {
        var pos = listView.getScrollPosition()
        numRepeats++
        //listView.restoreScrollPosition(pos, )
      }
    }

    Column{
        spacing: 4
        anchors.fill:parent
        Rectangle{
            height: 100
            width: parent.width
            AppButton{
                id:sendFile
                anchors.horizontalCenter: parent.horizontalCenter
                text:"F";
                backgroundColor: "red"
                /*background: Rectangle {
                    implicitWidth: 40
                    implicitHeight: 40
                    opacity: enabled ? 1 : 0.3
                    border.color: sendFile.down ? "#17a81a" : "#21be2b"
                    border.width: 1
                    radius: 22
                }*/
               onClicked: {
                   var comp=Qt.createComponent("chooseFile.qml");
                    comp.createObject(chatCom
                                      ,{
                                          "ipAddr":usrMsg,
                                          "usrName":username
                                      }
                                      );
                }
            }
            /*AppButton{

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: "back"
                onClicked: {
                    chatCom.visible=false;
                    chatCom.destroy();
                }
            }
            AppButton{
                 anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                text:"clear"
                onClicked: {
                    console.log("US")
                    //console.log(psword)
                    console.log(client.getUsr())
                }
                Material.background: Material.Red
            }*/
        }
        ListView{
            spacing: 8
            id:charView
            height: 400
            clip: true;
            width: parent.width
            model:1
//                JsonListModel{
//             source: {
//               var model = newMsgs
//               for(var i = 0; i < numRepeats; i++) {
//                 model = blindTextMsgs.concat(model)
//               }
//               return model
//             }
//            }
            Component.onCompleted: {
                positionViewAtEnd();
            }

           delegate: chatDelegate
            Component{
                id:chatDelegate

                Rectangle{
                    //color: "red"
                    width: parent.width-20
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: childrenRect.height
                    /*Text {
                        id: userName
                        text: "Jack:"
                        //text: qsTr("text")
                        color: "black"
                        width: parent.width-40
                        font.pixelSize: 13
                        wrapMode: Text.WrapAnywhere
                    }*/
                    Text {
                        id:textContent
                        color: "black"
                        width: parent.width-40
                        font.pointSize: 13
                        wrapMode: Text.WrapAnywhere
                    }
                    Connections{
                        target: client
                              onSignalMsg: {
                                      if(ipAddr==send_content.text){
                                          //是我发送的文字
                                          textContent.horizontalAlignment=Text.AlignRight;
                                          textContent.anchors.right=parent.right
                                          textContent.text=ipAddr;
                                      }
                                      else{
                                      textContent.text="usrName"+ipAddr;
                                      //userName.text=usrName;
                                      }
                                  console.debug(usrName);
                                  usrMsg=ipAddr;
                                  userIpAddr=msg;
                                  //username=usrName;


                    }
                    }
                }
            }//chatDelegate-end
        }//chatView-end
        Row{
            // input & send
            height: 50
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter
            TextArea{
                id:send_content
                anchors.bottom: btn_send.bottom
                focus: true
                width: 200

                KeyNavigation.priority: KeyNavigation.BeforeItem;
                KeyNavigation.tab: btn_send;
            }
            AppButton{
                id:btn_send
                text:"send"
                Keys.onEnterPressed: {
                    //clicked();
                    newMsgs = newMsgs.concat({me: true, text: send_content.text})
                    send_content.text = ""
                    listView.positionViewAtEnd()
                }
//                Keys.onAccepted: {
//                  newMsgs = newMsgs.concat({me: true, text: inputBox.text})
//                  inputBox.text = ""
//                  listView.positionViewAtEnd()
//                }
                onClicked: {
                    if(send_content.text.trim().length==0){
                        send_content.focus=true;
                        return;
                    }

                    var type = 0
                    console.debug(client.getIP());
                    client.sndMsg(client.Msg,send_content.text,username,userIpAddr);//send
                    send_content.text="";
                    send_content.focus=true;

                }
            }
        }

        Row{
            anchors.horizontalCenter: parent.horizontalCenter

       }
    }



}


