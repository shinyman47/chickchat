import Felgo 3.0
import QtQuick 2.12
//import client 1.0
import QtQuick.Window 2.12
import "../qml"
//import QtQuick.Controls.Material 2.3
//import QtQuick.Controls 2.5

App {
    id:app
    onInitTheme: {
        Theme.colors.tintColor = "black"
      }
   MainPage{}
}
    // You get free licenseKeys from https://felgo.com/licenseKey
    // With a licenseKey you can:
    //  * Publish your games & apps for the app stores
    //  * Remove the Felgo Splash Screen or set a custom one (available with the Pro Licenses)
    //  * Add plugins to monetize, analyze & improve your apps (available with the Pro Licenses)
    //licenseKey: "<generate one from https://felgo.com/licenseKey>"
//   NavigationStack {

//        Page {
//            title: qsTr("QTC-Chat")
//            id:root
//            visible: true
//            width: 380
//            height: 600
//    Window{
////            title: qsTr("ChickenChat")
//            property var currentPage: null
//            property bool hasLogin: false

//            Component.onCompleted: {
//                //@ set start position
//                setX(Screen.width / 2 - width / 2);
//                setY(Screen.height / 2 - height / 2);

//                var login=Qt.createComponent("login.qml");
//                currentPage=login.createObject(root);
//                //currentPage.connect(onLogin());
//               currentPage.login.connect(onLogin);
//                //onClicked{onLogin();}
//            }

//            function onLogin(name,password){
//                hasLogin=true;
//                currentPage.destroy()
//                //@ show home page
//                var home=Qt.createComponent("chat.qml");
//                currentPage=home.createObject(root,{
//                                                  "username":name

//                                              },{"password":password});

//            }
////            Image {
////                source: "../assets/felgo-logo.png"
////                anchors.centerIn: parent
////            }
//        }

//    }
//}
//}
