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
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> VC;
    readGoZAndTriangulate(meshFilePath, meshName, V, F, VC);

    // load parameter (i.e. dynamesh resolution)
    const int resolution = json.at("ZBrush").at("resolution").get<int>();

    // apply remesh with Dual Contouring
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> NV;
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> NQ;
    igl::remeshWithDualContouring(V, F, NV, NQ, resolution);

    // export GoZ file
    //   covert matrix to vector style
    std::vector<std::vector<double>> NVVec(NV.rows(), std::vector<double>(3));
    std::vector<std::vector<int>> NQVec(NQ.rows(), std::vector<int>(4));
    for (int v = 0; v < NV.rows(); ++v)
    {
        for (int xyz = 0; xyz < 3; ++xyz)
        {
            NVVec.at(v).at(xyz) = NV(v, xyz);
        }
    }
    for (int f = 0; f < NQ.rows(); ++f)
    {
        for (int fv = 0; fv < 4; ++fv)
        {
            NQVec.at(f).at(fv) = NQ(f, fv);
        }
    }

    fs::path remeshedPath(meshFilePath.parent_path());
    remeshedPath.append("remeshed.GoZ");
    FromZ::writeGoZFile(remeshedPath.string(), meshName, NVVec, NQVec);

    return 1.0f;
}
