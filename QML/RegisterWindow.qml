import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.0

Window{
    id: registerWindow
    title: qsTr("RegisterDialog")
    width: 400
    height: 400
    visible: true

    Column{
        anchors.centerIn: parent
        spacing: 10

        TextField{
            id: userNameField
            placeholderText: qsTr("UserName:")    // 文本提示信息
        }

        TextField{
            id: passwordField
            placeholderText: qsTr("Password:")
            echoMode: TextInput.Password    // 采用密文的方式展示
        }

        TextField{
            id: emailField
            placeholderText: qsTr("email:")
        }

        Row{
            spacing: 10
            Button{
                id: chooseAccountLogoButton
                text: qsTr("choose your account logo:")
                onClicked: fileDialog.open()
            }

            TextField{
                id: logoPathField
                placeholderText: qsTr("logo Image Path")
                readOnly: true
            }
        }// Row
        FileDialog{     // 打开指定类型的文件
            id: fileDialog
            title: qsTr("Please choose your account logo")
            nameFilters: ["Image files(*.png *.jpg *.jpeg)"]    // 设定图像类型
            onAccepted: {                                       // 接受事件
                logoPathField.text=fileDialog.fileUrl
            }
        }// FileDialog

        Button{
            id: registerButton
            text:  qsTr("Register your new account")
            onClicked: {
                browserController.registerUser(userNameField.text,passwordField.text,emailField.text,logoPathField.text)
                registerWindow.visible=false
            }
        }// register Button
    }
}
