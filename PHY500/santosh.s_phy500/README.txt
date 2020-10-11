Name: Santosh Shedbalkar
Project: PHY500 Numerical Integration
-------------------------------------------------------------------------------

1. The Project contains 3 scenes. Planets can be added to the third scene 
by the user.

2. When the program runs, its loads the first scene.

Press: n to go to the next scene.

3. First Scene: It has a sun and 6 planets 
revolving around the sun, all using the explicit integrator.

Press: 1 to change to Explicit integrator
	   2 to change to Semi explicit integrator
	   3 to change to Verlet integrator
	   4 to change to RK4 integrator

4. Second scene: It has a sun and 4 planets
revolving around the sun. All using different integrators.

No Controls.

5. Third scene: It has a sun and 12 planets
revolving around the sun. 3 using Explicit, 3 using semi explicit,
3 using verlet and the remaining 3 using RK4 integrators

Press: 1 to add a new planet which uses explicit integrator
	   2 to add a new planet which uses semi explicit integrator
	   3 to add a new planet which uses verlet integrator
	   4 to add a new planet which uses RK4 integrator
-------------------------------------------------------------------------------

Damping used:
Explicit:		0.99
Semi explicit:	0.99
Verlet:			0.99
RK4:			0.99
-------------------------------------------------------------------------------

Integration methods are implemented in Integration.h and Integration.cpp