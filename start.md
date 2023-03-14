<!--
 * @Author: luxun59 luxun59@126.com
 * @Date: 2023-03-13 10:20:22
 * @LastEditors: luxun59 luxun59@126.com
 * @LastEditTime: 2023-03-13 15:30:57
 * @FilePath: \c2000\start.md
 * @Description: 
 * 
 * Copyright (c) 2023 by luxun59@126.com, All Rights Reserved. 
-->

# CCS安装教程
![picture](/picture/ccs.png)
## 下载

下载地址：[下载地址](https://www.ti.com.cn/tool/cn/CCSTUDIO)

![picture](/picture/ccs-download.png)
选择第二个``CCSTUDIO``。第一个为在线开发环境，支持处理器较少一般mcu并不支持。
![picture](/picture/ccs-download2.png)
根据平台选择安装包，这里选择的是离线方式windows平台。


## 安装

[TI官方安装教程](https://software-dl.ti.com/ccs/esd/documents/users_guide/ccs_installation.html)。

- 必须将ccs安装包放于非中文路径下。
- ccs的安装位置必须为非中文。

### 1.解压软件压缩包
（本文为CCS12.2.0.00009_win64.zip）到非中文路径下（桌面也行，如果路径包含中文会导致安装的时候字符检查项错误）。

![picture](/picture/ccs-setup.png)

### 2.安装过程
打开setup.exe

点击``next``
![picture](/picture/ccs-1.png)


接受协议并继续。

![picture](/picture/ccs-2.png)
这是对安装环境进行检查，安装目录出现中文会在这一步报错。当检查全部OK，点击``next``。
![picture](/picture/ccs-3.png)
选择安装位置,默认为c盘，可以自定义位置，安装位置不能含中文字符。
![picture](/picture/ccs-4.png)
选择安装方式，第一项是自定义安装支持设备和仿真器。方式二是安装全部支持设备和仿真器。一般选择方式一，空间足够可以选择方式二。
![picture](/picture/ccs-5.png)
选择支持的设备：
此处选择是是 msp432 、 msp432 、 c2000 、tiva (TM4C12)系列，这是一般常芯片可根据自己需求自主选择。
![picture](/picture/ccs-6.png)
选择支持的仿真器，这里可以根据需求选择。本文全部选择。
![picture](/picture/ccs-7.png)
显示缺少的支持。此处可忽略。
![picture](/picture/ccs-8.png)
点击``next``，开始安装。
![picture](/picture/ccs-9.png)
安装过程，需等待几分钟。
![picture](/picture/ccs-10.png)
中途会提示要重启电脑完成安装，点击确认即可。


