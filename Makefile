ifeq ($(OS),Windows_NT)
	UNAME = win
else
	UNAME = unix
endif

ifeq ($(UNAME),win)
	RMFILE = del /Q *.obj output\*
	RMDIR = 
	CC = cl.exe
	CFLAGS = /EHsc /D_USRDLL /D_WINDLL
	OUTPUT = /link /DLL /OUT:output/Robot.dll
else
	RMFILE = rm -f *.bin *.exe *.so *.pid *.o
	RMDIR = rm -rf *.induct *.outfiles *.dSYM kernels
	CC = g++
	CFLAGS = -c
	OUTPUT = -o output/Robot.o
endif

all:
	$(CC) $(CFLAGS) *.cpp $(OUTPUT)

clean:
	$(RMFILE)
	$(RMDIR)
