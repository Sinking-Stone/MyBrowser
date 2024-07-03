import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.0

Window {
    id: loginWindow
    title: qsTr("LoginDialog")
    width: 400
    height: 400
    visible: true

    Column{
        anchors.centerIn: parent
        spacing: 10

        TextField{
            id: userNameField
            placeholderText: qsTr("userName:")
        }

        TextField{
            id: passwordField
            placeholderText: qsTr("password:")
            echoMode: TextInput.Password
        }

        Row{
            spacing: 10
            Button{
                id: loginButton
                text: qsTr("login")
                onClicked: {
                    // 需要先判断一下，用户是否存在
                    browserController.loginUser(userNameField.text,passwordField.text)
                    loginWindow.visible=false
                }
            }

        }// Row
    }// Column
}
