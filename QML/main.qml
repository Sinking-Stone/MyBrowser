/**
 * wfl:2024.06.28
 * qml运行主页，用于创建浏览器窗口
 */

import QtQuick 2.9
import QtQuick.Window 2.2
import QtWebEngine 1.8
import QtQuick.Controls 2.0

QtObject{
    id:root

    property  QtObject defaultProfile: WebEngineProfile{        // 用来管理浏览器的信息，如cookie、js等
        storageName: "default"
    }

    property  QtObject otrProfile: WebEngineProfile{            // 设置为无痕的浏览器
        storageName: "incognito"
        offTheRecord: true                                      // 表示不做任何的记录
    }

    property  Component browserDialogComponent: BrowserDialog{  // 用browserDialogComponent来表示BrowserDialog文件
        // @disable-check M16                                   // 这个注释表示不要有警告，不然onClosing:destroy()会有警告
        onClosing:destroy()
    }

    property  Component browserWindowComponent: BrowserWindow{  // 用browserWindowComponent来表示BrowserWindow文件
        onClosing:destroy()
    }

    property  Component browserBookMarkComponent: BookMarkWindow{   // 用browserBookMarkComponent来表示BookMarkWindow文件
        onClosing:destroy()
    }

    property  Component registerWindowComponent: RegisterWindow{  // 用registerWindowComponent来表示RegisterWindow文件
        // @disable-check M16
        onClosing:destroy()
    }

    property  Component loginWindowComponent: LoginWindow{   // 用loginWindowComponent来表示LoginWindow文件
        // @disable-check M16
        onClosing:destroy()
    }

    function createDialog(profile){
        var newDialog=browserDialogComponent.createObject(root) // 用browserDialogComponent创建一个对象
        newDialog.currentWebView.profile=profile                // newDialog中的webView窗口
        return newDialog
    }

    function createWindow(profile){
        var newWindow=browserWindowComponent.createObject(root) // 用browserWindowComponent创建一个对象
        newWindow.currentWebView.profile=profile                // newWindow中的webView窗口
        return newWindow
    }

    function createBookMarkWindow(profile){
        var newWindow=browserBookMarkComponent.createObject(root) // 用browserWindowComponent创建一个对象
        return newWindow
    }

    function createRegisterUserWindow(profile){
        var newWindow=registerWindowComponent.createObject(root) // 用registerWindowComponent创建一个对象
        newWindow.currentWebView.profile=profile                // newWindow中的webView窗口
        return newWindow
    }

    function createLoginUserWindow(profile){
        var newWindow=loginWindowComponent.createObject(root) // 用loginWindowComponent创建一个对象
        return newWindow
    }

    function load(url){
        // var browserWindow=createDialog(defaultProfile)       // 调用上面的方法生成浏览器对象
        var browserWindow=createWindow(defaultProfile)          // 调用上面的createWindow方法生成浏览器对象
        browserWindow.currentWebView.url=url
    }
}
