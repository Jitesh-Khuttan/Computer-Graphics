#ifndef COHEN
#define COHEN

#include<GL/glut.h>
#include "object.h"
#include "simpleDDA.h"
#include "symmetricDDA.h"
#include "midpoint.h"
#include "bresenham.h"
using namespace std;

class Cohen
{

public:
    static int computeRegionCode(int X,int Y,int minX,int maxX,int minY,int maxY);
    static void clipLine(int startX,int endX,int startY,int endY,int width,int height,int minX,int maxX,int minY,int maxY,int algoType);
};


#endif // COHEN
