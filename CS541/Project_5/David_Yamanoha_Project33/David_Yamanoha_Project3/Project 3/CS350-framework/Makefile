########################################################################
# $Id$
#
# Makefile for Linux
#
# Copyright © 2005 DigiPen Institute of Technology
########################################################################

CXXFLAGS = -I. -g -I/usr/X11R6/include/GL/ -I/usr/include/GL/
LIBS = -lglut -lGLU -lGL -L/usr/X11R6/lib -lXi -lXmu

root = CS350-framework
target = ../$(root).exe
bindir = ../$(root)-bin

src = framework.cpp geomlib.cpp scenelib.cpp transformation.cpp scene.cpp render.cpp geomlib-advanced.cpp

objects = $(patsubst %.cpp,$(bindir)/%.o,$(src))

$(target): $(bindir) $(objects)
	g++ -g  -o $@  $(patsubst %,$(bindir)/%,$(objects)) $(LIBS)

$(bindir):
	mkdir $(bindir)

$(objects): $(bindir)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $(bindir)/$@

run: $(target)
	$(target)
