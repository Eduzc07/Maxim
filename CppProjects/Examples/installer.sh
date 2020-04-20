#!/bin/bash
# Installer for all Examples Programs
# Check if build folder Exist
##--------------------------------------------------------
# Build Folder
##--------------------------------------------------------
buildfolder="build/Examples"
if [ -d "../${buildfolder}" ]
then
	echo "Directory ../${buildfolder} exists."
else
	echo "Directory ../${buildfolder} does not exists."
	mkdir "../${buildfolder}"
	echo "../${buildfolder} has been created."
fi

##--------------------------------------------------------
# bin Folder
##--------------------------------------------------------
binFolder="${buildfolder}/bin"
if [ -d "../${binFolder}" ]
then
	echo "Directory ../${binFolder} exists."
else
	echo "Directory ../${binFolder} does not exists."
	mkdir "../${binFolder}"
	echo "../${binFolder} has been created."
fi

## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## boost Examples
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

##--------------------------------------------------------
# boost
##--------------------------------------------------------
boostFolder="${buildfolder}/boost"
if [ -d "../${boostFolder}" ]
then
	echo "Directory ../${boostFolder} exists."
else
	echo "Directory ../${boostFolder} does not exists."
	mkdir "../${boostFolder}"
	echo "../${boostFolder} has been created."
fi

##--------------------------------------------------------
# boost Bin
##--------------------------------------------------------
boostBinFolder="${binFolder}/boost"
if [ -d "../${boostBinFolder}" ]
then
	echo "Directory ../${boostBinFolder} exists."
else
	echo "Directory ../${boostBinFolder} does not exists."
	mkdir "../${boostBinFolder}"
	echo "../${boostBinFolder} has been created."
fi

##--------------------------------------------------------
# boost - test_1
##--------------------------------------------------------
test="test_1"
boostFolder="${buildfolder}/boost/${test}"
boostCmake="boost/${test}"

if [ -d "../${boostFolder}" ]
then
	echo "Directory ../${boostFolder} exists."
else
	echo "Directory ../${boostFolder} does not exists."
	mkdir "../${boostFolder}"
	echo "../${boostFolder} has been created."
fi

#Run Cmake
cmake -S ${boostCmake} -B "../${boostFolder}"

#Run Make
make -j2 -C "../${boostFolder}" --no-print-directory

#Move to bin
cp "../${boostFolder}/${test}" "../${boostBinFolder}"

##--------------------------------------------------------
# boost - test_1
##--------------------------------------------------------
test="test_2"
boostFolder="${buildfolder}/boost/${test}"
boostCmake="boost/${test}"

if [ -d "../${boostFolder}" ]
then
	echo "Directory ../${boostFolder} exists."
else
	echo "Directory ../${boostFolder} does not exists."
	mkdir "../${boostFolder}"
	echo "../${boostFolder} has been created."
fi

#Run Cmake
cmake -S ${boostCmake} -B "../${boostFolder}"

#Run Make
make -j2 -C "../${boostFolder}" --no-print-directory

#Move to bin
cp "../${boostFolder}/${test}" "../${boostBinFolder}"
cp "${boostCmake}/cusco.jpg" "../${boostBinFolder}"

##--------------------------------------------------------
# boost - DayTime -Client
##--------------------------------------------------------
test="Daytime/Client"
boostFolder="${buildfolder}/boost/${test}"
boostCmake="boost/${test}"

if [ -d "../${boostFolder}" ]
then
	echo "Directory ../${boostFolder} exists."
else
	echo "Directory ../${boostFolder} does not exists."
	mkdir "../${boostFolder}"
	echo "../${boostFolder} has been created."
fi

#Run Cmake
cmake -S ${boostCmake} -B "../${boostFolder}" 

#Run Make
make -j2 -C "../${boostFolder}" --no-print-directory

#Move to bin
cp "../${boostFolder}/Client" "../${boostBinFolder}"

##--------------------------------------------------------
# boost - DayTime - Server
##--------------------------------------------------------
test="Daytime/Server"
boostFolder="${buildfolder}/boost/${test}"
boostCmake="boost/${test}"

if [ -d "../${boostFolder}" ]
then
	echo "Directory ../${boostFolder} exists."
else
	echo "Directory ../${boostFolder} does not exists."
	mkdir "../${boostFolder}"
	echo "../${boostFolder} has been created."
fi

#Run Cmake
cmake -S ${boostCmake} -B "../${boostFolder}" 

#Run Make
make -j2 -C "../${boostFolder}" --no-print-directory

#Move to bin
cp "../${boostFolder}/Server" "../${boostBinFolder}"

## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## CUDA test
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

##--------------------------------------------------------
# CUDA test
##--------------------------------------------------------
projectFolder="${buildfolder}/CUDA_test"
if [ -d "../${projectFolder}" ]
then
	echo "Directory ../${projectFolder} exists."
else
	echo "Directory ../${projectFolder} does not exists."
	mkdir "../${projectFolder}"
	echo "../${projectFolder} has been created."
fi

