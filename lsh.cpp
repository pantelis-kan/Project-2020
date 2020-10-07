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
double w = 2000.0;

//const int M = 4294967291; 3.43597e+10

 int M = pow(2,32/k);
const long long int m = 4294967291;
//const long long int M = 673109;

std::default_random_engine rand_generator(time(NULL));


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
	Exact_NN(input,queries,N,N_q);

	cout << endl << "--Stage 1: Preprocessing stage...-- "<<endl;

	// every h has its own parameters;
	double**  s_params = new double*[L*k];
	
	for (int i = 0; i < L*k; i++){
			
		s_params[i] = new double[dimension];
	} 


	std::uniform_real_distribution<double> distribution(0.0,w);

	// create s parameters for each hi. Since there are L hash tables, there are L*k rows in the table
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

	Preprocessing(H_Tables,input, N, TableSize,s_params,L,k,M,m,w);

	cout << "Stage 1 completed! " << endl;

	cout << "--Stage 2: Checking each query --" <<endl;

	Nearest_Neighbors(H_Tables,input,queries,N_q,TableSize,s_params,L,k,M,m,w);

	cout << "Stage 2 completed!" << endl;
	
}


