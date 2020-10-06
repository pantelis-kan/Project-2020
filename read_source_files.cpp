#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int reverseInteger (int i) 
{
    unsigned char c1, c2, c3, c4;

    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;

    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

void read_input_file(const char* input_fp){			/*check that in main string is made with c_str()*/

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
	

	/* Allocating the array with the images */
	Point* array_with_images;
	array_with_images = new Point [number_of_images];

	/* Reading the images, pixel-by-pixel and storing them to the array */
	for(int i=0; i < number_of_images; i++){
	    for(int row = 0; row < num_rows; row++){
	        for(int col = 0; col < num_cols; col++){
	            unsigned char temp = 0;
	            myfile.read((char*)&temp, sizeof(temp));
				
	        }
	    }
	}

	myfile.close();
	if(!myfile.good()) {
	  cout << "Error occurred at reading!" << endl;
	  return 1;
	}
    
	return 0;
}

void read_query_file(){





}