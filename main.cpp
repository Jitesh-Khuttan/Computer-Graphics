#include<bits/stdc++.h>
#include<GL/glut.h>
#include "object.h"
#include "simpleDDA.h"
#include "symmetricDDA.h"
#include "bresenham.h"
#include "midpoint.h"
#include "circle.h"
#include "ellipse.h"
#include "bezier.h"
#include "cohensutherland.h"
#include "liang.h"
#include "pattern.h"
#include "color.h"
#include "thickness.h"
#include "axis.h"
#include "ngon.h"
using namespace std;

list<Object*> allObjects;
GLubyte *CURRENTCOLOR = Color::RED;
string CURRENTPATTERN = Pattern::HEX_15;
int CURRENTTHICKNESS = Thickness::THICKNESS1;

static int scaleSubMenu,thickSubMenu,mainMenu,patternSubMenu,colorSubMenu,angleSubMenu,
fillMenus,fillSubMenu4,fillSubMenu8,ngonSubMenu,transformationSubMenu,drawingSubMenu,clipSubMenu,curveSubMenu,bezierSubMenu;
int currentAlgo = 0;

int clipHeight=0,clipWidth=0,tempClipHeight,tempClipWidth;
int minX,minY,maxX,maxY;
bool clipWindowON = false;
bool cohenLineClipAlgo=false,liangLineClipAlgo=false;
int tempSX,tempSY,tempEX,tempEY;
int rotationAngle=0;
int ngonRadius;
pair<int,int> scalingValue;

pair<int,int> selectedCoordinates;
pair<int,int> translateCoordinates;
pair<int,int> pivotPoint = make_pair(0,0);
list< pair<int,int> > controlPoints;
pair<int,int> seedPoint;
GLubyte* fillColor;
Object* selectedObject;
bool boundaryFill = false;


GLsizei width = 1000, height = 800;
GLint clickCount = 0;
GLint startX,startY,endX,endY;
int currentX,currentY;

void printTotalObjects()
{
	cout<<"\n\tTotal Objects: "<<allObjects.size()<<endl;
	list<Object*>::iterator it;
	for(it = allObjects.begin();it!= allObjects.end();it++)
	{
		cout<<(*it)->objectName<<endl;
		(*it)->printCoordinates();
	}
}

