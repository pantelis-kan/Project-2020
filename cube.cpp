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
int k = 3, M=60000, probes=3, N=14;
double R = 10000;
double w = 3000.0;

int M_lsh = pow(2,32/k);
const long long int m_lsh = 4294967291;


std::default_random_engine rand_generator(time(NULL));

int main(){

	string filename = "train-images-idx3-ubyte";
	string filename2 = "t10k-images-idx3-ubyte";

	int input_count = NumberOfPoints(filename);   // number of input points
	int queries_count = NumberOfPoints(filename2); // number of query points

	cout << "Number of input images is : " << input_count <<endl;
	cout << "Number of queries is : " << queries_count <<endl;

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

	cout << "Step 1: Mapping each image to a vertex" <<endl;
	
	Hypercube cube(k);  // k = d'
	
	auto t1 = std::chrono::high_resolution_clock::now();		
	cube.Map_images(input, input_count, k, s_params, M_lsh, m_lsh, w, &cube);
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

//	cube.print_vertex_table();
	cout << "Stage 1 completed in " << duration << " seconds" << endl;

	cout << "Step 2: Finding Nearest Neighbors" <<endl;
	Results results[queries_count];
	Cube_Nearest_Neighbors(results, &cube, input, input_count, queries, queries_count, s_params, M_lsh, m_lsh, w, k, M, probes, N, R);

	Cube_Range_Search(results, &cube, input, input_count, queries, queries_count, s_params, M_lsh, m_lsh, w, k, M, probes, N, R);

	cout << "Step 2 complete"<<endl;

	cout << "Step 3: Exporting results to file" << endl;
	ofstream final_results;
	final_results.open("hypercube_results.txt", ios::out | ios::trunc);

//	for (int i = 0; i < queries_count; i++){
	for (int i = 0; i < 1; i++){
		final_results << "Query: " << results[i].get_query_id() << endl;
		
        vector <int> temp_N_nearest_id = results[i].get_N_nearest_id();
        vector <double> temp_N_nearest_distance = results[i].get_N_nearest_distance();
//      vector <double> temp_exact_N_nearest = results[i].get_exact_N_nearest();
        vector <int> temp_Range_nearest = results[i].get_Range_nearest();

		int counter = 1;
		auto it_distance = temp_N_nearest_distance.cbegin();
//		auto it_exact_distance = temp_exact_N_nearest.cbegin();

		for(auto it_id = temp_N_nearest_id.cbegin(); it_id != temp_N_nearest_id.cend(); ++it_id){
    		final_results << "Nearest neighbor-" << counter << ": " << *it_id << endl;
			final_results << "distanceHypercube: " << *it_distance << endl;
//			final_results << "distanceTrue: " << *it_exact_distance << endl;

			it_distance++;
//			it_exact_distance++;
			counter++;
		}

		//print times here also


		final_results << endl;
		final_results << "R-near neighbors:" << endl;		

		for(auto it_range = temp_Range_nearest.cbegin(); it_range != temp_Range_nearest.cend(); ++it_range){
			final_results << *it_range << endl;
		}

	}


	final_results.close();

	for (int i = 0; i < k; i++){
		delete[] s_params[i];
	}
	delete[] s_params;

}


