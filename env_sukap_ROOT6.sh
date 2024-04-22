#!/bin/bash

############################
# ROOT/CERN
############################
# Note: official ROOT 5.34 installation on sukap is not compatible with cmake
#  Temporary use Guillaume Pronost's version. Should change in the future.
#
#export ROOT_DIR=/home/pronost/software/root-5.34.38-build
# Note: There is no official ROOT 6 installation 
#  Temporary use Guillaume Pronost's version. Should change in the future.
export ROOT_DIR=/home/pronost/software/root-6.22.00-build
source ${ROOT_DIR}/bin/thisroot.sh

alias root='root -l'

############################
# GEANT4
############################

pwd=$PWD
GEANT4VERSION=geant4.10.05.p01
cd /usr/local/sklib_gcc8/geant4.10.05.p01/bin/
source geant4.sh
cd /usr/local/sklib_gcc8/geant4.10.05.p01/share/Geant4-10.5.1/geant4make/
source geant4make.sh
echo "Connect a PATH of Geant4.10"
cd "$pwd"

############################
# WCSim
############################
export BRANCH_NAME=$(git rev-parse --abbrev-ref HEAD)
export WCSIMDIR=$(pwd)-build/ROOT6/$BRANCH_NAME
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WCSIMDIR

