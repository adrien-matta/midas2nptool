#!/bin/bash

# find script path
if [ -n "$ZSH_VERSION" ]; then
   SCRIPTPATH="$( cd "$( dirname "${(%):-%x}" )" && pwd )"
elif [ -n "$BASH_VERSION" ]; then
   SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
else
   echo "neither bash or zsh is used, abort"
   exit 1
fi

# export NPTOOL environment variable
export M2N=$SCRIPTPATH

M2NARCH=$(uname)
# mac os x case
if [ "${M2NARCH}" = "Darwin" ] ; 
then
  export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$M2N/lib
else 
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$M2N/lib
fi

export PATH=$PATH:$M2N/bin
