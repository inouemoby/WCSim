#!/bin/bash

source env_sukap_ROOT6.sh

#/usr/local/cmake-3.18.1/bin/cmake --version 
/usr/local/cmake-3.26.0/build/bin/cmake --version 

wcsim_name=${PWD##*/}
#branch_name=$(git rev-parse --abbrev-ref HEAD)
branch_name="IWCD/develop"   

wcsim_directory=${PWD}
build_directory=${wcsim_directory}/../${wcsim_name}-build/ROOT6/${branch_name} 

if [ ! -d ${build_directory} ]; then
	
	# Clean G4
	if [ -d ${G4WORKDIR} ]; then
		rm -r ${G4WORKDIR}
	fi
	rm *.o *.a *.so *~ */*~ *Dict*
	
	echo "Creating build directory ${build_directory}"
	mkdir -p ${build_directory}
	
	cd ${build_directory}
	/usr/local/cmake-3.26.0/build/bin/cmake -DCMAKE_PREFIX_PATH=${G4INSTALLDIR} ${wcsim_directory}
	
	cp -r ${wcsim_directory}/sample-root-scripts ${build_directory}/.
	cp -r ${wcsim_directory}/macros ${build_directory}/.    
	cp ${wcsim_directory}/WCSim.mac ${build_directory}/src/.  
	cp ${wcsim_directory}/tuningNominal.mac ${build_directory}/src/. 
	mkdir ${wcsim_directory}/rootfile

else 
	cd ${build_directory}
fi


if [ -d ${build_directory} ]; then
	make clean
	make -j7
	
	cd ${wcsim_directory}
fi	
