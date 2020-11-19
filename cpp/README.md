# C++ Camera Drivers & Benchmarking

## Setting up Docker Environment

Use the provided Dockerfile to create a Docker image.
```bash
docker build -t maav-camera-driver . # Note the . at the end
```

Alternatively, download the Docker image, and use this command to install it.
```bash
docker load < maav-camera-driver.tar.gz

# Save a loaded Docker image to a tarball:
docker save maav-camera-driver | gzip > maav-camera-driver.tar.gz
```

Once you have the Docker image built / installed, spin up a Docker container
```bash
docker run --device=/dev -it -v `pwd`:/source --rm --name maav-cv maav-camera-driver
```

You should be inside the Docker container with a Bash terminal. From here, you
can use the Makefile to build the driver you would like to benchmark, then link
with the benchmarking code.
