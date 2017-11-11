#include "bezier.h"
#include "color.h"
#include "axis.h"
#include "thickness.h"


Bezier :: Bezier(GLubyte* color,int thickness,string pattern)
{
	this->color = color;
    this->thickness = thickness;
    this->pattern = pattern;
    this->patternIndex = 0;
    this->objectName = "Bezier";
}

void Bezier :: reDrawSelectedObject(GLubyte* colorToDraw,int thicknessToDraw)
{
	//cout<<"Redrawing Selected Circle "<<endl;
	glColor3ubv(colorToDraw);
	glPointSize(Thickness::THICKNESS10);
	if(colorToDraw == Color::BLACK)
	{
		list< pair<int,int> > :: iterator it;
		for(it = coordinates.begin(); it != coordinates.end();it++)
		{
			glBegin(GL_POINTS);
				glVertex2i((*it).first,(*it).second);
			glEnd();	
		}
	}
	glPointSize(thicknessToDraw);
	list< pair<int,int> >:: iterator it;
	for(it = coordinates.begin(); it!= coordinates.end();it++)
	{
		glBegin(GL_POINTS);
			glVertex2i((*it).first, (*it).second);
		glEnd();
	}
	glFlush();
}

int Bezier :: factorial(int number)
{
	int result = 1;
	for(int i = 2;i <= number; i++)
	{
		result *= i;
	}
	return result;
}

int Bezier :: calculateCoefficient(int n,int k)
{
	return (factorial(n)/(factorial(k)*factorial(n-k)));
}

void Bezier :: drawCurve(list< pair<int,int> > controlPoints)
{
	int k,n;
	cout<<"\n\tDrawing Curve";
	this->controlPoints = controlPoints;
	totalControlPoints = controlPoints.size();
	
	n = totalControlPoints-1;
	
	list< pair<int,int> > :: iterator it;
	
	float u;
	for(u = 0.0;u<=1.0;u+=0.0001)
	{
		//cout<<"\n\tCalculating Coordinates";
		pair<int,int> pixelCoordinate;
		for(k=0;k<=n;)
		{
			for(it = controlPoints.begin();it != controlPoints.end(); it++)
			{
				pixelCoordinate.first += (*it).first * calculateCoefficient(n,k) * pow(u,k) * pow(1-u,n-k);
				pixelCoordinate.second += (*it).second * calculateCoefficient(n,k) * pow(u,k) * pow(1-u,n-k);
				k++;
			}
		}
		//cout<<"\n\t("<<pixelCoordinate.first<<","<<pixelCoordinate.second<<")";
		coordinates.push_back(pixelCoordinate);
	}
	
	glColor3ubv(color);
	glPointSize(thickness);
	
	for(it = coordinates.begin();it != coordinates.end();it++)
	{
		patternIndex = patternIndex%10;
		if(pattern[patternIndex] == 49)
		{
			glBegin(GL_POINTS);
				glVertex2i((*it).first,(*it).second);
			glEnd();
		}
		patternIndex++;
	}	
}
