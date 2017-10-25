#include "liang.h"


extern list<Object*> allObjects;
extern float *CURRENTCOLOR;
extern string CURRENTPATTERN;
extern int CURRENTTHICKNESS;

void Liang::clipLineLiang(int startX,int endX,int startY,int endY,int width,int height,int minX,int maxX,int minY,int maxY,int algoType)
{
	int dx, dy;
	int P1, P2, P3, P4, Q1, Q2, Q3, Q4;
	float R1, R2, R3, R4, U1, U2;
	float temp;

	//cout << "\n\nXVMin: " << XVMin << "\tYVMin: " << YVMin << "\tXVMax: " << XVMax << "\tYVMax: " << YVMax << endl;

	startX = startX - width/2;
    startY = height/2 - startY;
    endX = endX - width/2;
    endY = height/2 - endY;

	dx = endX - startX;
	dy = endY - startY;

	P1 = -dx;
	P2 = dx;
	P3 = -dy;
	P4 = dy;

	Q1 = startX - minX;
	Q2 = maxX - startX;
	Q3 = startY - minY;
	Q4 = maxY - startY;

	cout << "P1: " << P1 << "\tP2: " << P2 << "\tP3: " << P3 << "\tP4: " << P4 << endl;
	cout << "Q1: " << Q1 << "\tQ2: " << Q2 << "\tQ3: " << Q3 << "\tQ4: " << Q4 << endl;

	// Vertical line
	if(P1 == 0 && P2 == 0 && (Q1 < 0 || Q2 < 0))
	{
		cout << "P1-P2 Case" << endl;
		cout << "Full Line Clipped" << endl;
		//return false;
	}

	// Horizontal Line
	if(P3 == 0 && P4 == 0 && (Q3 < 0 || Q4 < 0))
	{
		cout << "P3-P4 Case" << endl;
		cout << "Full Line Clipped" << endl;
		//return false;
	}

	// Calculating values of R
	if(P1 < 0 && P3 < 0)
	{
		cout << "IF: (P1 - P3)" << endl;
		R1 = (float)Q1 / (float)P1;
		R2 = (float)Q3 / (float)P3;

		cout << "R1: " << R1 << "\tR2: " << R2 << endl;

		// Minimum of R values
		temp = (R1 > R2) ? R1 : R2;
		U1 = (temp > 0) ? temp : 0;
		cout << "U1: " << U1 << endl;
	}
	else if(P1 >= 0 && P3 >= 0)
	{
		cout << "ELSE: (P1 - P3)" << endl;
		R3 = (float)Q1 / (float)P1;
		R4 = (float)Q3 / (float)P3;

		cout << "R3: " << R3 << "\tR4: " << R4 << endl;

		// Minimum of R values
		temp = (R3 > R4) ? R4 : R3;
		U2 = (temp > 1) ? 1 : temp;
		cout << "U2: " << U2 << endl;
	}

	// Calculating values of R
	if(P2 < 0 && P4 < 0)
	{
		cout << "IF: (P2 - P4)" << endl;
		R1 = (float)Q2 / (float)P2;
		R2 = (float)Q4 / (float)P4;

		cout << "R1: " << R1 << "\tR2: " << R2 << endl;

		// Minimum of R values
		temp = (R1 > R2) ? R1 : R2;
		U1 = (temp > 0) ? temp : 0;
		cout << "U1: " << U1 << endl;
	}
	else if(P2 >= 0 && P4 >= 0)
	{
		cout << "ELSE: (P2 - P4)" << endl;
		R3 = (float)Q2 / (float)P2;
		R4 = (float)Q4 / (float)P4;

		cout << "R3: " << R3 << "\tR4: " << R4 << endl;

		// Minimum of R values
		temp = (R3 > R4) ? R4 : R3;
		U2 = (temp > 1) ? 1 : temp;
		cout << "U2: " << U2 << endl;
	}

	// Calculating values of R
	if(P1 < 0 && P4 < 0)
	{
		cout << "IF: (P1 - P4)" << endl;
		R1 = (float)Q1 / (float)P1;
		R2 = (float)Q4 / (float)P4;

		cout << "R1: " << R1 << "\tR2: " << R2 << endl;

		// Minimum of R values
		temp = (R1 > R2) ? R1 : R2;
		U1 = (temp > 0) ? temp : 0;
		cout << "U1: " << U1 << endl;
	}
	else if(P1 >= 0 && P4 >= 0)
	{
		cout << "ELSE: (P1 - P4)" << endl;
		R3 = (float)Q1 / (float)P1;
		R4 = (float)Q4 / (float)P4;

		cout << "R3: " << R3 << "\tR4: " << R4 << endl;

		// Minimum of R values
		temp = (R3 > R4) ? R4 : R3;
		U2 = (temp > 1) ? 1 : temp;
		cout << "U2: " << U2 << endl;
	}

	// Calculating values of R
	if(P2 < 0 && P3 < 0)
	{
		cout << "IF: (P2 - P3)" << endl;
		R1 = (float)Q2 / (float)P2;
		R2 = (float)Q3 / (float)P3;

		cout << "R1: " << R1 << "\tR2: " << R2 << endl;

		// Minimum of R values
		temp = (R1 > R2) ? R1 : R2;
		U1 = (temp > 0) ? temp : 0;
		cout << "U1: " << U1 << endl;
	}
	else if(P2 >= 0 && P3 >= 0)
	{
		cout << "ELSE: (P2 - P3)" << endl;
		R3 = (float)Q2 / (float)P2;
		R4 = (float)Q3 / (float)P3;

		cout << "R3: " << R3 << "\tR4: " << R4 << endl;

		// Minimum of R values
		temp = (R3 > R4) ? R4 : R3;
		U2 = (temp > 1) ? 1 : temp;
		cout << "U2: " << U2 << endl;
	}

	if(U1 > U2)
	{
//		cout << "U Case." << endl;
		//return false;
	}

	if(U2 < 1)
	{
		endX = (float)startX + ((float)U2 * (float)dx);
		endY = (float)startY + ((float)U2 * (float)dy);
	}
	if(U1 > 0)
	{
		startX = (float)startX + ((float)U1 * (float)dx);
		startY = (float)startY + ((float)U1 * (float)dy);
	}

	//cout << "X1: " << startX << "\tY1: " << YCoord1 << "\tX2: " << endX << "\tY2: " << endY << endl;

	if(algoType == 1)
    {
        SimpleDDA *simpleLine = new SimpleDDA(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
        simpleLine->draw(startX + width/2, endX + width/2,height/2 - startY, height/2 - endY,width,height);
        allObjects.push_back(simpleLine);
    }
    else if(algoType == 2)
    {
        SymmetricDDA *symmetricLine = new SymmetricDDA(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
        symmetricLine->draw(startX + width/2, endX + width/2,height/2 - startY, height/2 - endY,width,height);
        allObjects.push_back(symmetricLine);
    }
    else if(algoType == 3)
    {
        Bresenham *bresenhamLine = new Bresenham(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
        bresenhamLine->draw(startX + width/2, endX + width/2,height/2 - startY, height/2 - endY,width,height);
        allObjects.push_back(bresenhamLine);
    }
    else if(algoType == 4)
    {
        MidPoint *midLine = new MidPoint(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
        midLine->draw(startX + width/2, endX + width/2,height/2 - startY, height/2 - endY,width,height);
        allObjects.push_back(midLine);
    }
	//return true;
}

