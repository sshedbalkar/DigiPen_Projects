########################################################################
# $Id$
#
# Makefile for Linux
#
# Copyright © 2005 DigiPen Institute of Technology
########################################################################

CXXFLAGS = -I. -I../lib -g -I/usr/X11R6/include/GL/ -I/usr/include/GL/
LIBS = -lglut -lGLU -lGL -L/usr/X11R6/lib

target = framework.exe

src = framework.cpp geomlib.cpp scenelib.cpp transformation.cpp scene.cpp render.cpp

headers = framework.h geomlib.h scenelib.h transformation.h
glut = freeglut.h freeglut.dll freeglut_ext.h freeglut.lib freeglut_std.h
extras = README CS241-framework.vcproj Makefile

pkgFiles = $(src) $(headers) $(glut) $(extras)
pkgName = CS241-$(notdir $(CURDIR))-framework

objects = $(patsubst %.cpp,%.o,$(src))

$(target): $(objects)
	@echo Link $(target)
	@g++ -g  -o $@  $(objects) $(LIBS)

%.o: %.cpp
	@echo Compile $<
	@$(CXX) -c $(CXXFLAGS) $< -o $@

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
	g++ -MM  $(src) > make.depend

include make.depend
