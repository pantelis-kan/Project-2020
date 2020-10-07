#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <random>

using namespace std;

class Point{


	public:
		Point();
		~Point();
		int get_dimension();
		void PrintPoint();
		void AddtoPoint(int pos,int val);
		

		long long int LSH_Manhattan(int M, const long long int m, double w, double** s_params,int current_k);

		friend double Distance(Point& p1, Point& p2, int metric); // returns the distance between two points

	private:

		int point[784]; // a collection of integers for each point
		int dimension;
};

#endif
