/**
 * wfl:2024.06.28
 * 主窗口，管理所有布局与组件
 */

import QtQuick 2.7
import QtQml 2.2
import QtWebEngine 1.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.0

ApplicationWindow {
    id: browserWindow
    property Item currentWebView: browserViewLayout.children[browserViewLayout.currentIndex]  // 创建属性，获取当前的页面

    width: 1300
    height: 900
    visible: true
    title: currentWebView&&currentWebView.title!==" "?currentWebView.title:qsTr("My Browser")

    // MAC OS UI
    readonly property bool platformIsMac: Qt.platform==="osx"   //  设置属性platformIsMac判断是否为MacOS
    // 在MacOS的全屏处理操作，主要是做一个或运算
    Component.onCompleted: flags=flags|Qt.WindowFullScreenButtonHint

    Settings{
        id: appSettings
        category: "Browser"
        property alias autoLoadImages: loadImages.checked;
        property alias javascriptEnabled: javaScriptEnabled.checked;
        property alias errorPageEnabled: errorPageEnabled.checked;
        property alias fullScreenSupportEnabled: fullScreenSupportEnabled.checked;
    }

    Shortcut{
        sequence: StandardKey.Close     // 表示关闭页面
        onActivated: currentWebView.triggerWebAction(WebEngineView.RequestClose);
    }
    Shortcut{                           // 表示快捷键
        sequence: StandardKey.ZoomOut   // 表示缩小 Ctrl+-
        onActivated: {
            if(currentWebView.zoomFactror>0.25){
                currentWebView.zoomFactror-=0.1;
            }
        }
    }
    Shortcut{
        sequence: StandardKey.ZoomIn   // 表示放大  Ctrl++
        onActivated: {
            if(currentWebView.zoomFactror<5.0){
                currentWebView.zoomFactror+=0.1;
            }
        }
    }
    Shortcut{
        sequence: StandardKey.Undo      //         Ctrl+z
        onActivated: currentWebView.triggerWebAction(WebEngineView.Undo)
    }
    Shortcut{
        sequence: StandardKey.Redo      // redo    Ctrl+y
        onActivated: currentWebView.triggerWebAction(WebEngineView.Redo)
    }
    Shortcut{
        sequence: StandardKey.Back      // 后退     Alt+<-
        onActivated: currentWebView.triggerWebAction(WebEngineView.Back)
    }
    Shortcut{
        sequence: StandardKey.Forward   // 前进   Alt+->
        onActivated: currentWebView.triggerWebAction(WebEngineView.Forward)
    }

    Shortcut{           // Ctrl+Tab 主要用于切换标签页，从左向右切换标签页
        sequence: "Ctrl+Tab"
        onActivated: tabs.currentIndex=Math.min(tabs.currentIndex+1,tabs.count-1)
    }
    Shortcut{           // Ctrl+Shift+Tab 主要用于切换标签页，从右向左切换标签页
        sequence: "Ctrl+Shift+Tab"
        onActivated: tabs.currentIndex=Math.max(tabs.currentIndex-1,0)
    }

    Shortcut{
        sequence: StandardKey.Copy      // 复制    Ctrl+c
        onActivated: currentWebView.triggerWebAction(WebEngineView.Copy)
    }
    Shortcut{
        sequence: StandardKey.Cut       // 剪切    Ctrl+x
        onActivated: currentWebView.triggerWebAction(WebEngineView.Cut)
    }
    Shortcut{
        sequence: StandardKey.Paste     // 粘贴    Ctrl+v
        onActivated: currentWebView.triggerWebAction(WebEngineView.Paste)
    }
    Shortcut{
        sequence: StandardKey.SelectAll // 全选    Ctrl+a
        onActivated: currentWebView.triggerWebAction(WebEngineView.SelectAll)
    }
    Shortcut{
        sequence: "Ctrl+Alt+1"          // 缩小
        onActivated: currentWebView.triggerWebAction(WebEngineView.ZoomIn)
    }
    Shortcut{
        sequence: "Ctrl+Alt+2"          // 放大
        onActivated: currentWebView.triggerWebAction(WebEngineView.ZoomOut)
    }


    header: ToolBar{         // 创建头部信息
        id: naviationBar
        RowLayout{           // 行方向对齐
            width: parent.width
            height: parent.height

            ToolbarButton{
                id: backButton
                iconSource: "qrc:/icons/previous.png"

                tooltip: qsTr("Back")
                onClicked: currentWebView.goBack()  // 返回到上一个页面中
                enabled: currentWebView && currentWebView.canGoBack
                activeFocusOnTab: !browserWindow.platformIsMac    // 对MacOS的全屏处理
                onPressAndHold: backMenu.open()
                onRightClicked: backMenu.open()     // 鼠标行为，右键按压
                Menu{                               // 返回到指定的页面
                    id: backMenu
                    y:parent.y+parent.height        // 紧贴x轴，之变换y轴

                    Instantiator{                   // 动态数组
                        model: currentWebView&&currentWebView.navigationHistory.backItems
                        delegate: MenuItem{
                            text: model.title       // 显示网页的标题
                            onTriggered: currentWebView.goBackOrForward(model.offset)   // 被触发的事件，将页面切换到数组指定的位置
                            enabled: model.offset
                            checked: !enabled
                            checkable: !enabled
                        }
                        onObjectAdded: backMenu.insertItem(index,object)        // 添加数据。加入到数组中，下标与当前对象
                        onObjectRemoved: backMenu.removeItem(index)             // 移除数组页面
                    }
                }
            }// ToolbarButton

            ToolbarButton{
                id: forwardButton
                iconSource: "qrc:/icons/next.png"
                tooltip: qsTr("Forward")
                onClicked: currentWebView.goForward()
                enabled: currentWebView&&currentWebView.canGoForward
                activeFocusOnTab: !browserWindow.platformIsMac    // 对MacOS的全屏处理
                onPressAndHold: forwardMenu.open()  // 鼠标行为
                onRightClicked: forwardMenu.open()
                Menu{                               // 返回到指定的页面
                    id:forwardMenu
                    y:parent.y+parent.height        // 紧贴x轴，之变换y轴

                    Instantiator{                   // 动态数组
                        model: currentWebView&&currentWebView.navigationHistory.forwardItems
                        delegate: MenuItem{
                            text: model.title
                            onTriggered: currentWebView.goBackOrForward(model.offset)
                            enabled: model.offset
                            checked: !enabled
                            checkable: !enabled
                        }
                        onObjectAdded: forwardMenu.insertItem(index,object)
                        onObjectRemoved: forwardMenu.removeItem(index)
                    }
                }
            }// ToolbarButton

            ToolbarButton{
                id: reloadButton
                readonly property bool loading: currentWebView&&currentWebView.loading // 判断当前是否为loading状态
                iconSource: loading?"qrc:/icons/reload.png":"qrc:/icons/stop.png"
                tooltip: loading?qsTr("Stop"):qsTr("Refresh")
                onClicked: loading?currentWebView.stop():currentWebView.reload()
                activeFocusOnTab: !browserWindow.platformIsMac    // 对MacOS的全屏处理
            }// ToolbarButton
            ToolbarButton{
                id: newPageButton
                iconSource: "qrc:/icons/new.png"
                tooltip: qsTr("NewPage")
                onClicked: createWindow(defaultProfile)
                activeFocusOnTab: !browserWindow.platformIsMac    // 对MacOS的全屏处理
            }// ToolbarButton

            AddressBar{
                id: addressBar
                Layout.fillWidth: true
                currentWebView: browserWindow.currentWebView
            }// AddressBar


            ToolbarButton{  // 用户登录注册
                id: loginRegisterButton
                iconSource: (browserController.userLogin!="test")?(browserController.getUserLogo(browserController.userLogin)):"qrc:/icons/login.png"
                tooltip: qsTr("My Account")
                onClicked: loginRegisterMenu.open()
                activeFocusOnTab: !browserWindow.platformIsMac    // 对MacOS的全屏处理
                Menu{           // 弹出菜单选项
                    id: loginRegisterMenu
                    y:parent.y+parent.height
                    MenuItem{   // 设置是否加载图像
                        id: registerMenu
                        text: qsTr("Register")
                        onTriggered: createRegisterUserWindow()
                    }
                    MenuItem{   // 设置是否加载js脚本
                        id: loginMenu
                        text:  qsTr("Login")
                        onTriggered: createLoginUserWindow()
                    }
                    MenuItem{   // 设置是否支持全屏
                        id: unloginMenu
                        text:  qsTr("Unlogin")
                        onTriggered: browserController.loginUser("test","123456");
                    }
                }
            }// ToolbarButton

            ToolbarButton{
                id: favoriteButton
                iconSource: "qrc:/icons/collect.png"
                tooltip: qsTr("My BookMark")
                onRightClicked: createBookMarkWindow()
                enabled: currentWebView&&currentWebView.title!==""&&currentWebView.url!==""
                onClicked: {
                    browserController.bookMark=currentWebView.title+",--,"+currentWebView.url
                    bookMarkText.text=currentWebView.title+"add to My Bookmark"
                    resetbookMarkText.start()
                }
                activeFocusOnTab: !browserWindow.platformIsMac    // 对MacOS的全屏处理

                Text{   // 当用户添加书签时，展示书签信息在图标界面上
                    objectName: "bookMarkInfo"
                    id: bookMarkText
                    color: "red"
                    width: addressBar.x
                    height: favoriteButton.buttom+200
                    anchors.centerIn: parent
                    elide: Qt.ElideMiddle   // 若展示不全，则中间省略
                }
                Timer{
                    id: resetbookMarkText
                    interval: 750
                    onTriggered: {
                        bookMarkText.text=""
                        resetbookMarkText.stop()
                    }
                }
            }// ToolbarButton

            ToolbarButton{  // 历史按钮
                id: historyPageButton
                iconSource: "qrc:/icons/history.png"
                tooltip: qsTr("History")
                onClicked: createHistoryWindow()
                activeFocusOnTab: !browserWindow.platformIsMac    // 对MacOS的全屏处理
            }// ToolbarButton

            ToolbarButton{  // 返回主页按钮
                id: homePageButton
                iconSource: "qrc:/icons/home.png"
                tooltip: qsTr("HomePage")
                onClicked: currentWebView.url="https://www.baidu.com"
                activeFocusOnTab: !browserWindow.platformIsMac    // 对MacOS的全屏处理
            }// ToolbarButton

            ToolbarButton{  //浏览器设置按钮
                id: settingButton
                iconSource: "qrc:/icons/setting.png"
                tooltip: qsTr("Setting")
                onClicked: settingsMenu.open()
                activeFocusOnTab: !browserWindow.platformIsMac    // 对MacOS的全屏处理
                Menu{           // 弹出菜单选项
                    id:settingsMenu
                    y:parent.y+parent.height
                    MenuItem{   // 设置是否加载图像
                        id: loadImages
                        text: qsTr("Autoload Images")
                        checkable: true
                        checked: WebEngine.settings.autoLoadImages
                    }
                    MenuItem{   // 设置是否加载js脚本
                        id: javaScriptEnabled
                        text: qsTr("JavaScript On")
                        checkable: true
                        checked: WebEngine.settings.javascriptEnabled
                    }
                    MenuItem{   // 设置是否开启错误页面
                        id: errorPageEnabled
                        text: qsTr("ErrorPage On")
                        checkable: true
                        checked: WebEngine.settings.errorPageEnabled
                    }
                    MenuItem{   // 设置是否支持全屏
                        id: fullScreenSupportEnabled
                        text: qsTr("FullScreen Support")
                        checkable: true
                        checked: WebEngine.settings.fullScreenSupportEnabled
                    }
                    MenuItem{   // 设置是否为无痕浏览器
                        id: offTheRecordEnabled
                        text: qsTr("Incognito")
                        checkable: true
                        checked: !browserController.isRecord
                        onCheckedChanged: {
                            checked == !checked
                            if(checked) browserController.isRecord=false    //如果不记录表示无痕
                            else browserController.isRecord=true
                        }
                    }

                    MenuItem{   // 设置是否显示广告
                        id: offPreventAds
                        text: qsTr("Prevent Ads")
                        checkable: true
                        checked: !browserController.isPreventAds
                        onCheckedChanged: {
                            checked == !checked
//                            if(checked) browserController.isPreventAds=true    // 表示过滤广告
//                            else browserController.isPreventAds=false
                        }
                    }

                    ComboBox{   // 语言选择
                        id: languageComboBox
                        model: ["中文","English"]
                        onCurrentIndexChanged: {
                            switch(currentIndex){
                            case 0: browserController.setLanguage(0);break;
                            case 1: browserController.setLanguage(1);break;
                            }
                        }
                    }

                    MenuItem{   // 放大
                        id: zoomInMenu
                        text: qsTr("zoom in")
                        onTriggered: currentWebView.triggerWebAction(WebEngineView.ZoomIn)
                    }
                    MenuItem{   // 缩小
                        id: zoomOutMenu
                        text: qsTr("zoom out")
                        onTriggered: currentWebView.triggerWebAction(WebEngineView.ZoomOut)
                    }
                }
            }// ToolbarButton

            ToolbarButton{
                id: pluginsButton
                iconSource: "qrc:/icons/plugins.png"
                tooltip: qsTr("Plugins")
                onClicked: pluginsMenu.open()
                activeFocusOnTab: !browserWindow.platformIsMac    // 对MacOS的全屏处理
                Menu{           // 弹出菜单选项
                    id: pluginsMenu
                    y:parent.y+parent.height
                    MenuItem{   // 设置是否加载图像
                        id: snapshotMenu
                        text: qsTr("snapshot")
                        onTriggered: browserController.doSnapshot("snapshot")
                    }
                }
            }// ToolbarButton
        }// RowLayout

        // ProgressBar进度条
        ProgressBar{
            id:progressBar
            height: 5
            width: parent.width
            anchors{
                left: parent.left
                top: parent.bottom
                right: parent.right
            }
            from:0
            to:100
            value: (currentWebView&&currentWebView.loadProgress<100)?currentWebView.loadProgress:0
        }
    }// ToolBar Header

    footer: TabBar{ // 页脚
        id:tabs
        currentIndex: 0
        visible: count>1

        function createTab(browserView){        // 定义函数，创建一个browserView的页面
            var component=Qt.createComponent("BrowserTab.qml");             // 创建的对象是哪一个页面
            var tab=component.createObject(tabs,{"webview":browserView});   // 创建一个browserView的页面
            tabs.addItem(tab)
        }
    }// footer: TabBar

    Component{      // 表示组件，这是WebEngineView的一个实例,
        id: browserTabComponent
        BrowserWebView{                 // 这个表示BrowserWebView文件，表示在这里会创建BrowserWebView的实例
            onWindowCloseRequested: {   // 关闭事件，主要是实现标签页的关闭
                if(tabs.count==1){
                    browserWindow.close();
                }else{
                    var tabCount=browserViewLayout.children.length;
                    for(var i=0;i<tabCount;i++){    //
                        var tab=browserViewLayout.children[i];
                        if(tab===this){
                            browserViewLayout.children[i].destroy();
                            tabs.removeItem(i);
                            tabs.currentIndex=tabs.count-1;
                            break;
                        }
                    }
                }
            }
        }
    }

    function createEmptyTab(profile){   // 创建新的标签页
        var browserView=browserTabComponent.createObject(browserViewLayout);    // 表示创建出来的browserView是StackLayout的一部分
                                                        // browserTabComponent是前面定义的组件 所以传来的是父类——browserViewLayout
        if(browserView===null){
            console.error("create tab view error!!!");
            return;
        }
        browserView.profile=profile;
        tabs.createTab(browserView);    // 创建新的标签页
        browserViewLayout.children[browserViewLayout.children.length]=browserView;  // 将当前的这个页面设为最新的页面
        return browserView;
    }


    StackLayout {                       // 表示堆叠式布局，子放在父之上
        id:browserViewLayout
        anchors.fill: parent
        currentIndex: tabs.currentIndex // 实现不同标签页之间的切换

        Component.onCompleted: {        // 表示组件加载完成之后要进行的行为
            createEmptyTab(defaultProfile)
        }
    }

    MouseArea{
        anchors.fill: browserViewLayout
        acceptedButtons: Qt.BackButton|Qt.ForwardButton
        // @disable-check M2
        cursorShape: undefined
        onClicked: {
            if(!currentWebView||currentWebView.url==="")
                return;
            if(mouse.button===Qt.BackButton){
                currentWebView.triggerWebAction(WebEngineView.Back)
            }else if(mouse.button===Qt.ForwardButton){
                currentWebView.triggerWebAction(WebEngineView.Forward)
            }
        }
    }

    Pane{       // 添加鼠标悬停时显示链接
        id:statusBubble

        anchors.left: parent.left
        anchors.bottom: parent.bottom   // 显示在父类的底部
        visible: statusText.text!==""

        Text {
            id: statusText
            anchors.centerIn: parent
            elide: Qt.ElideMiddle       // 信息过多时会显示省略号
            Timer{
                id:resetStatusText
                interval: 750           // 设置文本显示时间
                onTriggered: statusText.text="" // 时间到了就置空（不再显示）
            }
        }
    }
}
