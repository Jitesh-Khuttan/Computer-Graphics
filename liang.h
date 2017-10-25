#ifndef LIANG
#define LIANG

#include "object.h"
#include "simpleDDA.h"
#include "symmetricDDA.h"
#include "midpoint.h"
#include "bresenham.h"
using namespace std;

class Liang
{
public:
    static void clipLineLiang(int startX,int endX,int startY,int endY,int width,int height,int minX,int maxX,int minY,int maxY,int algoType);

};

#endif // LIANG
