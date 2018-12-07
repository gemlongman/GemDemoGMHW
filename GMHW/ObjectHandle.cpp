#include "ObjectHandle.h"


void loadObject(string filename, vector<Point> & pts, vector<Face> & faces)
{
	string inputStr;
	ifstream fileSteam(filename);
	int faceCounts = 0;

	while (!fileSteam.eof())
	{
		getline(fileSteam, inputStr);	
		if (fileSteam.fail()) 
		{
			break;
		}

		stringstream strStream(inputStr);
		string temp, point1, point2, point3, point4;
		if (inputStr[0] == 'v')
		{
			strStream >> temp >> point1 >> point2 >> point3;
			Point point( atof( point1.c_str() ), atof( point2.c_str() ), atof( point3.c_str() ), pts.size() );
			//cout << inputStr << endl;
			//cout << "test: " << atof(point1.c_str()) << "point1" << point1 << endl;
			pts.push_back( point );
		}
		else if (inputStr[0] == 'f')
		{

			strStream >> temp >> point1 >> point2 >> point3 >> point4;
			//cout << inputStr << endl;
			//cout << "add Face: " << point1 << "-" << point2 << "-" << point3 << "-" << endl;
			pts[ stoi(point1) - 1 ].AddFacesIncluded(faceCounts);
			pts[ stoi(point2) - 1 ].AddFacesIncluded(faceCounts);
			pts[ stoi(point3) - 1 ].AddFacesIncluded(faceCounts);
			pts[ stoi(point4) - 1 ].AddFacesIncluded(faceCounts);
			Face face( pts[ stoi(point1) - 1 ], pts[ stoi(point2) - 1 ], pts[ stoi(point3) - 1 ], pts[ stoi(point4) - 1 ] );
			faces.push_back(face);
			faceCounts++;
		}
	}
	cout << "load face:" << faceCounts << endl;
	fileSteam.close();
}