FROM ubuntu:21.10
#--platform=linux/amd64 
RUN apt update && DEBIAN_FRONTEND='noninteractive' apt install -y clang make libkqueue-dev python3-pip
