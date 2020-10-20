#ifndef NN_FUNCTIONS_HPP
#define NN_FUNCTIONS_HPP


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
#include "Hypercube.hpp"

void Preprocessing(Hash_Table** H_Tables,Point_Array& input, int input_count, int TableSize, double** s_params, int L,
					int k, int M, long long int m, double w);

void LSH_Nearest_Neighbors(Results* results, Hash_Table** H_Tables, Point_Array& input, Point_Array& queries,
				int queries_count, int TableSize, double** s_params,int L, int k, int M, long long int m, double w, int N);

void LSH_Range_Search(Results* results, Hash_Table** H_Tables, Point_Array& input, Point_Array& queries,
				int queries_count, int TableSize, double** s_params,int L, int k, int M, long long int m, double w, double R);

void Exact_NN(Point_Array& input, Point_Array& queries,int N,int N_q,ofstream& outfile,int* time_passed);
double compute_w(Point_Array& input, int N);

void Cube_Nearest_Neighbors(Results* results, Hypercube* hcube, Point_Array& input, int input_count, Point_Array& queries, int queries_count,
					double** s_params, int M_lsh, long long int m_lsh, double w, int k, int M, int probes, int N, double R);

void Cube_Range_Search(Results* results, Hypercube* hcube, Point_Array& input, int input_count, Point_Array& queries, int queries_count, 
					double** s_params, int M_lsh, long long int m_lsh, double w, int k, int M, int probes, int N, double R);


#endif
