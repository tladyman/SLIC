WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres


INC_DEBUG = -Iinclude $(INC) `pkg-config --cflags opencv`
CFLAGS_DEBUG = $(CFLAGS) -g -std=c++0x -pthread
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS) -L/usr/lib -lboost_program_options `pkg-config --libs opencv`
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG =
OUT_DEBUG = bin/Debug/slic

OBJ_DEBUG = $(OBJDIR_DEBUG)/main.o $(OBJDIR_DEBUG)/src/Slic.o $(OBJDIR_DEBUG)/src/Centre.o $(OBJDIR_DEBUG)/src/Pixel.o

all: debug

clean: clean_debug

before_debug:
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)
	test -d $(OBJDIR_DEBUG)/src || mkdir -p $(OBJDIR_DEBUG)/src

after_debug:

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/main.o: main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.cpp -o $(OBJDIR_DEBUG)/main.o

$(OBJDIR_DEBUG)/src/Slic.o: src/Slic.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Slic.cpp -o $(OBJDIR_DEBUG)/src/Slic.o

$(OBJDIR_DEBUG)/src/Centre.o: src/Centre.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Centre.cpp -o $(OBJDIR_DEBUG)/src/Centre.o

$(OBJDIR_DEBUG)/src/Pixel.o: src/Pixel.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Pixel.cpp -o $(OBJDIR_DEBUG)/src/Pixel.o

clean_debug:
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)
	rm -rf $(OBJDIR_DEBUG)/src

.PHONY: before_debug after_debug clean_debug
