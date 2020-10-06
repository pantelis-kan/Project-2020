
#include "Point.hpp"
#include "utilities.hpp"

using namespace std;

//std::default_random_engine generator(time(NULL));


Point::Point(){

}

Point::~Point(){


}


int Point::get_dimension(){

	return point.size();
}



void Point::PrintPoint(){

		for(vector<int>::const_iterator j = point.begin(); j != point.end(); j++)
			cout << *j << ' ';

	
		cout << endl << endl;

}


void Point::AddtoPoint(int val){ // add val to point vector

	point.push_back(val);

}


void Point::DeletefromPoint(vector<int>::const_iterator index){

	point.erase(index);

}


 // computes h(x) 
long long int Point::LSH_Manhattan(int M, const long long int m, double w, double** s_params,int current_k){ 	

	long long int dimension = point.size();

	
	//std::uniform_real_distribution<double> distribution(0.0,w);


	int* coeff = new int[dimension];
	long long hash = 0;
	long long result;
	
	//cout << "Random number from 0 to "<<w << " = " << FRandomGen(0.0,w,generator) << endl;

	vector<int>::const_iterator it = point.begin();

	//cout << "Chosen coefficients : "; 

	//cout << current_k << endl;

	for(int i = 0; i < dimension; i++){

		//cout <<  s_params[current_k][i] << endl;
		coeff[i] = ceil( ((*it) - s_params[current_k][i]) / w );
		//cout << coeff[i] << "  ";
		++it;
	}


	//int m = *max_element(coeff,coeff+dimension) + 1;

	//cout << "m = " << m<<endl;
	//cout << "M = " << M << endl;
	
	int k = 0;
	for(int j = dimension-1; j >= 0; j--){
		result = moduloMultiplication(powxy(m,k,M),coeff[j]%M,M); // (m^k mod M * aj mod M ) mod M
		//cout << "Adding " << result <<endl;
		hash += result%M; 
		++k;
	}

	delete[] coeff;
	return hash%M;

}


vector<int>::const_iterator Point::Start(){
	
	return point.begin();
}



double Distance(Point& v1, Point& v2, int metric){

	double dist = 0.0;
	double sum = 0.0;


	int dimension = v1.point.size();
	vector<int>::const_iterator it1 = v1.point.begin();
	vector<int>::const_iterator it2 = v2.point.begin();

	for(int i = 0; i < dimension; i++){
		
		sum = abs((*it1) - (*it2));
		sum = pow(sum,metric);


		//cout << "Iteration "<< i << ". Vector numbers : " << (*it1)<<"  "<<(*it2)<< ". Adding  "<<sum<<endl;
		dist += sum;
		//cout << "Dist in while : " << dist <<endl;
		++it1;
		++it2;

	}

	
	//cout << "dist before root : "<<dist <<endl;
	dist = pow(dist,1.0/metric);

	return dist;

}



