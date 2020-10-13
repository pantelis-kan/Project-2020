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
	
	// find Exact NN before approximate
	//Exact_NN(input,queries,N,N_q);

	// every h (h1, h2, ..., hk) has its own parameters for the amplification
	// definition of si parameter with i = 0,1,...,d-1
	double**  s_params = new double*[L*k];
	
	for (int i = 0; i < L*k; i++){
		s_params[i] = new double[dimension];
	} 

//nomizw edw prepei na kanoume ton ypologismo tou w opws eipe o xamodragon me th mesi apostasi epi 4 me 10 fores
	//w = compute_w(input, N);


	//Limiting rand function to take values from 0.0 to w
	std::uniform_real_distribution<double> distribution(0.0,w);

	// create s parameters for each h(i). Since there are L hash tables, there are L*k rows in the table
	for(int i = 0; i < L*k; i++){
		
		for(int j = 0; j < dimension; j++) {
			double rand = distribution(rand_generator);
			s_params[i][j] = rand;
		}
	}

	//create L hash tables
	Hash_Table**  H_Tables = new Hash_Table*[L];
	
	for (int i = 0; i < L; i++){	
		H_Tables[i] = new Hash_Table(TableSize);
	}



	cout << endl << "--Stage 1: Preprocessing stage...-- "<<endl;

	auto t1 = std::chrono::high_resolution_clock::now();		
	
	Preprocessing(H_Tables, input, N, TableSize, s_params, L, k, M, m, w);
	auto t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count();
//	Preprocessing(H_Tables, input, N, TableSize, s_params, L, k, M, m, w);

	cout << "Stage 1 completed in " << duration << " seconds" << endl;

	cout << "--Stage 2: Checking each query --" <<endl;

	Nearest_Neighbors(H_Tables, input, queries, N_q, TableSize, s_params, L, k, M, m, w);
//	Nearest_Neighbors(H_Tables, input, queries, N_q, TableSize, s_params, L, k, M, m, w);

	cout << "Stage 2 completed!" << endl;
	
}


