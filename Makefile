TARGET = Robot



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
	RMFILE = rm -f *.bin *.exe *.so *.pid *.o
	RMDIR = rm -rf *.induct *.outfiles *.dSYM kernels
	CC = g++
	CFLAGS = -c
	OUTPUT = -o output/$(TARGET).o
endif

CPP_SRCS:=$(wildcard *.cpp)
TEST_SRC:=$(wildcard test_src/*.cpp)

all:
	$(CC) $(CFLAGS) $(CPP_SRCS) $(OUTPUT)

test:
	$(CC) -std=c++11 $(TEST_SRC) -o output/test
	./output/test

clean:
	$(RMFILE)
	$(RMDIR)
