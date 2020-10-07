
#include "Point.hpp"
#include "utilities.hpp"

using namespace std;

//std::default_random_engine generator(time(NULL));


Point::Point(){
	dimension = 784;
}

Point::~Point(){


}


int Point::get_dimension(){

	return dimension;
}



void Point::PrintPoint(){

		for(int j = 0; j < dimension;  j++)
			cout << point[j] << ' ';

	
		cout << endl << endl;

}


void Point::AddtoPoint(int pos,int val){ // add val to point vector

	//cout << "Pushing " << val <<endl;
	point[pos] = val;

}



 // computes h(x) 
long long int Point::LSH_Manhattan(int M, const long long int m, double w, double** s_params,int current_k){ 	

	
	//std::uniform_real_distribution<double> distribution(0.0,w);

	int Dimension = get_dimension();

	int* coeff = new int[Dimension];
	long long hash = 0;
	long long result;
	
	//cout << "Random number from 0 to "<<w << " = " << FRandomGen(0.0,w,generator) << endl;


	//cout << "Chosen coefficients : "; 

	//cout << current_k << endl;

	for(int i = 0; i < Dimension; i++){

		//cout <<  s_params[current_k][i] << endl;
		coeff[i] = ceil( (point[i] - s_params[current_k][i]) / w );
		//cout << coeff[i] << "  ";
		
	}


	//int m = *max_element(coeff,coeff+dimension) + 1;

	//cout << "m = " << m<<endl;
	//cout << "M = " << M << endl;
	
	int k = 0;
	for(int j = Dimension-1; j >= 0; j--){
		result = moduloMultiplication(powxy(m,k,M),coeff[j]%M,M); // (m^k mod M * aj mod M ) mod M
		//cout << "Adding " << result <<endl;
		hash += result%M; 
		++k;
	}

	delete[] coeff;
	return hash%M;

}




double Distance(Point& v1, Point& v2, int metric){

	double dist = 0.0;
	double sum = 0.0;

	int Dimension = v1.get_dimension();
	

	for(int i = 0; i < Dimension; i++){
		
		sum = abs(v1.point[i] - v2.point[i]);
		sum = pow(sum,metric);


		//cout << "Iteration "<< i << ". Vector numbers : " << (*it1)<<"  "<<(*it2)<< ". Adding  "<<sum<<endl;
		dist += sum;
		//cout << "Dist in while : " << dist <<endl;

	}

	
	//cout << "dist before root : "<<dist <<endl;
	dist = pow(dist,1.0/metric);

	return dist;

}



