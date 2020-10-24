

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


int Cluster::Cluster_Size(){

    return points.size();
}

void Cluster::Assign_Point(int id){
   
    points.push_back(id);
   // cout << "push back successful current size = " << points.size() << endl;
}

void Cluster::Compute_New_Centroid(Point_Array& input,int cluster_num){

	/*
	int points_in_cluster = points.size();
    int min_centroid;
    double min_distance = std::numeric_limits<double>::max();
    int next_id;

    if(points_in_cluster == 0) return;

    

    int old_centroid_id = centroid_id;
    cout << "Old centroid id = "<< old_centroid_id <<endl;

    //cout << "Before loop ok" <<endl;

    int dimension = this->centroid->get_dimension();

    for(int j = 0; j < points_in_cluster; j++){

        long double sum = 0;
        

        next_id = points.at(j);
        Point& possible_centroid = input.Retrieve(next_id);
        Point& centr = *centroid;

        sum += Distance(centr,possible_centroid,1);

        for(int m = 0; m < points_in_cluster; m++){
            if(m == j) continue;

            int id = points.at(m);
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
        cout << "New centroid id = " << next_id << endl;
        Point* new_centroid = input.Retrieve_ptr(next_id);
        new_centroid->set_centroid(true);

        Remove_Point(next_id);
        Assign_Point(old_centroid_id);
        
        centroid->set_centroid(false);
        centroid->Assign_Cluster(cluster_num);

        centroid = new_centroid;
        centroid_id = next_id;
    }

    */

    int dimension = input.get_dimension();

    int points_in_cluster = points.size();
    int nth_position = ceil(points_in_cluster/2); 

    int coordinates[points_in_cluster]; 

	// for each dimension
	for(int i = 0; i < dimension; i++){

		//for each id that belongs in the cluster
		for (int j = 0; j < points_in_cluster; j++){
			int id = points.at(j);

			Point* p = input.Retrieve_ptr(id);
			coordinates[j] = p->get_coordinate(i);
		}

        // sort the coordinate array and put the n/2-th maximum value in the centroid
        // the function sorts in ascending order 
		quickSort(coordinates, 0, points_in_cluster - 1);
		centroid->set_coordinate(i,coordinates[nth_position]);
	}
    
    // compute new objective function
    //Compute_Objective(input);
    /*
    cout << "Printing new centroid " <<endl;
    centroid->PrintPoint();
    */
}


void Cluster::Compute_Objective(Point_Array& input){

    long double sum = 0.0;
    int points_in_cluster = points.size();

    Point& centr = *centroid;

    for (int j = 0; j < points_in_cluster; j++){
        int id = points.at(j);

        Point& p = input.Retrieve(id);
        sum += Distance(centr,p,1);
    }

    //cout << endl << "Old objective function value : " << objective_function_value << endl;
   // cout << "New objective function value : " << sum << endl;
    objective_function_value = sum;

}


void Cluster::Remove_Point(int id){

	//points.remove(id);

    auto it = std::find(points.begin(), points.end(), id);

    points.erase(it);
}

int Cluster::Retrieve_ID(int position){

    return points.at(position);
}