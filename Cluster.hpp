#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <iostream>
#include <list>

#include "Point.hpp"

using namespace std;

class Cluster{

    public:
        Cluster();
        ~Cluster();
        void Assign_Point(int id); // assign the point id to the cluster
        void Assign_Centroid(Point* c);
        void Compute_New_Centroid();


    private:

        Point* centroid; // centroid is a Point
        list<int> points; // the point ids that belong to the cluster

};

#endif