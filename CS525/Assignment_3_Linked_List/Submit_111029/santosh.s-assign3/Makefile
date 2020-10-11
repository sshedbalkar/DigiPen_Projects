GCC=g++
BCC=bcc32
MSC=cl
GCCFLAGS=-Wall -Wextra -ansi -Wold-style-cast -Woverloaded-virtual -Wsign-promo  -Wctor-dtor-privacy -Wnon-virtual-dtor -Wreorder
GCCOPTIMIZE=-O3
GCC_COVERAGE_OPTIONS=--coverage -fprofile-arcs -fbranch-probabilities
BCCFLAGS=-w
MSCFLAGS=                 /EHa      /MDd /W4 /Za /Zc:forScope /nologo /D_CRT_SECURE_NO_DEPRECATE
MSCFLAGSOPTIMIZED=/O2 /GL /EHsc /Za /MD  /W4 /Za /Zc:forScope /nologo /D_CRT_SECURE_NO_DEPRECATE /D"_SECURE_SCL 0"
OBJECTS0=vector.cpp
DRIVER0=driver.cpp
INCLUDE1=-I ../../../common-code/
MSCINCLUDE=/I"../../../common-code/"
MSCDEFINE=
DEFINE=

OSTYPE := $(shell uname)
#ifeq ($(strip $(OSTYPE)),Linux)
ifeq ($(OSTYPE),Linux)
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

gcc0:
	$(GCC) -o $@.exe $(DRIVER0) $(OBJECTS0) $(GCCFLAGS) -pedantic $(GCCOPTIMIZE) $(INCLUDE1) $(DEFINE) $(CYGWIN)
msc0:
	$(MSC) /Fe$@.exe $(DRIVER0) $(OBJECTS0) $(MSCFLAGS) $(MSCINCLUDE)  $(MSCDEFINE)
bcc0:
	$(BCC) -e$@.exe  $(DRIVER0) $(OBJECTS0) $(BCCFLAGS) $(DEFINE)
bcc_cg:
	$(BCC) -e$@.exe -v -vG $(DRIVER0) $(OBJECTS0) $(BCCFLAGS) $(DEFINE)
nocompile1:
	g++ driver-nocompile1.cpp $(OBJECTS0)
nocompile2:
	g++ driver-nocompile2.cpp $(OBJECTS0)
additional:
	g++ driver.2.cpp $(OBJECTS0)

clean:
	rm -f *gcno *gcda *gcov *.exe *.o *.obj *.tds
