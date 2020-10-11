########################################################################
# $Id$
#
# Makefile for Linux
#
# Copyright © 2005 DigiPen Institute of Technology
########################################################################

CXXFLAGS = -I. -g -I/usr/X11R6/include/GL/ -I/usr/include/GL/
LIBS = -lglut -lGLU -lGL 

target = framework.exe

src = framework.cpp geomlib-advanced.cpp geomlib.cpp render.cpp scene.cpp scenelib.cpp transformation.cpp


headers = framework.h geomlib.h transformation.h scenelib.h

extras = framework.vcproj Makefile

pkgFiles = $(src) $(headers) $(extras)
pkgName = CS541-$(notdir $(CURDIR))-framework

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
	cp ../src/freeglut* ../$(pkgName)
	sed -i -n '1,/^include make.depend/p' ../$(pkgName)/Makefile
	cd ..;  zip -r $(pkgName).zip $(pkgName); #rm -rf $(pkgName)

clean:
	rm -f *.o *~ framework.exe make.depend

make.depend: 
	g++ -MM  $(src) > make.depend

include make.depend
