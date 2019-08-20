# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories("include")
include_directories("RpcApp")
include_directories("TunerApp")
#include_directories("/")
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

file(GLOB_RECURSE RPCAPP "RpcApp/*.cpp")
add_library(rpc-lib SHARED ${RPCAPP})
target_link_libraries(rpc-lib ${log-lib})



file(GLOB_RECURSE TUNERAPP "TunerApp/*.cpp")
add_library(tuner-lib SHARED ${TUNERAPP})
target_link_libraries(tuner-lib ${log-lib})
target_link_libraries(tuner-lib rpc-lib)


file(GLOB_RECURSE RPCJNI "RPCJNI/RPCJNI.cpp")
add_library(RPCJNI SHARED ${RPCJNI})
target_link_libraries(RPCJNI ${log-lib})


target_link_libraries(RPCJNI tuner-lib)
