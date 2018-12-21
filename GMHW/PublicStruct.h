#pragma once

#include <time.h>
#include <Windows.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <climits>
#include <string>
#include <stdio.h> 
#include <stdlib.h> 
#include <sstream> 
#include <vector>
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>


#define EPS 0.00001
// if it too small it will crush

using namespace std;

class SubdModelPoint
{
public:
	double X, Y, Z;
	int Index;
	int PastIndex;
	vector<int> FacesIncluded;
	bool IsMoved = false;

	SubdModelPoint(void)
	{
		X = Y = Z = 0.0;
		PastIndex = -1;
	}

	SubdModelPoint(double iX, double iY, double iZ, int i)
	{
		X = iX;
		Y = iY;
		Z = iZ;
		Index = i;
		PastIndex = -1;
	}

	SubdModelPoint(double iX, double iY, double iZ, int i, int pI)
	{
		X = iX;
		Y = iY;
		Z = iZ;
		Index = i;
		PastIndex = pI;
	}

	void Move(double iX, double iY, double iZ)
	{
		X = iX;
		Y = iY;
		Z = iZ;
		IsMoved = true;
	}

	void MoveTo(SubdModelPoint p)
	{
		X = p.X;
		Y = p.Y;
		Z = p.Z;
		IsMoved = true;
	}

	bool IsEqual(double iX, double iY, double iZ)
	{
		return (iX == X && iY == Y && iZ == Z);
	}

	void AddFacesIncluded(int count)
	{
		FacesIncluded.push_back(count);
	}

	bool operator == (SubdModelPoint  otherP)
	{
		return (fabs(X - otherP.X) < EPS && fabs(Y - otherP.Y) < EPS && fabs(Z - otherP.Z) < EPS);
		//return (otherP.X == X && otherP.Y == Y && otherP.Z == Z);
	}

	SubdModelPoint operator + (SubdModelPoint  otherP) 
	{
		//SubdModelPoint tempP = this;
		return SubdModelPoint(
			X + otherP.X,
			Y + otherP.Y,
			Z + otherP.Z,
			Index,
			PastIndex
		);
	}
	SubdModelPoint operator * (double a) 
	{
		return SubdModelPoint(
			X *a,
			Y *a,
			Z *a,
			Index,
			PastIndex
		);
	}

	void Print() 
	{
		cout << "pointIdex: " << Index << " pastIdex: " << PastIndex << " isMoved: " << IsMoved << "x: " << X << " y: " << Y << " z: " << Z << endl;
		cout << "\tFacesIncluded: ";
		for (int j = 0; j < FacesIncluded.size(); j++)
		{
			cout << " " << FacesIncluded[j];
		}
		cout << endl << endl;
	}
};

class NormalizedVec
{
public:
	double X, Y, Z;

	NormalizedVec() {}

	NormalizedVec(SubdModelPoint point1, SubdModelPoint point2, SubdModelPoint point3, SubdModelPoint point4)
	{
		SubdModelPoint v(point3.X - point1.X, point3.Y - point1.Y, point3.Z - point1.Z, -1);
		SubdModelPoint w(point4.X - point2.X, point4.Y - point2.Y, point4.Z - point2.Z, -1);
		X = -v.Y*w.Z + v.Z*w.Y;
		Y = -v.Z*w.X + v.X*w.Z;
		Z = -v.X*w.Y + v.Y*w.X;
		double magnitude = sqrt(X * X + Y * Y + Z * Z);
		X = X / magnitude;
		Y = Y / magnitude;
		Z = Z / magnitude;
	}
};

class Face
{
public:
	bool IsDeleted = false;

	SubdModelPoint Point[4];
	//SubdModelPoint Center;//this is uselss,becase this points are moved

	NormalizedVec Normal;

	Face(SubdModelPoint p1, SubdModelPoint p2, SubdModelPoint p3, SubdModelPoint p4)
	{
		Point[0] = p1;
		Point[1] = p2;
		Point[2] = p3;
		Point[3] = p4;

		Normal = NormalizedVec(Point[0], Point[1], Point[2], Point[3]);

		////center //this is uselss
		//double x = 0.0;
		//double y = 0.0;
		//double z = 0.0;

		//x += Point[0].X + Point[1].X + Point[2].X + Point[3].X;
		//y += Point[0].Y + Point[1].Y + Point[2].Y + Point[3].Y;
		//z += Point[0].Z + Point[1].Z + Point[2].Z + Point[3].Z;

		//x /= 4.0;
		//y /= 4.0;
		//z /= 4.0;

		//Center = SubdModelPoint(x, y, z, -1);
	}

	void Normalize()
	{
		Normal = NormalizedVec(Point[0], Point[1], Point[2], Point[3]);
	}

	//SubdModelPoint GetFacePoint() 
	//{
	//	//return Center;
	//	double x = 0.0;
	//	double y = 0.0;
	//	double z = 0.0;

	//	x += Point[0].X + Point[1].X + Point[2].X + Point[3].X;
	//	y += Point[0].Y + Point[1].Y + Point[2].Y + Point[3].Y;
	//	z += Point[0].Z + Point[1].Z + Point[2].Z + Point[3].Z;

	//	x /= 4.0;
	//	y /= 4.0;
	//	z /= 4.0;

	//	return SubdModelPoint(x, y, z, -1);
	//}
};
