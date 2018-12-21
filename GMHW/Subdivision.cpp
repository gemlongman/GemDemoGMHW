#include "Subdivision.h"

void Subdivision::divideFace(int p, int p1, int p2, int p3)
{
	//adds the new face to the points' lists
	tempPoints[p].AddFacesIncluded(tempFaces.size());
	tempPoints[p1].AddFacesIncluded(tempFaces.size());
	tempPoints[p2].AddFacesIncluded(tempFaces.size());
	tempPoints[p3].AddFacesIncluded(tempFaces.size());

	//pushes the new face to the face list
	tempFaces.push_back(Face(tempPoints[p], tempPoints[p1], tempPoints[p2], tempPoints[p3]));
}

SubdModelPoint Subdivision::getCorrectPoint(int pointI)
{
	// if the point is already on the list
	for (int j = 0; j < tempPoints.size(); j++)
	{
		// if the past index is already
		if (tempPoints[j].PastIndex == pointI)
		{
			return tempPoints[j];
		}
	}

	//add it
	SubdModelPoint newP = SubdModelPoint(points[pointI].X, points[pointI].Y, points[pointI].Z, tempPoints.size(), pointI);
	tempPoints.push_back(newP);
	return newP;
}

void Subdivision::SetVar(vector<Face>  faces, vector<SubdModelPoint>  points)
{
	this->faces = faces;
	//this->tempFaces = faces;
	this->points = points;
	//this->tempPoints = points;
}

// get facepoint
SubdModelPoint Subdivision::calcuateFaceMidPoint(Face f)
{
	SubdModelPoint p;// = f.GetFacePoint();
	for (int i = 0; i < 4; i++)
	{
		p = p + points[f.Point[i].Index];
	}
	p = p * 0.25;
	return p;
}

// get facepoint
SubdModelPoint Subdivision::getFaceMidPoint(Face f) 
{
	//	add face mid point
	SubdModelPoint p = calcuateFaceMidPoint(f);// = f.GetFacePoint();//is wrong

	//this func is called once,so need not check
	//for (int i = 0; i < tempPoints.size(); i++)
	//{
	//	if (tempPoints[i] == p)
	//	{
	//		return tempPoints[i];
	//	}
	//}
	p.Index = tempPoints.size();
	tempPoints.push_back(p);
	return p;
}

SubdModelPoint Subdivision::addEdgeMedianPoint(SubdModelPoint a, SubdModelPoint b)
{
	//cout << a.Index << " a-b " << b.Index << endl;
	//cout << a.PastIndex << " aPastIndexb " << b.PastIndex << endl;
	// find face point
	int num = 0;
	SubdModelPoint retPoint;

	for (int i = 0; i < faces.size(); i++)
	{
		for (int j = 0; j < 4; j++) {
			int id1 = faces[i].Point[j].Index;
			int id2 = faces[i].Point[(j + 1) % 4].Index;
			//cout << id1 << "----" << id2 << endl;
			if ( (a.PastIndex == id1 && b.PastIndex == id2)
				|| (a.PastIndex == id2 && b.PastIndex == id1) )
			{
				retPoint = retPoint + calcuateFaceMidPoint(faces[i]);// faces[i].GetFacePoint();
				num++;
				j = 4;
			}
		}
	}

	if (num == 2) 
	{
		retPoint = retPoint + a;
		retPoint = retPoint + b;
		retPoint = retPoint * 0.25;//(1 / 4.0)
	}
	else 
	{
		retPoint = retPoint + a;
		retPoint = retPoint + b;
		num += 2;
		retPoint = retPoint * (1.0 / num);
		//debug
		cout<< "singluar edge" << num << endl;
		a.Print();
		b.Print();
	}

	for (int i = 0; i < tempPoints.size(); i++)
	{
		if ( tempPoints[i] == retPoint )
		{
			return tempPoints[i];
		}
	}

	//add
	retPoint.Index = tempPoints.size();
	tempPoints.push_back(retPoint);
	//cout << retPoint.PastIndex << endl;
	return retPoint;
}

void Subdivision::CCsubDivideAddFaces()
{
	SubdModelPoint p1, p2, p3, p4, p14, p12, p23, p34, pm;
	for (int i = 0; i < faces.size(); i++)
	{
		p1 = getCorrectPoint(faces[i].Point[0].Index);
		p2 = getCorrectPoint(faces[i].Point[1].Index);
		p3 = getCorrectPoint(faces[i].Point[2].Index);
		p4 = getCorrectPoint(faces[i].Point[3].Index); // add point if dosen't exit

		//cout << "face " << i << " p " << p1.Index << " p " << p2.Index << " p " << p3.Index << " p " << p4.Index << endl;
		p14 = addEdgeMedianPoint(p1, p4);
		p12 = addEdgeMedianPoint(p1, p2);
		p23 = addEdgeMedianPoint(p2, p3);
		p34 = addEdgeMedianPoint(p3, p4); //add point
		pm = getFaceMidPoint(faces[i]); //add point

		divideFace(p1.Index, p12.Index, pm.Index, p14.Index);
		divideFace(p2.Index, p23.Index, pm.Index, p12.Index);
		divideFace(p3.Index, p34.Index, pm.Index, p23.Index);
		divideFace(p4.Index, p14.Index, pm.Index, p34.Index);

	}
}

