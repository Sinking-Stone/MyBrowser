/**
 * wfl:2024.06.28
 * 浏览器对话框，目前用于测试
 */

import QtQuick 2.4
import QtQuick.Window 2.2
import QtWebEngine 1.4

Window {
    property alias currentWebView: webView  // 给id为webView的控件起别名为currentWebView
    flags: Qt.Dialog                        // 采用对话框的形式的窗口
    width: 800
    height: 600
    visible: true
    title: qsTr("My Browser")
    // @disable-check M16                   // 这个注释表示不要有警告，不然onClosing:destroy()会有警告
    onClosing:destroy()

    WebEngineView{      // 网页
        id:webView
        anchors.fill: parent                // 将网页放入在父类窗口中
    }
}
