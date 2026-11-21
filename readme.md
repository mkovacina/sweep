# sweep

`sweep` is a C engine for running agent-based swarm simulations. Agent
behavior is driven by finite state automata (FSAs) rather than hardcoded
logic, and agents sense/modify shared "support grids" (fields the whole
swarm reads from and writes to, similar to stigmergy/pheromone grids).
This lets a single engine run many different emergent-behavior
experiments — Conway's Game of Life, flocking/leader-following, auctions,
bridge-building, beacon signaling, and more — purely by swapping out
config files.

## Layout

- `src/` — the C engine (agents, FSAs, support grids, swarm update loop,
  display/trace, unit tests).
- `exp/` — one `.exp` pointer file per experiment, listing the config
  files that define it (`.fsa`, `.agt`, `.swm`, `.lst`, `.sgd`).
- `exps/` — the actual experiment definitions referenced by `exp/`, one
  subdirectory per experiment (e.g. `exps/life/`).
- `docs/` — notes on the experiment file format and support grid
  initialization/update methods.
- `viewerjs/` — a small browser-based viewer for animation output.
- `animations/` — pre-recorded `.ani.gz` animation captures.
- `.devcontainer/` — Docker-based dev environment (see below).

## Running an experiment

Build and run an experiment with the top-level makefile, e.g. Conway's
Game of Life:

```sh
make life
```

This builds `src/` for the `life` experiment and runs
`./sweep exp/life.exp 1234` (the trailing number seeds the RNG).

To build and run a different experiment directly:

```sh
./sweep exp/<experiment>.exp <seed>
```

or use the `run` helper, which rebuilds `src/` for the named experiment
first:

```sh
./run <experiment> <seed>
```

## Running tests

```sh
make build-tests
make run-tests
```

This builds and runs `sweep-tests` (unit tests for the FSA and
string-util modules).

## Checking for memory issues

```sh
./run_valgrind
```

Rebuilds the `life` experiment and runs it under `valgrind`
(`--leak-check=full --track-origins=yes`), writing results to
`valgrind.txt`.

## Visualizing output

Running an experiment can produce a `swarm_sim.dat` animation file.
Open `viewerjs/index.html` in a browser and load that file to step
through the simulation on a canvas.

## Cleaning up

```sh
make clean
```

## Developing with containers

### Using Visual Studio Code

Install the `Remote - Containers` extension for VS Code.

Open the `sweep` folder in VS Code and it will autodetect the settings
and ask to relaunch the window with container support.

### Using the command line

Build the docker container (the dockerfile is in `.devcontainer/`):

```sh
docker build --tag sweep/shell .devcontainer
```

Launch the container:

```sh
docker run -it -v ${pwd}:/sweep sweep/shell
```
