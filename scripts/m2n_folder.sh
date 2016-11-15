#!/bin/bash

if [ $# -ne 4 ]; then
  printf "Wrong number of argument: usage is m2n_folder configuration.txt path/to/midas/ path/to/root treename\n"
else
  for midas in $2*
  do
    filename=$(basename "$midas")
    extension="${filename##*.}"
    printf "$extension"
    if [ $extension == "gz" ]; then
      gzip -d $2/$filename
      filename="${filename%.*}"
    else
      filename="${filename%.*}"
    fi

    root="$3$filename.root"
    if [ -f $root ] ; then
      printf "**** NOTE : File $root already exist, skipping conversion ****\n"
    else
        midas2nptool $1 $midas $root $4
    fi
  done
fi
