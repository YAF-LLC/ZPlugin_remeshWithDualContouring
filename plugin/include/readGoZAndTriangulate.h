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
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &UV_u,
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &UV_v,
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &VC,
	Eigen::Matrix<double, Eigen::Dynamic, 1> &M,
	Eigen::Matrix<int, Eigen::Dynamic, 1> &G);

#endif
