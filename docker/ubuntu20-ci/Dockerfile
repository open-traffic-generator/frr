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
RUN apt-get update
RUN apt-get install -y apt-utils 
RUN apt-get install -y \
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
#      openssh-server

# groupadd [options] group
# -r -- create a system group
# -g -- assign group id
RUN groupadd -r -g 92 frr && \
      groupadd -r -g 85 frrvty
# |----------------------------------------------------------------------------------------|
# | The key difference between adduser and useradd is that adduser is used to add users    |
# | with setting up account’s home folder and other settings while useradd is a low-level  |
# | utility command to add users.                                                          |
# | Adduser is the command to add users to the system according to the command line        |
# | options and configuration information in /etc/adduser.conf.                            |
# | The command adduser creates the user and sets up the account’s home folders and other  |
# | settings. The command useradd just creates the user.                                   |
# | The command adduser creates a user directory in the home (/home/user ) automatically.  |
# | The command useradd does not create a user directory in the home, if not specified     |
# | with -m.                                                                               |
# |----------------------------------------------------------------------------------------|

# adduser [options] LOGIN
#  --system -- adduser adds a system user
#  --ingroup -- To place the new system user in an already existing group
# --home DIR -- Use DIR as the user's home directory, rather than the default specified by the configuration file. If the directory does not exist, it is created and skeleton files are copied.
# --gecos GECOS -- Set the gecos field for the new entry generated. adduser will not ask for finger information if this option is given.
# --shell SHELL -- Use SHELL as the user's login shell, rather than the default specified by the configuration file.

#RUN adduser --system --ingroup frr --home /home/frr \
#              --gecos "FRR suite" --shell /bin/bash frr 

# Chaning the default shell to vtysh not bash 
RUN adduser --system --ingroup frr --home /home/frr \
              --gecos "FRR suite" --shell /usr/bin/vtysh frr

# usermod command to add a user to a group, change a user shell, login name, home directory,
# and more              
# To add an existing user to a secondary group, use the -a -G
# add user frr into frrvty group
RUN usermod -a -G frrvty frr 

# useradd - create a new user or update default new user information
# -d HOME_DIR The new user will be created using HOME_DIR as the value for the user's login directory.
# -s  --  The name of the user's login shell
RUN useradd -d /var/run/exabgp/ -s /bin/false exabgp

RUN echo 'frr ALL = NOPASSWD: ALL' | tee /etc/sudoers.d/frr && \
      mkdir -p /home/frr && chown frr.frr /home/frr

#for libyang 2
RUN apt-get install -y cmake libpcre2-dev

USER frr:frr
#USER root:root
#RUN mkdir ~/frr

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


# +++kingshuk : In WSL facing error "/bin/sh: 1: ./bootstrap.sh: not found"
#               To avoid this I had pasted the content of btostrap.sh directly.
#               At the time of working (7-dec-2021) bootstarp only had the follwing line
#               exec autoreconf -is -Wall,no-override
#               Additionally I also enabled isisd and bgpd
#RUN cd ~/frr && \
#    exec autoreconf -is -Wall,no-override && \
#    ./configure \
#       --prefix=/usr \
#       --localstatedir=/var/run/frr \
#       --sbindir=/usr/lib/frr \
#       --sysconfdir=/etc/frr \
#       --enable-vtysh \
#       --enable-isisd \
#       --enable-bgpd \        
#       --enable-pimd \
#       --enable-sharpd \
#       --enable-multipath=64 \
#       --enable-user=frr \
#       --enable-group=frr \
#       --enable-vty-group=frrvty \
#       --enable-snmp=agentx \
#       --enable-scripting \
#       --with-pkg-extra-version=-my-manual-build && \
#    make -j $(nproc) && \
#    sudo make install

# Own the config / PID files
RUN sudo mkdir -p /var/run/frr
RUN sudo chown -R frr:frr /var/run/frr

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

#Additional components to be installed
RUN sudo apt-get -y install vlan
RUN sudo apt-get update
#RUN sudo apt install openssh-server
RUN sudo apt-get -y install openssh-server

COPY docker/ubuntu20-ci/docker-start /usr/sbin/docker-start
COPY docker/ubuntu20-ci/vtysh.conf /etc/frr/vtysh.conf
COPY docker/ubuntu20-ci/frr-daemon-conf.txt  /etc/frr/daemons
ENTRYPOINT ["/usr/sbin/docker-start"]
#CMD ["/usr/sbin/docker-start"]
