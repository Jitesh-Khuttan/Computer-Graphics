#ifndef NGON
#define NGON
#include "object.h"
#include <bits/stdc++.h>
using namespace std;

class Ngon : public Object
{
	public:
		int totalVertices;
		float rotationAngle;
		list< pair<int,int> > verticeList;
		pair<int,int> pivotPoint;
		pair<int,int> startPoint;
		Ngon(GLubyte* color,int thickness,string pattern);
		void setVertices(int);
		void setRotationAngle(float);
		void draw(int startX,int endX,int startY,int endY,int width,int height);
		void reDrawSelectedObject(GLubyte*,int);
};

#endif
