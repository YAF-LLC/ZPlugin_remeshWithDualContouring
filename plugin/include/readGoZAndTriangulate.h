#ifndef READ_GOZ_AND_TRIANGULATE_H
#define READ_GOZ_AND_TRIANGULATE_H

#if defined(_WIN32) || defined(_WIN64)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include "boost/filesystem.hpp"
namespace fs = boost::filesystem;
#endif

#include "Eigen/Core"

void readGoZAndTriangulate(
    const fs::path &GoZFilePath,
	std::string &meshName,
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &V,
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &F,
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &VC);

#endif
