#ifndef UTILITIES_HPP
#define UTILITIES_HPP


#include <iostream>
#include <cmath>
#include <chrono>
#include <random>
#include <string.h>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

int modular(int base,unsigned int exp , unsigned int mod);
int powxy(long long int x, int y,int M);
int string_mod(string num, int a);
int NumberOfPoints(string&);
double FRandomGen(double low, double high, std::default_random_engine generator);
int reverseInteger (int);

int power(long long int x, int y, int p);
 int mod( int k,  int n);
int bigMod(int  a,int  b,int c);

int moduloMultiplication(long long int a, 
                            int b, 
                            int mod);

int partition (int* arr, int low, int high);
void quickSort(int* arr, int low, int high);
void swap(int* a, int* b);
int binary_search_index(double *A,double key,int left, int right);


#endif
