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
using namespace std::chrono;

//default values in case no other given from user
int k = 14, M=10, probes=2, N=1;

double R = 10000.0;
string filename = "train-images-idx3-ubyte";
string filename2 = "t10k-images-idx3-ubyte";
string outputfile = "hypercube_results.txt";
double w = 40000.0;

const long long int m_lsh = 4294967291;


std::default_random_engine rand_generator(time(NULL));

int main(int argc, char* argv[]){
	
	/******************************************
	 * Parse execution arguments on runtime.
	*******************************************/

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
			if (k <= 0){
				cout << "Wrong -k parameter! Please try again!" << endl;
				exit(1);
			}
		}
		else if (arg == "-M"){
			M = atoi(argv[i+1]);
			if (M <= 0){
				cout << "Wrong -M parameter! Please try again!" << endl;
				exit(1);
			}				
		}
		else if (arg == "-probes"){
			probes = atoi(argv[i+1]);
			if (probes <= 0){
				cout << "Wrong -probes parameter! Please try again!" << endl;
				exit(1);
			}				
		}
		else if (arg == "-o"){
			outputfile = argv[i+1];
		}
		else if (arg == "-N"){
			N = atoi(argv[i+1]);
			if (N <= 0){
				cout << "Wrong -N parameter! Please try again!" << endl;
				exit(1);
			}
		}
		else if (arg == "-R"){
			R = atof(argv[i+1]);
			if (R <= 0.0){
				cout << "Wrong -R parameter! Please try again!" << endl;
				exit(1);
			}
		}
		else{
			cout << "Wrong parameteres! Please try again!" << endl;
			exit(1);
		}

	}
	
	int M_lsh = pow(2,32/k);

	int option;
	
	cout << "Press 1 you want to run programme with default input data file. Press 2 if you want to choose other filename. Any other option will exit programme." << endl;
	cin >> option;
	if (option == 1){
		cout << "You have chosen the default filename!" << endl;
	}
	else if(option == 2){
		cout << "Insert input filename: "; 
		cin >> filename;
	}
	else{
		cout << "Programme is exiting!" << endl;
		exit(1);
	}
	
	cout << "Programme will run with filename: " << filename << " for input data" << endl; 

	/******************************************
	 * Reading input dataset.
	******************************************/

	int input_count = NumberOfPoints(filename);   // number of input points

	cout << "Number of input images is : " << input_count <<endl;

	Point_Array input(input_count);
	
	if(input.FillPoints(filename) == 0) cout << "Filling input points successful"<<endl;
	else exit(-1);
	

	int dimension = input.get_dimension();
	cout << endl << "Dimension = "<< dimension <<endl;

	/******************************************
	 * Building si parameters needed for amplification
	*******************************************/

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

	/******************************************
	 * Building Hypercube and storing input data
	*******************************************/	

	cout << "Stage 1: Mapping each image to a vertex" <<endl;
	
	Hypercube cube(k);  // k = d'

	auto t1 = std::chrono::high_resolution_clock::now();
	cube.Map_images(input, input_count, k, s_params, M_lsh, m_lsh, w, &cube);
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
	
	cout << "Stage 1 completed in " << duration << " microseconds" << endl;

	/******************************************
	 * Reading query set and output filenames from user
	 * Creating necessary structures for the queries
	 * Running until user terminates the programme
	*******************************************/	
	do{
		cout << "Press 1 you want to run programme with default search and output data file. Press 2 if you want to choose other filename. Any other option will exit programme" << endl;
		cin >> option;
		if (option == 1){
			cout << "You have chosen the default filename!" << endl;
		}
		else if(option == 2){
			cout << "Insert queries filename: "; 
			cin >> filename2;
			cout << "Insert output filename: "; 
			cin >> outputfile;
		}
		else{
			cout << "Programme is exiting!" << endl;
			exit(1);
		}

		cout << "Search will be done using file with name: " << filename2 << endl;
		cout << "Output will be exported to file with name: " << outputfile << endl;

		int queries_count = NumberOfPoints(filename2); // number of query points
		cout << "Number of queries is : " << queries_count <<endl;
		Point_Array queries(queries_count);
		if(queries.FillPoints(filename2) == 0) cout << "Filling query points successful"<<endl;
		else exit(-1);		

		/******************************************
		 * Finding nearest neighbors with approximate and Range methods.
		*******************************************/

		cout << "Stage 2: Finding Nearest Neighbors" <<endl;
		Results results[queries_count];
				
		Cube_Nearest_Neighbors(results, &cube, input, input_count, queries, queries_count, s_params, M_lsh, m_lsh, w, k, M, probes, N, R);

		Cube_Range_Search(results, &cube, input, input_count, queries, queries_count, s_params, M_lsh, m_lsh, w, k, M, probes, N, R);

		cout << "Stage 2 - Completed!"<<endl;

		/******************************************
		 * Exporting results to output file with the required format
		*******************************************/

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
				final_results << "distanceHypercube: " << *it_distance << endl;
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

		cout << "Stage 3 - Completed!" << endl;

		for (int i = 0; i < k; i++){
			delete[] s_params[i];
		}
		delete[] s_params;


		cout << "If you want to repeat search press 1! Any other answer will terminate the programme!" << endl; 
		cin >> option; 

	}while (option == 1);
	
	return 0;
}


