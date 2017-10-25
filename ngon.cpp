#include "ngon.h"
#include "color.h"
#include "axis.h"
#include "thickness.h"
#include "bresenham.h"
#include <math.h>

extern bool clipWindowON;
extern int minX,maxX,minY,maxY;
//GLubyte *interiorColor = new GLubyte[3];
extern list<Object*> allObjects;

Ngon ::Ngon(float* color,int thickness,string pattern)
{
    this->color = color;
    this->thickness = thickness;
    this->pattern = pattern;
    this->patternIndex = 0;
    this->objectName = "Circle";
}

void Ngon :: printCoordinates()
{
	list< pair<int,int> > :: iterator it;
	for(it = coordinates.begin(); it != coordinates.end();it++)
	{
		cout<<"\n\tCoordinates: ("<<(*it).first<<","<<(*it).second<<")"<<endl;
	}
}

bool Ngon :: selectObject(pair<int,int> clickedCoordinates)
{
	cout<<"Inside Select Object Of Circle"<<endl;
	for(list< pair<int,int> >:: iterator it = coordinates.begin(); it != coordinates.end(); it++)
	{
		if( (*it).first == clickedCoordinates.first && (*it).second == clickedCoordinates.second)
		{
			reDrawSelectedObject(Color::NAVYBLUE,thickness+2);
			return true;
		}
	}
	return false;
}


void Ngon :: reDrawSelectedObject(float* colorToDraw,int thicknessToDraw)
{
	cout<<"Redrawing Selected Circle "<<endl;
	glColor3fv(colorToDraw);
	glPointSize(Thickness::THICKNESS10);
	if(colorToDraw == Color::BLACK)
	{
//		glBegin(GL_POINTS);
//			glVertex2i(centerX,centerY);
//		glEnd();
	}
	glPointSize(thicknessToDraw);
	list< pair<int,int> >:: iterator it;
	for(it = coordinates.begin(); it!= coordinates.end();it++)
	{
		patternIndex = patternIndex%10;
		if(pattern[patternIndex] == 49)
		{
			glBegin(GL_POINTS);
				glVertex2i((*it).first, (*it).second);
			glEnd();
		}
		patternIndex++;
	}
	glFlush();
}


void Ngon :: translateObject(int dx,int dy)
{
	reDrawSelectedObject(Color::BLACK,Thickness::THICKNESS10);
	Axis::drawAxis();
	
	list< pair<int,int> >:: iterator it;
	for(it = coordinates.begin(); it!= coordinates.end();it++)
	{
		(*it).first += dx;
		(*it).second += dy;
	}

	list<Object*>:: iterator i;
	for(i = allObjects.begin(); i!= allObjects.end();i++)
	{
		(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
	}
}


void Ngon ::rotateObject(int rotationAngle,pair<int,int> pivotPoint) 
{
	int tempx,tempy;
	reDrawSelectedObject(Color::BLACK,Thickness::THICKNESS10);
	Axis::drawAxis();
	
	list< pair<int,int> >:: iterator it;
	for(it = coordinates.begin(); it!= coordinates.end();it++)
	{
		tempx = (*it).first;
		tempy = (*it).second;
		(*it).first = pivotPoint.first + (tempx - pivotPoint.first)*cos(rotationAngle*3.14159/180) - (tempy - pivotPoint.second)*sin(rotationAngle*3.14159/180);
		(*it).second = pivotPoint.second + (tempx - pivotPoint.first)*sin(rotationAngle*3.14159/180) + (tempy - pivotPoint.second)*cos(rotationAngle*3.14159/180);
	}

	list<Object*>:: iterator i;
	for(i = allObjects.begin(); i!= allObjects.end();i++)
	{
		(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
	}
}


void Ngon ::setColor(float* color)
{
    this->color = color;
}

void Ngon ::setPattern(string pattern)
{
    this->pattern = pattern;
}

void Ngon :: setThickness(int thickness)
{
    this->thickness = thickness;
}

void Ngon :: setVertices(int vertices)
{
	this->totalVertices = vertices;
}

void Ngon :: setRotationAngle(float angle)
{
	this->rotationAngle = angle;
}

void Ngon::draw(int startX,int endX,int startY,int endY,int width,int height)
{
	int radius;
//	cout<<"INSIDE NGON!"<<endl;
	pair<int,int> tempCoordinates, fromPoint,toPoint;
	int tempx,tempy;
	startX = startX - width/2;
    startY = height/2 - startY;
    endX = endX - width/2;
    endY = height/2 - endY;
    
    glColor3fv(Color::BLACK);
	glPointSize(Thickness::THICKNESS10);
	glBegin(GL_POINTS);
		glVertex2i(startX,startY);
		glVertex2i(endX,endY);
	glEnd();
    
//    cout<<"StartX,StartY: ("<<startX<<","<<startY<<")"<<endl;
//	cout<<"endX,endY: ("<<endX<<","<<endY<<")"<<endl;
    
    radius = sqrt((endX-startX)*(endX-startX) + (endY - startY)*(endY - startY));
    pivotPoint.first = startX;
    pivotPoint.second = startY;
    
    startPoint.first = endX;
    startPoint.second = endY;
    

	tempCoordinates = startPoint;
	glColor3fv(Color::NAVYBLUE);
	for(int counter=0;counter<totalVertices;counter++)
	{
//		cout<<"INSIDE Coordinate initialisation!"<<endl;
		verticeList.push_back(tempCoordinates);
		tempx = tempCoordinates.first;
		tempy = tempCoordinates.second;
		glBegin(GL_POINTS);
			glVertex2i(tempx,tempy);
		glEnd();
		tempCoordinates.first = pivotPoint.first + (tempx - pivotPoint.first)*cos(rotationAngle*3.14159/180) - (tempy - pivotPoint.second)*sin(rotationAngle*3.14159/180);
		tempCoordinates.second = pivotPoint.second + (tempx - pivotPoint.first)*sin(rotationAngle*3.14159/180) + (tempy - pivotPoint.second)*cos(rotationAngle*3.14159/180);	
	}
    
    list< pair<int,int> >:: iterator from,to;
    for(from = verticeList.begin(); from!= verticeList.end();from++)
    {
    	fromPoint = (*from);
//    	cout<<"Drawing Line From: ("<<fromPoint.first<<","<<fromPoint.second<<")"<<endl;
    	for(to = verticeList.begin(); to!= verticeList.end();to++)
    	{
    		toPoint = *to;
//    		cout<<"Drawing Line To: ("<<toPoint.first<<","<<toPoint.second<<")"<<endl;
    		Bresenham *bresenhamLine = new Bresenham(this->color,this->thickness,this->pattern);
            bresenhamLine->draw(fromPoint.first + width/2 ,toPoint.first + width/2 ,height/2 - fromPoint.second,height/2 - toPoint.second,width,height);
		}
	}
	glFlush();
}

