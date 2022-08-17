#include "remesh.h"
#include "remeshWithDualContouring.h"

#include "nlohmann/json.hpp"

#if defined(_WIN32) || defined(_WIN64)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include "boost/filesystem.hpp"
namespace fs = boost::filesystem;
#endif

#include "igl/barycentric_interpolation.h"

#include "readGoZAndTriangulate.h"
#include "writeGoZFile.h"

extern "C" DLLEXPORT float remesh(char *someText, double optValue, char *outputBuffer, int optBuffer1Size, char *pOptBuffer2, int optBuffer2Size, char **zData)
{
    ////
    // parse parameter (JSON)
    fs::path jsonPath(someText);
    std::ifstream ifs(jsonPath);
    nlohmann::json json = nlohmann::json::parse(ifs);
    ifs.close();

    std::cout << json.dump(4) << std::endl;

    const std::string rootString = json.at("root").get<std::string>();
    const fs::path rootPath(rootString);

    // load GoZ file
    const std::string meshFileRelPathStr = json.at("meshFile").get<std::string>();
    fs::path meshFileRelPath(meshFileRelPathStr);
    fs::path meshFilePath(rootPath);
    meshFilePath /= meshFileRelPath;

    std::string meshName;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> V;
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> F;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> UV_u;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> UV_v;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> VC;
    Eigen::Matrix<double, Eigen::Dynamic, 1> M;
    Eigen::Matrix<int, Eigen::Dynamic, 1> G;
    readGoZAndTriangulate(meshFilePath, meshName, V, F, UV_u, UV_v, VC, M, G);

    // load parameter (i.e. dynamesh resolution)
    const int resolution = json.at("ZBrush").at("resolution").get<int>();

    // apply remesh with Dual Contouring
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> NV;
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> NQ;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> NUV_u;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> NUV_v;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> NVC;
    Eigen::Matrix<double, Eigen::Dynamic, 1> NM;
    Eigen::Matrix<int, Eigen::Dynamic, 1> NG;
    igl::remeshWithDualContouring(V, F, NV, NQ, resolution);

    const bool keepMask = json.at("ZBrush").at("mask").get<bool>() && (V.rows() == M.rows());
    const bool keepGroup = json.at("ZBrush").at("polygroup").get<bool>() && (F.rows() == G.rows());
    const bool keepPaint = json.at("ZBrush").at("polypaint").get<bool>() && (V.rows() == VC.rows());

    // vertex attributes
    if (keepMask || keepPaint)
    {
        Eigen::Matrix<double, Eigen::Dynamic, 1> S;
        Eigen::Matrix<int, Eigen::Dynamic, 1> I;
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> C;
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> N;

        igl::signed_distance(NV, V, F, igl::SIGNED_DISTANCE_TYPE_FAST_WINDING_NUMBER, S, I, C, N);

        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> L;
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> cornerA;
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> cornerB;
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> cornerC;
        Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> F_;
        Eigen::Matrix<int, Eigen::Dynamic, 1> XYZ;
        XYZ.resize(3, 1);
        XYZ << 0, 1, 2;
        igl::slice(F, I, XYZ, F_);
        igl::slice(V, F_.col(0), XYZ, cornerA);
        igl::slice(V, F_.col(1), XYZ, cornerB);
        igl::slice(V, F_.col(2), XYZ, cornerC);

        igl::barycentric_coordinates(C, cornerA, cornerB, cornerC, L);

        if (keepMask)
        {
            igl::barycentric_interpolation(M, F, L, I, NM);
        }
        if (keepPaint)
        {
            igl::barycentric_interpolation(VC, F, L, I, NVC);
        }
    }

    // face attributes
    if (keepGroup)
    {
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> BC;
        igl::barycenter(NV, NQ, BC);

        Eigen::Matrix<double, Eigen::Dynamic, 1> S;
        Eigen::Matrix<int, Eigen::Dynamic, 1> I;
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> C;
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> N;

        igl::signed_distance(BC, V, F, igl::SIGNED_DISTANCE_TYPE_FAST_WINDING_NUMBER, S, I, C, N);

        NG.resize(NQ.rows(), 1);
        for (int nq = 0; nq < NQ.rows(); ++nq)
        {
            NG(nq, 0) = G(I(nq, 0), 0);
        }
    }

    // export GoZ file
    //   covert matrix to vector style
    std::vector<std::vector<double>> NVVec(NV.rows(), std::vector<double>(3));
    for (int v = 0; v < NV.rows(); ++v)
    {
        for (int xyz = 0; xyz < 3; ++xyz)
        {
            NVVec.at(v).at(xyz) = NV(v, xyz);
        }
    }

    std::vector<std::vector<int>> NQVec(NQ.rows(), std::vector<int>(4));
    for (int f = 0; f < NQ.rows(); ++f)
    {
        for (int fv = 0; fv < 4; ++fv)
        {
            NQVec.at(f).at(fv) = NQ(f, fv);
        }
    }

    std::vector<std::vector<std::pair<double, double>>> NUVVec(NUV_u.rows(), std::vector<std::pair<double, double>>(4));
    // for (int f = 0; f < NQ.rows(); ++f)
    // {
    //     for (int fv = 0; fv < 4; ++fv)
    //     {
    //         NQVec.at(f).at(fv) = NQ(f, fv);
    //     }
    // }

    std::vector<std::vector<double>> NVCVec(NVC.rows(), std::vector<double>(4));
    for (int v = 0; v < NVC.rows(); ++v)
    {
        for (int rgba = 0; rgba < 4; ++rgba)
        {
            NVCVec.at(v).at(rgba) = NVC(v, rgba);
        }
    }

    std::vector<double> NMVec(NM.rows(), 1.0);
    for (int v = 0; v < NM.rows(); ++v)
    {
        NMVec.at(v) = NM(v, 0);
    }

    std::vector<int> NGVec(NG.rows(), 1.0);
    for (int f = 0; f < NG.rows(); ++f)
    {
        NGVec.at(f) = NG(f, 0);
    }

    FromZ::writeGoZFile(meshFilePath.string(), meshName, NVVec, NQVec, NUVVec, NVCVec, NMVec, NGVec);

    return 1.0f;
}
