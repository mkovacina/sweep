This will be a basic implemention of Conway's Game Of Life.

The agents will additively write '1' to the 'neighbor' support grid in their Moore neighboorhood.  Then each agent will look at the value in the 'neighbor' support grid at their location and use that to drive their internal state.

The support grids will be of size 'nAgents+1' squared to allow for a buffer fence around the perimeter to eliminate the need for bounds checking.  The grid will be initialized such that there will be exactly one agent on each square in the inner 'nAgent' squared center grid.
