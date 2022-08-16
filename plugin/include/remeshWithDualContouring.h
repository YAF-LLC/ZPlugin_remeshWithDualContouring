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
