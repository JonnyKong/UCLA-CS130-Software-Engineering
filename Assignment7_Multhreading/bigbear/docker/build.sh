# This file build the docker conainter from docker/base.Dockerfile
docker build -f docker/base.Dockerfile -t bigbear:base .
docker build -f docker/Dockerfile -t my_image .
