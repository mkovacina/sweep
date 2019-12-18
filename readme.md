# setting up the dev environment

## build the docker container

`docker build --tag sweep/shell shell`

## launch the container 

`docker run -it -v ${pwd}:/sweep sweep/shell`
