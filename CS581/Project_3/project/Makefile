OSTYPE := $(shell uname)
ifeq ($(OSTYPE),Linux)
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

GCC=g++
GCCFLAGS=-O2 -Wall -Wextra -ansi -pedantic -Wold-style-cast -Woverloaded-virtual -Wsign-promo  -Wctor-dtor-privacy -Wnon-virtual-dtor -Wreorder
DEFINE=-DINLINE_VARIABLE -DINLINE_CONSTRAINT_GRAPH -DINLINE_CONSTRAINT -DINLINE_CSP 

MSC=cl
MSCFLAGS=/EHa /W4 /Za /Zc:forScope /nologo /D_CRT_SECURE_NO_DEPRECATE /D"_SECURE_SCL 0" /O2i /GL
MSCDEFINE=/DINLINE_VARIABLE /DINLINE_CONSTRAINT_GRAPH /DINLINE_CONSTRAINT /DINLINE_CSP 

BCC=bcc32
BCCFLAGS=-w

VALGRIND_OPTIONS=-q --leak-check=full
DIFF_OPTIONS=-y --strip-trailing-cr --suppress-common-lines

#everything is templetized
OBJECTS0=

OSTYPE := $(shell uname)
ifeq ($(OSTYPE),Linux)
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif


example:
	$(GCC) main.cpp -DEXAMPLE -DDFS $(CYGWIN) $(OBJECTS0) $(GCCFLAGS) $(DEFINE) -o $@.exe #ARC,DFS
simple:
	$(GCC) main.cpp -DSIMPLE -DDFS $(CYGWIN) $(OBJECTS0) $(GCCFLAGS) $(DEFINE) -o $@.exe #ARC,DFS

queen-28-dfs:
	$(GCC) main.cpp -DQUEEN -DSIZE=28 -DDFS $(CYGWIN) $(OBJECTS0) $(GCCFLAGS) $(DEFINE) -o $@.exe #ARC,DFS
queen-100-fc:
	$(GCC) main.cpp -DQUEEN -DSIZE=100 -DFC $(CYGWIN) $(OBJECTS0) $(GCCFLAGS) $(DEFINE) -o $@.exe #ARC,DFS
queen-100-arc:
	$(GCC) main.cpp -DQUEEN -DSIZE=100 -DARC $(CYGWIN) $(OBJECTS0) $(GCCFLAGS) $(DEFINE) -o $@.exe #ARC,DFS

ms5-fc:
	$(GCC) main.cpp -DMS   -DSIZE=5 -DFC $(CYGWIN) $(OBJECTS0) $(GCCFLAGS) $(DEFINE) -o $@.exe #ARC,DFS
msbc5-fc:
	$(GCC) main.cpp -DMSBC -DSIZE=5 -DFC $(CYGWIN) $(OBJECTS0) $(GCCFLAGS) $(DEFINE) -o $@.exe #ARC,DFS
ms5-arc:
	$(GCC) main.cpp -DMS   -DSIZE=5 -DARC $(CYGWIN) $(OBJECTS0) $(GCCFLAGS) $(DEFINE) -o $@.exe #ARC,DFS
msbc5-arc:
	$(GCC) main.cpp -DMSBC -DSIZE=5 -DARC $(CYGWIN) $(OBJECTS0) $(GCCFLAGS) $(DEFINE) -o $@.exe #ARC,DFS

ms6-fc:
	$(GCC) main.cpp -DMS   -DSIZE=6 -DFC $(CYGWIN) $(OBJECTS0) $(GCCFLAGS) $(DEFINE) -o $@.exe #ARC,DFS
msbc6-fc:
	$(GCC) main.cpp -DMSBC -DSIZE=6 -DFC $(CYGWIN) $(OBJECTS0) $(GCCFLAGS) $(DEFINE) -o $@.exe #ARC,DFS

#MS compiler
msc-example:
	$(MSC) main.cpp -DEXAMPLE -DDFS  $(OBJECTS0) $(MSCFLAGS) $(MSCDEFINE) /Fe$@.exe #ARC,DFS
msc-simple:
	$(MSC) main.cpp -DSIMPLE -DDFS  $(OBJECTS0) $(MSCFLAGS) $(MSCDEFINE) /Fe$@.exe #ARC,DFS

msc-queen-28-dfs:
	$(MSC) main.cpp -DQUEEN -DSIZE=28 -DDFS  $(OBJECTS0) $(MSCFLAGS) $(MSCDEFINE) /Fe$@.exe #ARC,DFS
msc-queen-100-fc:
	$(MSC) main.cpp -DQUEEN -DSIZE=100 -DFC  $(OBJECTS0) $(MSCFLAGS) $(MSCDEFINE) /Fe$@.exe #ARC,DFS
msc-queen-100-arc:
	$(MSC) main.cpp -DQUEEN -DSIZE=100 -DARC  $(OBJECTS0) $(MSCFLAGS) $(MSCDEFINE) /Fe$@.exe #ARC,DFS

msc-ms5-fc:
	$(MSC) main.cpp -DMS   -DSIZE=5 -DFC  $(OBJECTS0) $(MSCFLAGS) $(MSCDEFINE) /Fe$@.exe #ARC,DFS
msc-msbc5-fc:
	$(MSC) main.cpp -DMSBC -DSIZE=5 -DFC  $(OBJECTS0) $(MSCFLAGS) $(MSCDEFINE) /Fe$@.exe #ARC,DFS
msc-ms5-arc:
	$(MSC) main.cpp -DMS   -DSIZE=5 -DARC  $(OBJECTS0) $(MSCFLAGS) $(MSCDEFINE) /Fe$@.exe #ARC,DFS
msc-msbc5-arc:
	$(MSC) main.cpp -DMSBC -DSIZE=5 -DARC  $(OBJECTS0) $(MSCFLAGS) $(MSCDEFINE) /Fe$@.exe #ARC,DFS

msc-ms6-fc:
	$(MSC) main.cpp -DMS   -DSIZE=6 -DFC  $(OBJECTS0) $(MSCFLAGS) $(MSCDEFINE) /Fe$@.exe #ARC,DFS
msc-msbc6-fc:
	$(MSC) main.cpp -DMSBC -DSIZE=6 -DFC  $(OBJECTS0) $(MSCFLAGS) $(MSCDEFINE) /Fe$@.exe #ARC,DFS

clean:
	rm -f *exe. *.obj *.o
