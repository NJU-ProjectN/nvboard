# nvboard

# 近期迭代较快，readme的更新可能不够及时，见谅
#### 介绍
用SDL开发的虚拟FPGA开发板，配合Verilator使用

#### 软件架构
软件架构说明
```
.
├── emu                     演示项目目录
│   └── ...
├── include
│   ├── clock.h
│   ├── configs.h
│   ├── constrs.h
│   ├── nvboard.h
│   └── render.h
├── LICENSE
├── Makefile                NVBOARD项目构建makefile
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
└── src                     NVBORAD源码
    ├── component.cpp
    ├── event.cpp
    ├── nvboard.cpp
    ├── render.cpp
    └── vga.cpp
```

#### 安装教程

1.  将项目拷贝到本地，`git clone https://github.com/NJU-ProjectN/nvboard.git`；
2.  接下来，把本项目的目录设置成环境变量`NVBOARD_HOME`。

#### 使用说明

##### 编写makefie

你可以在任意运行在NVBOARD的项目目录下编写makefile，makefile格式如下

```makefile
# 需要手动指定项目目录
DIR = .
## 默认项目源码在项目目录的src文件夹下，可手动指定
### SRC_DIR ?= $(DIR)/src
## 参与verilator编译的文件默认为$(SRC_DIR)下的所有*.v, *.c, *.cc, *.cpp文件，可手动指定
### SRCS ?= $(shell find $(SRC_DIR) -name "*.v" -or -name "*.c" -or -name "*.cc" -or -name "*.cpp")


# 需要指定顶层模块名称
TOPNAME = top

# 需要在最后将NVBOARD的makefile包含进来
-include $(NVBOARD_HOME)/Makefile

```

##### 编写项目文件

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

这是一个简单的计数器。你可以将其放入$SRC_DIR文件夹中

##### 编写两个辅助函数

接下来，你需要在src文件中新建一个c++代码文件(如`update.cpp`)。

在`emu`下有一个`update.cpp`的示例。
可以看到`input_map`和`output_map`，这两个`std::map`就是nvboard和verilog通信的渠道。

NVBOARD会在输入有了变化或时钟边沿处调用`dut_update()`，当你在虚拟FPGA上改变输入（比如按下`btnc`）的时候，
`input_map`里面对应的键值会随之更新，

接下来`dut_update()`就会执行`update_input()`， 把`input_map`里面的键值赋值给模块的引脚，让你编写的模块能够收到这个改变。

之后，模块会进行模拟计算，最终将结果更新到输出引脚。

模块得到新的输出之后，`dut_update()`会执行`update_output()`，把模块的输出存入`output_map`。

`dut_update()`调用结束后，nvboard会把模块的输出同步到GUI上。

在`update.cpp`里，你需要编写以下内容
- 创建一个顶层模块对象`dut`
- 你需要手动编写辅助函数`dut_update()`，其行为如下：
  1. 从`input_map`中更新顶层模块输入
  2. 执行模块模拟过程`dut.eval();`
  3. 向`output_map`中更新顶层模块输出

##### 体验虚拟FPGA

最后，你只需要在项目目录下执行
```shell
make run
```
命令，即可在NVBOARD上模拟运行你自己的verilog代码！

#### 特技

1.  可以用`include/configs.h`选择贴图，让自己的虚拟FPGA开发板更有特色，贴图放在`pic`目录下；
