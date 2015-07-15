# Evan Wide
#
# MAC CHORD Makefile
#
# LIBS
# pthread  	Threading
#
# Flags
# march=native
# mfpath=sse
# O3 optimization on release build
# g on debug build
# Wall on debug build
#
# Defines
# DEBUG	on debug build
# TEST on test build
#



EXEC 	= mac_chord

# Commands
RM	= rm -f
RMD	= $(RM) -r
CC	= gcc
CXX	= g++
AR	= ar
TAR	= tar
MKDIR	= mkdir -p
CP	= cp

# Directories
D_SRC	= src/
D_BUILD	= build/
D_BIN	= bin/
D_LIB	= libs/
D_INC	= inc/
D_TEST	= tests/
D_DOCS	= docs/
D_PROT	= protocols/

# Flags
CFLAGS	= -c -iquote $(D_INC)
LIBS	= -pthread -lssl -lcrypto
CXFLAGS = $(CFLAGS) -std=c++11

# Generate Object file names
SRCS	= \
	  $(wildcard $(D_SRC)*.c) \
	  $(wildcard $(D_SRC)*.cpp) \
	  $(wildcard $(D_SRC)*.C) \
	  $(wildcard $(D_SRC)*.cc) \
	  $(wildcard $(D_SRC)*.c++)

TEST_SRCS	= \
	  $(wildcard $(D_TEST)*.c) \
	  $(wildcard $(D_TEST)*.cpp) \
	  $(wildcard $(D_TEST)*.C) \
	  $(wildcard $(D_TEST)*.cc)

LIBSS		= \
	  $(wildcard $(D_LIB)*.a)

OBJS	= \
	  $(patsubst $(D_SRC)%.c, $(D_BUILD)%.o, \
	  $(patsubst $(D_SRC)%.cpp, $(D_BUILD)%.o, \
	  $(patsubst $(D_SRC)%.C, $(D_BUILD)%.o, \
	  $(patsubst $(D_SRC)%.cc, $(D_BUILD)%.o, \
	  $(patsubst $(D_SRC)%.c++, $(D_BUILD)%.o, $(SRCS))))))

TEST_OBJS	= \
	  $(patsubst $(D_TEST)%.c, $(D_BUILD)%.o, \
	  $(patsubst $(D_TEST)%.cpp, $(D_BUILD)%.o, \
	  $(patsubst $(D_TEST)%.C, $(D_BUILD)%.o, \
	  $(patsubst $(D_TEST)%.cc, $(D_BUILD)%.o, $(TEST_SRCS)))))

.PHONY:	all protocols release debug test clean rebuild docs
# No optimizations
all: $(D_BIN)$(EXEC)


protocols: $(D_INC)chord_message.h

$(D_INC)chord_message.h: $(D_PROT)chord_message.fbs
	flatc -c -o $(D_INC) $(D_PROT)chord_message.fbs
	mv $(D_INC)chord_message_generated.h $(D_INC)chord_message.h


# protocols: $(D_SRC)chord_message.capnp.cpp
# 
#  $(D_INC)chord_message.capnp.h $(D_SRC)chord_message.capnp.cpp: $(D_PROT)chord_message.capnp
# 	capnp compile -I=$(D_PROT) -oc++ $(D_PROT)chord_message.capnp
# 	mv $(D_PROT)chord_message.capnp.h $(D_INC)chord_message.capnp.h
# 	mv $(D_PROT)chord_message.capnp.c++ $(D_SRC)chord_message.capnp.cpp


# Optimizations
release: CFLAGS += -O3
release: all

# Debug flags, function names, and all errors reported
debug: CFLAGS += -g -DDEBUG -Wall
debug: all

# Unit testing enabled
test: CFLAGS += -Wall -DTEST
test: LIBS += -lcppunit -ldl
test: clean  $(TEST_OBJS) all

clean:
	$(RM) $(D_BUILD)*.o
	$(RM) $(D_BIN)$(EXEC)
	$(RMD) $(D_DOCS)html

rebuild: clean all

docs:
	doxygen $(D_DOCS)Doxyfile
# Create directories
$(D_TEST):
	$(MKDIR) $(D_TEST)

$(D_BUILD):
	$(MKDIR) $(D_BUILD)

$(D_BIN):
	$(MKDIR) $(D_BIN)


# Build files
# Files with headers
$(D_BUILD)%.o : $(D_SRC)%.c $(D_INC)%.h
	$(CC) $(CFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_SRC)%.cpp $(D_INC)%.h
	$(CXX) $(CXFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_SRC)%.cc $(D_INC)%.h
	$(CXX) $(CXFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_SRC)%.C $(D_INC)%.h
	$(CXX) $(CXFLAGS) $< -o $@

# Files with no headers
$(D_BUILD)%.o : $(D_SRC)%.c
	$(CC) $(CFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_SRC)%.C
	$(CXX) $(CXFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_SRC)%.cpp
	$(CXX) $(CXFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_SRC)%.cc
	$(CXX) $(CXFLAGS) $< -o $@

# Build tests
$(D_BUILD)%.o : $(D_TEST)%.c
	$(CC) $(CFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_TEST)%.cc
	$(CXX) $(CXFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_TEST)%.cpp
	$(CXX) $(CXFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_TEST)%.C
	$(CXX) $(CXFLAGS) $< -o $@


# Link Objects
$(D_BIN)$(EXEC): $(D_BIN) $(D_BUILD) $(LIBSS) $(OBJS) 
	$(CXX) -o $(D_BIN)$(EXEC)  $(OBJS) $(LIBS)