##--------------------------------------------------------
# CUDA test src
##--------------------------------------------------------
test="Cuda_Test"
projectCmake="CUDA/cmake/src"

#Run Cmake
cmake -S ${projectCmake} -B "../${projectFolder}"

#Run Make
make -j2 -C "../${projectFolder}" --no-print-directory

#Move to bin
cp "../${projectFolder}/${test}" "../${binFolder}"
cp "CUDA/install/cusco.jpg" "../${binFolder}"

## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## Digit
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

##--------------------------------------------------------
# Digit
##--------------------------------------------------------
projectFolder="${buildfolder}/digit"
if [ -d "../${projectFolder}" ]
then
	echo "Directory ../${projectFolder} exists."
else
	echo "Directory ../${projectFolder} does not exists."
	mkdir "../${projectFolder}"
	echo "../${projectFolder} has been created."
fi

##--------------------------------------------------------
# Digit src
##--------------------------------------------------------
test="Digit"
projectCmake="digit/cmake/src"

#Run Cmake
cmake -S ${projectCmake} -B "../${projectFolder}"

#Run Make
make -j2 -C "../${projectFolder}" --no-print-directory

#Move to bin
cp "../${projectFolder}/${test}" "../${binFolder}"
cp -r "digit/install/images" "../${binFolder}"
cp -r "digit/install/images" "../${projectFolder}"

## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## Eigen Examples
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

##--------------------------------------------------------
# Eigen
##--------------------------------------------------------
eigenFolder="${buildfolder}/Eigen"
if [ -d "../${eigenFolder}" ]
then
	echo "Directory ../${eigenFolder} exists."
else
	echo "Directory ../${eigenFolder} does not exists."
	mkdir "../${eigenFolder}"
	echo "../${eigenFolder} has been created."
fi

##--------------------------------------------------------
# Eigen
##--------------------------------------------------------
eigenBinFolder="${binFolder}/Eigen"
if [ -d "../${eigenBinFolder}" ]
then
	echo "Directory ../${eigenBinFolder} exists."
else
	echo "Directory ../${eigenBinFolder} does not exists."
	mkdir "../${eigenBinFolder}"
	echo "../${eigenBinFolder} has been created."
fi

##--------------------------------------------------------
# Eigen - test
##--------------------------------------------------------
test="test_eigen"
eigenFolder="${eigenFolder}/${test}"
eigenCmake="Eigen"

if [ -d "../${eigenFolder}" ]
then
	echo "Directory ../${eigenFolder} exists."
else
	echo "Directory ../${eigenFolder} does not exists."
	mkdir "../${eigenFolder}"
	echo "../${eigenFolder} has been created."
fi

#Run Cmake
cmake -S ${eigenCmake} -B "../${eigenFolder}"

#Run Make
make -j2 -C "../${eigenFolder}" --no-print-directory

#Move to bin
cp "../${eigenFolder}/${test}" "../${eigenBinFolder}"

## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## PCA Examples
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

##--------------------------------------------------------
# PCA
##--------------------------------------------------------
pcaFolder="${buildfolder}/PCA"
if [ -d "../${pcaFolder}" ]
then
	echo "Directory ../${pcaFolder} exists."
else
	echo "Directory ../${pcaFolder} does not exists."
	mkdir "../${pcaFolder}"
	echo "../${pcaFolder} has been created."
fi

##--------------------------------------------------------
# PCA Bin
##--------------------------------------------------------
pcaBinFolder="${binFolder}/PCA"
if [ -d "../${pcaBinFolder}" ]
then
	echo "Directory ../${pcaBinFolder} exists."
else
	echo "Directory ../${pcaBinFolder} does not exists."
	mkdir "../${pcaBinFolder}"
	echo "../${pcaBinFolder} has been created."
fi

##--------------------------------------------------------
# PCA - test_1
##--------------------------------------------------------
test="test_1"
projectFolder="${buildfolder}/PCA/${test}"
projectCmake="PCA/${test}/cmake"

if [ -d "../${projectFolder}" ]
then
	echo "Directory ../${projectFolder} exists."
else
	echo "Directory ../${projectFolder} does not exists."
	mkdir "../${projectFolder}"
	echo "../${projectFolder} has been created."
fi

#Run Cmake
cmake -S ${projectCmake} -B "../${projectFolder}"

#Run Make
make -j2 -C "../${projectFolder}" --no-print-directory

#Move to bin
cp "../${projectFolder}/${test}" "../${pcaBinFolder}"
find "PCA/${test}/install/" -iname "*.jpg" -type f -exec cp {} "../${pcaBinFolder}" 
find "PCA/${test}/install/" -iname "*.jpg" -type f -exec cp {} "../${projectFolder}"

##--------------------------------------------------------
# PCA - test_2
##--------------------------------------------------------
test="test_2"
projectFolder="${buildfolder}/PCA/${test}"
projectCmake="PCA/${test}/cmake"

if [ -d "../${projectFolder}" ]
then
	echo "Directory ../${projectFolder} exists."
