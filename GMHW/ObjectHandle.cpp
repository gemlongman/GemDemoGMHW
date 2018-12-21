#include "ObjectHandle.h"


void LoadObject(string filename, vector<SubdModelPoint> & points, vector<Face> & faces)
{
	string inputStr;
	ifstream fileSteam(filename);
	int faceCounts = 0;

	while (fileSteam >> inputStr)
	{
		//cout << inputStr << endl;

		string temp, point1, point2, point3, point4;
		if (inputStr == "v")
		{
			// localtion
			fileSteam >> point1 >> point2 >> point3;
			SubdModelPoint point( atof( point1.c_str() ), atof( point2.c_str() ), atof( point3.c_str() ), points.size() );
			//cout  << point1 << point2 << point3 << endl;
			//cout << atof(point1.c_str()) << "point1" << point1 << endl;
			points.push_back( point );
		}
		else if (inputStr == "f")
		{
			// index
			fileSteam >> point1 >> point2 >> point3 >> point4;
			//cout << "Face: " << point1 << "-" << point2 << "-" << point3 << "-" << point4 << endl;
			points[ stoi(point1) - 1 ].AddFacesIncluded(faceCounts);
			points[ stoi(point2) - 1 ].AddFacesIncluded(faceCounts);
			points[ stoi(point3) - 1 ].AddFacesIncluded(faceCounts);
			points[ stoi(point4) - 1 ].AddFacesIncluded(faceCounts);
			Face face( points[ stoi(point1) - 1 ], points[ stoi(point2) - 1 ], points[ stoi(point3) - 1 ], points[ stoi(point4) - 1 ] );
			faces.push_back(face);
			faceCounts++;
		}
	}
	fileSteam.close();
	cout << "load faces:" << faces.size() << endl;
	cout << "load vertexes:" << points.size() << endl << endl;
	
}