import Felgo 3.0
import QtQuick 2.9
import "../qml"
import client 1.0
import QtQuick.Window 2.3


Page {
    id:menupage
    backgroundColor: "black"
  // make navigation public, so app-demo launcher can track navigation changes and log screens with Google Analytics
  property alias childNavigationStack: globalNavStack
  property alias navigation: navigation
  useSafeArea: false // full screen
  anchors.fill:parent
 // property bool userLoggedIn: false
  property var username
  /*UdpClient{
    id:client
    Component.onCompleted: {
        console.debug(username)
        console.debug("name test")
        client.setName(username)
    }
  }*/
  Component.onCompleted: {
        //console.debug(username)
        setName(username);
        //console.debug(getUsr());
        //console.debug("name test")
        //setName(username);
  }

  NavigationStack {
    id: globalNavStack

    // Wrapper page
    Page {
      navigationBarHidden: Theme.isAndroid
      useSafeArea: false // full screen

      title: navigation.currentNavigationItem ? navigation.currentNavigationItem.title : "" //大标题

      Navigation {
        id: navigation

        navigationMode: navigationModeTabs

        NavigationItem {
          title: qsTr("Recent")

          icon: IconType.clocko

          SignalChatPage {}
        }

        NavigationItem {
          id: groupsItem
          title: qsTr("Groups")
          icon: IconType.group

          GroupChatPage{
          }
        }

        NavigationItem {
          id: peopleItem
          title: qsTr("People")
          icon: IconType.list
          ListPage {
            title: peopleItem.title
            emptyText.text: qsTr("No contacts.")
          }
        }

        NavigationItem {
          id: settingsItem
           title: qsTr("Settings")
          icon: IconType.cog

        ChatSettingPage{
            //title: settingsItem.title
        }
        }

      }
    }
  }
}
