#ifndef SYMMETRICDDA
#define SYMMETRICDDA

#include "object.h"
#include<bits/stdc++.h>
#include<GL/glut.h>
using namespace std;

class SymmetricDDA : public Object
{
public:
    SymmetricDDA(float* color,int thickness,string pattern);
    void draw(int startX,int endX,int startY,int endY,int width,int height);
    void reDrawSelectedObject(float*,int);
};


#endif // SYMMETRICDDA
