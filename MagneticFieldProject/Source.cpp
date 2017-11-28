#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
using namespace std;

/* GLOBALS */
double PI = 3.1415926535897932384626433832;
double ROE = 6378.1; // Radius of earth in kilometers
					 /*END GLOBALS*/

class Coordinate {
public:
	double latitude;
	double longitude;
	double latitude_degree;
	double longitude_degree;
	double magnetic_x;
	double magnetic_y;
	double magnetic_z;

	Coordinate() {
		latitude, longitude, latitude_degree, longitude_degree, magnetic_x, magnetic_y, magnetic_z = 0;
	}

	Coordinate(double lat, double lon) {
		latitude = lat;
		longitude = lon;
	}
};

double deg2rad(double degree) {
	return ((degree * PI) / 180);
}

vector<Coordinate> PointsCalculator(double latitude, double longitude, double baring, double distance) {
	double lat1 = latitude; // start latitude
	double lon1 = longitude; // start longitude
	double d = distance; // distance bettween each point in meters
	double brng1 = baring; // degree between reference points

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
	double HBarings[4] = { deg2rad(279.0128), deg2rad(98.75389) }; //horizontal barings
	double VBarings[2] = { deg2rad(188.94), deg2rad(8.89667) }; // vertical barings

	vector<vector<Coordinate>> Axis; // coordinates on x axis
	vector<vector<Coordinate>> Q1;
	vector<vector<Coordinate>> Q2;
	vector<vector<Coordinate>> Q3;
	vector<vector<Coordinate>> Q4;

	double lat = deg2rad(26.305995); // starting latitude
	double lon = deg2rad(-98.194208); // starting longitude

									  // 
	for (int i = 0; i < 4; i++)
	{
		Axis.push_back(PointsCalculator(lat, lon, HBarings[i], 0.01));
	}


	/* Finding Quadrants */
	for (int q = 0; q < 100; q++) { // Quadrant 1 and 4
		Q4.push_back(PointsCalculator(Axis[0][q].latitude, Axis[0][q].longitude, VBarings[0], 0.01));
		Q1.push_back(PointsCalculator(Axis[0][q].latitude, Axis[0][q].longitude, VBarings[1], 0.01));
		Q3.push_back(PointsCalculator(Axis[1][q].latitude, Axis[1][q].longitude, VBarings[0], 0.01));
		Q2.push_back(PointsCalculator(Axis[1][q].latitude, Axis[1][q].longitude, VBarings[1], 0.01));
	}

	for (int x = 0; x < 100; x++) {
		cout << "Coordinate 0deg: " << Q3[x][0].latitude_degree << Q3[x][0].longitude_degree << endl;
		cout << "Coordinate 50deg: " << Q3[x][49].latitude_degree << Q3[x][49].longitude_degree << endl;
		cout << "Coordinate 100deg: " << Q3[x][99].latitude_degree << Q3[x][99].longitude_degree << endl;
	}


	system("pause");
	return 0;
} 