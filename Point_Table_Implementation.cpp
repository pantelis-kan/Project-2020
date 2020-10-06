#include "Point_Table.hpp"

#include "utilities.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <iterator>

using namespace std;

Point_Array::Point_Array(int num){

	points = new Point[num];
	ArraySize = num;

}


Point_Array::~Point_Array(){

	delete[] points;

}

int Point_Array::get_dimension(){

	if(ArraySize > 0) return points[0].get_dimension();

	return 0;

}

/*
long long int Point_Array::LSH_Manhattan( int M, const long long int m, double w,int position){

	return points[position].LSH_Manhattan(M,m,w);

}*/



void Point_Array::FillPoints(string filename){

	string line2;
	ifstream myfile2(filename);

	int n = 0;

	while(getline(myfile2,line2)){


		int value;
		stringstream parse(line2);

		while(parse >> value)
			points[n].AddtoPoint(value);

		// remove the id since the index of the array implies the id
		points[n].DeletefromPoint(points[n].Start());
		++n;
	}
}

int Point_Array::get_ArraySize(){
	return ArraySize;
}

int Point_Array::Compute_g(int position,int k,int M, const long long int m, double w,int tablesize, double** s_params,int l){

	// Call h(x) k times and concatenate to a string. Then compute mod HashTableSize

	int g_x;
	string concat = "";
	long long h;
	int h_int;
	int conc = 0;

	for(int i = 0; i < k; i++){
		h = points[position].LSH_Manhattan(M,m,w,s_params,i+l*k);

		h_int = h;

		//concat += to_string(h);
		conc = (conc << 16) | h_int; // bitwise concatenation
	}

	//int conc = stoi(concat);

	//cout << "String : " << concat << "   int : " << conc <<endl;
	//g_x = string_mod(concat,tablesize);  //  ( h1 | h2 | ... | hk ) % tablesize
	
	g_x = conc % tablesize;
	return g_x;

}


Point& Point_Array::Retrieve(int position){

	return points[position];

}



