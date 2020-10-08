#include "Cluster.hpp"

#include <iostream>
#include <string>
#include <iterator>
#include <random>

using namespace std;

#include "Point_Table.hpp"
#include "Point.hpp"
#include "utilities.hpp"

// Initilize uniform random generator
std::default_random_engine rand_generator(time(NULL));


void Cluster::Assign_Centroid(Point* c){
    centroid = c;
}

void Initialize_Centroids(Point_Array& input,Cluster* clusters,int k){

    int input_points = input.get_ArraySize();

    // Pick the first centroid uniformly at random
    std::uniform_real_distribution<int> first_centroid(0,input_points); 
    int first = first_centroid(rand_generator);

    input.set_centroid(first,true); // mark the first centroid

    Point* point = input.Retrieve_ptr(first);
    clusters[0].Assign_Centroid(point); // assign first centroid

    for(int t = 1; t < k; t++ ){

        int* prob = new int[input_points - t];

        

        // Set up a uniform random distribution from (0,P(r)]
        std::uniform_real_distribution<int> distribution(1,max_P); 
        int x = distribution(rand_generator);

        delete[] prob;
    }


}


