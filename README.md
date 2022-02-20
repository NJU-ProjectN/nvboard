# NVBoard

## 介绍

NVBoard(NJU Virtual Board)是基于SDL开发的虚拟FPGA开发板，可以在Verilator仿真环境中模拟FPGA，支持LED，七段数码管，开关，键盘和VGA。

## 软件架构

软件架构说明

```
.
├── board                   开发板配置文件
│   └── ...
├── example                 示例项目
│   └── ...
├── include
│   ├── at_scancode.h
│   ├── component.h
│   ├── configs.h
│   ├── constrs.h
│   ├── keyboard.h
│   ├── macro.h
│   ├── nvboard.h
│   ├── render.h
│   └── vga.h
├── LICENSE
├── Makefile                NVBoard项目构建Makefile
├── pic                     NVBoard图片信息
├── README.en.md
├── README.md
└── src                     NVBorad源码
    ├── component.cpp
    ├── event.cpp
    ├── nvboard.cpp
    ├── render.cpp
    └── vga.cpp
```

## 安装教程

1. 将项目拷贝到本地，`git clone https://github.com/NJU-ProjectN/nvboard.git`；
2. 接下来，把本项目的目录设置成环境变量 `NVBOARD_HOME`。

## 示例

`example`目录下包含一个示例项目，在该目录下通过 `make run` 命令可运行该项目。

## 使用说明

### 编写C++文件

NVBoard提供了以下几组API

- `nvboard_init()`: 初始化NVBoard
- `nvboard_quit()`: 退出NVBoard
- `nvboard_bind_pin(pin, signal)`: 将HDL的信号signal连接到NVBoard里的引脚pin上
- `nvboard_update()`: 更新NVBoard中各组件的状态，每当电路状态发生改变时都需要调用该函数

为了方便进行信号的绑定，可以在你的项目目录下编写约束文件(.cons)，通过命令`make cons`能够依据编写的约束文件生成c++文件，
调用该文件中的`nvboard_bind_all_pins(dut)`函数即可完成所有信号的绑定。约束文件的格式如下所示

```
top=top_name

signal pin

signal (pin1,pin2,... pink)
```

在约束文件的开头，需要指定顶层模块名为`top_name`。约束文件支持两种信号绑定方式，`signal pin`表示将顶层模块的`signal`端口信号绑定到引脚`pin`上，
`signal (pin1,pin2,...,pink)`表示将顶层模块的`signal`信号的每一位从高到低依次绑定到`pin1,pin2,...,pink`上。

可以在该项目的board目录下的板卡配置文件中查看NVBoard的引脚信息。

### 编写makefile

你可以在任意运行在NVBoard的项目目录下编写makefile，makefile格式如下

```makefile
# 需要手动指定项目目录
DIR = .
## 默认项目源码在项目目录的src文件夹下，可手动指定
### SRC_DIR ?= $(DIR)/src
## 参与verilator编译的文件默认为$(SRC_DIR)下的所有*.v, *.c, *.cc, *.cpp文件，可手动指定
### SRCS ?= $(shell find $(SRC_DIR) -name "*.v" -or -name "*.c" -or -name "*.cc" -or -name "*.cpp")

# 需要指定顶层模块名称
TOPNAME = top

# 需要在最后将NVBoard的makefile包含进来
-include $(NVBOARD_HOME)/Makefile
```

### 体验NVBoard

最后，你只需要在项目目录下执行

```shell
make run
```

命令，即可在NVBoard上模拟运行你自己的verilog代码！

## 特技

1. 可以用 `include/configs.h`选择贴图，让自己的虚拟FPGA开发板更有特色，贴图放在 `pic`目录下；
