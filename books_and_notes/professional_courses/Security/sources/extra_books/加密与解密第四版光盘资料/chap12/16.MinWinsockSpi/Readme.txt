
说明
=================================================
最小化的 Winsock 2 服务提供者示例

开发工具：VC6.0
运行平台：Windows 2000, Windows 98


安装方法
-------------------------------------------
1. 运行Release\install -install
2. Win98需要重启计算机，Win2000不需要
3. 运行dbgview.exe(本书没有附带，
   www.sysinternals.com)，用来查看调试信息
4. 运行Release\install -remove 卸载
-------------------------------------------

[common]
存放公用源代码的目录

[InstallExe]
存放安装程序的目录

[Release]
存放编译后创建的可执行文件的目录

[TcpIpDog]
存放Winsock 2 SPI封包截获的DLL工程

[PacketCapture.dsw]
VC 工作区文件