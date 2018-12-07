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

using namespace std;

class Point
{
public:
	float X, Y, Z;
	int Index;
	int PastIndex;
	vector<int> FacesIncluded;
	bool IsMoved = false;

	Point(void)
	{
		X = Y = Z = 0;
		PastIndex = -1;
	}

	Point(float iX, float iY, float iZ, int i)
	{
		X = iX;
		Y = iY;
		Z = iZ;
		Index = i;
		PastIndex = -1;
	}

	Point(float iX, float iY, float iZ, int i, int pI)
	{
		X = iX;
		Y = iY;
		Z = iZ;
		Index = i;
		PastIndex = pI;
	}

	void Move(float iX, float iY, float iZ)
	{
		X = iX;
		Y = iY;
		Z = iZ;
		IsMoved = true;
	}

	bool IsEqual(float iX, float iY, float iZ)
	{
		return (iX == X && iY == Y && iZ == Z);
	}

	void AddFacesIncluded(int count)
	{
		FacesIncluded.push_back(count);
	}
};

class NormalizedVec
{
public:
	float X, Y, Z;

	NormalizedVec() {}

	NormalizedVec(Point point1, Point point2, Point point3, Point point4)
	{
		Point v(point3.X - point1.X, point3.Y - point1.Y, point3.Z - point1.Z, -1);
		Point w(point4.X - point2.X, point4.Y - point2.Y, point4.Z - point2.Z, -1);
		X = -v.Y*w.Z + v.Z*w.Y;
		Y = -v.Z*w.X + v.X*w.Z;
		Z = -v.X*w.Y + v.Y*w.X;
		float magnitude = sqrt(X*X + Y * Y + Z * Z);
		X = X / magnitude;
		Y = Y / magnitude;
		Z = Z / magnitude;
	}
};

class Triangle
{
public:
	Point Centroid;
	Triangle(Point point1, Point point2, Point point3)
	{
		float nx = (point1.X + point2.X + point3.X) / 3;
		float ny = (point1.Y + point2.Y + point3.Y) / 3;
		float nz = (point1.Z + point2.Z + point3.Z) / 3;

		Centroid = Point(nx, ny, nz, -1);
	}
};

class Face
{
public:
	bool IsDeleted = false;

	Point Point1;
	Point Point2;
	Point Point3;
	Point Point4;
	Point Center;

	NormalizedVec Normal;

	Face(Point p1, Point p2, Point p3, Point p4)
	{
		Point1 = p1;
		Point2 = p2;
		Point3 = p3;
		Point4 = p4;

		Normal = NormalizedVec(Point1, Point2, Point3, Point4);

		Triangle t1(Point1, Point2, Point3);
		Triangle t2(Point3, Point4, Point1);

		Triangle t3(Point1, Point2, Point4);
		Triangle t4(Point4, Point3, Point2);

		float nx = (t1.Centroid.X + t2.Centroid.X + t3.Centroid.X + t4.Centroid.X) / 4;
		float ny = (t1.Centroid.Y + t2.Centroid.Y + t3.Centroid.Y + t4.Centroid.Y) / 4;
		float nz = (t1.Centroid.Z + t2.Centroid.Z + t3.Centroid.Z + t4.Centroid.Z) / 4;
		Center = Point(nx, ny, nz, -1);
	}

	void Normalize()
	{
		Normal = NormalizedVec(Point1, Point2, Point3, Point4);

		Triangle t1(Point1, Point2, Point3);
		Triangle t2(Point3, Point4, Point1);

		Triangle t3(Point1, Point2, Point4);
		Triangle t4(Point4, Point3, Point2);

		float aX = (t1.Centroid.X + t2.Centroid.X + t3.Centroid.X + t4.Centroid.X) / 4;
		float aY = (t1.Centroid.Y + t2.Centroid.Y + t3.Centroid.Y + t4.Centroid.Y) / 4;
		float aZ = (t1.Centroid.Z + t2.Centroid.Z + t3.Centroid.Z + t4.Centroid.Z) / 4;
		Center = Point(aX, aY, aZ, -1);
	}

	void Warp(int index, float aX, float aY, float aZ)
	{
		if (Point1.Index == index)
		{
			Point1.Move(aX, aY, aZ);
		}
		else if (Point2.Index == index)
		{
			Point2.Move(aX, aY, aZ);
		}
		else if (Point3.Index == index)
		{
			Point3.Move(aX, aY, aZ);
		}
		else if (Point4.Index == index)
		{
			Point4.Move(aX, aY, aZ);
		}
	}

};
