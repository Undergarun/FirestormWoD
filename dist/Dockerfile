FROM milleniumstudio/drone-clang-mariadb-ace:latest
MAINTAINER Millenium Studio <contact@millenium-studio.fr>

RUN apt-get install -y \
    gdb \
    curl

# Add compiled bins and some data into the container
ADD bin /usr/local/bin/
ADD etc /usr/local/etc/
ADD sql /usr/local/share/firestorm/sql/

# Declare volumes to share files between host and container (configuration file, logs, dbc, maps ....)
VOLUME /usr/local/etc/firestorm /usr/local/opt/firestorm /usr/local/var/log/firestorm

ENTRYPOINT ["run-worldserver"]
