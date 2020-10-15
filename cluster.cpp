#include <iostream>
#include <string.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iterator>
#include <limits>
#include <cmath>
#include <vector>
#include <chrono>
#include <random>
#include <vector>

#include "Point_Table.hpp"
#include "Point.hpp"
#include "NN_Functions.hpp"
#include "Hash_Table.hpp"
#include "utilities.hpp"
#include "Cluster.hpp"
#include "Cluster_Functions.hpp"

using namespace std;

int k = 10; // k = number of clusters



int main(){

    string filename = "train-images-idx3-ubyte";
	string filename2 = "t10k-images-idx3-ubyte";

    int N = NumberOfPoints(filename2);   // number of input points
	int TableSize = N/8;
	cout << "Number of points is : " << N <<endl;
	cout << "TableSize = " <<TableSize <<endl;


	Point_Array input(N);
	
	if(input.FillPoints(filename2) == 0) cout << "Filling input points successful"<<endl;
	else exit(-1);
	

	int dimension = input.get_dimension();
	cout << endl << "Dimension = "<< dimension <<endl;

	// create k clusters
    Cluster* clusters = new Cluster[k];  

	Initialize_Centroids(input,clusters,k); // k-means++
	cout << "Initialization complete"<<endl;

	//Loyds_Clusters(input,clusters,k);
	
	Reverse_Assignment(input,clusters,k,true);

	delete[] clusters;
}


