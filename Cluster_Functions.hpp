#ifndef CLUSTER_FUNCTIONS_HPP
#define CLUSTER_FUNCTIONS_HPP


#include <iostream>
#include <vector>


#include "Point_Table.hpp"
#include "Point.hpp"
#include "NN_Functions.hpp"
#include "Hash_Table.hpp"
#include "utilities.hpp"
#include "Cluster.hpp"

using namespace std;

void Initialize_Centroids(Point_Array&,Cluster*,int );

// Loyd's functions
void Loyds_Clusters(Point_Array& input,Cluster* clusters,int k);
bool Loyds_Assign(Point_Array& input,Cluster* clusters,int k,int* assigned);
double Min_Centroid_Distance(Point& point, Cluster* clusters,int size);

void Distance_From_Centroids(Cluster* clusters, int k);
int Min_Centroid_From_Point(Point& point, Cluster* clusters,int size);

// Reverse Assignment Functions
void Reverse_Assignment(Point_Array& input,Cluster* clusters,int k,bool lsh);
bool LSH_Reverse_Assignment(Point_Array& input,Cluster* clusters,int k,
						Hash_Table** H_Tables,double** params,int* assigned,int TableSize);

// Update is common 
void Update(Point_Array& input,Cluster* clusters,int k);

#endif