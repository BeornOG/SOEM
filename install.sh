#!/bin/sh
RED='\033[0;31m'
NC='\033[0m' # No Colour
YELLOW ='\033[1;33m'
#make the message into a colour
echo -e "${RED}removing old SOEM${NC}"
rm -rf SOEM
echo "${YELLOW}removed the old version of SOEM, downloading new version"
git clone https://github.com/BeornOG/SOEM
echo "downloaded the repository, using cmake to build included code to make the makefile"
cp SOEM/install.sh install.sh #update the installer
mkdir SOEM/build
cd SOEM/build
cmake ..
echo "cmake complete, building main.cpp"
cd master
make
echo "make finished"