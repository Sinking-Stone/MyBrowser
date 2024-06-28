import QtQuick 2.0
import QtQuick.Controls 2.0


TextField {         // 地址文本框
    id: addressBar
    selectByMouse: true                         // 能让鼠标选中
    activeFocusOnPress: true                    // 点击的时候聚焦
    activeFocusOnTab: true                      // 捕获按下tab键
    persistentSelection: true                   // 地址是可选中的状态
    placeholderText: qsTr("Type URL here...")

    property Item currentWebView                // 创建属性，为了交互

    Image {
        id: faviconImage
        anchors.verticalCenter: parent.verticalCenter   // 位置居中
        anchors.verticalCenterOffset: (parent.height-parent.contentHeight)/-5

        width: height
        height: addressBar.contentHeight
        sourceSize: Qt.size(width,height)
        source: currentWebView&&currentWebView.icon
        visible: source!==""
    }
    leftPadding: faviconImage.source!==""?faviconImage.width*1.5:0  // 图标与左边框的相对位置
    focus: true
    text: currentWebView&&currentWebView.url
    onAccepted: currentWebView.url=text
}
