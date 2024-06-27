#!/bin/bash

dir_lib='/usr/local/lib'
dir_include='/usr/local/include'
dir_range='/usr/local/share/range_fit'
dir_range_org='/Users/hayakawa/calc/srim/SRIM-2013/SR Module/range'
dir_macros='/usr/local/macros'

g++ -std=c++20 -c Mass.cc -fPIC 
g++ -std=c++20 -shared -o libmass.so Mass.o
sudo rsync -auv libmass.so $dir_lib/
sudo rsync -auv Mass.h $dir_include/

$ROOTSYS/bin/rootcling -f TSrimDict.cc -c TSrim.h LinkDef.h
g++ `$ROOTSYS/bin/root-config --cflags --libs` -shared TSrimDict.cc TSrim.cc -o libtsrim.so -L$dir_lib -lmass

sudo rsync -auv libtsrim.so TSrimDict_rdict.pcm $dir_lib/
sudo rsync -auv TSrim.h $dir_include/
sudo rsync -auv "$dir_range_org"/*/range_fit_pol16* $dir_range/
sudo rsync -auv macros/load_srim.C macros/srim_all.C $dir_macros/

