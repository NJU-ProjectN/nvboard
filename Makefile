VERILATOR = verilator
VERILATOR_CFLAGS += -MMD -Wall --build -cc -Wno-lint \
				-O3 --x-assign fast --x-initial fast --noassert

# include path
INC_PATH ?= 

# files of emu
TOPNAME ?= top
DIR ?= $(NVBOARD_HOME)/emu
OBJ_DIR = $(DIR)/obj_dir

# generated executable file
DST_EXE = NVBOARD_$(TOPNAME)
DST_BIN = $(OBJ_DIR)/$(DST_EXE)

# SRC of the project

SRC_DIR ?= $(DIR)/src
SRCS ?= $(shell find $(SRC_DIR) -name "*.v" -or -name "*.c" -or -name "*.cc" -or -name "*.cpp")

### Default rules...

# Build rules of nvboard
include $(NVBOARD_HOME)/scripts/nvboard.mk

INCFLAGS = $(addprefix -I, $(INC_PATH))
CFLAGS += $(INCFLAGS) -DTOP_NAME="\"V$(TOPNAME)\""

LDFLAGS += -lSDL2 -lSDL2_image

$(DST_BIN): $(SRCS) $(NBD_ARCHIVE)
	@rm -rf $(OBJ_DIR)
	@echo $(INC_PATH)
	@$(VERILATOR) $(VERILATOR_CFLAGS) \
		-top $(TOPNAME) $^ \
		$(addprefix -CFLAGS , $(CFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(OBJ_DIR) --exe -o $(DST_EXE)

run: $(DST_BIN)
	@$(DST_BIN)

clean:
	rm -rf $(OBJ_DIR)

.PHONY: clean run