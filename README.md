# VirtualFPGA

#### 介绍
用SDL开发的虚拟FPGA，配合Verilator使用

#### 软件架构
软件架构说明


#### 安装教程

1.  xxxx
2.  xxxx
3.  xxxx

#### 使用说明

##### 使用Verilator

使用Veriloator，把Verilog转换成C++。
Virtual FPGA需要使用Verilator生成的一些接口。

##### 编写两个辅助函数

接下来，你需要编写两个辅助函数`update_input()`和`update_output()`。
当你在虚拟FPGA上改变输入（比如拨动一个开关）的时候，
虚拟FPGA会自动调用`update_input()`，让你编写的模块能够收到这个改变。
模块得到新的输出之后，虚拟FPGA会调用`update_output()`，把模块的输出同步到GUI上。

##### 体验虚拟FPGA

要改变某个按键或开关的状态，可以使用键盘快捷键，也可以点击鼠标。

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  可以用`include/configs.h`选择贴图，让自己的虚拟FPGA开发板更有特色，贴图放在`pic`目录下；
