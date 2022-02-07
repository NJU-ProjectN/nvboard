# 需要手动指定项目目录
DIR = .
## 默认项目源码在项目目录的src文件夹下，可手动指定
### SRC_DIR ?= $(DIR)/src
## 参与verilator编译的文件默认为$(SRC_DIR)下的所有*.v, *.c, *.cc, *.cpp文件，可手动指定
### SRCS ?= $(shell find $(SRC_DIR) -name "*.v" -or -name "*.c" -or -name "*.cc" -or -name "*.cpp")


# 需要指定顶层模块名称
TOPNAME = top

# 需要在最后将NVBOARD的makefile包含进来
-include $(NBOARD_HOME)/Makefile