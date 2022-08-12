#include "remeshWithDualContouring.h"

#include <iostream>

template <
    typename DerivedV,
    typename DerivedF,
    typename DerivedNV,
    typename DerivedNF>
IGL_INLINE void igl::remeshWithDualContouring(
    const Eigen::MatrixBase<DerivedV> &V,
    const Eigen::MatrixBase<DerivedF> &F,
    Eigen::PlainObjectBase<DerivedNV> &NV,
    Eigen::PlainObjectBase<DerivedNF> &NF)
{
    // do something!!
    std::cout << V.rows() << std::endl;
}
