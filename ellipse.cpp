#include "ellipse.h"
#include "color.h"
#include "axis.h"
#include "thickness.h"
#include "math.h"

extern list<Object*> allObjects;
extern bool clipWindowON;
extern int minX,maxX,minY,maxY;
extern int width,height;



EllipseObject:: EllipseObject(float* color,int thickness,string pattern)
{
    this->color = color;
    this->thickness = thickness;
    this->pattern = pattern;
    this->patternIndex = 0;
    this->objectName = "Ellipse";
}

void EllipseObject :: printCoordinates()
{
	list< pair<int,int> > :: iterator it;
	for(it = coordinates.begin(); it != coordinates.end();it++)
	{
		cout<<"\n\tCoordinates: ("<<(*it).first<<","<<(*it).second<<")"<<endl;
	}
}

bool EllipseObject :: selectObject(pair<int,int> clickedCoordinates)
{
	cout<<"Inside Select Object Of Ellipse"<<endl;
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


void EllipseObject :: reDrawSelectedObject(float* colorToDraw,int thicknessToDraw)
{
	cout<<"Redrawing Selected Ellipse "<<endl;
	glColor3fv(colorToDraw);
	glPointSize(Thickness::THICKNESS10);
	if(colorToDraw == Color::BLACK)
	{
		glBegin(GL_POINTS);
			glVertex2i(centerX,centerY);
			glVertex2i(endX,endY);
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


void EllipseObject :: translateObject(int dx,int dy)
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
void EllipseObject ::setColor(float* color)
{
    this->color = color;
}

void EllipseObject ::setPattern(string pattern)
{
    this->pattern = pattern;
}

void EllipseObject :: setThickness(int thickness)
{
    this->thickness = thickness;
}


void EllipseObject ::rotateObject(int rotationAngle,pair<int,int> pivotPoint) 
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


void EllipseObject ::scaleObject(pair<int,int> scaleValue,pair<int,int> pivotPoint)
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



void EllipseObject::clipPointsOfEllipse(int X,int Y)
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

void EllipseObject::putEllipsePixel(int Xc,int Yc,int currentx,int currenty)
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
				clipPointsOfEllipse(Xc + currentx,Yc + currenty);
				clipPointsOfEllipse(Xc - currentx, Yc + currenty);
				clipPointsOfEllipse(Xc - currentx,Yc - currenty);
				clipPointsOfEllipse(Xc + currentx, Yc - currenty);
			glEnd();
		}
		else
		{
            glBegin(GL_POINTS);
                currentCoordinates.first = Xc + currentx;
                currentCoordinates.second = Yc + currenty;
                coordinates.push_back(currentCoordinates);
                //cout<<"\n\tPlotting Ellipse Point at: ("<<currentCoordinates.first<<","<<currentCoordinates.second<<")"<<endl;
                glVertex2i(Xc + currentx,Yc + currenty);

                currentCoordinates.first = Xc - currentx;
                currentCoordinates.second = Yc + currenty;
                coordinates.push_back(currentCoordinates);
                //cout<<"\n\tPlotting Ellipse Point at: ("<<currentCoordinates.first<<","<<currentCoordinates.second<<")"<<endl;
                glVertex2i(Xc - currentx, Yc + currenty);

                currentCoordinates.first = Xc - currentx;
                currentCoordinates.second = Yc - currenty;
                coordinates.push_back(currentCoordinates);
                //cout<<"\n\tPlotting Ellipse Point at: ("<<currentCoordinates.first<<","<<currentCoordinates.second<<")"<<endl;
                glVertex2i(Xc - currentx,Yc - currenty);

                currentCoordinates.first = Xc + currentx;
                currentCoordinates.second = Yc - currenty;
                coordinates.push_back(currentCoordinates);
                //cout<<"\n\tPlotting Ellipse Point at: ("<<currentCoordinates.first<<","<<currentCoordinates.second<<")"<<endl;
                glVertex2i(Xc + currentx, Yc - currenty);
            glEnd();
        }
	}
	patternIndex++;
}

void EllipseObject::draw(int startX,int endX,int startY,int endY,int width,int height)
{
	int currentx=0,currenty,Rx,Ry;
	int Rx_square,Ry_square,Rx_square_2,Ry_square_2;
	int decision_parameter,decision_parameter_x,decision_parameter_y;
	int Xc,Yc;

	startX = startX - width/2;
    startY = height/2 - startY;
    endX = endX - width/2;
    endY = height/2 - endY;
    
    vertices.clear();
    vertices.push_back(pair<int,int>(startX,startY));
    vertices.push_back(pair<int,int>(endX,endY));
//	startX = 0;
//	startY = 0;
//	Rx = 10;
//	Ry = 20;

    cout<<"startX : "<<startX<<endl;
    cout<<"startY : "<<startY<<endl;
    cout<<"endX : "<<endX<<endl;
    cout<<"endY : "<<endY<<endl;

    Xc = startX; //X center
    Yc = startY; //Y center
    Rx = abs(endX - startX);
    Ry = abs(endY - startY);
    
    this->centerX = Xc;
	this->centerY = Yc;
	this->endX = endX;
	this->endY = endY; 

    cout<<"Center X: "<<Xc<<endl;
    cout<<"Center Y: "<<Yc<<endl;
    cout<<"Rx: "<<Rx<<endl;
    cout<<"Ry: "<<Ry<<endl;


    Rx_square = Rx*Rx;
    Ry_square = Ry*Ry;

    Rx_square_2 = 2*Rx_square;
    Ry_square_2 = 2*Ry_square;

    currenty = Ry;
//	decision_parameter_x = 0;
//	decision_parameter_y = currenty *  Rx_square_2;

    putEllipsePixel(Xc,Yc,currentx,currenty);

	//Coordinates For Region 1
	decision_parameter = round(Ry_square - (Rx_square*Ry)+(0.25*Rx_square));

    while(2*Ry_square*currentx < 2*Rx_square*currenty)
    {
//    	cout<<"Decision Parameter: "<<decision_parameter<<endl;
//		cout<<"Decision Parameter x: "<<decision_parameter_x<<endl;
//		cout<<"Decision Parameter y: "<<decision_parameter_y<<endl;
    	currentx++;
    	decision_parameter_x = Ry_square_2*currentx;
		if(decision_parameter < 0)
			 decision_parameter += 2*Ry_square*currentx + Ry_square;
		else
		{
			currenty--;
			//decision_parameter_y = Rx_square_2*currenty;
			decision_parameter += 2*Ry_square*currentx + Ry_square - 2*Rx_square*currenty;
		}
		putEllipsePixel(Xc,Yc,currentx,currenty);
	}

	//Coordinates For Region 2
	decision_parameter = round(Ry_square * (currentx + 0.5)*(currentx+0.5) + Rx_square*(currenty-1)*(currenty-1) - Rx_square*Ry_square);
	while(currenty >= 0)
	{
		currenty--;
		//decision_parameter_y -= Rx_square_2;
		if(decision_parameter < 0)
		{
			currentx++;
			decision_parameter += 2*Ry_square*currentx - 2*Rx_square*currenty + Rx_square;
		}
		else
		{
			decision_parameter += -2*Rx_square*currenty + Rx_square;
		}
		putEllipsePixel(Xc,Yc,currentx,currenty);
	}
}

