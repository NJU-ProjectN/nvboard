ifndef TOPNAME
$(error TOPNAME should be given)
endif

# include path
INC_PATH ?= 

# files of emu
DIR ?= $(NBOARD_HOME)/new_emu
OBJ_DIR = $(DIR)/obj_dir
VARCHIVE := $(OBJ_DIR)/V$(TOPNAME)__ALL.a
INC_PATH += $(OBJ_DIR)

# generated executable file
DST_BIN = $(OBJ_DIR)/NVBOARD_$(TOPNAME)

# SRC of the project
VSRC_DIR ?= $(DIR)/vsrc
VSRCS ?= $(shell find $(VSRC_DIR) -name "*.v")

CSRC_DIR ?= $(DIR)/csrc
CSRCS ?= $(shell find $(CSRC_DIR) -name "*.c" -or -name "*.cc" -or -name "*.cpp")
COBJS = $(CSRCS:%.cpp=$(OBJ_DIR)/%.o)

### Default rules...
# Include global rules

include $(NBOARD_HOME)/scripts/vtrace.mk
include $(VERILATOR_ROOT)/include/verilated.mk

# Build rules of nboard
include $(NBOARD_HOME)/scripts/nboard.mk


-include V$(TOPNAME)_all.d
-include V$(TOPNAME)_ver.d
$(VARCHIVE): $(VSRCS)
	@echo + VERILATOR "->" $^
	@$(VERILATOR) $(VFLAGS) -top $(TOPNAME) -cc $(VSRCS) --Mdir $(OBJ_DIR)

-include $(COBJS:.o=.d)
$(OBJ_DIR)/%.o: %.cpp $(VARCHIVE)
	@echo + CXX "->" $<
	@mkdir -p $(dir $@)
	@$(OBJCACHE) $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OPT_FAST) \
		-DTOP_NAME="V$(TOPNAME)" -c -o $@ $<

$(DST_BIN): $(VK_GLOBAL_OBJS) $(COBJS) $(NBD_ARCHIVE) $(VARCHIVE)
	@echo + LD "->" $@
	@mkdir -p $(dir $@)
	@$(LINK) $(LD_FLAGS) $^ $(LOADLIBES) $(LDLIBS) $(LIBS) $(SC_LIBS) -o $@ $(SDL_FLAGS)


run: $(DST_BIN)
	@$(DST_BIN)

clean:
	rm -rf $(OBJ_DIR)
	rm -f verilated.d
	rm -f verilated.o

.PHONY: clean run