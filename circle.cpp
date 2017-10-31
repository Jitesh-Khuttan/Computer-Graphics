#include "circle.h"
#include "color.h"
#include "axis.h"
#include "thickness.h"
#include <math.h>

extern bool clipWindowON;
extern int minX,maxX,minY,maxY;
extern list<Object*> allObjects;
extern int width,height;

Circle ::Circle(float* color,int thickness,string pattern)
{
    this->color = color;
    this->thickness = thickness;
    this->pattern = pattern;
    this->patternIndex = 0;
    this->objectName = "Circle";
}

void Circle :: reDrawSelectedObject(float* colorToDraw,int thicknessToDraw)
{
	//cout<<"Redrawing Selected Circle "<<endl;
	glColor3fv(colorToDraw);
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
	glFlush();
}



void Circle :: fillBoundary(int x,int y,float* fillColor,float* boundaryColor)
{
	GLubyte interiorColor[3];
	GLubyte tempFillColor[3];
	GLubyte tempBoundaryColor[3];
	tempFillColor[0] = fillColor[0]*255;
	tempFillColor[1] = fillColor[1]*255;
	tempFillColor[2] = fillColor[2]*255;
	
	tempBoundaryColor[0] = boundaryColor[0]*255;
	tempBoundaryColor[1] =  boundaryColor[1]*255;
	tempBoundaryColor[2] = boundaryColor[2]*255;
	
	//cout<<"Reading Pixel from: "<<
	cout<<"\n\tReading Pixel From Original Coordinates: ("<<x <<","<<y<<")";
	cout<<"\n\tReading Pixel From Shifted Coordinates: ("<<x - width/2 <<","<<height/2 - y<<")";
	glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,interiorColor);
	cout<<"Interior Color R: "<<int(interiorColor[0])<<" G: "<<int(interiorColor[1])<<" B: "<<int(interiorColor[2])<<endl;
	cout<<"Fill Color Color R: "<<int(tempFillColor[0])<<" G: "<<int(tempFillColor[1])<<" B: "<<int(tempFillColor[2])<<endl;
	cout<<"Boundary Color R: "<<int(tempBoundaryColor[0])<<" G: "<<int(tempBoundaryColor[1])<<" B: "<<int(tempBoundaryColor[2])<<endl<<endl;
	if(interiorColor[0] == Color::BLACK[0] && interiorColor[1] == Color::BLACK[1] && interiorColor[2] == Color::BLACK[2] )
		cout<<"\n\tInterior Color: BLACK";
	else if(interiorColor[0] == Color::NAVYBLUE[0] && interiorColor[1] == Color::NAVYBLUE[1] && interiorColor[2] == Color::NAVYBLUE[2])
		cout<<"\n\tInterior Color: NAVY";	
	
	list< pair<int,int> >::iterator it;
	for(it = coordinates.begin();it != coordinates.end();it++)
	{
		if(x - width/2 == (*it).first && height/2 - y == (*it).second)
			cout<<"COORDINATESSSSS MATCHEDDDDDD!";
	}
	
//	cout<<"Checking: "<<int(interiorColor[0]*10)<<" with "<<int(fillColor[0]*10)<<endl;
//	if(int(interiorColor[0]*10) != int(fillColor[0]*10))
//	{
//		cout<<"Condition 1 True"<<endl;
//	}
//	
//	cout<<"Checking: "<<int(interiorColor[1]*10) <<" with "<<int(fillColor[1]*10)<<endl;
//	if(int(interiorColor[1]*10) != int(fillColor[1]*10))
//		cout<<"Condition 2 True"<<endl;
//	
//	cout<<"Multiplicaton Result Of"<<interiorColor[2]<<" with "<<10<<" is :"<<interiorColor[2]*10;
//	cout<<"Checking: "<<(interiorColor[2]*10) <<" with "<< int(fillColor[2]*10)<<endl;
//	if(int(interiorColor[2]*10) != int(fillColor[2]*10))
//		cout<<"Condition 3 True"<<endl;
//	
//	cout<<"Checking: "<<int(interiorColor[0]*10) <<" with "<< int(boundaryColor[0]*10)<<endl;
//	if(int(interiorColor[0]*10) != int(boundaryColor[0]*10))
//		cout<<"Condition 4 True"<<endl;
//	
//	cout<<"Checking: "<<int(interiorColor[1]*10) <<" with "<< int(boundaryColor[1]*10)<<endl;
//	if(int(interiorColor[1]*10) != int(boundaryColor[1]*10))
//		cout<<"Condition 5 True"<<endl;
//	
//	cout<<"Checking: "<<int(interiorColor[2]*10) <<" with "<< int(boundaryColor[2]*10)<<endl;	
//	if(int(interiorColor[2]*10) != int(boundaryColor[2]*10))
//		cout<<"Condition 6 True"<<endl;

	if((int(interiorColor[0]) != int(tempFillColor[0]) || int(interiorColor[1]) != int(tempFillColor[1]) || int(interiorColor[2]) != int(tempFillColor[2])) || (int(interiorColor[0]) != int(tempBoundaryColor[0]) || int(interiorColor[1]) != int(tempBoundaryColor[1]) || int(interiorColor[2]) != int(tempBoundaryColor[2])))
	{
		glColor3fv(fillColor);
		glPointSize(Thickness::THICKNESS1);
		glBegin(GL_POINTS);
			glVertex2i(x - width/2,height/2 - y);
		glEnd();
		glFlush();
		fillBoundary(x+1,y,fillColor,boundaryColor);
		fillBoundary(x-1,y,fillColor,boundaryColor);
		fillBoundary(x,y+1,fillColor,boundaryColor);
		fillBoundary(x,y-1,fillColor,boundaryColor);
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
	glColor3fv(color);
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

