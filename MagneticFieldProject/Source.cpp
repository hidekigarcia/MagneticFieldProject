#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <ctime>
#include <algorithm>

using namespace std;

double PI = 3.1415926535897932384626433832;
double ROE = 6378.1; // Radius of earth in kilometers
					 
class Coordinate {
public:
	double latitude;
	double longitude;
	double latitude_degree;
	double longitude_degree;
	double magnetic_field;

	Coordinate() {
		latitude, longitude, latitude_degree, longitude_degree, magnetic_field = 0;
	}

	Coordinate(double lat, double lon) {
		latitude = lat;
		longitude = lon;
	}
};

double deg2rad(double degree) {
	return ((degree * PI) / 180);
}

bool cmd_lon(Coordinate const & a, Coordinate const & b) {
	return a.longitude < b.longitude;
}

bool cmd_lat(Coordinate const & a, Coordinate const & b) {
	return a.latitude < b.latitude;
}

vector<Coordinate> PointsCalculator(double latitude, double longitude, double bearing, double distance) {
	double lat1 = latitude; // start latitude
	double lon1 = longitude; // start longitude
	double d = distance; // distance bettween each point in meters
	double brng1 = bearing; // degree between reference points

	double lat2 = 0; // update latitude point
	double lon2 = 0; //update longitude point
	double latD1 = 0; //degree of latitude 1
	double lonD1 = 0; //degree of longitude 1;

	vector<Coordinate> p(100); // 100 lat and long points

	int i = 0;

	// start point into array

	p[0].latitude = latitude;
	p[0].longitude = longitude;

	do
	{
		d = d + .010;
		lat2 = asin(sin(lat1)*cos(d / ROE) + cos(lat1)*sin(d / ROE)*cos(brng1));  //in radians
		lon2 = lon1 + atan2(sin(brng1)*sin(d / ROE)*cos(lat1), cos(d / ROE) - sin(lat1)*sin(lat2));
		lon2 = fmod((lon2 + 540), 360) - 180; //in radians

		p[i].latitude = lat2;
		p[i].longitude = lon2;

		latD1 = (lat2) * 180 / PI;
		lonD1 = (lon2) * 180 / PI;
		p[i].latitude_degree = latD1;
		p[i].longitude_degree = lonD1;
		i++;
	} while (i < 100);

	return p;
}

int main() {
	double HBearings[2] = { deg2rad(279.0128), deg2rad(98.75389) }; //horizontal bearings + - 
	double VBearings[2] = { deg2rad(188.94), deg2rad(8.89667) }; // vertical bearings + -

	vector<vector<Coordinate>> Axis; // coordinates on x axis
	vector<vector<Coordinate>> Q1;
	vector<vector<Coordinate>> Q2;
	vector<vector<Coordinate>> Q3;
	vector<vector<Coordinate>> Q4;
	vector<vector<Coordinate>> Temp1,Temp2, Temp3,Temp4;

	double lat = deg2rad(26.305995); // starting latitude
	double lon = deg2rad(-98.194208); // starting longitude

									  // 
	for (int i = 0; i < 4; i++)
	{
		Axis.push_back(PointsCalculator(lat, lon, HBearings[i], 0.01));
	}


	for (int a = 0; a < 100; a++) { // Quadrant 1 and 4

		Q4.push_back(PointsCalculator(Axis[0][a].latitude, Axis[0][a].longitude, VBearings[0], 0.01));
		Q1.push_back(PointsCalculator(Axis[0][a].latitude, Axis[0][a].longitude, VBearings[1], 0.01));
		Q3.push_back(PointsCalculator(Axis[1][a].latitude, Axis[1][a].longitude, VBearings[0], 0.01));
		Q2.push_back(PointsCalculator(Axis[1][a].latitude, Axis[1][a].longitude, VBearings[1], 0.01));
	}

	/*
	for (int e = 0; e < 100; e++) {
		cout << "Coordinate 0deg: " << Q3[e][0].latitude_degree << Q3[e][0].longitude_degree << endl;
		cout << "Coordinate 50deg: " << Q3[e][49].latitude_degree << Q3[e][49].longitude_degree << endl;
		cout << "Coordinate 100deg: " << Q3[e][99].latitude_degree << Q3[e][99].longitude_degree << endl;
	}
	*/

	Temp3 = Q3;
	cout << Temp3[0].size() << endl;

	std::sort(Temp3[0].begin(), Temp3[0].end(), cmd_lon);
	for (int i = 0; i < Temp3[0].size(); ++i)
		cout << Temp3[0][i].longitude_degree << " ";

	cout << endl;
		
	system("pause");
	return 0;
} 