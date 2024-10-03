
# Define V=1 for a more verbose compilation
ifndef V
        QUIET_CXX           = @echo '   ' CXX $@;
        QUIET_LINK          = @echo '   ' LINK $@;
        QUIET_MKDIR         = @echo '   ' MKDIR $@;
        QUIET_RUN           = @echo '   ' RUN $@;
        QUIET_UNZIP         = @echo '   ' UNZIP $@;
        QUIET_WGET          = @echo '   ' WGET $@;
        QUIET_CLEAN         = @echo '   ' CLEAN $@;
endif
RM = rm -rf

HW_ROOT ?= $(shell realpath ../..)

IMAGE ?= airplane

INPUT_PATH ?= $(HW_ROOT)/scripts/preprocess

IMGS = $(wildcard $(INPUT_PATH)/*.bmp)
BMPS = $(foreach j, $(IMGS), $(shell basename $(j)))
BINS = $(BMPS:.bmp=.bin)

MODELS ?= $(HW_ROOT)/models
MOJO_PRETRAINED ?= $(MODELS)/dwarf7.mojo

SYSTEMC ?= /opt/systemc
TARGET_ARCH = linux64

PV ?= $(HW_ROOT)/pv

INCDIR ?=
INCDIR += -I$(PV)/mojo
INCDIR += -I../src -I../tb
INCDIR += -I$(SYSTEMC)/include

CXXFLAGS ?= -O3 -Wall -Werror -fmax-errors=5
CXXFLAGS += $(INCDIR)

LDLIBS ?=
LDLIBS += -L$(SYSTEMC)/lib-$(TARGET_ARCH)

LDFLAGS ?=
LDFLAGS += -lsystemc


VPATH :=
VPATH += ../src
VPATH += ../tb

SRCS := fc_layer.cpp fc_layer_tb.cpp system.cpp sc_main.cpp

OBJS := $(SRCS:.cpp=.o)

HDRS := $(wildcard $(PV)/mojo/*.h)
HDRS += $(wildcard $(PV)/*.h)
HDRS += $(wildcard ../src/*.hpp)
HDRS += $(wildcard ../tb/*.hpp)

all: dwarf

.SUFFIX: .c .cpp .o

dwarf: $(OBJS)
	$(QUIET_LINK) $(CXX) -o $@ $^ $(LDLIBS) $(LDFLAGS)

.cpp.o:
	$(QUIET_CXX) $(CXX) $(CXXFLAGS) -c $<

$(OBJS): $(HDRS)

clean:
	$(QUIET_CLEAN) $(RM) $(OBJS) dwarf *.vcd out_*_systemc.txt *.bin

distclean: clean
	@make -C $(INPUT_PATH) clean

$(INPUT_PATH)/preprocess:
	@$(MAKE) -C $(INPUT_PATH)

$(MOJO_PRETRAINED):
	@mkdir -p $(MODELS)
	@cp -f /opt/dwarf_model/dwarf7.mojo $@

$(BINS): $(INPUT_PATH)/preprocess
	@ln -s $(INPUT_PATH)/$@

dwarf-run: dwarf $(BINS) $(MOJO_PRETRAINED)
	$(QUIET_RUN) LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(SYSTEMC)/lib-$(TARGET_ARCH) ./$< $(IMAGE)

.PHONY: all clean distclean dwarf-run
