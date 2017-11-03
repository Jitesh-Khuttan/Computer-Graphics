#include "ellipse.h"
#include "color.h"
#include "axis.h"
#include "thickness.h"
#include "math.h"

extern list<Object*> allObjects;
extern bool clipWindowON;
extern int minX,maxX,minY,maxY;
extern int width,height;



EllipseObject:: EllipseObject(GLubyte* color,int thickness,string pattern)
{
    this->color = color;
    this->thickness = thickness;
    this->pattern = pattern;
    this->patternIndex = 0;
    this->objectName = "Ellipse";
}

void EllipseObject :: reDrawSelectedObject(GLubyte* colorToDraw,int thicknessToDraw)
{
	//cout<<"Redrawing Selected Ellipse "<<endl;
	glColor3ubv(colorToDraw);
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
		glBegin(GL_POINTS);
			glVertex2i((*it).first, (*it).second);
		glEnd();
	}
	glFlush();
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
	glColor3ubv(color);
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
    coordinates.clear();
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

//    cout<<"Center X: "<<Xc<<endl;
//    cout<<"Center Y: "<<Yc<<endl;
//    cout<<"Rx: "<<Rx<<endl;
//    cout<<"Ry: "<<Ry<<endl;


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

