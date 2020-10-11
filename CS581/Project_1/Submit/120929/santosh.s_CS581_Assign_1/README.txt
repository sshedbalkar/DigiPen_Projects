santosh Shedbalkar
Assignment 1

Design:
The algorithm used to implement vacuum cleaner basically uses DFS.
The agent starts by exploring a possible place(child) to go from the current location. If it finds one, it will go there 
and will keep doing the same until there is no child available from the current node. In that case the agent will go to its 
parent and look for another child. This process will happen until there is no parent to go to. At that point the agent will 
shutoff.

The agent has a local structure (std::map) where it keeps all the nodes it hass visited. When the agent moves to a new node 
it does the following:

1. If the node has dirt the agent will suck it.
2. If the agent bumped to a wall it will check if it can go to any other node(child) from the previous node, that has not been visited so far.
3. If none of the above happened the node will check if it can go to any other node(child) from that node, that has not been visited before 

Every time the agent moves to a new node it marks that node as visited so the agent won't have to go to that node again unless it 
needs to go back to the parent node. When the agent checks all the children and finds no child that has not been visited, 
it will go to its parent. When the node moves to a node that has no children and no parent, the agent will shutoff. 