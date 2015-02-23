# Some notes:
#	In order to run correctly this Makefile, you have to set this variables:
SYSTEMC_DIR=$$SYSTEMC_HOME
SCAMS_DIR=$$SCAMS_HOME
SCAMS_INC   = $(SCAMS_DIR)/include
SCAMS_LIB   = $(SCAMS_DIR)/lib-linux64
SYSTEMC_INC = $(SYSTEMC_DIR)/include
SYSTEMC_LIB = $(SYSTEMC_DIR)/lib-linux64
#
#	So, you need to install both SystemC and SystemC-AMS.
#	Good Luck!

CXX        = g++
LD         = g++
RM         = rm -rf
CXXFLAGS   = -c

EXECUTABLE=./bin/lesson_07.x

INCS =	-I $(SCAMS_INC) \
		-I $(SYSTEMC_INC)\
	-I./include/\
	-I.

LIBS =	$(SYSTEMC_LIB)/libsystemc.a \
    -lpthread \
		$(SCAMS_LIB)/libsystemc-ams.a \
	 	#-lsystemc-ams \
	 	#-lsystemc \
	 	-lm \

OBJS =     ./obj/system.o \
	   ./obj/physical_plant.o \
	   ./obj/controller.o \
	   ./obj/controller_error.o \
	   ./obj/main.o \
     ./obj/testbench_TLM.o \
     ./obj/tlm_transactor.o \
     ./obj/rtl_transactor.o \

all: $(EXECUTABLE)

install: all

$(EXECUTABLE): $(OBJS)
	@echo Linking $@
	@$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	
#------------------------------------------------------------
#---------------------------System-----------------------------
obj/system.o: src/system.cc
	@$(CXX) $(DEBUG) $(CXXFLAGS) $(INCS) -o $@ $<

#------------------------------------------------------------
#--------------------TDF Modules-----------------------------

obj/tlm_transactor.o: src/tlm_transactor.cc
	$(CXX) $(DEBUG) $(CXXFLAGS) $(INCS) -o $@ $<

obj/rtl_transactor.o: src/rtl_transactor.cc
	$(CXX) $(DEBUG) $(CXXFLAGS) $(INCS) -o $@ $<

obj/testbench_TLM.o: src/testbench_TLM.cc
	$(CXX) $(DEBUG) $(CXXFLAGS) $(INCS) -o $@ $<

obj/controller.o: src/controller.cc
	$(CXX) $(DEBUG) $(CXXFLAGS) $(INCS) -o $@ $<

obj/controller_error.o: src/controller_error.cc
	$(CXX) $(DEBUG) $(CXXFLAGS) $(INCS) -o $@ $<

obj/physical_plant.o: src/physical_plant.cc
	$(CXX) $(DEBUG) $(CXXFLAGS) $(INCS) -o $@ $<

obj/main.o: src/main.cc
	$(CXX) $(DEBUG) $(CXXFLAGS) $(INCS) -o $@ $<



clean:
	@rm -rf obj/*.o
	@rm -rf *.x
	@rm -rf *.vcd
	@rm -rf $(EXECUTABLE)
	@echo "Clean"

.PHONY: all clean install
