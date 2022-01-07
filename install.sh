#!/bin/sh
echo "removing old SOEM"
rm -rf SOEM
echo "removed the old version of SOEM, downloading new version"
git clone https://github.com/BeornOG/SOEM
echo "downloaded the repository, using cmake to build included code to make the makefile"
mkdir SOEM/build
cd SOEM/build
cmake ..
echo "cmake complete, building main.cpp"
cd master
make
echo "make finished"