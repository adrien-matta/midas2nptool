# midas2nptool
A conversion package for data file from MIDAS (Daresbury) to nptool

Pre-requisites: 
NPTool 
checkout http://nptool.org/manual/ for installation 

Install:
$ cd /path/to/the/packages/diretory
$ git clone https://github.com/adrien-matta/midas2nptool
$ cd ~/midas2nptool 
$ source midas2nptool.sh

from here two ways to build
$ cmake -GNinja ./
$ make -jn install (e.g. make -j2 install, n is number of cores)

or if google Ninja is installed (faster build): 
$ cmake -GNinja ./
$ ninja install

for how to execute
$ midas2nptool -h
