FROM milleniumstudio/drone-clang-mariadb-ace:latest
MAINTAINER Millenium Studio <contact@millenium-studio.fr>

RUN apt-get update && \
    apt-get install -y \
    gdb \
    curl

# Prepare directories for worldserver data
RUN mkdir -p /usr/local/lib/firestorm \
             /usr/local/etc/firestorm \
             /usr/local/opt/firestorm \
             /usr/local/var/log/firestorm/crashlogs

# Add compiled bins and libs into the container
ADD bin/* /usr/local/bin/
#ADD lib/* /usr/local/lib/firestorm
ADD etc/* /usr/local/etc/

# Declare volumes to share files between host and container (configuration file, logs, dbc, maps ....)
VOLUME /usr/local/etc/firestorm /usr/local/opt/firestorm /usr/local/var/log/firestorm

# Open the port use by the worldserver on the container
EXPOSE 8085

#ENTRYPOINT ["screen", "-dmS", "worldserver", "run-worldserver"]
