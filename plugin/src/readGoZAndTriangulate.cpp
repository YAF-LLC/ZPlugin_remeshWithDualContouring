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
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &UV_u,
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &UV_v,
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> &VC,
	Eigen::Matrix<double, Eigen::Dynamic, 1> &M,
	Eigen::Matrix<int, Eigen::Dynamic, 1> &G)
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

	if (FVec.size() == UVVec.size())
	{
		UV_u.resize(triCount, 3);
		UV_v.resize(triCount, 3);
		fIdx = 0;
		for (int f = 0; f < UVVec.size(); ++f)
		{
			for (int tri = 0; tri < UVVec.at(f).size() - 2; ++tri)
			{
				UV_u.row(fIdx) << UVVec.at(f).at(0).first, UVVec.at(f).at(1 + tri).first, UVVec.at(f).at(2 + tri).first;
				UV_v.row(fIdx) << UVVec.at(f).at(0).second, UVVec.at(f).at(1 + tri).second, UVVec.at(f).at(2 + tri).second;
				fIdx++;
			}
		}
	}

	if (VVec.size() == VCVec.size())
	{
		VC.resize(VCVec.size(), 4);
		for (int vc = 0; vc < VCVec.size(); ++vc)
		{
			VC.row(vc) << VCVec.at(vc).at(0), VCVec.at(vc).at(1), VCVec.at(vc).at(2), VCVec.at(vc).at(3);
		}
	}

	if (VVec.size() == MVec.size())
	{
		M.resize(MVec.size(), 1);
		for (int v = 0; v < MVec.size(); ++v)
		{
			M(v, 0) = MVec.at(v);
		}
	}

	if (FVec.size() == GVec.size())
	{
		G.resize(triCount, 1);
		fIdx = 0;
		for (int f = 0; f < GVec.size(); ++f)
		{
			for (int tri = 0; tri < FVec.at(f).size() - 2; ++tri)
			{
				G(fIdx, 0) = GVec.at(f);
				fIdx++;
			}
		}
	}
}

#endif
