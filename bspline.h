#ifndef BSPLINE
#define BSPLINE

#include<bits/stdc++.h>
#include "object.h"
using namespace std;


class Bspline : public Object
{
	private:
		float calculateCoefficient(float u,int k,int degree);
	public:
		int totalControlPoints;
		int degree;
		list< pair<int,int> > controlPoints;
		
		Bspline(GLubyte* color,int thickness,string pattern);
		
		void draw(int startX,int endX,int startY,int endY,int width,int height){
		}
    	void reDrawSelectedObject(GLubyte* colorToDraw,int thicknessToDraw);
    	void drawCurve(list< pair<int,int> > controlPoints,int degree);
};


#endif

