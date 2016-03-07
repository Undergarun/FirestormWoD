FROM debian:latest
MAINTAINER Millenium Studio <contact@millenium-studio.fr>

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && \
    apt-get install -y \
    screen

RUN mkdir -p /home/realm/{bin,lib,etc,data,logs,crashlogs}

WORKDIR /home/realm


ADD /home/realm/bin/worldserver_* /home/realm/bin
ADD contrib/deploy/realm_restarter /home/realm/bin

ENTRYPOINT ["screen", "-dmS", "realm", "./bin/realm_restarter"]
