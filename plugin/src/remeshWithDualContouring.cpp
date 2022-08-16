#include "remeshWithDualContouring.h"

#include "igl/signed_distance.h"
#include "igl/fast_winding_number.h"
#include "igl/dual_contouring.h"

#include <iostream>

#include "igl/grid.h"
#include "igl/marching_cubes.h"
#include "igl/parallel_for.h"
#include "igl/writePLY.h"

template <
    typename DerivedV,
    typename DerivedF,
    typename DerivedNV,
    typename DerivedNF>
IGL_INLINE void igl::remeshWithDualContouring(
    const Eigen::MatrixBase<DerivedV> &V,
    const Eigen::MatrixBase<DerivedF> &F,
    Eigen::PlainObjectBase<DerivedNV> &NV,
    Eigen::PlainObjectBase<DerivedNF> &NQ,
    const int &dynameshResolution)
{
    typedef Eigen::Matrix<typename DerivedV::Scalar, 1, 3> RowVector3S;

    ////
    // setup signed distance (with precompute for frequent queryings)
    //   AABB
    igl::AABB<DerivedV, 3> tree3;
    tree3.init(V, F);

    //   FastWindingNumberBVH
    igl::FastWindingNumberBVH fwn_bvh;
    igl::fast_winding_number(V.template cast<float>().eval(), F, 2, fwn_bvh);

    //   convert to bounds on (unsiged) squared distances
    typedef typename DerivedV::Scalar Scalar;
    const Scalar lower_bound = std::numeric_limits<Scalar>::min();
    const Scalar upper_bound = std::numeric_limits<Scalar>::max();
    const Scalar max_abs = std::max(std::abs(lower_bound), std::abs(upper_bound));
    const Scalar up_sqr_d = std::pow(max_abs, 2.0);
    const Scalar low_sqr_d =
        std::pow(std::max(max_abs - (upper_bound - lower_bound), (Scalar)0.0), 2.0);

    ////
    // construct std::function for distance/gradient
    //   for signed distance
    const auto &f = [&V, &F, &tree3, &low_sqr_d, &up_sqr_d, &fwn_bvh](const RowVector3S &q3)
    {
        Scalar s = 1, sqrd = 0;
        Eigen::Matrix<Scalar, 1, Eigen::Dynamic> c;
        Eigen::Matrix<Scalar, 1, 3> c3;
        int i = -1;
        // in all cases compute squared unsiged distances
        sqrd = tree3.squared_distance(V, F, q3, low_sqr_d, up_sqr_d, i, c3);
        if (sqrd >= up_sqr_d || sqrd < low_sqr_d)
        {
            return std::numeric_limits<Scalar>::quiet_NaN();
        }
        else
        {
            // assert above ensured 3D
            Scalar w = fast_winding_number(fwn_bvh, 2, q3.template cast<float>().eval());
            s = 1. - 2. * std::abs(w);

            return (s * sqrt(sqrd));
        }
    };

    //   for gradient
    const Scalar eps = 0.25 * static_cast<Scalar>(2.0) / static_cast<Scalar>(dynameshResolution);
    const auto &fd = [&eps](const std::function<Scalar(const RowVector3S &)> &f,
                            const RowVector3S &x)
    {
        RowVector3S g;
        for (int c = 0; c < 3; c++)
        {
            const RowVector3S xp = x + eps * RowVector3S(c == 0, c == 1, c == 2);
            const Scalar fp = f(xp);
            const RowVector3S xn = x - eps * RowVector3S(c == 0, c == 1, c == 2);
            const Scalar fn = f(xn);
            g(c) = (fp - fn) / (2 * eps);
        }
        return g;
    };
    const auto &f_grad = [&fd, &f](const RowVector3S &x)
    {
        return fd(f, x).normalized();
    };

    ////
    // remesh with Dual Contouring
    const RowVector3S min_corner = V.colwise().minCoeff();
    const RowVector3S max_corner = V.colwise().maxCoeff();
    const RowVector3S BBSize = V.colwise().maxCoeff() - V.colwise().minCoeff();
    const Scalar unitSize = static_cast<Scalar>(2.0) / static_cast<Scalar>(dynameshResolution);
    int nx = static_cast<int>(std::ceil(BBSize(0) / unitSize)) + 1;
    int ny = static_cast<int>(std::ceil(BBSize(1) / unitSize)) + 1;
    int nz = static_cast<int>(std::ceil(BBSize(2) / unitSize)) + 1;

    // Dual contouring parameters
    bool constrained = false;
    bool triangles = false;
    bool root_finding = true;

    igl::dual_contouring(
        f, f_grad, min_corner, max_corner, nx, ny, nz, constrained, triangles, root_finding, NV, NQ);

    // std::cout << "Input : " << "#V " << V.rows() << " / " << "#F (tri)  " << F.rows() << std::endl;
    // std::cout << "Output: " << "#V " << NV.rows() << " / " << "#F (quad) " << NQ.rows() << std::endl;

    // Eigen::MatrixXd GV;
    // Eigen::MatrixXd mcV;
    // Eigen::MatrixXi mcF;
    //   igl::grid(Eigen::RowVector3i(nx,ny,nz),GV);
    //   Eigen::VectorXd Gf(GV.rows());
    //   igl::parallel_for(GV.rows(),[&](const int i)
    //   {
    //     GV.row(i).array() *= (max_corner-min_corner).array();
    //     GV.row(i) += min_corner;
    //     Gf(i) = f(GV.row(i));
    //   },1000ul);
    //   igl::marching_cubes(Gf,GV,nx,ny,nz,0,mcV,mcF);
    // igl::writePLY("example/Armadillo_reduced_mc.ply", mcV, mcF);

}

