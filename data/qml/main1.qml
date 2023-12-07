import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
//import QtQuick.VirtualKeyboard 2.4
//import QtQuick.VirtualKeyboard.Settings 2.2

Rectangle
{
    id: window
    visible: true
    //visibility: Window.FullScreen
    //x: (Screen.width - width) / 2
    //y: (Screen.height - height) / 2
    //width: 1280
    //height: 800
    //flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    Rectangle
    {
        anchors.fill: parent
        color: "red"
    }
    Component.onCompleted:
    {
       // VirtualKeyboardSettings.wordCandidateList.alwaysVisible = true
      //  VirtualKeyboardSettings.styleName = "retro"
    }
}
/*
Rectangle
{
    id: window
    width: Qt.platform.os === "windows" ? 1280 : Screen.desktopAvailableWidth
    height: Qt.platform.os === "windows" ? 800 : Screen.desktopAvailableHeight

    color: "#161817"

    Component.onCompleted:
    {
        VirtualKeyboardSettings.wordCandidateList.alwaysVisible = true
      //  VirtualKeyboardSettings.styleName = "retro"
    }
}*/
