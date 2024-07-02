
import QtQuick 2.7
import QtQml 2.2
import QtWebEngine 1.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.0

ApplicationWindow {
    id: historyWindow
    width: 400
    height: 500
    visible: true
    title: "My Browser history"

    Component.onCompleted: {    // 加载页面完成时，从数据库中获取书签列表
        dataModel.clear()
        dataModel.append(browserController.fetchHistory())
    }

    RowLayout{
        anchors.fill: parent
        ListView{
            anchors.fill: parent
            model: dataModel
            delegate: Item{
                height: 100
                Row{
                    spacing: 10
                    Text{text: model.title}
                    Text {
                        text: model.url
                        color: "#FF0000"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: load(model.url)
                        }
                    }// Text
                    Text {
                        text: model.createTime
                        color: "#00FF00"
                    }

                    Button{ // 添加remove按钮，调用removeBookMark删除数据库中的书签
                        id: removeBookMarkBtn
                        height: 20
                        width: 70
                        text: "remove"
                        onClicked: {
                            browserController.removeHistory(model.historyID)
                            dataModel.clear()
                            dataModel.append(browserController.fetchHistory()) // 删除掉历史之后及时更新历史内容
                        }
                    }
                }// Row
            }// Item
        }//ListView
        Button{     // 添加reload按钮，查询所有书签
            id: reloadBtn
            visible: true
            height: 20
            width: historyWindow.width
            z:9999
            anchors{
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }

            text: "reload"
            onClicked: {
                dataModel.clear()
                dataModel.append(browserController.fetchHistory())
            }
        }// Button
    }// RowLayout

    ListModel{
        id: dataModel
    }
}
