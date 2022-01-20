# nvboard

# 近期迭代较快，readme的更新可能不够及时，见谅
#### 介绍
用SDL开发的虚拟FPGA开发板，配合Verilator使用

#### 软件架构
软件架构说明
```
.
├── emu
│   └── ...
├── include
│   ├── clock.h
│   ├── configs.h
│   ├── constrs.h
│   ├── event.h
│   ├── nboard.h
│   └── render.h
├── LICENSE
├── Makefile
├── pic
│   ├── vbg_1.png
│   ├── vbg_2.png
│   ├── vbtn_off.png
│   ├── vbtn_on.png
│   ├── vsegled_bg.png
│   ├── vsw_off.png
│   └── vsw_on.png
├── README.en.md
├── README.md
└── src
    ├── clock.cpp
    ├── constrs.cpp
    ├── event.cpp
    ├── main.cpp
    ├── render.cpp
    └── update.cpp -> $NBOARD_HOME/emu/update.cpp
```

#### 安装教程

1.  将项目拷贝到本地，`git clone https://github.com/NJU-ProjectN/nvboard.git`；
2.  接下来，把本项目的目录设置成环境变量`NBOARD_HOME`。

#### 使用说明

##### 使用Verilator

使用Veriloator，把Verilog转换成C++。
比如，新建verilog文件`top.v`：

```
//top.v
module top (
  input clk,
  input reset,
  output reg [3:0] out);
  
  always @(posedge clk) begin
    out <= reset ? 0 : out + 1;
  end  
endmodule
```

这是一个简单的计数器。我们可以执行以下命令：

```
verilator -Wall -cc -build top.v
```

生成nboard需要的头文件（`Vtop.h`）、静态链接库文件（`Vtop__ALL.a`）、makefile文件（`Vtop_classes.mk`）。

##### 编写两个辅助函数

接下来，你需要在`update.cpp`里编写两个辅助函数`update_input()`和`update_output()`。

在`emu`下有一个`update.cpp`的示例。
可以看到`input_map`和`output_map`，这两个`std::map`就是nboard和verilog通信的渠道。

当你在虚拟FPGA上改变输入（比如按下`btnc`）的时候，
`input_map`里面对应的键值会随之更新，
接下来nvboard就会调用`update_input()`，
你需要把`input_map`里面的键值赋值给模块的引脚，
让你编写的模块能够收到这个改变。

模块得到新的输出之后，nvboard会随即调用`update_output()`，
你也需要把模块的输出存入`output_map`，
nvboard会把模块的输出同步到GUI上。

##### 让nvboard知道模块的名称

新建`topname.h`，在这里把verilator生成的头文件包含进来，再加一条宏定义：

```
#define TOP_NAME Vtop
```

##### 体验虚拟FPGA

把头文件（`Vtop.h`）、静态链接库文件（`Vtop__ALL.a`）、makefile文件（`Vtop_classes.mk`）、辅助更新函数（`update.cpp`）、在`$NBOARD_HOME`下执行`make TOPNAME=top run`即可体验虚拟FPGA。

要改变某个按键或开关的状态，可以使用键盘快捷键，也可以用鼠标单击。

#### 特技

1.  可以用`include/configs.h`选择贴图，让自己的虚拟FPGA开发板更有特色，贴图放在`pic`目录下；
