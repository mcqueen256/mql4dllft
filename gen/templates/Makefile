TARGET = Robot
TARGET_TEST = test


ifeq ($(OS),Windows_NT)
	UNAME = win
	EXT = dll
else
	UNAME = unix
	EXT = o
endif

ifeq ($(UNAME),win)
	RMFILE = del /Q *.obj output\*
	RMDIR = 
	CC = cl.exe
	CFLAGS = /EHsc /D_USRDLL /D_WINDLL
	OUTPUT = /link /DLL /OUT:output/$(TARGET).dll
else
	RMFILE = rm -f *.bin *.exe *.so *.pid *.o output/*
	RMDIR = rm -rf *.induct *.outfiles *.dSYM kernels 
	CC = g++
	CFLAGS = -c
	OUTPUT = -o output/$(TARGET).o
endif

FLAGS += -std=c++11

CPP_SRCS:=$(wildcard *.cpp)
TEST_SRC:=$(wildcard test_src/*.cpp)

# directories
DIR_SOURCE = ./
DIR_TEST_SOURCE = test_src/
DIR_OUTPUT = output/

SRC_MAIN = $(wildcard $(DIR_SOURCE)*.cpp)
SRC_TEST = $(wildcard $(DIR_TEST_SOURCE)*.cpp)

OBJ_MAIN_FILES = $(addprefix $(DIR_OUTPUT), $(notdir $(SRC_MAIN:.cpp=.o)))
OBJ_TEST_FILES = $(addprefix $(DIR_OUTPUT), $(notdir $(SRC_TEST:.cpp=.o)))

all:
	$(CC) $(CFLAGS) $(CPP_SRCS) $(OUTPUT)

$(DIR_OUTPUT)$(TARGET_TEST): $(OBJ_TEST_FILES) $(OBJ_MAIN_FILES)
	$(CC) -std=c++11 $^ -o $@

test: $(DIR_OUTPUT)$(TARGET_TEST)
	./$(DIR_OUTPUT)$(TARGET_TEST)

$(DIR_OUTPUT)%.o: %.cpp
	$(CC) $(FLAGS) $^ -c -o $@

$(DIR_OUTPUT)%.o: $(DIR_TEST_SOURCE)%.cpp
	$(CC) $(FLAGS) $^ -c -o $@ -I.

clean:
	$(RMFILE)
	$(RMDIR)
