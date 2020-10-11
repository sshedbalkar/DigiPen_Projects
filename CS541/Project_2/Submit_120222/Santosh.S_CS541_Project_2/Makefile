########################################################################
# $Id$
#
# Makefile for Linux
#
# Copyright © 2009 DigiPen Institute of Technology
########################################################################

CXXFLAGS = -I. -g -I/usr/X11R6/include/GL/ -I/usr/include/GL/
LIBS = -lglut -lGLU -lGL -L/usr/X11R6/lib -lXi -lXmu

target = framework.exe

src1 = framework.cpp shader.cpp scene.cpp
src2 = GLee.c
headers = GLee.h scene.h shader.h
extras = framework.vcproj Makefile shader.frag shader.vert
glut = freeglut.h freeglut.dll freeglut_ext.h freeglut.lib freeglut_std.h


pkgFiles = $(src1) $(src2) $(headers) $(glut) $(extras)
pkgName = CS541-$(notdir $(CURDIR))-framework

objects = $(patsubst %.cpp,%.o,$(src1)) $(patsubst %.c,%.o,$(src2))

$(target): $(objects)
	@echo Link $(target)
	@g++ -g  -o $@  $(objects) $(LIBS)

%.o: %.cpp
	@echo Compile $<
	@$(CXX) -c $(CXXFLAGS) $< -o $@

%.o: %.c
	@echo Compile $<
	@$(CC) -c $(CXXFLAGS) $< -o $@

run: $(target)
	./$(target)

zip: $(pkgFiles)
	rm -rf ../$(pkgName) ../$(pkgName).zip
	mkdir ../$(pkgName)
	cp $(pkgFiles) ../$(pkgName)
	cd ..;  zip -r $(pkgName).zip $(pkgName); rm -rf $(pkgName)

clean:
	rm -f *.o *~ framework make.depend

make.depend: 
	g++ -MM  $(src1) > make.depend

include make.depend
