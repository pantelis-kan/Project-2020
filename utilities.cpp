

#include "utilities.hpp"

using namespace std;

int string_mod(string num, int a) 
{ 
    // Initialize result 
    int res = 0; 
  
    // One by one process all digits of 'num' 
    for (int i = 0; i < num.length(); i++) 
         res = (res*10 + (int)num[i] - '0') %a; 
  
    return res; 
}



// Returns (a * b) % mod 
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




int NumberOfPoints(string filename){

	int number_of_lines = 0;


	string line;
	ifstream myfile(filename);

	while(getline(myfile,line)){

		++number_of_lines;
	}

	return number_of_lines;

}


double FRandomGen(double low, double high, std::default_random_engine generator){

	//Create random (uniform distribution) float number	
	
	uniform_real_distribution<double> distribution (low,high);

	double k = distribution(generator);
	
	return k;
}



