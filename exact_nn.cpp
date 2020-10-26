#include <iostream>
#include <string.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator>


#include "Point_Table.hpp"
#include "Point.hpp"
#include "utilities.hpp"
#include "NN_Functions.hpp"


int main(){

	/******************************************
	 * Reading input and query datasets.
	*****************************************/

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
	
	/******************************************
	 * Exporting results to output file with the required format
	*****************************************/

    ofstream outfile;
    outfile.open("exact_results.txt", ios::out | ios::trunc );

    int time_passed;
    Exact_NN(input, queries, input_count, queries_count, outfile, &time_passed);

    outfile << time_passed << endl;
    outfile.close();
}