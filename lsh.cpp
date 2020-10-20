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
#include "Hash_Table.hpp"
#include "NN_Functions.hpp"

using namespace std;

int N = 5;
double R = 10000.0;

int k = 4,L = 5;
double w = 30000.0;

//const int M = 4294967291; 3.43597e+10

int M = pow(2,32/k);

//const long long int m = 536870912;  // 2^29
const long long int m = 4294967291; // 2^32  - 5
//const long long int M = 673109;

std::default_random_engine rand_generator(time(NULL));

int FillPoints_static(string &input_fp,int** array);

int main(){

	string filename = "train-images-idx3-ubyte";
	string filename2 = "t10k-images-idx3-ubyte";

	int input_count = NumberOfPoints(filename);   // number of input points
	int queries_count = NumberOfPoints(filename2); // number of query points
	int TableSize = input_count/8;

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
	
	// find Exact NN before approximate
	//Exact_NN(input,queries,input_count,queries_count);

	// every h (h1, h2, ..., hk) has its own parameters for the amplification
	// definition of si parameter with i = 0,1,...,d-1
	double**  s_params = new double*[L*k];
	
	for (int i = 0; i < L*k; i++){
		s_params[i] = new double[dimension];
	} 

//nomizw edw prepei na kanoume ton ypologismo tou w opws eipe o xamodragon me th mesi apostasi epi 4 me 10 fores
	//w = compute_w(input, input_count);


	//Limiting rand function to take values from 0.0 to w
	std::uniform_real_distribution<double> distribution(0.0,w);

	// create s parameters for each h(i). Since there are L hash tables, there are L*k rows in the table
	for(int i = 0; i < L*k; i++){
		
		for(int j = 0; j < dimension; j++) {
			double rand = distribution(rand_generator);
			s_params[i][j] = rand;
		}
	}


	cout << endl << "--Stage 1: Preprocessing stage...-- "<<endl;

	//create L hash tables
	Hash_Table**  H_Tables = new Hash_Table*[L];
	
	for (int i = 0; i < L; i++){	
		H_Tables[i] = new Hash_Table(TableSize);
	}

	auto t1 = std::chrono::high_resolution_clock::now();		
	Preprocessing(H_Tables, input, input_count, TableSize, s_params, L, k, M, m, w);
	auto t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count();

	cout << "Stage 1 completed in " << duration << " seconds" << endl;

	cout << "Stage 2: Finding Nearest Neighbors" <<endl;
	Results results[queries_count];

//raname this function to start with LSH_ , add N and also give access to results class
	LSH_Nearest_Neighbors(results, H_Tables, input, queries, queries_count, TableSize, s_params, L, k, M, m, w, N);

//	LSH_Range_Search(results, H_Tables, input, queries, queries_count, TableSize, s_params, L, k, M, m, w, R);

	cout << "Stage 2 completed!" << endl;

	cout << "Stage 3: Exporting results to file" << endl;
	ofstream final_results;
	final_results.open("lsh_results.txt", ios::out | ios::trunc);

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
			final_results << "distanceLSH: " << *it_distance << endl;
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


