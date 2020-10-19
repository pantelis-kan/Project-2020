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
	
    ofstream outfile;
    outfile.open("exact_nn.txt", ios::out | ios::trunc );

    int time_passed;
    Exact_NN(input,queries,N,N_q,outfile,&time_passed);

    outfile << time_passed << endl;
    outfile.close();
}