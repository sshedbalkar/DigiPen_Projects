OSTYPE := $(shell uname)
ifeq ($(OSTYPE),Linux)
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

GCC=g++
GCCFLAGS=-O2 -Wall -Wextra -ansi -pedantic -Wold-style-cast -Woverloaded-virtual -Wsign-promo  -Wctor-dtor-privacy -Wnon-virtual-dtor -Wreorder
DEFINE=-DINLINE_FIELD -DINLINE_ANALYZER

MSC=cl
MSCFLAGS=/EHa /W4 /Za /Zc:forScope /nologo /D_CRT_SECURE_NO_DEPRECATE /D"_SECURE_SCL 0" /O2i /GL
MSCDEFINE=/DINLINE_VARIABLE /DINLINE_CONSTRAINT_GRAPH /DINLINE_CONSTRAINT /DINLINE_CSP 

VALGRIND_OPTIONS=-q --leak-check=full
DIFF_OPTIONS=-y --strip-trailing-cr --suppress-common-lines

OBJECTS0=field.cpp analyzer.cpp

demo driver1 driver2 driver3:
	g++  $@.cpp $(OBJECTS0) $(GCCFLAGS) $(GCCOPTIMIZE) $(INCLUDE) $(DEFINE) -o $@.exe
clean:
	rm -f *.exe *.o *.obj
