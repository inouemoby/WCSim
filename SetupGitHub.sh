#!/bin/bash

#Check if connected to https or ssh
IsHTTPS=$(git remote -v | grep "https" | wc -l)

if [ "${IsHTTPS}"  -gt 0 ] ; then
	echo "Adding https Upstream to inouemoby/WCSim"
	git remote add upstream https://github.com/inouemoby/WCSim
else
	echo "Adding ssh Upstream to inouemoby/WCSim"
	git remote add upstream git@github.com:inouemoby/WCSim.git
fi
	
