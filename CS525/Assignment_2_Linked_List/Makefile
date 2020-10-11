GCC=gcc
BCC=bcc32
MSC=cl
GCCFLAGS=-ansi -Wall -Wextra -pedantic -Wstrict-prototypes -Wmissing-prototypes -Wdeclaration-after-statement
GCCOPTIMIZE=-O3
GCC_COVERAGE_OPTIONS=--coverage -fprofile-arcs -fbranch-probabilities
BCCFLAGS=-w
MSCFLAGS=/MDd /W4 /Za /Zc:forScope /nologo /D_CRT_SECURE_NO_DEPRECATE
MSCOPTIMIZE=/O2 /MD
GCCOPTIMIZE=-O3
OBJECTS0=spvector.c
DRIVER0=driver.c
INCLUDE_EXT=-I ../../../common-code
DEFINE=

gcc0:
	$(GCC) -o gcc0.exe $(DRIVER0) $(OBJECTS0) $(GCCFLAGS) 
msc0:
	$(MSC) -Femsc0.exe $(DRIVER0) $(OBJECTS0) $(MSCFLAGS) 
bcc0:
	$(BCC) -ebcc0.exe $(DRIVER0) $(OBJECTS0) $(BCCFLAGS) 
bcc_cg:
	$(BCC) -ebcc_cg.exe -v -vG $(DRIVER0) $(OBJECTS0) $(BCCFLAGS) 
clean:
	rm -f *.exe *.tds *.o *.obj *manifest* bcc_cg.cgl
