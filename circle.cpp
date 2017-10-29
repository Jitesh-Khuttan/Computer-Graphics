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

void Circle :: printCoordinates()
{
	list< pair<int,int> > :: iterator it;
	for(it = coordinates.begin(); it != coordinates.end();it++)
	{
		cout<<"\n\tCoordinates: ("<<(*it).first<<","<<(*it).second<<")"<<endl;
	}
}

bool Circle :: selectObject(pair<int,int> clickedCoordinates)
{
	cout<<"Inside Select Object Of Circle"<<endl;
	for(list< pair<int,int> >:: iterator it = coordinates.begin(); it != coordinates.end(); it++)
	{
		if(((*it).first >= clickedCoordinates.first - 1 && (*it).first <= clickedCoordinates.first + 1 ) && ((*it).second >= clickedCoordinates.second) - 1 && (*it).second <= clickedCoordinates.second+1)
		{
			reDrawSelectedObject(Color::NAVYBLUE,thickness+2);
			return true;
		}
	}
	return false;
}


void Circle :: reDrawSelectedObject(float* colorToDraw,int thicknessToDraw)
{
	cout<<"Redrawing Selected Circle "<<endl;
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


void Circle :: translateObject(int dx,int dy)
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


void Circle ::rotateObject(int rotationAngle,pair<int,int> pivotPoint) 
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

void Circle ::scaleObject(pair<int,int> scaleValue,pair<int,int> pivotPoint)
{
	list< pair<int,int> > :: iterator it;
	for(it = vertices.begin(); it!= vertices.end();it++)
	{
				(*it).first += (*it).first * scaleValue.first + pivotPoint.first * (1 - scaleValue.first);
				(*it).second += (*it).second * scaleValue.second + pivotPoint.second * (1 - scaleValue.second);
	}
	int startX,startY,endX,endY;
	int counter = 1;

	for(it = vertices.begin(); it!= vertices.end();it++)
	{
		if(counter == 1)
		{
			startX = (*it).first;
			startY = (*it).second;
		}
		else if(counter == 2)
		{
			endX =(*it).first;
			endY = (*it).second;
		}
		counter++;
	}
	reDrawSelectedObject(Color::BLACK,Thickness::THICKNESS10);
	Axis::drawAxis();
	draw(startX + width/2, endX + width/2,height/2 - startY, height/2 - endY,width,height);
}

//void Circle :: fillBoundary(int x,int y,float* fillColor,float* boundaryColor)
//{
//	float interiorColor[3];
//	//cout<<"Reading Pixel from: "<<
//	cout<<"\n\tReading Pixel From Original Coordinates: ("<<x <<","<<y<<")";
//	cout<<"\n\tReading Pixel From Shifted Coordinates: ("<<x - width/2 <<","<<height/2 - y<<")";
//	glReadPixels(x,y,1,1,GL_RGB,GL_FLOAT,interiorColor);
//	cout<<"Interior Color R: "<<(interiorColor[0]*10)<<" G: "<<(interiorColor[1]*10)<<" B: "<<(interiorColor[2])*10<<endl;
//	cout<<"Fill Color Color R: "<<(fillColor[0]*10)<<" G: "<<(fillColor[1]*10)<<" B: "<<(fillColor[2]*10)<<endl;
//	cout<<"Boundary Color R: "<<(boundaryColor[0]*10)<<" G: "<<(boundaryColor[1]*10)<<" B: "<<(boundaryColor[2]*10)<<endl<<endl;
//	if(interiorColor[0] == Color::BLACK[0] && interiorColor[1] == Color::BLACK[1] && interiorColor[2] == Color::BLACK[2] )
//		cout<<"\n\tInterior Color: BLACK";
//	else if(interiorColor[0] == Color::NAVYBLUE[0] && interiorColor[1] == Color::NAVYBLUE[1] && interiorColor[2] == Color::NAVYBLUE[2])
//		cout<<"\n\tInterior Color: NAVY";	
//	
//	list< pair<int,int> >::iterator it;
//	for(it = coordinates.begin();it != coordinates.end();it++)
//	{
//		if(x - width/2 == (*it).first && height/2 - y == (*it).second)
//			cout<<"COORDINATESSSSS MATCHEDDDDDD!";
//	}
//	
////	cout<<"Checking: "<<int(interiorColor[0]*10)<<" with "<<int(fillColor[0]*10)<<endl;
////	if(int(interiorColor[0]*10) != int(fillColor[0]*10))
////	{
////		cout<<"Condition 1 True"<<endl;
////	}
////	
////	cout<<"Checking: "<<int(interiorColor[1]*10) <<" with "<<int(fillColor[1]*10)<<endl;
////	if(int(interiorColor[1]*10) != int(fillColor[1]*10))
////		cout<<"Condition 2 True"<<endl;
////	
////	cout<<"Multiplicaton Result Of"<<interiorColor[2]<<" with "<<10<<" is :"<<interiorColor[2]*10;
////	cout<<"Checking: "<<(interiorColor[2]*10) <<" with "<< int(fillColor[2]*10)<<endl;
////	if(int(interiorColor[2]*10) != int(fillColor[2]*10))
////		cout<<"Condition 3 True"<<endl;
////	
////	cout<<"Checking: "<<int(interiorColor[0]*10) <<" with "<< int(boundaryColor[0]*10)<<endl;
////	if(int(interiorColor[0]*10) != int(boundaryColor[0]*10))
////		cout<<"Condition 4 True"<<endl;
////	
////	cout<<"Checking: "<<int(interiorColor[1]*10) <<" with "<< int(boundaryColor[1]*10)<<endl;
////	if(int(interiorColor[1]*10) != int(boundaryColor[1]*10))
////		cout<<"Condition 5 True"<<endl;
////	
////	cout<<"Checking: "<<int(interiorColor[2]*10) <<" with "<< int(boundaryColor[2]*10)<<endl;	
////	if(int(interiorColor[2]*10) != int(boundaryColor[2]*10))
////		cout<<"Condition 6 True"<<endl;
//
//	if((int(interiorColor[0]*10) != int(fillColor[0]*10) || int(interiorColor[1]*10) != int(fillColor[1]*10) || int(interiorColor[2]*10) != int(fillColor[2]*10)) && (int(interiorColor[0]*10) != int(boundaryColor[0]*10) || int(interiorColor[1]*10) != int(boundaryColor[1]*10) || int(interiorColor[2]*10) != int(boundaryColor[2]*10)))
//	{
//		glColor3fv(fillColor);
//		glPointSize(Thickness::THICKNESS1);
//		glBegin(GL_POINTS);
//			glVertex2i(x - width/2,height/2 - y);
//		glEnd();
//		glFlush();
//		fillBoundary(x+1,y,fillColor,boundaryColor);
//		fillBoundary(x-1,y,fillColor,boundaryColor);
//		fillBoundary(x,y+1,fillColor,boundaryColor);
//		fillBoundary(x,y-1,fillColor,boundaryColor);
//	}
//}

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

void Circle ::setColor(float* color)
{
    this->color = color;
}

void Circle ::setPattern(string pattern)
{
    this->pattern = pattern;
}

void Circle :: setThickness(int thickness)
{
    this->thickness = thickness;
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
    
    vertices.clear();
    vertices.push_back(pair<int,int>(startX,startY));
    vertices.push_back(pair<int,int>(endX,endY));

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

