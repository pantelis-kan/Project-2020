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
#include "NN_Functions.hpp"

using namespace std;

//default values in case no other given from user
int k = 3, M=10, probes=2, N=1;
double R = 1.0;
double w = 2000.0;

int M_lsh = pow(2,32/k);
const long long int m_lsh = 4294967291;


std::default_random_engine rand_generator(time(NULL));

int main(){

	string filename = "train-images-idx3-ubyte";
	string filename2 = "t10k-images-idx3-ubyte";

	int input_count = NumberOfPoints(filename);   // number of input points
	int queries_count = NumberOfPoints(filename2); // number of query points
	int TableSize = input_count/4;												// giati input_count/4?

	cout << "Number of points is : " << input_count <<endl;
	cout << "Number of queries is : " << queries_count <<endl;
	cout << "TableSize = " << TableSize <<endl;

	Point_Array input(input_count);
	Point_Array queries(queries_count);
	
	if(input.FillPoints(filename) == 0) cout << "Filling input points successful"<<endl;
	else exit(-1);
	
	if(queries.FillPoints(filename2) == 0) cout << "Filling query points successful"<<endl;
	else exit(-1);


	int dimension = input.get_dimension();
	cout << endl << "Dimension = "<< dimension <<endl;

	//d'=logN
//	int d = log2(input_count);


	// s_params for h functions
	// every h has its own parameters;
	double**  s_params = new double*[k];
	for (int i = 0; i < k; i++){
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

	Hypercube cube(k);  // k = d'

	cout << "Step 1: Mapping each image to a vertex" <<endl;
	auto t1 = std::chrono::high_resolution_clock::now();		
	cube.Map_images(input, input_count, k, s_params, M_lsh, m_lsh, w, &cube);
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count();

//	cube.print_vertex_table();
	cout << "Stage 1 completed in " << duration << " seconds" << endl;

	cout << "Step 2: Finding Nearest Neighbors" <<endl;
	Cube_Nearest_Neighbors(&cube, input, input_count, queries, queries_count, s_params, M_lsh, m_lsh, w, k, M, probes, N, R);

	cout << "Step 2 complete"<<endl;

}


