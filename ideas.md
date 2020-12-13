allow for dynamically sized support grids
- maybe specify an environment variable instead of a size
- maybe provide a default size if no environment variable present

setup indent to reformat all the source files

replace usages of string functions with secure versions

update agent placement methods with better names

separate out the reading of config from the application of configuration
- promotes better testability
- will allow for situations where the size of the grid is needed to place agents, for example
- ok, it is there via GGETC/GGETR but it is global...can this be better?

add in a way such that if an FSA doesn't find a transition then it just stays in the current state

add in a way such that if an FSA doesn't find a transition then it goes to a default state
