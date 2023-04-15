# ARG - Define build-time variable
# ADD - command helps in copying data (files, directory) into a Docker image. Additionally it lets you use URL location and unzip files into image.
# COPY - copies new files or directories from <src> and adds them to the filesystem of the container at the path <dest> in a Docker image
# CMD - allows you to set a default command which will be executed only when you run a container without 
#       specifying a command. There can be only one CMD.
# ENTRYPOINT - allows you to configure a container that will run as an executable. ENTRYPOINT command and parameters are not ignored when 
#              Docker container runs with command line parameters.. provides command and arguments for an executing container.
# EXPOSE - expose ports
# ENV - provides default values for aenviornment variables that can be accessed within the container
# FROM - specifies the base(parent) image. Alpine version is the minimal docker image based on base image 
# LABEL - provides metada like maintainer.
# MAINTAINER - declares the author field of the images
# PULL - Adds files from your Docker repository
# RUN -  runs a Linux command in the container. Used to install packages into container, create folders, etc
# USER - By default, containers run as root. A container running as root has full control of the host system. Docker image should use the 
#        USER instruction to specify a non-root user for containers to run as.
# VOLUME - create a directory mount point to access and store persistent data.
# WORKDIR - sets the working directory for the instructions that follow.

FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
ENV APT_KEY_DONT_WARN_ON_DANGEROUS_USAGE=DontWarn
# Update Ubuntu Software repository
RUN apt update && \
    apt-get install -y \
      git autoconf automake libtool make libreadline-dev texinfo \
      pkg-config libpam0g-dev libjson-c-dev bison flex python3-pip \
      libc-ares-dev python3-dev python3-sphinx \
      install-info build-essential libsnmp-dev perl \
      libcap-dev python2 libelf-dev libprotobuf-c-dev protobuf-c-compiler \
      sudo gdb curl iputils-ping time \
      lua5.3 liblua5.3-dev \
      net-tools iproute2 tcpdump traceroute && \
      curl https://bootstrap.pypa.io/pip/2.7/get-pip.py --output /tmp/get-pip.py && \
      python2 /tmp/get-pip.py && \
      rm -f  /tmp/get-pip.py && \
      python3 -m pip install wheel && \
      python3 -m pip install pytest && \
      python3 -m pip install pytest-xdist && \
      python3 -m pip install "scapy>=2.4.2" && \
      python3 -m pip install xmltodict && \
      python2 -m pip install 'exabgp<4.0.0'

RUN groupadd -r -g 92 frr && \
      groupadd -r -g 85 frrvty && \
      adduser --system --ingroup frr --home /home/frr \
              --gecos "FRR suite" --shell /bin/bash frr && \
      usermod -a -G frrvty frr && \
      useradd -d /var/run/exabgp/ -s /bin/false exabgp && \
      echo 'frr ALL = NOPASSWD: ALL' | tee /etc/sudoers.d/frr && \
      mkdir -p /home/frr && chown frr.frr /home/frr

#for libyang 2
RUN apt-get install -y cmake libpcre2-dev

USER frr:frr

# build and install libyang2
RUN cd && pwd && ls -al && \
    git clone https://github.com/CESNET/libyang.git && \
    cd libyang && \
    git checkout v2.0.0 && \
    mkdir build; cd build && \
    cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr \
          -DCMAKE_BUILD_TYPE:String="Release" .. && \
    make -j $(nproc) && \
    sudo make install

COPY --chown=frr:frr . /home/frr/frr/

RUN cd && ls -al && ls -al frr

RUN sudo mkdir /etc/frr
RUN sudo chown frr:frr /etc/frr
RUN cd ~/frr && \
    ./bootstrap.sh && \
    ./configure \
       --prefix=/usr \
       --localstatedir=/var/run/frr \
       --sbindir=/usr/lib/frr \
       --sysconfdir=/etc/frr \
       --enable-vtysh \
       --enable-pimd \
       --enable-sharpd \
       --enable-multipath=64 \
       --enable-user=frr \
       --enable-group=frr \
       --enable-vty-group=frrvty \
       --enable-snmp=agentx \
       --enable-scripting \
       --with-pkg-extra-version=-my-manual-build && \
    make -j $(nproc) && \
    sudo make install

RUN cd ~/frr && make check || true

# Own the config / PID files
RUN sudo mkdir -p /var/run/frr
RUN sudo chown -R frr:frr /var/run/frr

COPY docker/ubuntu20-ci/docker-start /usr/sbin/docker-start
COPY docker/ubuntu20-ci/vtysh.conf /etc/frr/vtysh.conf
ENTRYPOINT ["/usr/sbin/docker-start"]
