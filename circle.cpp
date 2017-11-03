#include "circle.h"
#include "color.h"
#include "axis.h"
#include "thickness.h"
#include <math.h>

extern bool clipWindowON;
extern int minX,maxX,minY,maxY;
extern list<Object*> allObjects;
extern int width,height;

Circle ::Circle(GLubyte* color,int thickness,string pattern)
{
    this->color = color;
    this->thickness = thickness;
    this->pattern = pattern;
    this->patternIndex = 0;
    this->filled = 0;
    this->objectName = "Circle";
}

void Circle :: reDrawSelectedObject(GLubyte* colorToDraw,int thicknessToDraw)
{
	//cout<<"Redrawing Selected Circle "<<endl;
	glColor3ubv(colorToDraw);
	glPointSize(Thickness::THICKNESS10);
	if(colorToDraw == Color::BLACK)
	{
		glBegin(GL_POINTS);
			glVertex2i(centerX,centerY);
		glEnd();
	}
	glPointSize(thicknessToDraw);
	list< pair<int,int> >:: iterator it;
	for(it = coordinates.begin(); it!= coordinates.end();it++)
	{
		glBegin(GL_POINTS);
			glVertex2i((*it).first, (*it).second);
		glEnd();
	}
	
	if(filled)
	{
		if(colorToDraw != Color::NAVYBLUE)
			glColor3ubv(this->fillColor);
		for(it = filledCoordinates.begin(); it!= filledCoordinates.end();it++)
		{
			glBegin(GL_POINTS);
				glVertex2i((*it).first, (*it).second);
			glEnd();
		}	
	}
	glFlush();
}

void Circle :: rePaintFilledCoordinates()
{
	list< pair<int,int> > :: iterator it;
	glColor3ubv(Color::BLACK);
	for(it = filledCoordinates.begin(); it!= filledCoordinates.end();it++)
	{
		glBegin(GL_POINTS);
			glVertex2i((*it).first, (*it).second);
		glEnd();
	}
}
void Circle :: fillBoundary(int x,int y,GLubyte* fillColor,GLubyte* boundaryColor)
{
	this->fillColor = fillColor;
	filled = true;
	fillCircle(x,y,fillColor,boundaryColor);
	
	reDrawSelectedObject(Color::BLACK,this->thickness+2);
	reDrawSelectedObject(this->color,this->thickness);
	//Redrawing the objects to the screen
	list<Object*>:: iterator i;
	for(i = allObjects.begin(); i!= allObjects.end();i++)
	{
		if(*i != this)
			(*i)->reDrawSelectedObject((*i)->color,(*i)->thickness);
	}
}

void Circle :: fillCircle(int x,int y,GLubyte* fillColor,GLubyte* boundaryColor)
{	
	GLubyte interiorColor[3];
	glReadPixels(x,height - y,1,1,GL_RGB,GL_UNSIGNED_BYTE,interiorColor);

	if((interiorColor[0] != fillColor[0] || interiorColor[1] != fillColor[1] || interiorColor[2] != fillColor[2]) && (interiorColor[0] != boundaryColor[0] || interiorColor[1] != boundaryColor[1] || interiorColor[2] != boundaryColor[2]))
	{
		glColor3ubv(fillColor);
		glPointSize(Thickness::THICKNESS1);
		glBegin(GL_POINTS);
			glVertex2i(x - width/2,height/2 - y);
			filledCoordinates.push_back(make_pair(x-width/2,height/2 - y));
		glEnd();
		glFlush();
		fillCircle(x+1,y,fillColor,boundaryColor);
		fillCircle(x-1,y,fillColor,boundaryColor);
		fillCircle(x,y+1,fillColor,boundaryColor);
		fillCircle(x,y-1,fillColor,boundaryColor);
	}
}


void Circle::clipPointsOfCircle(int X,int Y)
{
    pair<int,int> currentCoordinates;
	//cout<<"\n\tGetting Plotted by algo";
	if(X > minX && X  < maxX &&  Y > minY && Y < maxY)
	{
        currentCoordinates.first = X;
        currentCoordinates.second = Y;
        coordinates.push_back(currentCoordinates);

		glBegin(GL_POINTS);
			glVertex2i(X,Y);
		glEnd();
	}
	glFlush();
}

