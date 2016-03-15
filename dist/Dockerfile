FROM debian:jessie
MAINTAINER Millenium Studio <contact@millenium-studio.fr>

ENV DEBIAN_FRONTEND noninteractive

# Add MariaDB repository
RUN apt-key adv --recv-keys --keyserver keyserver.ubuntu.com 0xcbcb082a1bb943db
RUN echo 'deb [arch=amd64,i386] http://ftp.igh.cnrs.fr/pub/mariadb/repo/10.1/debian jessie main' > /etc/apt/sources.list.d/mariadb.list

# Add missing packages to get worldserver running
RUN apt-get update && \
    apt-get install -y \
    libace-dev \
    libmariadbclient-dev \
    zlib1g-dev \
    libcurl4-openssl-dev \
#    libssl-dev \ # Installed with libcurl
    screen \
    gdb

# Prepare directories for worldserver data
RUN mkdir -p /usr/local/etc/firestorm /usr/local/lib/firestorm /opt/firestorm /var/log/firestorm/crashlogs

# Declare volumes to share files between host and container (configuration file, logs, dbc, maps ....)
VOLUME /usr/local/etc/firestorm /opt/firestorm /var/log/firestorm

# Add compiled bins and libs into the image
ADD bin/* /usr/local/bin/
#ADD lib/* /usr/local/lib/

# Open the port use by the worldserver on the container
EXPOSE 8085

#ENTRYPOINT ["screen", "-dmS", "worldserver", "run-worldserver"]
