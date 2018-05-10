# Autolocker
Camera and your face-based authorization.

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
This section assumes that built the OpenCV with *contrib* stuff by following the section above. 

Long story short, [set the OpenCV enironment variable and add it to the system's path](https://docs.opencv.org/master/d3/d52/tutorial_windows_install.html#tutorial_windows_install_path).

Long story long,

1. Open *Environment variables* window (see System Properties)
2. Under *System variables* group, create an `OPENCV_DIR` variable and set it to the location of your OpenCV's *bin* and *lib* folders, something like that: `C:\Program Files (x86)\OpenCV\opencv\build\x64\vc15`.
3. Under *System variables* group, find an existing `PATH` variable and click 'Edit'. In appeared dialog, add a new path with exactly the following value: `%OPENCV_DIR%\bin`.
4. Reboot your PC. After that you should be all set.

## Required stuff to launch an executable
1. AutoLocker.exe (duh)
2. opencv_world341.dll
3. 'patterns' folder with 'haarcascade_frontalface_alt.xml' file
4. 'authorized_face' folder with 'reference_face.jpg' file

## Add someone to the authorized users
1. Run the Autolocker.exe and type `learn`.
2. Wait until the program captures and shows you an image from camera.
3. If an image is vaild, type `Y` to save it or `N` to repeat (this prompt might be hidden behind image window).

The process will be repeated five times. Feel free to run `learn` when you feel that existing photos aren't allowing the program to recognize the user (due to different lighting conditions or whatnot). However, be mindful of amount of images the application has to load as it can easily chew through large amounts of RAM with relatively low image count (2GB of RAM for 90 images).

## Remove someone from authorized users
Navigate to `./authorized_faces` folder and remove images manually.

## Settings description
Set settings by typing `sm set <parameter name> <value>`

1. `engine_rpm` - a delay in milliseconds between engine iterations.
2. `detectionfailure_threshold` - amount of consequental failed face _detections_ after which lockdown is engaged.
3. `recognitionfailure_threshold` - amount of consequental failed face _recognitions_ after which lockdown is engaged.
4. `recognition_interval` - amount of _seconds_ between each recognition.
5. `confidence_threshold` - a recognition confidence value after which a face is considered unrecognized. Lower values mean higher recognition confidence.
6. `capturedevide_index` - an index of a capture device. Usually `0`.
7. `logverbosity` - log verbosity level. Acceptable values (following levels include preceeding ones): 0 - Silent, 1 - Warnings, 2 - Errors, 3 - Logs, 4 - Debug. 
8. `feedwindow` - boolean switch that specifies if camera feed must be show during program's operation.
9. `preventlockdown` - boolean switch that allows to prevent actual lockdown. Useful for thershold calibration.
10. `cascadetemplate_path` - a path to the template consumed by face detector.
11. `authorizedfaces_path` - a path tho the folder with images of authorized users.

## Some topics I've found useful
1. https://msdn.microsoft.com/en-us/library/ff381399(v=vs.85).aspx
3. https://habrahabr.ru/post/182610/
4. http://scrutator.me/post/2012/04/04/parallel-world-p1.aspx