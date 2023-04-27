#
# Copyright (C) 2022-2023 Kazutaka Nakashima (kazutaka.nakashima@n-taka.info)
#
# GPLv3
#
# This file is part of remeshWithDualContouring.
#
# remeshWithDualContouring is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#
# remeshWithDualContouring is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along with remeshWithDualContouring. If not, see <https://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.15)

if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

set(PROJECT ZBrush_remeshWithDualContouring)
project(${PROJECT})

# version
set(Version "1.0")
# basically, we use Release
if (NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
endif()

##################
##### target #####
##################
# Target for this build
add_executable(${PROJECT})

target_sources(
    ${PROJECT}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
########################
##### target (end) #####
########################


#####################
##### Libraries #####
#####################
###
# json
find_package(nlohmann_json CONFIG REQUIRED)

add_subdirectory(plugin)

# Add external libraries
target_link_libraries(
    ${PROJECT}
    PRIVATE
    nlohmann_json
    nlohmann_json::nlohmann_json
    remeshWithDualContouring
)
###########################
##### Libraries (end) #####
###########################

###################
##### Include #####
###################
# Add a include files
target_include_directories(
    ${PROJECT}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/include
    ${CMAKE_CURRENT_SOURCE_DIR}/submodule/libigl/include
    ${CMAKE_CURRENT_SOURCE_DIR}/submodule/eigen

)
#########################
##### Include (end) #####
#########################

###########################
##### compile options #####
###########################
if (WIN32)
    # because we want to use std::filesystem, we require C++17 for windows
    target_compile_features(${PROJECT} PRIVATE cxx_std_17)
elseif (APPLE)
    # because we support macOS Mojave, we only require C++14 for macOS
    # we use boost::filesystem for macOS
    target_compile_features(${PROJECT} PRIVATE cxx_std_14)
endif ()
target_compile_definitions(${PROJECT} PRIVATE _USE_MATH_DEFINES)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4819")
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif ()
#################################
##### compile options (end) #####
#################################

#####################
##### Resources #####
#####################
if (WIN32)
    add_custom_command(TARGET ${PROJECT} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/build/plugin/Release/remeshWithDualContouring.dll" "$<TARGET_FILE_DIR:${PROJECT}>")
    add_custom_command(TARGET ${PROJECT} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/build/plugin/Release/remeshWithDualContouring.dll" "${CMAKE_CURRENT_SOURCE_DIR}/ZPlugs64/remeshWithDualContouring_2022/remeshWithDualContouring.dll")
elseif (APPLE)
    add_custom_command(TARGET ${PROJECT} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/build/plugin/libremeshWithDualContouring.dylib" "$<TARGET_FILE_DIR:${PROJECT}>/remeshWithDualContouring.lib")
    add_custom_command(TARGET ${PROJECT} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/build/plugin/libremeshWithDualContouring.dylib" "${CMAKE_CURRENT_SOURCE_DIR}/ZPlugs64/remeshWithDualContouring_2022/remeshWithDualContouring.lib")
endif ()
###########################
##### Resources (end) #####
###########################
