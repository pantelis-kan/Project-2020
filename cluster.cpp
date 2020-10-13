#include <iostream>
#include <string.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iterator>
#include <limits>
#include <cmath>
#include <vector>
#include <chrono>
#include <random>
#include <list>

#include "Point_Table.hpp"
#include "Point.hpp"
#include "utilities.hpp"
#include "Cluster.hpp"


using namespace std;

int k = 4; // k = number of clusters

void Initialize_Centroids(Point_Array&,Cluster*,int );
bool Loyds_Assign(Point_Array& input,Cluster* clusters,int k,int* assigned);
void Loyds_Clusters(Point_Array& input,Cluster* clusters,int k);
void Update(Point_Array& input,Cluster* clusters,int k);


int main(){

    string filename = "train-images-idx3-ubyte";
	string filename2 = "t10k-images-idx3-ubyte";

    int N = NumberOfPoints(filename2);   // number of input points
	int N_q = NumberOfPoints(filename2); // number of query points
	int TableSize = N/8;
	cout << "Number of points is : " << N <<endl;
	cout << "Number of queries is : " << N_q <<endl;
	cout << "TableSize = " <<TableSize <<endl;


	Point_Array input(N);
	Point_Array queries(N_q);


	
	if(input.FillPoints(filename2) == 0) cout << "Filling input points successful"<<endl;
	else exit(-1);
	
	if(queries.FillPoints(filename2) == 0) cout << "Filling query points successful"<<endl;
	else exit(-1);


	int dimension = queries.get_dimension();
	cout << endl << "Dimension = "<< dimension <<endl;


    Cluster* clusters = new Cluster[k];  // create k clusters


	

	/*
	int i = 0;
	int j = 3;

	Point& centr1 = *(clusters[i].get_centroid());
	Point& centr2 = *(clusters[j].get_centroid());

	cout << "Distance from centroid " << i << " to centroid " << j << " : " << Distance(centr1,centr2,1) <<endl;
	*/

	Initialize_Centroids(input,clusters,k); // k-means++
	cout << "Initialization complete"<<endl;
	Loyds_Clusters(input,clusters,k);
}



void Loyds_Clusters(Point_Array& input,Cluster* clusters,int k){

	bool not_converged = true;
	int loops = 0;

	int old_assigned;
	// in the first loop, all the points will be assigned
	int new_assigned = input.get_ArraySize();

	while(not_converged == true){

		++loops;
		if(loops > 30) break;

		old_assigned = new_assigned;
		bool changed = Loyds_Assign(input,clusters,k,&new_assigned);

		if(loops > 1){
			if(abs(new_assigned - old_assigned) <= 10){
				not_converged = false;
				break;
			}
		}

		if(changed == false){
			not_converged = false;
			break;
		}

		Update(input,clusters,k);
	}

	if(not_converged == true) cout << "Clustering failed"<<endl;
	else cout << "Clustering successful" << endl;
}

void Update(Point_Array& input,Cluster* clusters,int k){


	// for each cluster
	for(int i = 0; i < k; i++){

		for(int j = 0; j < k; j++){
			cout << "Cluster " << j << " has : " << clusters[j].Cluster_Size() << " points " <<endl;
		}

		long double old_obj = clusters[i].Get_Objective();

		cout << "Cluster " << i << " has obj function before update : " << old_obj <<endl;
		
		clusters[i].Compute_New_Centroid(input,i);
		cout << "Cluster " << i << "has obj function after update : " << clusters[i].Get_Objective()<<endl;
	
	}

}



bool Loyds_Assign(Point_Array& input,Cluster* clusters,int k,int* assigned){

	int input_points = input.get_ArraySize();
	int nearest_cluster = 0;
	double min_distance;
    double distance;

	bool changed = true;

	int points_not_changed = 0;

	// for each point
	for(int i = 0; i < input_points; i++){

		min_distance = std::numeric_limits<double>::max();

		Point& point = input.Retrieve(i);
		if(point.check_centroid() == true) continue;

		// for each cluster
		for(int j = 0; j < k; j++){

			// compute the distance from a point to all the clusters
			Point& centr = *(clusters[j].get_centroid());
			distance = Distance(point,centr,1);
			
			if (distance < min_distance){
				min_distance = distance;
				nearest_cluster = j;

			}
			
		}

		int point_nearest_cluster = point.Nearest_Cluster_id();
		
		// check if point needs to change cluster
		if( point_nearest_cluster != nearest_cluster){

			// it's not the first time the point has been assigned to a cluster
			// remove the point from the old cluster
			if(point_nearest_cluster != -1){
				clusters[point_nearest_cluster].Remove_Point(i);

				Point& old_centr = *(clusters[point_nearest_cluster].get_centroid());
				int old_distance = Distance(point,old_centr,1);
				

				 // decrease the obj function of the old cluster
				clusters[point_nearest_cluster].Add_Objective( - old_distance);
			}

			//cout << "Nearest cluster for point " << i << " is " << nearest_cluster <<endl;
			
			clusters[nearest_cluster].Assign_Point(i);
			
			clusters[nearest_cluster].Add_Objective(min_distance);
			point.Assign_Cluster(nearest_cluster);
			
		}
		else{
			++points_not_changed;
		}
	}

	// There is no change in assignment
	if(points_not_changed == input_points) changed = false;

	*assigned = input_points - points_not_changed;
	cout << "New points assigned = " << input_points - points_not_changed <<endl;
	return changed;
}

