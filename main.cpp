#include <fstream>
#include <string>
#include "nlohmann/json.hpp"

#include "remesh.h"

int main()
{
    // nlohmann::json json;
    // json["root"] = "C:/PROGRAM FILES/PIXOLOGIC/ZBRUSH 2022/ZSTARTUP/ZPlugs64/remeshWithDualContouring_2022/";
    // json["meshFile"] = "data/Dog.GoZ";
    // json["ZBrush"] = nlohmann::json::object();
    // json["ZBrush"]["XYZSize"] = 2;
    // json["ZBrush"]["resolution"] = 128;

    // std::ofstream ofs("build/Release/data/parameters.txt");
    // ofs << json.dump(4) << std::endl;
    // ofs.close();

    char buf[255];
    char **zData = nullptr;
    remesh("C:/PROGRAM FILES/PIXOLOGIC/ZBRUSH 2022/ZSTARTUP/ZPlugs64/remeshWithDualContouring_2022/data/parameters.txt", 0.0, buf, 0, buf, 0, zData);
    return 0;
}
