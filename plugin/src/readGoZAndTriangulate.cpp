#ifndef READ_GOZ_AND_TRIANGULATE_CPP
#define READ_GOZ_AND_TRIANGULATE_CPP

#include "readGoZAndTriangulate.h"
#include <vector>
#include "readGoZFile.h"

void readGoZAndTriangulate(
	const fs::path &GoZFilePath,
	std::string &meshName,
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &V,
	Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &F,
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &VC)
{
	std::vector<std::vector<double>> VVec;
	std::vector<std::vector<int>> FVec;
	std::vector<std::vector<std::pair<double, double>>> UVVec;
	std::vector<std::vector<double>> VCVec;
	std::vector<double> MVec;
	std::vector<int> GVec;

	FromZ::readGoZFile(GoZFilePath.string(), meshName, VVec, FVec, UVVec, VCVec, MVec, GVec);
	int triCount = 0;
	for (const auto &f : FVec)
	{
		triCount += (f.size() - 2);
	}
	V.resize(VVec.size(), 3);
	for (int v = 0; v < VVec.size(); ++v)
	{
		V.row(v) << VVec.at(v).at(0), VVec.at(v).at(1), VVec.at(v).at(2);
	}
	VC.resize(VCVec.size(), 4);
	for (int vc = 0; vc < VCVec.size(); ++vc)
	{
		VC.row(vc) << VCVec.at(vc).at(0), VCVec.at(vc).at(1), VCVec.at(vc).at(2), VCVec.at(vc).at(3);
	}
	F.resize(triCount, 3);
	int fIdx = 0;
	for (int f = 0; f < FVec.size(); ++f)
	{
		for (int tri = 0; tri < FVec.at(f).size() - 2; ++tri)
		{
			F.row(fIdx) << FVec.at(f).at(0), FVec.at(f).at(1 + tri), FVec.at(f).at(2 + tri);
			fIdx++;
		}
	}
}

#endif
