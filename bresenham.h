#ifndef BRESENHAM
#define BRESENHAM

#include<bits/stdc++.h>
#include<GL/glut.h>
#include "object.h"
using namespace std;

class Bresenham : public Object
{
public:
    Bresenham(float* color,int thickness,string pattern);
    void BslopeGT1(int steps, int x, int y, int dx, int dy,int startX,int endX,int startY,int endY);
    void BslopeLT1(int steps, int x, int y, int dx, int dy,int startX,int endX,int startY,int endY);
    void draw(int startX,int endX,int startY,int endY,int width,int height);
    void reDrawSelectedObject(float*,int);
};
#endif // BRESENHAM
