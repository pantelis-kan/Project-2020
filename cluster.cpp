#include <iostream>
#include <string.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator>
#include <limits>
#include <cmath>
#include <vector>
#include <chrono>
#include <random>

#include "Point_Table.hpp"
#include "Point.hpp"
#include "utilities.hpp"
#include "Cluster.hpp"


using namespace std;

int k = 4; // k = number of clusters

int main(){

    string filename = "train-images-idx3-ubyte";
	string filename2 = "t10k-images-idx3-ubyte";

    int N = NumberOfPoints(filename);   // number of input points
	int N_q = NumberOfPoints(filename2); // number of query points
	int TableSize = N/8;
	cout << "Number of points is : " << N <<endl;
	cout << "Number of queries is : " << N_q <<endl;
	cout << "TableSize = " <<TableSize <<endl;


	Point_Array input(N);
	Point_Array queries(N_q);


	
	if(input.FillPoints(filename) == 0) cout << "Filling input points successful"<<endl;
	else exit(-1);
	
	if(queries.FillPoints(filename2) == 0) cout << "Filling query points successful"<<endl;
	else exit(-1);


	int dimension = input.get_dimension();
	cout << endl << "Dimension = "<< dimension <<endl;


    Cluster* clusters = new Cluster[k];  // create k clusters

	Initialize_Centroids(input,clusters,k); // k-means++
}