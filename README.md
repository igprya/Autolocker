# Autolocker
Web-camera and your face-based authorization.

## Foreword
This is my frist C++ project in probably 10 years, so there's a high chance that it's anywhere from somewhat to catastrophically poorly written.
Please don't use it as an example of excellent C++ code. ;)

## Dependencies
1. OpenCV 3.4.1 built with *contrib* libraries
2. Visual Studio 15 2017 with C++ development environment

## How to build OpenCV
Before you're start: it's still not too late to turn back. Don't be surprised when you find yourself cursing at your PC early tommorrow's morning. You've been warned.

Now, with legal shenaningans out of the way, let's start with [OpenCV's own tutorial](https://docs.opencv.org/master/d3/d52/tutorial_windows_install.html). Take a look at the *Installation using Git-bash and cmake* section. PROTIP: you might want to provision and dedicate VM and slap VS 2017 Community into it before proceeding with building, things are going to get messy. 

Here's the copy of OpenCV's tutorial in case if it's no longer there.
1. Download and install *cmake* (3.9.1 or higher). Make sure to add cmake to PATH during the installation.
2. Install *git-bash* (2.14.1 or higher). **Do not** add git to PATH variable during the installation.
3. Run git-bash, you should see a command window.
4. In git-bash command window enter following commands (mind the path, at this point you're basically selecting where do you want OpenCV to be installed):
```
mkdir /c/lib
cd /c/lib
```
5. Copy the script below and save it into `C:\lib` under name `installOCV.sh`:
```
#!/bin/bash -e
myRepo=$(pwd)
CMAKE_CONFIG_GENERATOR="Visual Studio 15 2017 Win64"
if [  ! -d "$myRepo/opencv"  ]; then
    echo "clonning opencv"
    git clone https://github.com/opencv/opencv.git
    mkdir Build
    mkdir Build/opencv
    mkdir Install
    mkdir Install/opencv
else
    cd opencv
    git pull --rebase
    cd ..
fi
if [  ! -d "$myRepo/opencv_contrib"  ]; then
    echo "clonning opencv_contrib"
    git clone https://github.com/opencv/opencv_contrib.git
    # mkdir Build < TRY UNCOMMENTING THAT LINE IF THINGS ARE FAILING
    mkdir Build/opencv_contrib
else
    cd opencv_contrib
    git pull --rebase
    cd ..
fi
RepoSource=opencv
pushd Build/$RepoSource
CMAKE_OPTIONS='-DBUILD_PERF_TESTS:BOOL=OFF -DBUILD_TESTS:BOOL=OFF -DBUILD_DOCS:BOOL=OFF  -DWITH_CUDA:BOOL=OFF -DBUILD_EXAMPLES:BOOL=OFF -DINSTALL_CREATE_DISTRIB=ON'
cmake -G"$CMAKE_CONFIG_GENERATOR" $CMAKE_OPTIONS -DOPENCV_EXTRA_MODULES_PATH="$myRepo"/opencv_contrib/modules -DCMAKE_INSTALL_PREFIX="$myRepo"/install/"$RepoSource" "$myRepo/$RepoSource"
echo "************************* $Source_DIR -->debug"
cmake --build .  --config debug
echo "************************* $Source_DIR -->release"
cmake --build .  --config release
cmake --build .  --target install --config release
cmake --build .  --target install --config debug
popd
```
This script supposes that you use Visual Studio 2017 in 64 bits
```
CMAKE_CONFIG_GENERATOR="Visual Studio 15 2017 Win64"
```
and that the OpenCV will be envetually built  into `C:\lib\Install`:
```
-DCMAKE_INSTALL_PREFIX="$myRepo"/install/"$RepoSource" "$myRepo/$RepoSource"
```
with no performance tests, unit test, no documentation, no CUDA support and no examples:
```
CMAKE_OPTIONS='-DBUILD_PERF_TESTS:BOOL=OFF -DBUILD_TESTS:BOOL=OFF -DBUILD_DOCS:BOOL=OFF -DWITH_CUDA:BOOL=OFF -DBUILD_EXAMPLES:BOOL=OFF'
```
6. In git-bash command line enter the following command:
```
./installOCV.sh
```
7. Try to solve the errors. God help you.
8. Eventually you'll have the thing building, it will take a while.
9. (Not sure if required) Once Cmake finishes building the project, navigate into `C:\lib\Build`, open the .sln file and build the entire solution.

## How to install OpenCV
This section assumes that built the OpenCV with *contrib* stuff following the section above. 

Long story short, [Set the OpenCV enironment variable and add it to the system's path](https://docs.opencv.org/master/d3/d52/tutorial_windows_install.html#tutorial_windows_install_path).

Long story long,

1. Open *Environment variables* window (see System Properties)
2. Under *System variables* group, create an `OPENCV_DIR` variable and set it to the location of your OpenCV's *bin* and *lib* folders, something like that: `C:\Program Files (x86)\OpenCV\opencv\build\x64\vc15`
3. Under *System variables* group, find an existing `PATH` variable and click 'Edit'. In appeared dialog, add a new path with exactly the following value: `%OPENCV_DIR%\bin`.
4. Reboot your PC. After that you should be all set.

## Required stuff to launch an executable
1. AutoLocker.exe (duh)
2. opencv_world341.dll
3. 'patterns' folder with 'haarcascade_frontalface_alt.xml' file
4. 'authorized_face' folder with 'reference_face.jpg' file

## Some topics I've found useful
1. https://msdn.microsoft.com/en-us/library/ff381399(v=vs.85).aspx
3. https://habrahabr.ru/post/182610/
4. http://scrutator.me/post/2012/04/04/parallel-world-p1.aspx