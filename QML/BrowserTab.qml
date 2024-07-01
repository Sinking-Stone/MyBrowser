/**
 * wfl:2024.06.28
 * 标签页按钮的各个组件
 */

import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtWebEngine 1.4

TabButton {
    id:control

    property var webview

    contentItem: RowLayout{
        Image {
            id: faviconImage
            source: webview&&webview.icon
            visible: source !== "" && !busyIndicator.running
        }
        BusyIndicator{      // 创建一个BusyIndicator加载的组件，表示页面是否正在加载
            id:busyIndicator
            visible: running
            running: webview && webview.loading
        }
        Text{               // 展示网页的信息在tab页面上
            id: pageTitle
            Layout.fillWidth: true
            text:webview && webview.title ? webview.title : qsTr("Untitled")
            font: control.font
            opacity: enabled ? 1.0 : 0.3            // 透明度
            horizontalAlignment: Text.AlignHCenter  // 展示的方式
            elide: Text.ElideMiddle                 // 若是展示不下，则省略中间数据
        }
        ToolButton{ // 用来关闭网页
            text: "✖"
            onClicked: webview.triggerWebAction(WebEngineView.RequestClose) // 设定事件
            font.pixelSize: 30
        }
    }
}
