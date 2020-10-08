/* Methods required globally in order to perform different computations*/

#include "utilities.hpp"

using namespace std;

/* Reversing an integer */
int reverseInteger (int i) 
{
    unsigned char c1, c2, c3, c4;

    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;

    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

/* ???? */
int string_mod(string num, int a) 
{ 
    // Initialize result 
    int res = 0; 
  
    // One by one process all digits of 'num' 
    for (int i = 0; i < num.length(); i++) 
         res = (res*10 + (int)num[i] - '0') %a; 
  
    return res; 
}



/* Returns (a * b) % mod */ 
long long moduloMultiplication(long long a, 
							long long b, 
							long long mod) 
{ 
	long long res = 0; // Initialize result 

	// Update a if it is more than 
	// or equal to mod 
	a %= mod; 

	while (b) 
	{ 
		// If b is odd, add a with result 
		if (b & 1) 
			res = (res + a) % mod; 

		// Here we assume that doing 2*a 
		// doesn't cause overflow 
		a = (2 * a) % mod; 

		b >>= 1; // b = b / 2 
	} 

	return res; 
} 

// x^y mod M
long long int powxy(long long int x, long long int y,int M){

	//cout << "Called with arguments : " << x << "   " << y <<endl;
	if(y==0) return 1;
	if(y%2 == 1) return moduloMultiplication(x, powxy(x,y-1,M), M);
	long long int t = powxy(x,y/2,M);
	//cout << "t = "<< t <<endl;
	return moduloMultiplication(t, t, M);

}


int modular(int base,unsigned int exp , unsigned int mod){

	int x = 1;
	int i;
	int power = base % mod;

	for(i = 0; i< sizeof(int)*8; i++){

		int least_sig_bit = 0x00000001 & (exp >> i);
		
		if(least_sig_bit)
			x = (x*power) % mod;

		power = (power*power) % mod;

	}

	return x;

}


/* Counting the total number of images available in the input file */
int NumberOfPoints(string& input_fp){
	ifstream myfile;
	
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

	return number_of_images;

	myfile.close();

}


double FRandomGen(double low, double high, std::default_random_engine generator){

	//Create random (uniform distribution) float number	
	
	uniform_real_distribution<double> distribution (low,high);

	double k = distribution(generator);
	
	return k;
}



