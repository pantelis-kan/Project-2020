

#include <iostream>
#include <string>
#include <iterator>
#include <random>
#include <algorithm>
#include <limits>
#include <vector>

using namespace std;

#include "Cluster.hpp"
#include "Point_Table.hpp"
#include "Point.hpp"
#include "utilities.hpp"

// Initilize uniform random generator
std::default_random_engine rand_generator(time(NULL));

Cluster::Cluster(){
    centroid = new Point;
    objective_function_value = 0.0;
}

void Cluster::Add_Objective(double val){
    //cout << "Adding to objective function : " << val<<endl;
    objective_function_value += val;
}

long double Cluster::Get_Objective(){
    return objective_function_value;
}


Cluster::~Cluster(){
    delete centroid;
}

void Cluster::Assign_Centroid(Point* c,int id){
   centroid_id = id; 
   Copy_Points(c,centroid);
}

Point* Cluster::get_centroid(){
    return centroid;
}


/* Min distance from a point to all the available centroids
    size = number of already assigned centroids
    Returns the nearest cluster index (0-k) 
*/
double Min_Centroid_Distance(Point& point, Cluster* clusters,int size){

    int nearest_neighbor_id;
	double min_distance = std::numeric_limits<double>::max();
    double distance;

    int nearest_centroid_id = 0;

    for(int i = 0; i < size; i++){
        Point& centr = *(clusters[i].get_centroid());

        distance = Distance(point,centr,1);

        if (distance < min_distance) min_distance = distance;
    }

    return min_distance;
}


/*
Returns the right index of key
example: 34.0 < key < 40.0
The function will return the index of 40.0 
*/
int binary_search_index(double *A,double key,int left, int right)
{

  while (left <= right) {
        int middle = left + (right - left) / 2;
        if (A[middle] < key)
            left = middle+1;
        else if(A[middle] > key)
            right = middle-1;
        else
            return middle;
    }
    return (right + 1);
}



void Initialize_Centroids(Point_Array& input,Cluster* clusters,int k){

    int input_points = input.get_ArraySize();

    // Pick the first centroid uniformly at random
    std::uniform_real_distribution<double> first_centroid(0.0,(double)input_points - 1); 
    double first = first_centroid(rand_generator);

    int first_centr = (int)first;

    input.set_centroid(first_centr,true); // mark the first centroid

    Point* point = input.Retrieve_ptr(first_centr);
    clusters[0].Assign_Centroid(point,first_centr); // assign first centroid

    cout << "1st centroid chosen at index " << first_centr + 1 << endl;


    for(int t = 1; t < k; t++ ){


        double prob[input_points - t];
        double D[input_points - t];

        prob[0] = 0;
        
        /* Compute distances and probabilities.
            They must be re-calculated each time a new
            centroid is selected.
        */

        int position = 0;
        for(int i = 0; i < input_points; i++){

            // if the point is a centroid, ignore
            if(input.check_centroid(i) == true) continue;
            
            Point& pt = input.Retrieve(i);
            D[position] = Min_Centroid_Distance(pt,clusters,t);

            // P[0] = 0
            // P[i] = P[i-1] + (D[i])^2
            if(position > 0){
                prob[position] = prob[position-1] + pow(D[position],2);
            }

            ++position;
        }

        // the maximum element is always the last element
        double max_P = prob[input_points -t-1];

        // Set up a uniform random distribution from (0,P(r)]
        std::uniform_real_distribution<double> distribution(1.0,max_P); 
        double x = distribution(rand_generator);

        int index_of_x = binary_search_index(prob, x, 0, input_points - t);

        bool is_centr = true;

        /*
            If the index of x is an already assigned centroid,
            increment the index until you find a point that isn't assigned as centroid
        */
        while(is_centr == true){

            Point* next_centroid = input.Retrieve_ptr(index_of_x);
            
            if(next_centroid->check_centroid() == true) index_of_x++;
            else{
                // centroid found. Assign to cluster
                cout << t+1 << " centroid chosen at index "<< index_of_x << endl;
                clusters[t].Assign_Centroid(next_centroid,index_of_x);
                is_centr = false;
            }
        }


        //delete[] prob;
        //delete[] D;
    }

}

int Cluster::Cluster_Size(){

    return points.size();
}

void Cluster::Assign_Point(int id){
   
    points.push_back(id);
   // cout << "push back successful current size = " << points.size() << endl;
}

void Cluster::Compute_New_Centroid(Point_Array& input,int cluster_num){

	
	int points_in_cluster = points.size();
    int min_centroid;
    double min_distance = std::numeric_limits<double>::max();
    int next_id;

    if(points_in_cluster == 0) return;

    

    int old_centroid_id = centroid_id;

    cout << "Before loop ok" <<endl;

    int dimension = this->centroid->get_dimension();

    for(int j = 0; j < points_in_cluster; j++){

        long double sum = 0;
        /* for each point x that belongs in the cluster:
            1) Compute the distances from x to all the other points int the cluster
            2) Compute the distance from x to the centroid
            3) Add the distances = objective function
            4) Find the minimum sum

            5) If one of the points has the minimum objective function,
            place the id of x in the centroid_id and delete x from the vector
            and add the old centroid_id to the vector

            6) In the Point_Array table, disassociate the old centroid, set x as nearest cluster
            and set x as the new centroid
        */

        std::list<int>::iterator it = points.begin();
        std::advance(it, j);

        next_id = *it;
        Point& possible_centroid = input.Retrieve(next_id);
        Point& centr = *centroid;

        sum += Distance(centr,possible_centroid,1);

        for(int m = 0; m < points_in_cluster; m++){
            if(m == j) continue;

            std::list<int>::iterator it = points.begin();
            std::advance(it, m);

            int id = *it;
            Point& pt = input.Retrieve(id);

            sum += Distance(pt,possible_centroid,1);
        }

        if(sum < min_distance){
            min_distance = sum;
            min_centroid = next_id;
        }
    }

    // 5)
    if(min_distance < objective_function_value){

        objective_function_value = min_distance;

        cout << "Found min < obj_function = " << min_distance <<endl;

        Point* new_centroid = input.Retrieve_ptr(next_id);
        new_centroid->set_centroid(true);

        Remove_Point(next_id);
        Assign_Point(old_centroid_id);
        
        centroid->set_centroid(false);
        centroid->Assign_Cluster(cluster_num);

        centroid = new_centroid;
        centroid_id = next_id;
    }

    /*
	// for each dimension
	for(int i = 0; i < dimension; i++){
		sum = 0;

		//for each id that belongs in the cluster
		for (it = points.begin(); it != points.end(); ++it){
			int id = *it;

			Point* p = input.Retrieve_ptr(id);
			sum += p->get_coordinate(i);
		}

		sum = sum/points_in_cluster;
		centroid->set_coordinate(i,sum);
	}
    */

    /*
    cout << "Printing new centroid " <<endl;
    centroid->PrintPoint();
    */
}


void Cluster::Remove_Point(int id){

	//points.remove(id);

    auto it = std::find(points.begin(), points.end(), id);

    points.erase(it);
}

int Cluster::Retrieve_ID(int position){

    std::list<int>::iterator it = points.begin();
    std::advance(it, position);

    return (*it);
}