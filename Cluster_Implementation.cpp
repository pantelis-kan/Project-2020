#include "Cluster.hpp"

#include <iostream>
#include <string>
#include <iterator>
#include <random>
#include <limits>

using namespace std;

#include "Point_Table.hpp"
#include "Point.hpp"
#include "utilities.hpp"
#include "NN_Functions.hpp"

// Initilize uniform random generator
std::default_random_engine rand_generator(time(NULL));


void Cluster::Assign_Centroid(Point* c){
    centroid = c;
}

Point& Cluster::get_centroid(){
    return centroid;
}

void Initialize_Centroids(Point_Array& input,Cluster* clusters,int k){

    int input_points = input.get_ArraySize();

    // Pick the first centroid uniformly at random
    std::uniform_real_distribution<int> first_centroid(0,input_points); 
    int first_centr = first_centroid(rand_generator);

    input.set_centroid(first_centr,true); // mark the first centroid

    Point& point = input.Retrieve(first_centr);
    clusters[0].Assign_Centroid(point); // assign first centroid

    for(int t = 1; t < k; t++ ){


        double* prob = new double[input_points - t];
        double* D = new double[input_points - t];

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

        

        // Set up a uniform random distribution from (0,P(r)]
        std::uniform_real_distribution<double> distribution(1,max_P); 
        double x = distribution(rand_generator);

        delete[] prob;
    }


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
        Point& centr = clusters[i].get_centroid();

        distance = Distance(point,centr,1);

        if (distance < min_distance) min_distance = distance;
    }

    return min_distance;
}