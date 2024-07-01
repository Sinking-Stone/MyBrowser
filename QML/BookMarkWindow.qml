
import QtQuick 2.7
import QtQml 2.2
import QtWebEngine 1.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.0

ApplicationWindow {
    id: bookMarkWindow
    width: 400
    height: 500
    visible: true
    title: browserController.userLogin+" in Browser BookMarks"

    Component.onCompleted: {    // 加载页面完成时，从数据库中获取书签列表
        dataModel.clear()
        dataModel.append(browserController.fetchBookMark())
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
                        color: "#0000FF"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: load(model.url)
                        }
                    }// Text

                    Button{ // 添加remove按钮，调用removeBookMark删除数据库中的书签
                        id: removeBookMarkBtn
                        height: 20
                        width: 70
                        text: "remove"
                        onClicked: {
                            browserController.removeBookMark(browserController.getUserId(browserController.userLogin),model.url)
                            dataModel.clear()
                            dataModel.append(browserController.fetchBookMark()) // 删除掉书签之后及时更新书签内容
                        }
                    }
                }// Row
            }// Item
        }//ListView
        Button{     // 添加reload按钮，查询所有书签
            id: reloadBtn
            visible: true
            height: 20
            width: 70
            text: "reload"
            onClicked: {
                dataModel.clear()
                dataModel.append(browserController.fetchBookMark())
            }
        }// Button
    }// RowLayout

    ListModel{
        id: dataModel
    }
}
