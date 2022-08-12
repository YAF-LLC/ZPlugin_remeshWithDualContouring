#include "remesh.h"

int main()
{
    char buf[255];
    char **zData = nullptr;
    remesh("", 0.0, buf, 0, buf, 0, zData);
    return 0;
}
