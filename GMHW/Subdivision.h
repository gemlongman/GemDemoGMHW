#pragma once

#include "PublicStruct.h"

class Subdivision
{
private:
	vector<Face> tempFaces;
	vector<Point> tempPts;

	vector<Face> faces;
	vector<Point> pts;

	Point addMedian(Point a, Point b)
	{
		float newX = (a.X + b.X) / 2;
		float newY = (a.Y + b.Y) / 2;
		float newZ = (a.Z + b.Z) / 2;

		for (int i = 0; i < tempPts.size(); i++)
		{
			if (tempPts[i].IsEqual(newX, newY, newZ))
			{
				return tempPts[i];
			}
		}

		Point p(newX, newY, newZ, tempPts.size());
		tempPts.push_back(p);
		return p;
	}

	void warp(Point &p)
	{
		Point center;
		float cx = 0;
		float cy = 0;
		float cz = 0;
		int j = 0;
		int counts = 0;
		if ( !pts[ p.Index ].IsMoved )
		{
			for ( j = 0; j < pts[ p.Index ].FacesIncluded.size(); j++ )
			{
				center = faces[ pts[ p.Index ].FacesIncluded[j] ].Center;
				cx += center.X;
				cy += center.Y;
				cz += center.Z;
				counts++;
			}
			cx /= counts;
			cy /= counts;
			cz /= counts;
			pts[ p.Index ].Move(cx, cy, cz);

			for ( j = 0; j < pts[ p.Index ].FacesIncluded.size(); j++ )
			{
				faces[ pts[ p.Index ].FacesIncluded[j] ].Warp(p.Index, cx, cy, cz);
			}
		}
	}


	void divideFace(int p, int p1, int p2, int p3)
	{
		//adds the new face to the points' lists that make it up
		tempPts[p].AddFacesIncluded( tempFaces.size() );
		tempPts[p1].AddFacesIncluded( tempFaces.size() );
		tempPts[p2].AddFacesIncluded( tempFaces.size() );
		tempPts[p3].AddFacesIncluded( tempFaces.size() );

		//pushes the new face to the face list
		tempFaces.push_back( Face( tempPts[p], tempPts[p1], tempPts[p2], tempPts[p3] ) );
	}

	Point getCorrectPoint(int pointI)
	{
		//checks to see if the point being created is already on the list
		for (int j = 0; j < tempPts.size(); j++)
		{
			//checks to see if the past index of the vertex has already been added to the list
			if (tempPts[j].PastIndex == pointI)
			{
				return tempPts[j];
			}
		}

		//the point was not already on the list, add it
		Point newP = Point( pts[pointI].X, pts[pointI].Y, pts[pointI].Z, tempPts.size(), pointI);
		tempPts.push_back(newP);
		return newP;
	}

public:

	void SetVar(vector<Face>  faces, vector<Point>  pts)
	{
		this->faces = faces;
		this->pts = pts;
	}

	vector<Face>  GetFaces() 
	{
		return faces;
	}

	vector<Point>  GetPts()
	{
		return pts;
	}

	void average()
	{
		int i;
		for (i = 0; i < faces.size(); i++)
		{
			warp( pts[ faces[i].Point1.Index ] );
			warp( pts[ faces[i].Point2.Index ] );
			warp( pts[ faces[i].Point3.Index ] );
			warp( pts[ faces[i].Point4.Index ] );
		}
		for (i = 0; i < faces.size(); i++)
		{
			faces[ i ].Normalize();
		}
	}

	void subDivide()
	{
		Point one, two, three, four, zero, ninety, one80, two70, middle;
		for (int i = 0; i < faces.size(); i++)
		{
			one = getCorrectPoint(faces[i].Point1.Index);
			two = getCorrectPoint(faces[i].Point2.Index);
			three = getCorrectPoint(faces[i].Point3.Index);
			four = getCorrectPoint(faces[i].Point4.Index);

			zero = addMedian(one, four);
			ninety = addMedian(one, two);
			one80 = addMedian(two, three);
			two70 = addMedian(three, four);
			middle = addMedian(one, three);

			divideFace(one.Index, ninety.Index, middle.Index, zero.Index);
			divideFace(two.Index, one80.Index, middle.Index, ninety.Index);
			divideFace(three.Index, two70.Index, middle.Index, one80.Index);
			divideFace(four.Index, zero.Index, middle.Index, two70.Index);

		}

		faces = tempFaces;
		pts = tempPts;

		tempFaces.clear();
		tempPts.clear();
	}


};