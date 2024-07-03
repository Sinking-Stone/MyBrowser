
# 基于QT的浏览器项目

本项目主要是基于Qt实现跨平台的浏览器项目，主要实现像主流浏览器一样的功能。如Firefox、Edge等。

本项目采用QtWebEngine（其本质为chromium内核）进行浏览器开发，前端采用QML实现页面布局，主要功能如下：

- [x] 浏览器的前进和后退，右键可以查看前进与后退的历史网页
- [x] 浏览器新建标签页与新建窗口，新建标签页在整体网页的页脚
- [x] 刷新浏览器当前页面或者停止加载当前页面数据
- [ ] 地址栏搜索，无需输入全部URL即可实现页面加载，按enter键进行搜索
- [x] 用户登陆、注册、退出
- [x] 存储用户数据采用sqlite3，用户密码采用SHA256加盐加密
- [x] 保存登录用户或匿名用户的浏览历史和收藏网站
- [x] 点击主页返回主界面（百度首页）
- [x] 实现浏览器设置：是否加载图像、是否加载JS脚本、是否为隐私模式浏览等
- [x] 浏览器多语言：中文与英文
- [ ] 浏览器去广告
- [x] 浏览器日志系统


## 配置环境

+ 安装VS2022
+ 安装QT5.14

本项目采用编译器为msvc2017 64bit

> 注意在安装QT Creator时需要勾选QtWebEngine

### 安装环境时遇到的问题

1. cannot run compiler 'cl'

原因是在window10环境中找不到`cl.exe`

> 解决办法：找到cl.exe 添加到环境变量

2. 无法打开包括文件:“type_traits”: No such file or directory

> 在项目中添加环境变量：C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.40.33807\include;C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.40.33807\ATLMFC\include;C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\VS\include;C:\Program Files (x86)\Windows Kits\10\include\10.0.22621.0\ucrt;C:\Program Files (x86)\Windows Kits\10\\include\10.0.22621.0\\um;C:\Program Files (x86)\Windows Kits\10\\include\10.0.22621.0\\shared;C:\Program Files (x86)\Windows Kits\10\\include\10.0.22621.0\\winrt;C:\Program Files (x86)\Windows Kits\10\\include\10.0.22621.0\\cppwinrt

## 项目中遇到的问题

1. 项目开发中关闭新建标签页会报错

> 原因是C++编译环境与QT版本不匹配，重新使用较新的msvc2017版本。另外在索引时，未能及时更新当前索引，页面不知道展示哪一个标签。

2. 在获取历史记录时，无法加载数据库中数据

> 原因：数据向数据库后中，插入出错，无法正常读取数据

+ 第一个问题是因为网页上不一定都会有标题，所在进行获取时会出现空字段，在执行插入时会插入空字符
+ 第二个问题是因为在执行查询代码时，判断错了业务逻辑 ，导致有数据的时候直接跳过了

3. 地址栏不能实现不输入http和https

未解决

4. 去广告功能没有正确的加载

