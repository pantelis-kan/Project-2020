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

#include "Hypercube.hpp"
#include "Point_Table.hpp"
#include "Point.hpp"
#include "utilities.hpp"

using namespace std;

int k = 3,M=10,probes=2,N=1;
double R = 1.0;


std::default_random_engine rand_generator(time(NULL));

int main(){

	string filename = "input_small_id";
	string filename2 = "query_small_id";

	//string filename = "testpoints";
	//string filename2 = "simple_query";

	int N = NumberOfPoints(filename);   // number of input points
	int N_q = NumberOfPoints(filename2); // number of query points
	int TableSize = N/4;
	cout << "Number of points is : " << N <<endl;
	cout << "Number of queries is : " << N_q <<endl;
	cout << "TableSize = " <<TableSize <<endl;


	Point_Array input(N);
	Point_Array queries(N_q);


	cout << "Filling input points"<<endl;
	input.FillPoints(filename);
	
	cout <<"Filling query points"<<endl;
	queries.FillPoints(filename2);


	int dimension = input.get_dimension();
	cout << endl << "Dimension = "<< dimension <<endl;
	

	// s_params for h functions

		// every h has its own parameters;
	double**  s_params = new double*[k];
	
	for (int i = 0; i < L*k; i++){
			
		s_params[i] = new double[dimension];
	} 


	std::uniform_real_distribution<double> distribution(0.0,w);

	// create s parameters for each hi. Since there is only one hash table, there are k rows in the table
	for(int i = 0; i < k; i++){
		
		for(int j = 0; j < dimension; j++) {

	
			double rand = distribution(rand_generator);
			s_params[i][j] = rand;

		}
	}


	/*
	Hypercube cube(k);  // k = d'

	cout << "Step 1: Mapping each point to a vertex" <<endl;
	cube.Map_points(input);
	cout << "Step 1 complete"<<endl;
	

	cout << "Step 2: Finding Nearest Neighbors" <<endl;
	Cube_Nearest_Neighbors();
	cout << "Step 2 complete"<<endl;
	*/

}


