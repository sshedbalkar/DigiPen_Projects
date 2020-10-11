Project Report:

Overview:
All of the bsp generation, and a bulk of the changes to the framework exist in the file BSPTools.cpp and 
BSPTools.h.  BSPTools contains a class which represents the a BSPtree data structure, and various statistics
on the tree itself, such as depth and the running total of split polygons.

Implementation:
Everything I did was by the book, no surprises.  I will, however, breifly describe the method implemented
for polygon picking (BSPTree::SelectAndRemove()).  You will see two loops iterating over the remaining
polygons to be categorized.  The first loop does a simple average of all the vertices of all the polygons
in 3D space.  This position is recorded.  The second loop moves over the list of polygons one more time, this
time searching for the vertex which is closest to the previously discovered average.  The polygon which owns
that particular vertex, is the polygon which is used to split the scene.

Reason (Polygon picking):
The goal of the above implementation is to divide space into halves as much as possible.  It does a decent job
of finding where the middle of polygons are at.  In retrospect, I could have used the median as a better
splitter, however that would have required additional data structures and more memory... the average seems to 
be "good enough" at a depth of 14.