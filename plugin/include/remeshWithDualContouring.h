//
// Copyright (C) 2022-2023 Kazutaka Nakashima (kazutaka.nakashima@n-taka.info)
// 
// GPLv3
//
// This file is part of remeshWithDualContouring.
// 
// remeshWithDualContouring is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// 
// remeshWithDualContouring is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with remeshWithDualContouring. If not, see <https://www.gnu.org/licenses/>.
//

#ifndef REMESH_WITH_DUAL_COREMESH_WITH_DUAL_CONTOURING_H
#define REMESH_WITH_DUAL_COREMESH_WITH_DUAL_CONTOURING_H

#include "igl/igl_inline.h"

#include <Eigen/Core>

namespace igl 
{
  template <
    typename DerivedV,
    typename DerivedF,
    typename DerivedNV,
    typename DerivedNF>
  IGL_INLINE void remeshWithDualContouring(
    const Eigen::MatrixBase<DerivedV> &V,
    const Eigen::MatrixBase<DerivedF> &F,
    Eigen::PlainObjectBase<DerivedNV> &NV,
    Eigen::PlainObjectBase<DerivedNF> &NQ,
    const int &dynameshResolution);
}

#include "../src/remeshWithDualContouring.cpp"

#endif
