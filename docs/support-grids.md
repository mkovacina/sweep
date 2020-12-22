# Support Grids

## Initialization Methods

### Uniform

Initialize all grid locations to the same value.

### File

File-based initialization is used to initialize a support grid from an external file.

The file can contain a list of values, whitespace delimited, and will be written to the support grid in a row by row manner.

### Random

Random initialization initializes a support grid with a random double value in the specified range.

### Computed

Not yet implemented.

### Distributed

Distributed initialization initializes a support grid by randomly assigning to each grid location a randomly selected element from a specified list of values.

### Proportioned

Proportioned initialization initializes a support grid by distribuing a given set of values accross the support grind in the specified proportions.

### Toss

Toss initialization initializes a support grid by randomly placing a set of values.

### Restricted Toss

Restricted toss initialization initializes a support grid by randomly placing a set number of values in a region within the difference between two regions.  

## Update Methods

### Static

The static update method is used when no updates to the support grid are required after an iteration.

### Clear

The clear update method is used to clear the support grid after each iteration.

### Movement

The movement update is used to shift the posiotion support grid values in a specified direction.

### Diffusion

The diffusion update is used to apply diffusion and decay methods to support grid values.


