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

void Preprocessing(Hash_Table** H_Tables,Point_Array& input, int N, int TableSize, double** s_params,int L,int k,int M,long long int m,double w);

void Nearest_Neighbors(Hash_Table** H_Tables,Point_Array& input, Point_Array& queries,
				int N_q,int TableSize, double** s_params,int L,int k,int M,long long int m,double w );

void Exact_NN(Point_Array& input, Point_Array& queries,int N,int N_q);

void Preprocessing_Static(Hash_Table** H_Tables, int** input_array, int N, int TableSize, double** s_params, int L, 
					int k, int M, long long int m, double w );

#endif
