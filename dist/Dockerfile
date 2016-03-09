FROM milleniumstudio/drone-clang-mariadb-ace:latest
MAINTAINER Millenium Studio <contact@millenium-studio.fr>

ENV DEBIAN_FRONTEND noninteractive

# Add missing packages to get worldserver running
RUN apt-get update && \
    apt-get install -y \
    screen

# Prepare directories for worldserver data
RUN mkdir /usr/local/etc/firestorm /usr/local/lib/firestorm /opt/firestorm /var/log/firestorm

# Declare volumes to share files between host and container (configuration file, logs, dbc, maps ....)
VOLUME /usr/local/etc/firestorm /opt/firestorm /var/log/firestorm

# Add compiled bins and libs into the image
ADD bin/* /usr/local/bin/
#ADD lib/* /usr/local/lib/

#ENTRYPOINT ["screen", "-dmS", "worldserver", "worldserver_restarter"]
