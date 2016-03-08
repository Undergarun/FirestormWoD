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

# Check space left on /
RUN df -h

ADD bin/* /usr/local/bin/
ADD lib/* /usr/local/lib/

#ENTRYPOINT ["screen", "-dmS", "worldserver", "worldserver_restarter"]
