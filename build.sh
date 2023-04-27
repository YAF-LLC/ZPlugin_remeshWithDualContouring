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

#!/bin/bash

############
# OS detection
############
triplet="x"
if [ "$(uname)" == "Darwin" ]; then
    # we use custom triplet (x64-osx-mojave)
    triplet="${triplet}64-osx-mojave"
    # copy custom triplet file (for supporting Mojave)
    cp "x64-osx-mojave.cmake" "submodule/vcpkg/triplets/x64-osx-mojave.cmake"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
    triplet="${triplet}86-windows"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    triplet="${triplet}64-windows-static"
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    triplet="${triplet}64-linux"
else
    echo "This OS is not supported..."
    exit 1
fi

############
# build project
############
if [ "$(uname)" == "Darwin" ]; then
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="submodule/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET="${triplet}" -DCMAKE_BUILD_TYPE="Release"
    cmake --build build --config "Release"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
    subst X: .
    cd X:
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="submodule/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET="${triplet}" -DCMAKE_BUILD_TYPE="Release"
    cmake --build build --config "Release"
    # revert subst command
    # "/" symbol was comprehended as separator for path in MINGW. Thus, we need to explicitly use "//"
    # echo "unbind ./submodule as X:"
    subst X: //D
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    subst X: .
    cd X:
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="submodule/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET="${triplet}" -DCMAKE_BUILD_TYPE="Release"
    cmake --build build --config "Release"
    # revert subst command
    # "/" symbol was comprehended as separator for path in MINGW. Thus, we need to explicitly use "//"
    # echo "unbind ./submodule as X:"
    subst X: //D
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="submodule/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET="${triplet}" -DCMAKE_BUILD_TYPE="Release"
    cmake --build build --config "Release"
else
    echo "This OS is not supported..."
    exit 1
fi
