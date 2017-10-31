#include "cohensutherland.h"

extern list<Object*> allObjects;
extern float *CURRENTCOLOR;
extern string CURRENTPATTERN;
extern int CURRENTTHICKNESS;

int Cohen::computeRegionCode(int X,int Y,int minX,int maxX,int minY,int maxY)
{
	int regionCodes[] = {0,9,8,10,5,4,6,1,2};

    //Assigninng Region Code To The Starting Points Of The Line
    if(X < minX && Y < minY)
    	return regionCodes[4];
	else if(X < minX && Y > minY && Y < maxY)
		return regionCodes[7];
	else if(X < minX && Y > maxY)
		return regionCodes[1];
	else if(X < maxX && X > minX && Y > maxY)
		return regionCodes[2];
	else if(X > maxX && Y > maxY)
		return regionCodes[3];
	else if(X > maxX && Y < maxY && Y > minY)
		return regionCodes[8];
	else if(X > maxX && Y < minY)
		return regionCodes[6];
	else if(X < maxX && X > minX && Y < minY)
		return regionCodes[5];
	else
		return regionCodes[0];

}

//clipLine(startX,endX,startY,endY,width,height,-clipWidth/2/*minX*/,clipWidth/2/*maxX*/,-clipHeight/2/*minY*/,clipHeight/2/*minx*/,1/*SimpleDDA*/);
void Cohen::clipLine(int startX,int endX,int startY,int endY,int width,int height,int minX,int maxX,int minY,int maxY,int algoType)
{
	int startRegionCode,endRegionCode;

	int LEFT = 1,RIGHT = 2,BOTTOM = 4, TOP = 8;
    //int tempSX = startX,tempSY = startY,tempEX = endX,tempEY = endY;

    startX = startX - width/2;
    startY = height/2 - startY;
    endX = endX - width/2;
    endY = height/2 - endY;

    cout<<"Start Points: "<<startX<<"/"<<startY;
	cout<<"End Points: "<<endX<<"/"<<endY;
    startRegionCode = computeRegionCode(startX,startY,minX,maxX,minY,maxY);
	endRegionCode = computeRegionCode(endX,endY,minX,maxX,minY,maxY);

	cout<<"\n\tRegion Codes: "<<startRegionCode <<" /"<<endRegionCode;

	//If both the points are inside the clip window,then draw them.
	if(startRegionCode == 0 && endRegionCode == 0)
	{
		cout<<"\n\tBoth Are Inside Region";
		if(algoType == 1)
		{
			SimpleDDA *simpleLine = new SimpleDDA(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
			cout<<"\n\tAlgo:1 Calling SimpleDDA";
            simpleLine->draw(startX + width/2, endX + width/2,height/2 - startY, height/2 - endY,width,height);
            allObjects.push_back(simpleLine);
        }
		else if(algoType == 2)
		{
			SymmetricDDA *symmetricLine = new SymmetricDDA(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
			cout<<"\n\tAlgo:2 Calling Symmetric";
            symmetricLine->draw(startX + width/2, endX + width/2,height/2 - startY, height/2 - endY,width,height);
            allObjects.push_back(symmetricLine);
        }
		else if(algoType == 3)
		{
			Bresenham *bresenhamLine = new Bresenham(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
			cout<<"\n\tAlgo:3 Calling Bresenham";
            bresenhamLine->draw(startX + width/2, endX + width/2,height/2 - startY, height/2 - endY,width,height);
            allObjects.push_back(bresenhamLine);
        }
		else if(algoType == 4)
		{
            MidPoint *midLine = new MidPoint(CURRENTCOLOR,CURRENTTHICKNESS,CURRENTPATTERN);
            cout<<"\n\tAlgo:4 Calling midLine";
            midLine->draw(startX + width/2, endX + width/2,height/2 - startY, height/2 - endY,width,height);
            allObjects.push_back(midLine);
        }
	}
	else
	{
		int out_point;
		//result = startRegionCode & endRegionCode;
		if(!(startRegionCode & endRegionCode)) 	//Line is partially inside
		{
			cout<<"\n\tLine is partially Inside";
			int Xpoint,Ypoint;
			int intersectX,intersectY;
			float slope = (float)(endY-startY)/(float)(endX-startX);
			cout<<"\n\tSlope: "<<slope;
			while(startRegionCode != 0 || endRegionCode != 0)
			{
				if(startRegionCode != 0)
					out_point = startRegionCode;
				else
					out_point = endRegionCode;

				if(out_point == startRegionCode)
				{
					cout<<"\n\tOut point is startRegionCode";
					Xpoint = startX; Ypoint = startY;
				}
				else
				{
					cout<<"\n\tOut point is endRegionCode";
					Xpoint = endX; Ypoint = endY;
				}

				if(out_point & LEFT)
				{
					cout<<"\n\tPoint intersects with Left Boundary";
					intersectY = Ypoint + slope*(minX - Xpoint);
					intersectX = minX;
				}
				if (out_point & TOP)	//point is above the clip rectangle
	            {
	            	cout<<"\n\tPoint intersects with Top Boundary";
	                intersectX = Xpoint + (maxY - Ypoint)/slope;
	                intersectY = maxY;
	            }
	            else if (out_point & BOTTOM)	 // point is below the rectangle
	            {
	            	cout<<"\n\tPoint intersects with Bottom Boundary";
	                intersectX = Xpoint + (minY - Ypoint) / slope;
	                intersectY = minY;
	            }
	            else if (out_point & RIGHT)	// point is to the right of rectangle
	            {
	            	cout<<"\n\tPoint intersects with Right Boundary";
					intersectY = Ypoint + slope*(maxX - Xpoint);
					intersectX = maxX;
	            }

	            if(out_point == startRegionCode)
	            {
	            	startX = intersectX; startY = intersectY;
	            	cout<<"\n\tStart Points changed to: "<<startX<<"/"<<startY;
	            	startRegionCode = computeRegionCode(startX,startY,minX,maxX,minY,maxY);
	            	cout<<"\n\tstartRegionCode now: "<<startRegionCode;
				}
				else if(out_point == endRegionCode)
				{
					endX = intersectX; endY = intersectY;
					cout<<"\n\tEnd Points changed to: "<<endX<<"/"<<endY;
	            	endRegionCode = computeRegionCode(endX,endY,minX,maxX,minY,maxY);
	            	cout<<"\n\tendRegionCode now: "<<endRegionCode;
				}

				cout<<"\n\tRegion Codes: "<<startRegionCode <<" /"<<endRegionCode;
			}
//			cout<<"\n\tCalling LDA with Points: (startX/startY): "<<(startX + width/2)<<"/"<<(startY, height/2);
//			cout<<"\n\tCalling LDA with Points: (endX/endY): "<<(endX + width/2)<<"/"<<(height/2 + endY);
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
		}
	}
}