void initGLUT()
{
	//set background color
	glClearColor(0.0,0.0,0.0,1.0); 
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	//gluOrtho2D(-800.0,800.0,-1000.0,1000.0);
	glLoadIdentity();
	gluOrtho2D(-(GLdouble)width/2,(GLdouble)width/2,-(GLdouble)height/2,(GLdouble)height/2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Axis::drawAxis();
	glFlush();

}

void createClipWindow()
{
	initGLUT();
	glColor3ubv(Color::NAVYBLUE);
	glLineWidth(Thickness::THICKNESS2);

	//int tempSX,tempSY,tempEX,tempEY;
	tempSX = startX - width/2;
    tempSY = height/2 - startY;
    tempEX = endX - width/2;
    tempEY = height/2 - endY;
	minX = min(tempSX,tempEX);
	maxX = max(tempSX,tempEX);
	minY = min(tempSY,tempEY);
	maxY = max(tempSY,tempEY);

	glBegin(GL_LINES);
		glVertex2i(minX,minY);
		glVertex2i(maxX,minY);

		glVertex2i(maxX,minY);
		glVertex2i(maxX,maxY);

		glVertex2i(maxX,maxY);
		glVertex2i(minX,maxY);

		glVertex2i(minX,maxY);
		glVertex2i(minX,minY);

		tempClipHeight = clipHeight;
		tempClipWidth = clipWidth;
	glEnd();
	glFlush();
}


void clipPoint(int X,int Y)
{
	//cout<<"\n\tGetting Plotted by algo";
	if(X - width/2 > minX && X - width/2 < maxX && height/2 - Y > minY && height/2 - Y < maxY)
	{
		glBegin(GL_POINTS);
			glVertex2i(X - width/2,height/2 - Y);
		glEnd();
		glFlush();
	}
}

void display()
{
  	bool result;
  	if(currentAlgo == 8)
  	{
  		cout<<"Inside Current Algo = 8"<<endl;
  		selectedCoordinates.first = currentX - width/2;
    	selectedCoordinates.second = height/2 - currentY;
    	
    	cout<<"Selected Coordinates: ("<<selectedCoordinates.first<<","<<selectedCoordinates.second<<")"<<endl;
    	
  		list<Object*>::iterator it;
  		int counter = 0;
  		for(it = allObjects.begin();it != allObjects.end(); it++)
  		{
  			result = (*it)->selectObject(selectedCoordinates);
  			cout<<"Result: "<<result<<endl;
  			if(result)
  			{
  				selectedObject = *it;
  				cout<<"\n\tSelected Object: "<<selectedObject->objectName;
  				cout<<"\n\tObject Number: "<<counter;
				break;	
			}
			counter++;
		}
	}
	
	if(currentAlgo == 9)
	{
		currentAlgo = 0;
		clickCount = 0;
		int dx,dy;
		translateCoordinates.first = currentX - width/2;
		translateCoordinates.second = height/2 - currentY;
		dx = translateCoordinates.first - selectedCoordinates.first;
		dy = translateCoordinates.second - selectedCoordinates.second;
		selectedObject->thickness = CURRENTTHICKNESS;
		selectedObject->color = CURRENTCOLOR;
		selectedObject->translateObject(dx,dy);
	}
	else if(currentAlgo == 10)
	{
		pivotPoint.first = currentX - width/2;
    	pivotPoint.second = height/2 - currentY;
    	cout<<"\n\tPivot Points: "<<pivotPoint.first<<" "<<pivotPoint.second;
	}
	else if(currentAlgo >= 31 && currentAlgo <= 35)
	{
//		cout<<"\n\tglReadPixel starts reading from the bottom left corner";
		seedPoint.first = currentX;
		seedPoint.second = currentY;
    	selectedObject->fillBoundary(seedPoint.first,seedPoint.second,fillColor,Color::NAVYBLUE,1);
    	cout<<"\n\tFigure Filled!";
    	glFlush();
    	
    	
	}
	else if(currentAlgo >= 62 && currentAlgo <= 66)
	{
//		cout<<"\n\tglReadPixel starts reading from the bottom left corner";
		seedPoint.first = currentX;
		seedPoint.second = currentY;
    	selectedObject->fillBoundary(seedPoint.first,seedPoint.second,fillColor,Color::NAVYBLUE,2);
    	cout<<"\n\tFigure Filled!";
    	glFlush();
	}
	else if(currentAlgo >= 51 && currentAlgo <= 58 )
	{
		currentAlgo = 0;
		clickCount = 0;
		selectedObject->thickness = CURRENTTHICKNESS;
		selectedObject->color = CURRENTCOLOR;
		selectedObject->rotateObject(rotationAngle,pivotPoint);
	}
	else if(currentAlgo >= 59 && currentAlgo <= 61 )
	{
		currentAlgo = 0;
		clickCount = 0;
		selectedObject->thickness = CURRENTTHICKNESS;
		selectedObject->color = CURRENTCOLOR;
		selectedObject->scaleObject(scalingValue,pivotPoint);
	}
	else if(currentAlgo == 71)
	{
		Bezier *curve = new Bezier(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
		curve->drawCurve(controlPoints);
		allObjects.push_back(curve);
	}
	
	if(clickCount%2 == 0)
	{
		switch(currentAlgo)
		{
			case 1:
				if(clipWindowON == true)    //TODO Create an object of SimpleDDA in Cohen and Liang files
				{
					if(cohenLineClipAlgo == true)
						Cohen::clipLine(startX,endX,startY,endY,width,height,minX,maxX,minY,maxY,1);
					else if(liangLineClipAlgo == true)
						Liang::clipLineLiang(startX,endX,startY,endY,width,height,minX,maxX,minY,maxY,1);
				}
				else
				{
                    
                    SimpleDDA *simpleLine = new SimpleDDA(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
                    cout<<"Drawing a simple Line"<<endl;
                    simpleLine->draw(startX,endX,startY,endY,width,height);
                    allObjects.push_back(simpleLine);
                }
                //printTotalObjects();
				break;

			case 2:
				if(clipWindowON == true)
				{
					if(cohenLineClipAlgo == true)
						Cohen::clipLine(startX,endX,startY,endY,width,height,minX,maxX,minY,maxY,2);
					else if(liangLineClipAlgo == true)
						Liang::clipLineLiang(startX,endX,startY,endY,width,height,minX,maxX,minY,maxY,2);

				}
				else
				{
                    cout<<"Drawing a symmetric Line"<<endl;
					SymmetricDDA *symmetricLine = new SymmetricDDA(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
                    symmetricLine->draw(startX,endX,startY,endY,width,height);
                    allObjects.push_back(symmetricLine);
                }
                //printTotalObjects();
				break;
			case 3:
				if(clipWindowON == true)
				{
					if(cohenLineClipAlgo == true)
						Cohen::clipLine(startX,endX,startY,endY,width,height,minX,maxX,minY,maxY,3);
					else if(liangLineClipAlgo == true)
						Liang::clipLineLiang(startX,endX,startY,endY,width,height,minX,maxX,minY,maxY,3);

				}
				else
				{
                    cout<<"Drawing a bresenham Line"<<endl;
                    Bresenham *bresenhamLine = new Bresenham(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
                    bresenhamLine->draw(startX,endX,startY,endY,width,height);
                    allObjects.push_back(bresenhamLine);
                }
               // printTotalObjects();
				break;

			case 4:
				if(clipWindowON == true)
				{
					if(cohenLineClipAlgo == true)
						Cohen::clipLine(startX,endX,startY,endY,width,height,minX,maxX,minY,maxY,4);
					else if(liangLineClipAlgo == true)
						Liang::clipLineLiang(startX,endX,startY,endY,width,height,minX,maxX,minY,maxY,4);

				}
				else
				{
                    cout<<"Drawing a midpoint Line"<<endl;
					MidPoint *midLine = new MidPoint(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
                    midLine->draw(startX,endX,startY,endY,width,height);
                    allObjects.push_back(midLine);
                }
                //printTotalObjects();
				break;

			case 5:
            {
            	cout<<"Drawing a circle"<<endl;
                Circle *circle = new Circle(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
                circle->draw(startX,endX,startY,endY,width,height);
                allObjects.push_back(circle);
                //printTotalObjects();
				break;
			}

			case 6:
            {
            	cout<<"Drawing a ellipse"<<endl;
                EllipseObject *ell = new EllipseObject(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
                ell->draw(startX,endX,startY,endY,width,height);
                allObjects.push_back(ell);
                //printTotalObjects();
                break;
       		}

			case 7:
			{
				clipHeight = abs(endY - startY);
				clipWidth = abs(endX - startX);
				clipWindowON = true;
				createClipWindow();
				break;
			}
			case 36:
			{
				
//				cout<<"Calling NGON"<<endl;
				Ngon *ngon = new Ngon(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
				ngon->setVertices(5);
				ngon->setRotationAngle(360/5.0);
				ngon->draw(startX,endX,startY,endY,width,height);
				allObjects.push_back(ngon);
				break;
			}
			case 37:
			{
				
//				cout<<"Calling NGON"<<endl;
				Ngon *ngon = new Ngon(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
				ngon->setVertices(15);
				ngon->setRotationAngle(360/15.0);
				ngon->draw(startX,endX,startY,endY,width,height);
				allObjects.push_back(ngon);
				break;
			}
			case 38:
			{
//				cout<<"Calling NGON"<<endl;
				Ngon *ngon = new Ngon(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
				ngon->setVertices(20);
				ngon->setRotationAngle(360/20.0);
				ngon->draw(startX,endX,startY,endY,width,height);
				allObjects.push_back(ngon);
				break;
			}
			case 39:
			{
//				cout<<"Calling NGON"<<endl;
				Ngon *ngon = new Ngon(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
				ngon->setVertices(25);
				ngon->setRotationAngle(360/25.0);
				ngon->draw(startX,endX,startY,endY,width,height);
				allObjects.push_back(ngon);
				break;
			}
			case 40:
			{
//				cout<<"Calling NGON"<<endl;
				Ngon *ngon = new Ngon(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
				ngon->setVertices(30);
				ngon->setRotationAngle(360/30.0);
				ngon->draw(startX,endX,startY,endY,width,height);
				allObjects.push_back(ngon);
				break;
			}
        }
	}
	glFlush();
}

void mouseClicked(GLint button,GLint state,GLint x,GLint y)
{
	currentX = x;
	currentY = y;
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && currentAlgo!=0 )
	{
		if(currentAlgo == 70)
		{
			controlPoints.push_back(make_pair(currentX - width/2,height/2- currentY));
		}
		glColor3f(.9,.4,.6);
		glPointSize(2.0);
		++clickCount;
		if(clickCount%2 == 1)
		{
			startX = x;
			startY = y;
			if(!clipWindowON && currentAlgo != 8 && currentAlgo!=9 && currentAlgo != 10 && !boundaryFill)
			{
				//cout<<"\n\tPlotting at: (startX/startY) ClipWindowOFF"<<startX<<"/"<<startY;
				glBegin(GL_POINTS);
					glVertex2i(startX - width/2,height/2 - startY);
				glEnd();
				glFlush();
			}
			else
			{
				clipPoint(startX,startY);
			}
		}
		else if(clickCount%2 == 0)
		{
			endX = x;
			endY = y;
			if(!clipWindowON && currentAlgo != 8 && currentAlgo!=9 && currentAlgo != 10 && !boundaryFill)
			{
				//cout<<"\n\tPlotting at: (endX/endY) ClipWindowOFF"<<endX<<"/"<<endY;
				glBegin(GL_POINTS);
					glVertex2i(endX - width/2,height/2 - endY);
				glEnd();
				glFlush();
			}	
			else
			{
				clipPoint(endX,endY);
			}
		}
		display();
	}
	//cout<<"Click Count: "<<clickCount<<endl;

}

void menuCallback(int value)
{
	clickCount = 0;
    if(value == -1)
    {
    	allObjects.clear();
        initGLUT();
		currentAlgo = 0;
		clickCount = 0;
		clipWindowON = false;
		boundaryFill = false;
    }
	else if(value >= 1 && value <= 13)
	{
        currentAlgo = value;
        boundaryFill = false;
	}
	else if( value >= 51 && value <= 58)
	{
		currentAlgo = value;
		boundaryFill = false;
		switch(value)
		{
			case 51:
				rotationAngle = 30;
				break;
			case 52:
				rotationAngle = 60;
				break;
			case 53:
				rotationAngle = 90;
				break;
			case 54:
				rotationAngle = 120;
				break;
			case 55:
				rotationAngle = 150;
				break;
			case 56:
				rotationAngle = 180;
				break;
			case 57:
				rotationAngle = 270;
				break;
			case 58:
				rotationAngle = 360;
				break;

		}
		display();
	}
	else if(value >=59 && value<= 61)
	{
		currentAlgo = value;
		switch(value)
		{
			case 59:
				scalingValue.first = 2;
				scalingValue.second = 2;
				break;
			case 60:
				scalingValue.first = 4;
				scalingValue.second = 4;
				break;
			case 61:
				scalingValue.first = 6;
				scalingValue.second = 6;
				break;	
		}
		display();
	}
	else if(value >= 31 && value <= 35)
	{
		boundaryFill = true;
		currentAlgo = value;
		switch(value)
		{
			case 31:
				fillColor = Color::RED;
				break;
			case 32:
				fillColor = Color::GREEN;
				break;
			case 33:
				fillColor = Color::BLUE;
				break;
			case 34:
				fillColor = Color::GRAY;
				break;
			case 35:
				fillColor = Color::YELLOW;
				break;
		}
	}
	else if(value >= 62 && value <= 66)
	{
		boundaryFill = true;
		currentAlgo = value;
		switch(value)
		{
			case 62:
				fillColor = Color::RED;
				break;
			case 63:
				fillColor = Color::GREEN;
				break;
			case 64:
				fillColor = Color::BLUE;
				break;
			case 65:
				fillColor = Color::GRAY;
				break;
			case 66:
				fillColor = Color::YELLOW;
				break;
		}
	}
	else if(value == 14)
	{
        cohenLineClipAlgo = true;
        liangLineClipAlgo = false;
        boundaryFill = false;
	}
	else if(value == 15)
	{
        cohenLineClipAlgo = false;
        liangLineClipAlgo = true;
        boundaryFill = false;
	}
	else if(value >= 16 && value <= 20) //Change Thickness
	{
		boundaryFill = false;
        if(value == 16)
            CURRENTTHICKNESS = Thickness ::THICKNESS1;
        else if(value == 17)
            CURRENTTHICKNESS = Thickness ::THICKNESS2;
        else if(value == 18)
            CURRENTTHICKNESS = Thickness ::THICKNESS3;
        else if(value == 19)
            CURRENTTHICKNESS = Thickness ::THICKNESS4;
        else if(value == 20)
            CURRENTTHICKNESS = Thickness ::THICKNESS5;
	}
	else if(value >=21 && value <=25) //Change Color
	{
		boundaryFill = false;
        if(value == 21)
            CURRENTCOLOR = Color::RED;
        else if(value == 22)
            CURRENTCOLOR = Color::GREEN;
        else if(value == 23)
            CURRENTCOLOR = Color::BLUE;
        else if(value == 24)
            CURRENTCOLOR = Color::GRAY;
        else if(value == 25)
            CURRENTCOLOR = Color::YELLOW;
	}
	else if(value >=26 && value<=30)    //Change Pattern
	{
		boundaryFill = false;
        if(value == 26)
            CURRENTPATTERN = Pattern::HEX_5;
        else if(value == 27)
            CURRENTPATTERN = Pattern::HEX_8;
        else if(value == 28)
            CURRENTPATTERN = Pattern::HEX_10;
        else if(value == 29)
            CURRENTPATTERN = Pattern::HEX_12;
        else if(value == 30)
            CURRENTPATTERN = Pattern::HEX_15;
	}
	else if(value>=36 && value <= 40)
	{
		boundaryFill = false;
		currentAlgo = value;
	}
	else if(value>=70 && value<=71)
	{
		boundaryFill = false;
		currentAlgo = value;
		if(currentAlgo == 70)
			controlPoints.clear();
		else if(currentAlgo == 71)
			display();
	}

}

void createMenu()
{

	thickSubMenu = glutCreateMenu(menuCallback);
	glutAddMenuEntry("1",16);
	glutAddMenuEntry("2",17);
	glutAddMenuEntry("3",18);
	glutAddMenuEntry("4",19);
	glutAddMenuEntry("5",20);


	colorSubMenu = glutCreateMenu(menuCallback);
	glutAddMenuEntry("Red",21);
	glutAddMenuEntry("Green",22);
	glutAddMenuEntry("Blue",23);
	glutAddMenuEntry("Gray",24);
	glutAddMenuEntry("Yellow",25);

	patternSubMenu = glutCreateMenu(menuCallback);
	glutAddMenuEntry("Hex 5",26);
	glutAddMenuEntry("Hex 8",27);
	glutAddMenuEntry("Hex 10",28);
	glutAddMenuEntry("Hex 12",29);
	glutAddMenuEntry("Hex 15",30);
	
	fillSubMenu4 = glutCreateMenu(menuCallback);
	glutAddMenuEntry("Red",31);
	glutAddMenuEntry("Green",32);
	glutAddMenuEntry("Blue",33);
	glutAddMenuEntry("Gray",34);
	glutAddMenuEntry("Yellow",35);
	
	ngonSubMenu = glutCreateMenu(menuCallback);
	glutAddMenuEntry("5",36);
	glutAddMenuEntry("15",37);
	glutAddMenuEntry("20",38);
	glutAddMenuEntry("25",39);
	glutAddMenuEntry("30",40);
		
	angleSubMenu = glutCreateMenu(menuCallback);
	glutAddMenuEntry("30",51);
	glutAddMenuEntry("60",52);
	glutAddMenuEntry("90",53);
	glutAddMenuEntry("120",54);
	glutAddMenuEntry("150",55);
	glutAddMenuEntry("180",56);
	glutAddMenuEntry("270",57);
	glutAddMenuEntry("360",58);
	
	scaleSubMenu = glutCreateMenu(menuCallback);
	glutAddMenuEntry("2",59);
	glutAddMenuEntry("4",60);
	glutAddMenuEntry("6",61);
	
	fillSubMenu8 = glutCreateMenu(menuCallback);
	glutAddMenuEntry("Red",62);
	glutAddMenuEntry("Green",63);
	glutAddMenuEntry("Blue",64);
	glutAddMenuEntry("Gray",65);
	glutAddMenuEntry("Yellow",66);
	
	transformationSubMenu = glutCreateMenu(menuCallback);
	glutAddMenuEntry("Select Object",8);
	glutAddMenuEntry("Translate Object",9);
	glutAddMenuEntry("Choose Pivot Point",10);
	glutAddSubMenu("Rotate Object",angleSubMenu);
	glutAddSubMenu("Scale Object",scaleSubMenu);
	
	drawingSubMenu = glutCreateMenu(menuCallback);
	glutAddMenuEntry("Simple DDA",1);
	glutAddMenuEntry("Symmetric DDA",2);
	glutAddMenuEntry("Bresenham",3);
	glutAddMenuEntry("MidPoint",4);
	glutAddMenuEntry("Circle Drawing",5);
	glutAddMenuEntry("Ellipse Drawing",6);
	glutAddSubMenu("N-Gon",ngonSubMenu);
	
	clipSubMenu = glutCreateMenu(menuCallback);
	glutAddMenuEntry("Clip Window",7);
	glutAddMenuEntry("Cohen Line Clipping",14);
	glutAddMenuEntry("Liang Line Clipping",15);
	
	fillMenus = glutCreateMenu(menuCallback);
	glutAddSubMenu("4 Boundary Fill",fillSubMenu4);
	glutAddSubMenu("8 Boundary Fill",fillSubMenu8);
	
	bezierSubMenu = glutCreateMenu(menuCallback);
	glutAddMenuEntry("Select Control Points",70);
	glutAddMenuEntry("Draw Curve",71);
	
	curveSubMenu = glutCreateMenu(menuCallback);
	glutAddSubMenu("Bezier Curve",bezierSubMenu);
	
	
	mainMenu = glutCreateMenu(menuCallback);
	glutAddSubMenu("Drawing Operations",drawingSubMenu);	
	glutAddSubMenu("Tranformation Operations",transformationSubMenu);
	glutAddSubMenu("Curve Generation",curveSubMenu);
	glutAddSubMenu("Clip Operations",clipSubMenu);
	glutAddSubMenu("Filling Operations",fillMenus);
	glutAddSubMenu("Thickness",thickSubMenu);
	glutAddSubMenu("Patterns",patternSubMenu);
	glutAddSubMenu("Color",colorSubMenu);
	glutAddMenuEntry("Erase All",-1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void resize(int changedWidth,int changedHeight)
{
	width = changedWidth;
	height = changedHeight;
	initGLUT();
	list<Object*>::iterator it;
	for(it = allObjects.begin();it != allObjects.end();it++)
	{
		(*it)->reDrawSelectedObject((*it)->color,(*it)->thickness);
	}
}

int main(int argc,char* argv[])
{
    //cout<<"LIST OF OBJECTS DECLARED!"<<endl;
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width,height);
	glutCreateWindow("Line Algorithms");
	glutInitWindowPosition(400,400);
	glutDisplayFunc(initGLUT);
	glutReshapeFunc(resize);
	glutMouseFunc(mouseClicked);
	createMenu();
	glutMainLoop();
	return 0;
}
