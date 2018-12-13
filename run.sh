#docker-machine create --driver virtualbox FtPingMachine;
eval $(docker-machine env FtPingMachine);
docker build -t ping-build .;
docker stop ping-run;
docker rm ping-run;
docker run --name ping-run -it ping-build;
