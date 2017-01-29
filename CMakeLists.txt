#
# Copyright 2016 Fraser Adams
#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#

cmake_minimum_required(VERSION 2.6)

project(gimbatuluk) # Gimbatuluk means "to find them all" in Tolkien's Black Speech
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-modules)

# Probe for C++14 or C++11 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX14)
    message(STATUS "Enabling c++14 support.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX11)
    message(STATUS "Enabling c++11 support.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    message(STATUS "Enabling c++0x support.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Probe for OpenCL support
FIND_PACKAGE(OpenCL REQUIRED)

# Check the cl.hpp header to see if it contains the string
# CL_DEVICE_IMAGE_MAX_BUFFER_SIZE, if it doesn't then we need to define our own
# __CL_DECLARE_PARAM_TRAITS(cl_device_info, CL_DEVICE_IMAGE_MAX_BUFFER_SIZE, std::size_t)
# This *should* be there for OpenCL 1.2 but has been omitted from cl.hpp.
# This test is to add a conditional compilation in case future versions of cl.hpp
# add this feature.
if(APPLE)
    file(READ ${OpenCL_INCLUDE_DIR}/OpenCL/cl.hpp TMPTXT)
else()
    file(READ ${OpenCL_INCLUDE_DIR}/CL/cl.hpp TMPTXT)
endif()

string(FIND "${TMPTXT}" "CL_DEVICE_IMAGE_MAX_BUFFER_SIZE" matchres)
if(${matchres} EQUAL -1)
    message(STATUS "CL_DEVICE_IMAGE_MAX_BUFFER_SIZE was not found in cl.hpp - Defining our own")
    add_definitions(-DDECLARE_CL_DEVICE_IMAGE_MAX_BUFFER_SIZE_PARAM_TRAITS)
endif()


# Set the build type
if(CMAKE_BUILD_TYPE)
    # Use specified build type.
else()
    # If CMAKE_BUILD_TYPE not specified then default to Release.
    #set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")

set(COMPILE_WARNING_FLAGS "-Werror -Wall -pedantic-errors")
set(COMPILE_PLATFORM_FLAGS "-pthread")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

set(pfac-source
    src/pfac.cpp
    src/dictionary.cpp
    src/scanner-cpu.cpp
    src/scanner-opencl.cpp
   )

set(executables
    simple-scan.cpp
    simple-scan-async.cpp
    simple-scan-compact.cpp
    simple-benchmark.cpp
    simple-benchmark-async.cpp
    simple-benchmark-compact.cpp
    simple-benchmark-threaded.cpp
    simple-benchmark-threaded-compact.cpp
    soak-test-async.cpp
    soak-test-compact.cpp
   )

# Create executable_targets variable from list of executables with .cpp removed.
string(REPLACE ".cpp" "" executable_targets "${executables}")

# Create executable_targets_fullpath variable from executable_targets by
# prepending ${PROJECT_SOURCE_DIR}/ to each target.
string(REGEX REPLACE "([^;]+)" "${PROJECT_SOURCE_DIR}/\\1" executable_targets_fullpath "${executable_targets}")

set_source_files_properties(
    ${pfac-source} ${executables}
    PROPERTIES
    COMPILE_FLAGS "${COMPILE_WARNING_FLAGS} ${COMPILE_PLATFORM_FLAGS}"
    )

# Define the targets for compiling the library
set(LIB_SOMAJOR 1)
set(LIB_SOMINOR "0.0")

add_library(
  pfac SHARED
  ${pfac-source}
  )

set_target_properties(
  pfac
  PROPERTIES
  VERSION   "${LIB_SOMAJOR}.${LIB_SOMINOR}"
  SOVERSION "${LIB_SOMAJOR}"
  )
target_link_libraries(pfac)

# Define the targets for compiling the executables
foreach(sourcefile ${executables})
    # Simple string replace, to cut off .cpp.
    string(REPLACE ".cpp" "" executable ${sourcefile})
    add_executable(${executable} ${sourcefile})
    target_link_libraries(${executable} pfac ${OpenCL_LIBRARY})
    add_custom_command(
        TARGET ${executable}
        COMMAND ${CMAKE_COMMAND}
                -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/${executable}
                ${PROJECT_SOURCE_DIR}/${executable}
        COMMENT "Copying ${executable} to ${PROJECT_SOURCE_DIR}"
    )
endforeach(sourcefile ${executables})

set_target_properties(
    ${executable_targets}
    PROPERTIES
    LINK_FLAGS "${COMPILE_PLATFORM_FLAGS}"
    )

set_property(
    DIRECTORY APPEND PROPERTY
    ADDITIONAL_MAKE_CLEAN_FILES ${executable_targets_fullpath}
)

# The cleanall target removes everything built by make
add_custom_target(
    cleanall
    COMMAND echo "Cleaning up everything"
    COMMAND ${CMAKE_COMMAND}
            -E remove
            ${executable_targets_fullpath}
    COMMAND ${CMAKE_COMMAND}
            -E remove_directory
            ${CMAKE_CURRENT_BINARY_DIR}
)

