#! /bin/bash

# +++kingshuk : in dockerfile adduser uses bash as shell. so changing the shell to bash from #!/bin/sh


# This file exists to document the proper way to initialize autotools,
# and so that those used to the presence of bootstrap.sh or autogen.sh
# will have an eaiser time.

exec autoreconf -is -Wall,no-override
