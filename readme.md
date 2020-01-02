# developing with containers

## using visual studio code

Install the `Remote - Containers` extension for VS Code.

Open the `sweep` folder in VS Code and it will autodetect the settings and ask to relaunch the window with container support.

## using the command line

### build the docker container

The dockerfile is in the .devcontainer folder

`docker build --tag sweep/shell shell`

### launch the container 

`docker run -it -v ${pwd}:/sweep sweep/shell`
