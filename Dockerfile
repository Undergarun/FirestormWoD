FROM debian:latest
MAINTAINER Millenium Studio <contact@millenium-studio.fr>

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && \
    apt-get install -y \
    screen

RUN mkdir /opt/firestorm
RUN mkdir /var/log/firestorm

VOLUME /usr/local/etc
VOLUME /opt/firestorm
VOLUME /var/log/firestorm

RUN df -h

ADD dist/bin/worldserver_* /usr/local/bin
ADD dist/bin/worldserver_restarter /usr/local/bin

ENTRYPOINT ["screen", "-dmS", "worldserver", "worldserver_restarter"]
