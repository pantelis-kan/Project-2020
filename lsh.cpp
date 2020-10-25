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

double w = 30000.0;

const long long int m = 4294967291; // 2^32  - 5

int N = 1;
double R = 10000.0;
int k = 4, L = 5;
string filename = "train-images-idx3-ubyte";
string filename2 = "t10k-images-idx3-ubyte";
string outputfile = "lsh_results.txt";


std::default_random_engine rand_generator(time(NULL));

int main(int argc, char* argv[]){

	/******************************************
	 * Parse execution arguments on runtime.
	 *****************************************/

	for (int i = 1; i < argc; i+=2){
		string arg = argv[i];
		if (arg == "-d"){
			filename = argv[i+1];
		}
		else if (arg == "-q"){
			filename2 = argv[i+1];
		}
		else if (arg == "-k"){
			k = atoi(argv[i+1]);
			if (k == 0){
				cout << "Wrong -k parameter! Please try again!" << endl;
				exit(1);
			}
		}
		else if (arg == "-L"){
			L = atoi(argv[i+1]);
			if (L == 0){
				cout << "Wrong -L parameter! Please try again!" << endl;
				exit(1);
			}				
		}
		else if (arg == "-o"){
			outputfile = argv[i+1];
		}
		else if (arg == "-N"){
			N = atoi(argv[i+1]);
			if (N == 0){
				cout << "Wrong -N parameter! Please try again!" << endl;
				exit(1);
			}
		}
		else if (arg == "-R"){
			R = atof(argv[i+1]);
			if (R == 0.0){
				cout << "Wrong -R parameter! Please try again!" << endl;
				exit(1);
			}
		}
		else{
			cout << "Wrong parameteres! Please try again!" << endl;
			exit(1);
		}

	}

	int M = pow(2,32/k);

	cout << k << " " << L << " " << N << " " << R << " " <<outputfile <<endl;
	
	/******************************************
	 * Reading input and query datasets.
	 *****************************************/

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
	
	/******************************************
	 * Building si parameters needed for amplification
	 *****************************************/

	// every h (h1, h2, ..., hk) has its own parameters for the amplification
	// definition of si parameter with i = 0,1,...,d-1
	double**  s_params = new double*[L*k];
	
	for (int i = 0; i < L*k; i++){
		s_params[i] = new double[dimension];
	} 

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

	/******************************************
	 * Building LSHashtable and storing input data
	 * Finding nearest neighbors with approximate and Range methods.
	 *****************************************/	

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

	LSH_Nearest_Neighbors(results, H_Tables, input, queries, queries_count, TableSize, s_params, L, k, M, m, w, N);

	LSH_Range_Search(results, H_Tables, input, queries, queries_count, TableSize, s_params, L, k, M, m, w, R);

	cout << "Stage 2 completed!" << endl;

	/******************************************
	 * Exporting results to output file with the required format
	 *****************************************/

	cout << "Stage 3: Exporting results to file" << endl;
	string exact_NN_fp = "exact_results.txt";
	Exact_NN_readonly(results, queries_count, N, exact_NN_fp);

	ofstream final_results;
	final_results.open(outputfile, ios::out | ios::trunc);

	for (int i = 0; i < queries_count; i++){
		final_results << "Query: " << results[i].get_query_id() << endl;
		
        vector <int> temp_N_nearest_id = results[i].get_N_nearest_id();
        vector <double> temp_N_nearest_distance = results[i].get_N_nearest_distance();
	    vector <double> temp_exact_N_nearest = results[i].get_exact_N_nearest();
        vector <int> temp_Range_nearest = results[i].get_Range_nearest();

		int counter = 1;
		auto it_distance = temp_N_nearest_distance.cbegin();
		auto it_exact_distance = temp_exact_N_nearest.cbegin();

		for(auto it_id = temp_N_nearest_id.cbegin(); it_id != temp_N_nearest_id.cend(); ++it_id){
    		final_results << "Nearest neighbor-" << counter << ": " << *it_id << endl;
			final_results << "distanceLSH: " << *it_distance << endl;
			final_results << "distanceTrue: " << *it_exact_distance << endl;

			it_distance++;
			it_exact_distance++;
			counter++;
		}

		final_results << "tHypercube: " << results[i].get_t_NN() << endl; 
		final_results << "tTrue: " << results[i].get_tTrue() << endl;


		final_results << "R-near neighbors:" << endl;		

		for(auto it_range = temp_Range_nearest.cbegin(); it_range != temp_Range_nearest.cend(); ++it_range){
			final_results << *it_range << endl;
		}

	}


	final_results.close();

	for(int i = 0; i < L;  i++) delete H_Tables[i]; 
	delete[] H_Tables;

	for (int i = 0; i < k; i++){
		delete[] s_params[i];
	}
	delete[] s_params;

	return 0;
}


