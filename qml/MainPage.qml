import Felgo 3.0
import QtQuick 2.12
//import "../qml"
//import client 1.0
//import QtQuick.Controls 2.5
//import QtQuick.Window 2.12

Page{
    title: qsTr("QTC-Chat")
    id:root
    visible: true
    width: 380
    height: 600
//LoginPage{
//    z:1
//    visible: true
//    enabled: visible
//}

    property var currentPage: null
    property bool hasLogin: false

    Component.onCompleted: {
        //@ set start position
    //             setX(Screen.width / 2 - width / 2);
    //             setY(Screen.height / 2 - height / 2);
        var login=Qt.createComponent("LoginPage.qml");
          currentPage=login.createObject(root);
          currentPage.login.connect(onLogin);
    }

    function onLogin(name,psword){
        hasLogin=true;
        currentPage.destroy();
        var home=Qt.createComponent("MenuPage.qml");
        currentPage=home.createObject(root,{
                                          "username":name},
                                      {"password":psword});
    }
    }
      // }
//}

         //signal LoginPage

////            Image {
////                source: "../assets/felgo-logo.png"
////                anchors.centerIn: parent
////            }
//     }

// }
//}
