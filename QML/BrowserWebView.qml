import QtQuick 2.7
import QtWebEngine 1.10
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.0


WebEngineView{
    id: webEngineView
    focus: true

    settings.autoLoadImages: appSettings.autoLoadImages;
    settings.javascriptEnabled: appSettings.javaScriptEnabled;
    settings.errorPageEnabled: appSettings.errorPageEnabled;
    settings.fullScreenSupportEnabled: appSettings.fullScreenSupportedEnabled;

    WebEngineProfile {      // 浏览器设置所必须的
        id: webProfile
    }

    onLinkHovered: {        // 鼠标悬停的信号
        if(hoveredUrl==""){
            resetStatusText.start()
        }else{
            resetStatusText.stop()
            statusText.text=hoveredUrl
        }
    }

    onUrlChanged: {         // url改变的信号

    }

    onCertificateError: {   // 证书错误，验证网站是否合法

    }

    onNewViewRequested: {   // 创建新标签请求信号
        if(!request.userInitiated){     // 判断当前的用户行为是什么
            console("Warning: request.userInitiated!!!")
        }else if(request.destination == WebEngineView.NewViewInTab){   // 请求创建新的标签页
            var tab=createEmptyTab(currentWebView.profile)
            tabs.currentIndex=tabs.count-1
            request.openIn(tab)     // 打开创建的标签
        }else if(request.destination==WebEngineView.NewViewInBackgroundTab){    // 请求打开新的窗口
            var tab=createEmptyTab(currentWebView.profile)
            request.openIn(tab)
        }/*else if(request.destination===WebEngineView.NewViewInDialog){
            var dialog=applicationRoot.createDialog(currentWebView.profile)
            request.openIn(dialog.currentWebView)
        }*/else{
            var window=root.createWindow(currentWebView.profile)
            request.openIn(window.currentWebView)
        }

    }

    onFullScreenRequested: {    // 全屏事件

    }

    onFeaturePermissionRequested: { // 获取特定功能请求事件，如获取地理位置

    }

    onRenderProcessTerminated: {    // 渲染出现异常事件

    }
}
