### Pull latest image

The image is what our containers are based from.

```
docker pull milleniumstudio/docker-wod:latest-ptr
```

Tags available : `latest` and `latest-ptr`


### Run a container aka a realm

```
docker run \
    --name ptr_623
    -v /home/ptr_623/etc:/usr/local/etc/firestorm \
    -v /home/ptr_623/shared:/opt/firestorm \
    -v /home/ptr_623/logs:/var/logs/firestorm \
    -p 8088:8085/tcp \
    milleniumstudio/docker-wod:latest-ptr
```

`-v` Mount a directory on the machine in the container (Format: `/path/on/the/machine:/path/on/the/container`)

`-p` Redirect a port on the machine to another in the container

Note: When you launch a container, the restarter is always started


### Stop a container aka a realm

```
docker stop ptr_623
```

If there is no name:

```
docker stop HASH
```

### Delete a container

```
docker rm ptr_623
```

If there is no name:

```
docker rm HASH
```

- Tip: Delete unused containers

```
docker rm `docker ps --no-trunc -aq`
```
