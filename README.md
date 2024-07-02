
# 基于QT的浏览器项目

本项目主要是基于Qt实现跨平台的浏览器项目，主要实现像主流浏览器一样的功能。

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

