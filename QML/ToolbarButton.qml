/**
 * wfl:2024.06.28
 * 浏览器上部各个按钮控件
 */

import QtQuick 2.0
import QtQuick.Controls 2.0


ToolButton{         // 工具按钮，内部有定义
    id: control
    hoverEnabled: true  // 是否可覆盖
    padding: width*.25

    property alias iconSource: icon.source      // 全局变量，在外部使用
    property string tooltip                     // 新增属性

    signal rightClicked(int modifiers)          // 右击事件

    contentItem: Image {
        id: icon
        fillMode: Image.PreserveAspectCrop      // 均匀缩放，必要时裁剪
        width: control.availableWidth           // 与父类宽度相同
        height: control.availableHeight         // 与父类高度相同
        sourceSize: Qt.size(width,height)
        horizontalAlignment: Image.AlignHCenter // 水平对齐
        verticalAlignment: Image.AlignVCenter
        opacity: control.enabled?1.0:0.5        // 透明度

        ToolTip.visible: control.hovered && control.tooltip!==""
        ToolTip.timeout: 750                    // 时间单位ms
        ToolTip.text: control.tooltip
    }
    MouseArea{                                  // 鼠标的范围，处理的事件
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            if(mouse.button==Qt.RightButton){   // 捕获右键行为
                control.rightClicked(mouse.modifiers)
            }
        }
    }
}