void Subdivision::CCsubDivideUpdatePoint()
{
	//update original points
	for (int pointIndex = 0; pointIndex < tempPoints.size(); pointIndex++)
	{

		if (tempPoints[pointIndex].PastIndex != -1 ) // is original vertex  points
		{
			int pastPointIndex = tempPoints[pointIndex].PastIndex;
			//tempPoints[pointIndex].PastIndex = -1; //need not do it,because in next subdivision it useless
			//cout << "move" << pointIndex << endl;

			SubdModelPoint newPoint;
			
			// find adjoint faces and edges to target vertex id
			//should find in old points
			int adjacentFaceNum = 0, adjacentEdgeNum = 0;
			SubdModelPoint averageFacePoint, averageEdgePoint;

			for (int i = 0; i < points[pastPointIndex].FacesIncluded.size(); i++) // can not use tempPoints[pointIndex].FacesIncluded.size() because it is new face
			{
				
				int faceIncludeIndex = points[pastPointIndex].FacesIncluded[i];
				SubdModelPoint tempFace = calcuateFaceMidPoint(faces[faceIncludeIndex]);// faces[faceIncludeIndex].GetFacePoint();
				averageFacePoint = averageFacePoint + tempFace;
				adjacentFaceNum++;
				//cout << "faceIncludeIndex " << faceIncludeIndex << endl;

				for (int j = 0; j < 4; j++) 
				{
					int p1Index = faces[faceIncludeIndex].Point[j % 4].Index;
					int p2Index = faces[faceIncludeIndex].Point[(j + 1) % 4].Index;
					/*cout << " p1Index " << p1Index << " p2Index " << p2Index << endl;*/
					if (p1Index == pastPointIndex || p2Index == pastPointIndex) // this edge include this point
					{
						//cout << " ---p1Index " << p1Index << " ----p2Index " << p2Index << endl;
						SubdModelPoint tempEdege = ( points[p1Index] + points[p2Index] ) * 0.5;
						//adjacentEdgePoints.push_back(tempEdege);
						averageEdgePoint = averageEdgePoint + tempEdege;
						adjacentEdgeNum++;
						//j=4; // this can not make sure to calulate this face once, however divide adjacentEdgeNum is enogh
					}
				}
			}


			if (adjacentEdgeNum == adjacentFaceNum << 1 ) 
			{
				int N = adjacentFaceNum;
				if (N > 0) 
				{
					averageFacePoint = averageFacePoint * (1.0 / adjacentFaceNum);
					averageEdgePoint = averageEdgePoint * (1.0 / adjacentEdgeNum);
					
					double Q = 1.0 / N;
					double R = 2.0 * Q;// 2 / N;
					double S = (N - 3) / double(N);
					//newPoint =  ( averageFacePoint  + averageEdgePoint * 2.0+ points[pastPointIndex]) *0.25;
					//cout << " N " << N << " Q " << Q << " R " << R << " S " << S << endl;
					newPoint = averageFacePoint * Q + averageEdgePoint * R + points[pastPointIndex] * S;
					tempPoints[pointIndex].MoveTo(newPoint);
				}
			
			}
			else 
			{
				// hole vertex
				cout << "singular vertex" << adjacentEdgeNum << " " << adjacentFaceNum << endl;
				newPoint = points[pastPointIndex];
				tempPoints[pointIndex].MoveTo(newPoint);
			}
		}
	}

}

void Subdivision::CCsubDivide()
{
	if (faces.size() > MAXsubFaces)
	{
		cout << endl << "now faces:" << faces.size() << ", which cost a lot of time! It is too maney to divide again! " << endl;
		return;
	}

	tempFaces.clear();
	tempPoints.clear();

	//printPoints();

	CCsubDivideAddFaces();

	CCsubDivideUpdatePoint();

	faces = tempFaces;
	points = tempPoints;

	//printPoints();

	cout << endl << "now faces:" << faces.size() << endl;
	cout << "now vertexes:" << points.size() << endl << endl;
}


#ifdef debuggy

void Subdivision::printFaces()
{
	cout << "debuggy: " << endl;
	for (int i = 0; i < faces.size(); i++) 
	{
		cout << " pointIdex: " << faces[i].Point[0].Index << endl;
	}
	cout << "-------" << endl;
}

void Subdivision::printPoints()
{
	cout << "debuggy: " << endl;
	for (int i = 0; i < points.size(); i++)
	{
		points[i].Print();
	}
	cout << "-------" << endl;
}

#endif // debuggy