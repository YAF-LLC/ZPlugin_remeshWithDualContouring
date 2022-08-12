#include "remesh.h"

#include "remeshWithDualContouring.h"

extern "C" DLLEXPORT float remesh(char *someText, double optValue, char *outputBuffer, int optBuffer1Size, char *pOptBuffer2, int optBuffer2Size, char **zData)
{
    // load GoZ file
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> V, NV;
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> F, NF;
    //   todo
    // apply remesh with Dual Contouring
    igl::remeshWithDualContouring(V, F, NV, NF);
    // export GoZ file
    //   todo
    // Hi-Five!
    return 1.0f;
}