void Circle::putPixel(int Xc,int Yc,int currentx,int currenty)
{
    pair<int,int> currentCoordinates;
	glColor3ubv(color);
	glPointSize(thickness);
	patternIndex = patternIndex%10;
	if(pattern[patternIndex] == 49)
	{
		if(clipWindowON)
		{
			glBegin(GL_POINTS);
				clipPointsOfCircle(Xc + currentx,Yc + currenty);
				clipPointsOfCircle(Xc + currenty, Yc + currentx);
				clipPointsOfCircle(Xc - currentx,Yc + currenty);
				clipPointsOfCircle(Xc - currenty, Yc + currentx);
				clipPointsOfCircle(Xc - currentx,Yc - currenty);
				clipPointsOfCircle(Xc - currenty,Yc - currentx);
				clipPointsOfCircle(Xc + currentx,Yc - currenty);
				clipPointsOfCircle(Xc + currenty , Yc - currentx);
			glEnd();
		}
		else
		{
			glBegin(GL_POINTS);
                currentCoordinates.first = Xc + currentx;
                currentCoordinates.second = Yc + currenty;
                coordinates.push_back(currentCoordinates);
				glVertex2i(Xc + currentx,Yc + currenty);

				currentCoordinates.first = Xc + currenty;
                currentCoordinates.second = Yc + currentx;
                coordinates.push_back(currentCoordinates);
				glVertex2i(Xc + currenty, Yc + currentx);

				currentCoordinates.first = Xc - currentx;
                currentCoordinates.second = Yc + currenty;
                coordinates.push_back(currentCoordinates);
				glVertex2i(Xc - currentx,Yc + currenty);

				currentCoordinates.first = Xc - currenty;
                currentCoordinates.second = Yc + currentx;
                coordinates.push_back(currentCoordinates);
				glVertex2i(Xc - currenty, Yc + currentx);

				currentCoordinates.first = Xc - currentx;
                currentCoordinates.second = Yc - currenty;
                coordinates.push_back(currentCoordinates);
				glVertex2i(Xc - currentx,Yc - currenty);

				currentCoordinates.first = Xc - currenty;
                currentCoordinates.second = Yc - currentx;
                coordinates.push_back(currentCoordinates);
				glVertex2i(Xc - currenty,Yc - currentx);

				currentCoordinates.first = Xc + currentx;
                currentCoordinates.second = Yc - currenty;
                coordinates.push_back(currentCoordinates);
				glVertex2i(Xc + currentx,Yc - currenty);

				currentCoordinates.first = Xc + currenty;
                currentCoordinates.second = Yc - currentx;
                coordinates.push_back(currentCoordinates);
				glVertex2i(Xc + currenty , Yc - currentx);
			glEnd();
		}
	}
	patternIndex++;
}

void Circle::draw(int startX,int endX,int startY,int endY,int width,int height)
{
	int currentx=0,currenty,decision_parameter,radius;
	int Xc,Yc;
	startX = startX - width/2;
    startY = height/2 - startY;
    endX = endX - width/2;
    endY = height/2 - endY;
    
//    cout<<"\n\tStartX,StartY:("<<startX<<","<<startY<<")";
//    cout<<"\n\endX,endY:("<<endX<<","<<endY<<")";
    
    vertices.clear();
    coordinates.clear();
    vertices.push_back(make_pair(startX,startY));
    vertices.push_back(make_pair(endX,endY));

    radius = sqrt((endX-startX)*(endX-startX) + (endY - startY)*(endY - startY));
    //cout<<"\n\tRadius: "<<radius<<endl;
	decision_parameter = 1 - radius;

	Xc = startX;
	Yc = startY;
	centerX = Xc;
	centerY = Yc;
	
	
	currenty = radius;

	putPixel(Xc,Yc,currentx,currenty);
	while(currentx < currenty)
	{
		//cout<<"\n\tdecision_parameter: "<<decision_parameter<<endl;
		currentx++;
		if(decision_parameter < 0)
		{
			decision_parameter = decision_parameter + 2*currentx + 2 + 1;
		}
		else
		{
			currenty--;
			decision_parameter = decision_parameter + 2*currentx + 2 + 1 - 2*currenty + 2;
		}
		putPixel(Xc,Yc,currentx,currenty);
	}
	glFlush();
}

