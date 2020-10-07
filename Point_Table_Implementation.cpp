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


/* OLD

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
*/

int Point_Array::FillPoints(string &input_fp){

	ifstream myfile;
	//string line;
	
	myfile.open(input_fp, ios::out | ios::binary);

	if (!myfile.is_open()) {						/* probably exception handling here */
	    cout << "Cannot open file!" << endl;
		return 1;									/* what return value is proper? NULL? */
	}


	/* Initialize all with zero */
	int magic_number = 0;								/* Read and ignore it! */
	int number_of_images = 0;
	int num_rows = 0;
	int num_cols = 0;
	
	myfile.read((char*)&magic_number, sizeof(magic_number)); 
	magic_number = reverseInteger(magic_number);
	
	myfile.read((char*)&number_of_images, sizeof(number_of_images));
	number_of_images = reverseInteger(number_of_images);
	
	myfile.read((char*)&num_rows, sizeof(num_rows));
	num_rows = reverseInteger(num_rows);
	
	myfile.read((char*)&num_cols, sizeof(num_cols));
	num_cols = reverseInteger(num_cols);
	
	cout << "Magic Number is:" << magic_number << endl;
	cout << "Number of images is:" << number_of_images << endl;
	cout << "Number of rows is:" << num_rows << endl;
	cout << "Number of columns is:" << num_cols << endl;

	/* Allocating the array with the images */
	Point* array_with_images;
	array_with_images = new Point [number_of_images];

	int n = 0;

	/* Reading the images, pixel-by-pixel and storing them to the array */
	for(int i=0; i < number_of_images; i++){	// change back to i < number_of_images
	    for(int row = 0; row < num_rows; row++){
	        for(int col = 0; col < num_cols; col++){

	            unsigned char temp = 0;
	            myfile.read((char*)&temp, sizeof(temp));
				points[n].AddtoPoint((int) temp);
				
				//array_with_images[i].PrintPoint();
				//cout << (int) temp << ", "; 
	        }
	    }
		++n;
		//array_with_images[i].PrintPoint();
		cout << endl;
	}

	myfile.close();
	if(!myfile.good()) {
	  cout << "Error occurred at reading!" << endl;
	  return 1;
	}
    
	return 0;

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