else
	echo "Directory ../${projectFolder} does not exists."
	mkdir "../${projectFolder}"
	echo "../${projectFolder} has been created."
fi

#Run Cmake
cmake -S ${projectCmake} -B "../${projectFolder}"

#Run Make
make -j2 -C "../${projectFolder}" --no-print-directory

#Move to bin
cp "../${projectFolder}/${test}" "../${pcaBinFolder}"
cp -r "PCA/${test}/install/Bilden" "../${projectFolder}"
cp -r "PCA/${test}/install/Bilden" "../${pcaBinFolder}"

##--------------------------------------------------------
# PCA - test_3
##--------------------------------------------------------
test="test_3"
projectFolder="${buildfolder}/PCA/${test}"
projectCmake="PCA/${test}/cmake"

if [ -d "../${projectFolder}" ]
then
	echo "Directory ../${projectFolder} exists."
else
	echo "Directory ../${projectFolder} does not exists."
	mkdir "../${projectFolder}"
	echo "../${projectFolder} has been created."
fi

#Run Cmake
cmake -S ${projectCmake} -B "../${projectFolder}"

#Run Make
make -j2 -C "../${projectFolder}" --no-print-directory

#Move to bin
cp "../${projectFolder}/${test}" "../${pcaBinFolder}"
cp -r "PCA/${test}/install/Bilden" "../${projectFolder}"
cp -r "PCA/${test}/install/Bilden" "../${pcaBinFolder}"

##--------------------------------------------------------
# PCA - test_4_face
##--------------------------------------------------------
test="test_4_face"
projectFolder="${buildfolder}/PCA/${test}"
projectCmake="PCA/${test}/cmake"

if [ -d "../${projectFolder}" ]
then
	echo "Directory ../${projectFolder} exists."
else
	echo "Directory ../${projectFolder} does not exists."
	mkdir "../${projectFolder}"
	echo "../${projectFolder} has been created."
fi

#Run Cmake
cmake -S ${projectCmake} -B "../${projectFolder}"

#Run Make
make -j2 -C "../${projectFolder}" --no-print-directory

#Move to bin
cp "../${projectFolder}/${test}" "../${pcaBinFolder}"
cp "PCA/${test}/install/test.txt" "../${pcaBinFolder}"
cp "PCA/${test}/install/test.txt" "../${projectFolder}"
tar -xvf "PCA/${test}/install/att_faces.tar" -C "../${pcaBinFolder}"
tar -xvf "PCA/${test}/install/att_faces.tar" -C "../${projectFolder}"
#cp -r "PCA/${test}/install/Bilden" "../${pcaBinFolder}"

## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## PCL
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

##--------------------------------------------------------
# PCL
##--------------------------------------------------------
projectFolder="${buildfolder}/PCL"
if [ -d "../${projectFolder}" ]
then
	echo "Directory ../${projectFolder} exists."
else
	echo "Directory ../${projectFolder} does not exists."
	mkdir "../${projectFolder}"
	echo "../${projectFolder} has been created."
fi

##--------------------------------------------------------
# PCL src
##--------------------------------------------------------
test="pcd_write"
projectCmake="PCL"

#Run Cmake
cmake -S ${projectCmake} -B "../${projectFolder}"

#Run Make
make -j2 -C "../${projectFolder}" --no-print-directory

#Move to bin
cp "../${projectFolder}/${test}" "../${binFolder}"

## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## Points classifier
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

##--------------------------------------------------------
# Points classifier
##--------------------------------------------------------
projectFolder="${buildfolder}/Points_classifier"
if [ -d "../${projectFolder}" ]
then
	echo "Directory ../${projectFolder} exists."
else
	echo "Directory ../${projectFolder} does not exists."
	mkdir "../${projectFolder}"
	echo "../${projectFolder} has been created."
fi

##--------------------------------------------------------
# Points classifier src
##--------------------------------------------------------
test="Points_classifier"
projectCmake="${test}/cmake"

#Run Cmake
cmake -S ${projectCmake} -B "../${projectFolder}"

#Run Make
make -j2 -C "../${projectFolder}" --no-print-directory

#Move to bin
cp "../${projectFolder}/${test}" "../${binFolder}"

## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## TestPen
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

##--------------------------------------------------------
# TestPen
##--------------------------------------------------------
projectFolder="${buildfolder}/test_Pen"
if [ -d "../${projectFolder}" ]
then
	echo "Directory ../${projectFolder} exists."
else
	echo "Directory ../${projectFolder} does not exists."
	mkdir "../${projectFolder}"
	echo "../${projectFolder} has been created."
fi

##--------------------------------------------------------
# TestPen src
##--------------------------------------------------------
test="test_Pen"
projectCmake="${test}/cmake/src"

#Run Cmake
cmake -S ${projectCmake} -B "../${projectFolder}"

#Run Make
make -j2 -C "../${projectFolder}" --no-print-directory

#Move to bin
cp "../${projectFolder}/${test}" "../${binFolder}"
cp -r "${test}/install/images" "../${binFolder}"
cp -r "${test}/install/images" "../${projectFolder}"


