#pragma once

#include "PublicStruct.h"
#include <algorithm>

#define MAXsubFaces 100000
//#define debuggy
class Subdivision
{
private:
	vector<Face> faces;
	vector<SubdModelPoint> points;

	vector<Face> tempFaces;
	vector<SubdModelPoint> tempPoints;

	SubdModelPoint getCorrectPoint(int pointI);
	SubdModelPoint calcuateFaceMidPoint(Face f);
	SubdModelPoint getFaceMidPoint(Face f);
	SubdModelPoint addEdgeMedianPoint(SubdModelPoint a, SubdModelPoint b);

	void divideFace(int p, int p1, int p2, int p3);
	void CCsubDivideAddFaces();
	void CCsubDivideUpdatePoint();

#ifdef debuggy
	void printPoints();
	void printFaces();
#endif // debuggy
public:

	vector<Face>  GetFaces() 
	{
		return faces;
	}

	vector<SubdModelPoint>  GetPts()
	{
		return points;
	}

	void SetVar(vector<Face>  faces, vector<SubdModelPoint>  points);
	
	void CCsubDivide();	
};